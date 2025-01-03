#include "_5727.h"
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <thread>
#include "_6221.h"
#include "_7683.h"
#include "_4326.h"

TFMClient::TFMClient(void* main)
    : main(main), bulle(nullptr), logged(false), id(0), pid(0), isSouris(false) {}

void TFMClient::packetReceived(bool outbound, std::shared_ptr<Connection> conn, std::shared_ptr<Packet> packet) {
    if (outbound) {
        uint8_t fp = packet->read8();
        dispatch("raw_socket_outbound", conn, fp, packet);
    }
    else {
        dispatch("raw_socket_inbound", conn, packet);
    }
}

void TFMClient::onTriggerPlugin(std::shared_ptr<void> plugin, bool sent, std::shared_ptr<Connection> conn, std::shared_ptr<Packet> packet) {
    auto method = sent ? &EventBased::packetSent : &EventBased::packetReceived;

    try {
        (plugin.get()->*method)(this, conn, packet);
    }
    catch (const std::exception& e) {
        std::ostringstream oss;
        oss << "Ignored exception on plugin \"" << plugin->name << "\" while parsing " << (sent ? "outbound" : "inbound") << " packet:\n\n" << e.what();
        std::cerr << oss.str() << std::endl;
    }
}

void TFMClient::onRawSocketOutbound(std::shared_ptr<Connection> conn, uint8_t fp, std::shared_ptr<Packet> packet) {
    if (logged) {
        auto CCC = packet->readCode();

        if (CCC == std::make_pair(6, 6)) {
            if (msgKeys.empty() && packet->buffer.size() > 22) {
                msgPacket = std::make_shared<Packet>(packet->readBytes(20));
            }
        }

        packet->pos = 1;
    }

    for (auto plugin : PluginsWatcher::watcher) {
        dispatch("trigger_plugin", plugin, true, conn, fp, packet->copy(true));
    }
}

void TFMClient::onRawSocketInbound(std::shared_ptr<Connection> conn, std::shared_ptr<Packet> packet) {
    if (!logged) {
        auto CCC = packet->readCode();

        if (CCC == std::make_pair(26, 2)) {
            logged = true;
            id = packet->read32();
            name = packet->readUTF();
            packet->read32();
            packet->read8();
            pid = packet->read32();
            isSouris = id == 0;
        }

        packet->pos = 0;
    }
    else if (msgKeys.empty() && msgPacket != nullptr) {
        auto CCC = packet->readCode();

        if (CCC == std::make_pair(6, 6)) {
            if (packet->readUTF() == name) {
                uint8_t fp = std::get<0>(*msgPacket);
                auto ciphered = std::get<1>(*msgPacket);
                auto deciphered = Packet::create(6, 6);
                deciphered->writeBytes(packet->readBytes(20));

                deciphered->xorCipher(ciphered, -1);
                deciphered->pos = 2;

                auto start = (fp + 1) % 20;
                auto last = deciphered->readBytes(20 - start);

                msgPacket = nullptr;
                msgKeys.clear();
                auto bytes = deciphered->readBytes(start);
                bytes.insert(bytes.end(), last.begin(), last.end());
                msgKeys.assign(bytes.begin(), bytes.end());
            }
        }

        packet->pos = 0;
    }

    for (auto plugin : PluginsWatcher::watcher) {
        dispatch("trigger_plugin", plugin, false, conn, packet->copy());
    }
}

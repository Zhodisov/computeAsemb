#ifndef _5727_H
#define _5727_H

#include <string>
#include <vector>
#include <memory>
#include "_6221.h"
#include "_7683.h"
#include "_4326.h"
#include "_9554.h"

class TFMClient : public EventBased {
public:
    TFMClient(void* main);

    void packetReceived(bool outbound, std::shared_ptr<Connection> conn, std::shared_ptr<Packet> packet);
    void onTriggerPlugin(std::shared_ptr<void> plugin, bool sent, std::shared_ptr<Connection> conn, std::shared_ptr<Packet> packet);
    void onRawSocketOutbound(std::shared_ptr<Connection> conn, uint8_t fp, std::shared_ptr<Packet> packet);
    void onRawSocketInbound(std::shared_ptr<Connection> conn, std::shared_ptr<Packet> packet);

private:
    std::vector<uint8_t> msgKeys;
    std::shared_ptr<Packet> msgPacket;

    void* main;
    std::shared_ptr<void> bulle;

    bool logged;
    int32_t id;
    std::string name;
    int32_t pid;
    bool isSouris;
};

#endif
#include "_1358.h"
#include "_9554.h"
#include <iostream>
#include <stdexcept>
#include <system_error>
#include <thread>
#include <vector>
#include <cstring>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

WinDivertDriver::WinDivertDriver(NetworkScanner* network, const std::string& ip, Connection* connection) {
    : DriverBase(network, ip, connection), w("ip.DstAddr == " + ip + " || ip.SrcAddr == " + ip) {}

void WinDivertDriver::scan() {
    try {
        w.open();
        while (true) {
            auto packet = w.receive();
            auto conn = getConnection(
                packet.src_addr,
                packet.dst_addr,
                packet.is_outbound
            );

            if (packet.tcp.fin) {
                conn->close();
            }
            else if (conn->ignored) {
                w.send(packet);
                continue;
            }
            else if (!packet.payload.empty()) {
                if (!conn->parsePacket(packet.payload, packet.is_outbound)) {
                    continue;
                }
            }
            w.send(packet);
        }
    }
    catch (const std::system_error& e) {
        if (e.code().value() == WSAEACCES) {
            throw std::runtime_error("Run me with admin privileges!") from e;
        }
        else if (e.code().value() == WSAEBADF) {
            return;
        }
        throw;
    }
    catch (...) {
        for (auto& conn : connections) {
            conn.second->close();
        }
        this->close();
        throw;
    }
}

void WinDivertDriver::close() {
    if (w.is_open()) {
        try {
            w.close();
        }
        catch (...) {
        }

        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            return;
        }

        SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
        if (sock != INVALID_SOCKET) {
            struct sockaddr_in sa;
            sa.sin_family = AF_INET;
            sa.sin_port = htons(6666);
            inet_pton(AF_INET, ip.c_str(), &sa.sin_addr);
            sendto(sock, "\x00", 1, 0, (struct sockaddr*)&sa, sizeof(sa));
            closesocket(sock);
        }

        WSACleanup();
    }
}

WinDivertDriver::WinDivert::WinDivert(const std::string& filter) : filter(filter) {}

bool WinDivertDriver::WinDivert::is_open() const {
    return true; //outdated
}

void WinDivertDriver::WinDivert::open() {
    //outdated
}

void WinDivertDriver::WinDivert::close() {
    //outdated
}

void WinDivertDriver::WinDivert::send(const std::string& packet) {
   //outdated
}

std::string WinDivertDriver::WinDivert::receive() {
    return "packet"; //outdated
}

#ifndef _6499_H
#define _6499_H

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <functional>
#include <stdexcept>
#include "_9554.h"
#include "_4326.h"
#include "_5727.h"

const std::string main_ip = "37.187.29.8";
extern std::unordered_map<std::string, std::shared_ptr<TFMClient>> bulle_keys;

class TFMPacketReader {
public:
    explicit TFMPacketReader(int extra);
    std::vector<std::shared_ptr<Packet>> consumePayload(const std::vector<uint8_t>& payload);

private:
    int extra;
    std::vector<uint8_t> buffer;
    int length;
};

class TFMConnection : public Connection {
public:
    TFMConnection(NetworkScanner* network, const std::string& local, const std::string& remote);

    std::shared_ptr<TFMPacketReader> createReader(int extra);
    std::shared_ptr<TFMClient> createClient();

    bool parsePacket(const std::vector<uint8_t>& payload, bool outbound) override;

private:
    std::shared_ptr<TFMPacketReader> outbound;
    std::shared_ptr<TFMPacketReader> inbound;

    std::shared_ptr<TFMClient> client;

    std::string name;
    bool needsHandshake;
    std::pair<uint8_t, uint8_t> handshakeCCC;
};

#endif

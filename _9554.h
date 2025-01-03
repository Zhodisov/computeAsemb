#ifndef _9554_H
#define _9554_H

#include <string>
#include <vector>
#include <ctime>
#include <memory>

class NetworkScanner;

class Connection : public std::enable_shared_from_this<Connection> {
public:
    Connection(NetworkScanner* network, const std::string& local, const std::string& remote);
    void ignore();
    void close();
    virtual bool parsePacket(const std::vector<uint8_t>& payload, bool outbound);

public:
    NetworkScanner* network;
    std::string local;
    std::string remote;
    bool ignored;
    bool closing;
    double closed_at;
};

#endif

#ifndef _5281_H
#define _5281_H

#include <string>
#include <unordered_map>
#include <memory>
#include <stdexcept>
#include <chrono>

class NetworkScanner;
class Connection;

class DriverBase {
public:
    DriverBase(NetworkScanner* network, const std::string& ip, Connection* connection);
    virtual ~DriverBase() = default;

    std::shared_ptr<Connection> createConnection(NetworkScanner* network, const std::string& local, const std::string& remote);
    std::shared_ptr<Connection> getConnection(const std::string& source, const std::string& dest, bool outbound);

    virtual void scan() = 0;
    virtual void close() = 0;

protected:
    NetworkScanner* network;
    std::string ip;
    Connection* connection;

private:
    std::unordered_map<std::string, std::shared_ptr<Connection>> connections;
};

#endif
#include "_5281.h"
#include "_9554.h"
#include "_5460.h"

DriverBase::DriverBase(NetworkScanner* network, const std::string& ip, Connection* connection)
    : network(network), ip(ip), connection(connection) {}

std::shared_ptr<Connection> DriverBase::createConnection(NetworkScanner* network, const std::string& local, const std::string& remote) {
    auto conn = std::make_shared<Connection>(network, local, remote);
    connections[local] = conn;
    return conn;
}

std::shared_ptr<Connection> DriverBase::getConnection(const std::string& source, const std::string& dest, bool outbound) {
    std::string local = outbound ? source : dest;
    std::string remote = outbound ? dest : source;

    if (connections.find(local) == connections.end()) {
        return createConnection(network, local, remote);
    }

    auto conn = connections[local];
    if (conn->closing && std::chrono::duration<double>(std::chrono::high_resolution_clock::now().time_since_epoch()).count() >= conn->closed_at) {
        connections.erase(local);
        return createConnection(network, local, remote);
    }

    return conn;
}

void DriverBase::scan() {
    throw std::runtime_error("NotImplementedError");
}

void DriverBase::close() {
    throw std::runtime_error("NotImplementedError");
}

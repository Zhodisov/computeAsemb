#include "_9554.h"
#include "_5460.h"
#include <chrono>
#include <stdexcept>

Connection::Connection(NetworkScanner* network, const std::string& local, const std::string& remote)
    : network(network), local(local), remote(remote), ignored(false), closing(false), closed_at(0) {}

void Connection::ignore() {
    ignored = true;
}

void Connection::close() {
    ignore();
    closing = true;
    closed_at = std::chrono::duration<double>(std::chrono::high_resolution_clock::now().time_since_epoch()).count() + 1.0;
}

bool Connection::parsePacket(const std::vector<uint8_t>& payload, bool outbound) {
    throw std::runtime_error("NotImplementedError");
}

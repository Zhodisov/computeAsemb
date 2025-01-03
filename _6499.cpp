#include "_5460.h"
#include "_3451.h"
#include "_9554.h"
#include <stdexcept>
#include <iostream>
#include <algorithm>
#include <thread>
#include <chrono>

NetworkScanner::NetworkScanner(Driver* driver, Connection* connection)
    : driver(driver), connection(connection), running(true) {
    poolThread = std::thread(&NetworkScanner::watchPool, this);
}

NetworkScanner::~NetworkScanner() {
    if (running) {
        stop();
    }
    if (poolThread.joinable()) {
        poolThread.join();
    }
}

void NetworkScanner::watchPool() {
    while (running) {
        std::vector<std::future<void>> toRemove;

        for (auto& future : futures) {
            if (future.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
                try {
                    future.get();
                }
                catch (const std::exception& e) {
                    std::cerr << e.what() << std::endl;
                }
                toRemove.push_back(std::move(future));
            }
        }

        for (auto& future : toRemove) {
            futures.erase(std::remove(futures.begin(), futures.end(), future), futures.end());
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void NetworkScanner::stop() {
    if (!running) {
        return;
    }

    for (auto& pair : scanners) {
        pair.second->close();
    }
    scanners.clear();

    running = false;
}

void NetworkScanner::add(const std::string& ip) {
    if (scanners.find(ip) == scanners.end()) {
        std::cout << ":: " << ip << std::endl;
        auto scanner = std::make_unique<Driver>(this, ip, connection);
        futures.push_back(std::async(std::launch::async, &Driver::scan, scanner.get()));
        scanners[ip] = std::move(scanner);
    }
}

void NetworkScanner::remove(const std::string& ip) {
    auto it = scanners.find(ip);
    if (it == scanners.end()) {
        return;
    }

    it->second->close();
    scanners.erase(it);
}

#ifndef _5460_H
#define _5460_H

#include <unordered_map>
#include <vector>
#include <memory>
#include <future>
#include <thread>
#include <chrono>
#include <iostream>
#include <string>
#include <atomic>
#include "_5281.h" 
class Connection;

class NetworkScanner {
public:
    NetworkScanner(DriverBase* driver, Connection* connection);
    ~NetworkScanner();
    void stop();
    void add(const std::string& ip);
    void remove(const std::string& ip);

private:
    void watchPool();

    std::vector<std::future<void>> futures;
    std::unordered_map<std::string, std::unique_ptr<DriverBase>> scanners;
    std::atomic<bool> running;
    std::thread poolThread;
    DriverBase* driver;
    Connection* connection;
};

#endif
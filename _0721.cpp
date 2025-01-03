#include <iostream>
#include <csignal>
#include <thread>
#include <chrono>
#include "_5460.h"
#include "_1358.h"
#include "_6499.h"

std::atomic<bool> running(true);

void signalHandler(int signum) {
    running = false;
}

int main() {
    std::signal(SIGINT, signalHandler);

    auto network = std::make_shared<NetworkScanner>(new WinDivertDriver(nullptr, "", nullptr), nullptr);
    network->add(main_ip);
    std::cout << "Network" << std::endl;
    while (running) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    network->stop();
    std::cout << "\rBye bye!" << std::endl;

    return 0;
}

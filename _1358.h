#ifndef _1358_H
#define _1358_H

#include "_5281.h"
#include <string>
#include <unordered_map>
#include <memory>
#include <stdexcept>

class WinDivertDriver : public DriverBase {
public:
    WinDivertDriver(NetworkScanner* network, const std::string& ip, Connection* connection);
    void scan() override;
    void close() override;

private:
    class WinDivert {
    public:
        WinDivert(const std::string& filter);
        bool is_open() const;
        void open();
        void close();
        void send(const std::string& packet);
        std::string receive();
    private:
        std::string filter;
    };

    WinDivert w;
};

#endif

#ifndef _3451_H
#define _3451_H

#include "_5281.h"

class Driver : public DriverBase {
public:
    Driver(NetworkScanner* network, const std::string& ip, Connection* connection);
    void scan() override;
    void close() override;
};

#endif
#pragma once

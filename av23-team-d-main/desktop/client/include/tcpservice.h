#ifndef TCPSERVICE_H
#define TCPSERVICE_H

#include "comservice.h"
#include <atomic>
#include <thread>

class TCPService : public COMService
{
    std::atomic<bool> running{true};

    std::thread thrd{&TCPService::run, this};

    void run() override;

public:
    TCPService() = default;

    ~TCPService()
    {
        running = false;
        thrd.join();
    }
};

#endif

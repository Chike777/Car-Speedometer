#ifndef TCPSERVICE_H
#define TCPSERVICE_H

#include <atomic>
#include <thread>
#include <unistd.h>
#include <arpa/inet.h>
#include "comservice.h"

class TCPService : public COMService
{
    int server_fd;
    std::atomic<bool> running{true};

    std::thread thrd{&TCPService::run, this};

    void run() override;

public:
    TCPService() = default;

    ~TCPService()
    {
        running = false;
        shutdown(server_fd, SHUT_RDWR);
        close(server_fd);
        thrd.join();
    }
};

#endif
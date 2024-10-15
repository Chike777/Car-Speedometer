#ifndef SERIALSERVICE_H
#define SERIALSERVICE_H

#include "comservice.h"
#include <atomic>
#include <QThread>

class SerialService : public COMService, public QThread
{
    std::atomic<bool> running{true};

    void run() override;

public:
    SerialService()
    {
        start();
    }

    ~SerialService()
    {
        running = false;
        wait();
    }
};

#endif
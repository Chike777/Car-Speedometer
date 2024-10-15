#ifndef COMSERVICE_H
#define COMSERVICE_H

#include <cstdint>
#include <cstring>
#include <atomic>
#include <mutex>
#include "setting.h"

// Abstract base class defining the communication service interface
class COMService
{
protected:
    std::atomic<bool> communication_status{false};

    uint8_t buffer[Settings::Signal::BUFSIZE]{0}; // Buffer size from Signal class

    std::mutex buffer_mutex; // Mutex to protect the buffer

    Settings::Signal &signal = Settings::Signal::instance();

public:
    int get_speed();         // Read speed from the buffer
    int get_temperature();   // Read temperature from the buffer
    int get_battery_level(); // Read battery level from the buffer

    bool get_left_signal();    // Check left signal status
    bool get_right_signal();   // Check right signal status
    bool get_warning_signal(); // Check warning signal status

    void extract(size_t bit_pos, size_t bit_length, uint32_t &value);
    void extract(size_t bit_pos, size_t bit_length, int &value);

    bool get_status(); // Get the status of the communication

    virtual void run() = 0; // Receive the buffer over the communication protocol
    virtual ~COMService() = default;
};
#endif // COMSERVICE_H
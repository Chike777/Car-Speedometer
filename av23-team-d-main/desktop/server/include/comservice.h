#ifndef COMSERVICE_H
#define COMSERVICE_H

#include "setting.h"
#include <mutex>
#include <atomic>

class COMService
{
protected:
    uint8_t buffer[Settings::Signal::BUFSIZE]{0}; // Buffer size from Signal class

    std::mutex buffer_mutex; // Mutex to protect the buffer

    Settings::Signal &signal = Settings::Signal::instance(); // Access the singleton instance

public:
    void set_speed(int speed);
    void set_temperature(int temp);

    void set_battery_level(int battery_level);
    void set_right_signal(bool right_signal);
    void set_left_signal(bool left_signal);
    void set_warning_signal(bool warning_signal);

    void buffer_insert(uint32_t value, size_t start_bit, size_t bit_length);

    virtual void run() = 0;
    virtual ~COMService() = default;
};
#endif // COMSERVICE_H

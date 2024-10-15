#include "comservice.h"
#include "setting.h"

// Read speed from the buffer
int COMService::get_speed()
{
    uint32_t speed{0};

    extract(signal["speed"].start, signal["speed"].length, speed);

    return speed;
}

// Read temperature from the buffer
int COMService::get_temperature()
{
    int temp{0};

    extract(signal["temperature"].start, signal["temperature"].length, temp);

    return temp;
}

// Read battery level from the buffer
int COMService::get_battery_level()
{
    uint32_t battery_level{0};

    extract(signal["battery_level"].start, signal["battery_level"].length, battery_level);

    return battery_level;
}

// Check left signal status
bool COMService::get_left_signal()
{
    uint32_t left_signal{0};

    extract(signal["left_light"].start, signal["left_light"].length, left_signal);

    return left_signal;
}

// Check right signal status
bool COMService::get_right_signal()
{
    uint32_t right_signal{0};

    extract(signal["right_light"].start, signal["right_light"].length, right_signal);

    return right_signal;
}

// Check warning signal status
bool COMService::get_warning_signal()
{
    uint32_t warning_signal{0};

    extract(signal["warning_light"].start, signal["warning_light"].length, warning_signal);

    return warning_signal;
}

// Extract bits from the buffer for unsigned integers
void COMService::extract(size_t bit_pos, size_t bit_length, uint32_t &value)
{
    std::scoped_lock<std::mutex> locker{buffer_mutex};
    uint32_t result = 0;

    // Extract bits from the buffer
    for (size_t i = 0; i < bit_length; ++i)
    {
        size_t current_bit = bit_pos + i;
        size_t byte_index = current_bit / 8;
        size_t bit_index = current_bit % 8;

        if (buffer[byte_index] & (1 << (7 - bit_index)))
        {
            result |= (1 << i);
        }
    }

    value = result;
}

// Extract bits from the buffer for signed integers (e.g., temperature)
void COMService::extract(size_t bit_pos, size_t bit_length, int &value)
{
    std::scoped_lock<std::mutex> locker{buffer_mutex};
    uint32_t result = 0;

    // Extract bits from the buffer
    for (size_t i = 0; i < bit_length; ++i)
    {
        size_t current_bit = bit_pos + i;
        size_t byte_index = current_bit / 8;
        size_t bit_index = current_bit % 8;

        if (buffer[byte_index] & (1 << (7 - bit_index)))
        {
            result |= (1 << i);
        }
    }

    uint32_t sign_bit = (result >> (bit_length - 1)) & 0x01;
    uint32_t magnitude_value = result & ((1u << (bit_length - 1)) - 1);
    value = (sign_bit != 0) ? -static_cast<int>((1u << (bit_length - 1)) - magnitude_value) : static_cast<int>(magnitude_value);
}

// Get the status of the communication
bool COMService::get_status()
{
    return communication_status;
}
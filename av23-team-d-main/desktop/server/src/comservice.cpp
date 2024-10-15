#include "comservice.h"
#include "setting.h"

// Set speed in the buffer
void COMService::set_speed(int speed)
{
    buffer_insert(static_cast<uint32_t>(speed), signal["speed"].start, signal["speed"].length);
}

// Set temperature in the buffer
void COMService::set_temperature(int temp)
{
    buffer_insert(static_cast<uint32_t>(temp), signal["temperature"].start, signal["temperature"].length);
}

// Set battery level in the buffer
void COMService::set_battery_level(int battery_level)
{
    buffer_insert(static_cast<uint32_t>(battery_level), signal["battery_level"].start, signal["battery_level"].length);
}

// Set right signal status in the buffer
void COMService::set_right_signal(bool right_signal)
{
    buffer_insert(static_cast<uint32_t>(right_signal), signal["right_light"].start, signal["right_light"].length);
}

// Set left signal status in the buffer
void COMService::set_left_signal(bool left_signal)
{
    buffer_insert(static_cast<uint32_t>(left_signal), signal["left_light"].start, signal["left_light"].length);
}

// Set warning signal status in the buffer
void COMService::set_warning_signal(bool warning_signal)
{
    buffer_insert(static_cast<uint32_t>(warning_signal), signal["warning_light"].start, signal["warning_light"].length);
}

// Insert value into the buffer
void COMService::buffer_insert(uint32_t value, size_t start_bit, size_t bit_length)
{
    std::scoped_lock<std::mutex> locker{buffer_mutex};
    size_t end_bit = start_bit + bit_length - 1;

    // Clear the target bits in the buffer
    for (size_t i = start_bit; i <= end_bit; ++i)
    {
        size_t byte_index = i / 8;
        size_t bit_index = i % 8;

        buffer[byte_index] &= ~(1 << (7 - bit_index)); // Clear the bit
    }

    // Place the number into the buffer
    for (size_t i = 0; i < bit_length; ++i)
    {
        size_t current_bit = start_bit + i;
        size_t byte_index = current_bit / 8;
        size_t bit_index = current_bit % 8;

        if (value & (1 << i))
        {
            buffer[byte_index] |= (1 << (7 - bit_index)); // Set the bit
        }
    }
}

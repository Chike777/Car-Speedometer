#ifndef SETTING_H
#define SETTING_H

#include <climits>
#include <map>
#include <string>

namespace Settings
{
    struct signal_info
    {
        const char *key;
        int min, max, start, length;

        constexpr signal_info(const char *str = "", int _min = 0, int _max = 0, int _start = 0, int _length = 0)
            : key{str}, min{_min}, max{_max}, start{_start}, length{_length} {}
    };

    // Buffer template to access lengths of the signals
    template <size_t N>
    struct buffer
    {
        static constexpr size_t get_length(const signal_info *list)
        {
            return list[N - 1].length + buffer<N - 1>::get_length(list);
        }
    };

    // Specialization for N=1 (base case)
    template <>
    struct buffer<1>
    {
        static constexpr size_t get_length(const signal_info *list)
        {
            return list[0].length;
        }
    };

    class Signal
    {
    private:
        static constexpr signal_info list[]{
            {"speed", 0, 240, 0, 8},
            {"temperature", -60, 60, 8, 7},
            {"battery_level", 0, 100, 15, 7},
            {"left_light", 0, 1, 22, 1},
            {"right_light", 0, 1, 23, 1},
            {"warning_light", 0, 1, 24, 1},
        };

        std::map<std::string, signal_info> signal;

        Signal()
        {
            for (const auto &elem : list)
            {
                signal.emplace(elem.key, elem);
            }
        }

    public:
        // Use buffer struct to get total length and calculate BUFSIZE outside buffer
        static constexpr size_t TOTAL_LENGTH = buffer<sizeof(list) / sizeof(list[0])>::get_length(list);
        static constexpr size_t BUFSIZE = TOTAL_LENGTH / (sizeof(list) / sizeof(list[0]));

        static Signal &instance()
        {
            static Signal instance;
            return instance;
        }

        signal_info operator[](const std::string &key) const
        {
            return signal.at(key);
        }
    };

    constexpr int INTERVAL{50};
}

namespace UART_SETTINGS
{
    constexpr int CAN_ID{0x123};
    constexpr int BAUD_RATE{115200};
    constexpr char CLIENT_PORT[]{"/dev/ttyUSB0"};
    constexpr char SERVER_PORT[]{"/dev/ttyUSB1"};
}

namespace TCPIP
{
    constexpr int PORT{8080};
    const char *const IP{"127.0.0.1"};
}

#endif // SETTING_H
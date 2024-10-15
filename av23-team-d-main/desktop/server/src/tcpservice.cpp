#include "tcpservice.h"

#include <iostream>
#include <cstring>

void TCPService::run()
{
    int opt = 1;
    int new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        std::cerr << "socket failed";
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        std::cerr << "setsockopt";
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(TCPIP::PORT);

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        std::cerr << "bind failed";
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 1) < 0)
    {
        std::cerr << "listen";
        exit(EXIT_FAILURE);
    }

    while (running)
    {
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);

        if (new_socket >= 0)
        {
            while (running) // Sending buffer with mutex protection
            {
                uint8_t temp[sizeof(buffer)]{0};

                {
                    std::scoped_lock<std::mutex> locker{buffer_mutex};
                    std::memcpy(temp, buffer, sizeof(temp));
                }

                if (sizeof(temp) != write(new_socket, temp, sizeof(temp)))
                {
                    std::cerr << "Send failed!" << std::endl;
                    break;
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(Settings::INTERVAL / 2));
            }

            close(new_socket);
        }
    }
}
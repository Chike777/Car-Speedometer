#include "tcpservice.h"

#include <netdb.h>
#include <cstring>
#include <unistd.h>
#include <iostream>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void TCPService::run()
{
    int sock = 0;
    struct sockaddr_in serv_addr;

    while (running)
    {
        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
            std::cerr << "Socket creation error" << std::endl;
            continue;
        }

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(TCPIP::PORT);

        // Convert IPv4 and IPv6 addresses from text to binary form
        if (inet_pton(AF_INET, TCPIP::IP, &serv_addr.sin_addr) <= 0)
        {
            std::cerr << "Invalid address/ Address not supported" << std::endl;
            close(sock);
            continue;
        }

        if (::connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        {
            close(sock);
            continue;
        }

        // Connection successful, now handle communication
        while (running)
        {

            // Write data to the serial port
            uint8_t temp[sizeof(buffer)]{0};

            int bytes_received = recv(sock, temp, sizeof(buffer), 0);

            {
                std::scoped_lock<std::mutex> locker{buffer_mutex};
                std::memcpy(buffer, temp, sizeof(buffer));
            }

            if (bytes_received <= 0)
            {
                if (bytes_received == 0)
                {
                    std::cout << "Connection closed by server" << std::endl;
                }
                else
                {
                    std::cerr << "Receiving data failed" << std::endl;
                }
                communication_status = false;
                break; // Exit the communication loop to reconnect
            }
            else
            {
                communication_status = true;
            }
        }
    }
    shutdown(sock, SHUT_RDWR);
    close(sock);
}
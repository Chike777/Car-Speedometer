#include "serialservice.h"
#include <QDebug>
#include <QSerialPort>

void SerialService::run()
{
    QSerialPort serialPort;

    // Set the port name and configure the serial port
    serialPort.setPortName(UART_SETTINGS::SERVER_PORT);
    serialPort.setBaudRate(UART_SETTINGS::BAUD_RATE);
    serialPort.setDataBits(QSerialPort::Data8);
    serialPort.setParity(QSerialPort::NoParity);
    serialPort.setStopBits(QSerialPort::OneStop);
    serialPort.setFlowControl(QSerialPort::NoFlowControl);

    while (running)
    {
        // Try to open the serial port once
        if (serialPort.open(QIODevice::WriteOnly))
        {
            qDebug() << "Serial port opened successfully!";

            while (running && serialPort.isWritable())
            {
                uint8_t temp[sizeof(buffer)]{0};

                {
                    std::scoped_lock<std::mutex> locker{buffer_mutex};
                    std::memcpy(temp, buffer, sizeof(temp));
                }

                if (sizeof(temp) == serialPort.write(reinterpret_cast<char *>(temp), sizeof(temp)))
                {
                    if (!serialPort.flush())
                    {
                        qDebug() << "Not all data was written to the serial port.";
                        break;
                    }
                }
                else
                {
                    qDebug() << "Not all data was written to the serial port.";
                    break;
                }

                msleep(Settings::INTERVAL / 2);
            }
        }
        else
        {
            qDebug() << "Failed to open serial port:" << serialPort.errorString();
        }

        // Close the serial port when done
        if (serialPort.isOpen())
        {
            serialPort.close();
        }
    }
}
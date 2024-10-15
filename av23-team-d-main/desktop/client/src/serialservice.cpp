#include "serialservice.h"
#include <QDebug>
#include <QSerialPort>

void SerialService::run()
{
    QSerialPort serialPort;
    // Set the port name and configure the serial port
    serialPort.setPortName(UART_SETTINGS::CLIENT_PORT);
    serialPort.setBaudRate(UART_SETTINGS::BAUD_RATE);
    serialPort.setDataBits(QSerialPort::Data8);
    serialPort.setParity(QSerialPort::NoParity);
    serialPort.setStopBits(QSerialPort::OneStop);
    serialPort.setFlowControl(QSerialPort::NoFlowControl);

    while (running)
    {
        // Try to open the serial port once
        if (!serialPort.open(QIODevice::ReadOnly))
        {
            qDebug() << "Failed to open serial port:" << serialPort.errorString();
        }
        else
        {
            while (running && serialPort.isReadable())
            {
                uint8_t temp[sizeof(buffer)]{0};
                (void)serialPort.clear();

                if (serialPort.waitForReadyRead(Settings::INTERVAL * 2))
                {
                    if (sizeof(temp) == serialPort.read(reinterpret_cast<char *>(temp), sizeof(temp)))
                    {
                        communication_status = true;
                        std::scoped_lock<std::mutex> locker{buffer_mutex};
                        std::memcpy(buffer, temp, sizeof(buffer));
                    }
                    else
                    {
                        qDebug() << "Failed to read data from serial port:" << serialPort.errorString();
                        communication_status = false;
                        break;
                    }
                }
                else
                {
                    qDebug() << "Timeout or error while reading from serial port:" << serialPort.errorString();
                    communication_status = false;
                    break;
                }
            }
        }

        // Close the serial port when done
        if (serialPort.isOpen())
        {
            serialPort.close();
        }
    }
}
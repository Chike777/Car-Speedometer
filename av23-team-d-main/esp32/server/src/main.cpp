#include <Arduino.h>
#include <CAN.h>
#include <CAN_config.h>

#include "setting.h"

CAN_device_t CAN_cfg;

void setup()
{
    Serial.begin(UART_SETTINGS::BAUD_RATE);

    // Configure CAN communication
    CAN_cfg.tx_pin_id = GPIO_NUM_5;
    CAN_cfg.rx_pin_id = GPIO_NUM_35;
    CAN_cfg.speed = CAN_SPEED_500KBPS;
    CAN_cfg.rx_queue = xQueueCreate(1, sizeof(CAN_frame_t));

    CAN_init(); // initialize the CAN Module
}

void loop()
{
    // Prepare CAN frame
    CAN_frame_t frame{0};
    frame.MsgID = UART_SETTINGS::CAN_ID; // Example ID, use an appropriate ID for your application
    frame.FIR.B.FF = CAN_frame_std;
    frame.FIR.B.DLC = Settings::Signal::BUFSIZE; // Set Data Length Code to the number of bytes read

    if (Settings::Signal::BUFSIZE == Serial.readBytes(frame.data.u8, Settings::Signal::BUFSIZE))
    {
        CAN_write_frame(&frame);
    }
}

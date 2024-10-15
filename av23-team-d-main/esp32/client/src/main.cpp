#include <Arduino.h>
#include <CAN.h>
#include <CAN_config.h>

#include "setting.h"

CAN_device_t CAN_cfg;

void setup()
{
    // Initialize Serial communication
    Serial.begin(UART_SETTINGS::BAUD_RATE);

    // Configure CAN communication
    CAN_cfg.tx_pin_id = GPIO_NUM_5;
    CAN_cfg.rx_pin_id = GPIO_NUM_35;
    CAN_cfg.speed = CAN_SPEED_500KBPS;
    CAN_cfg.rx_queue = xQueueCreate(1, sizeof(CAN_frame_t));

    // Initialize CAN module with configuration
    CAN_init();
}

void loop()
{
    // Check if a CAN frame has been received
    CAN_frame_t received_frame{0};

    if (pdTRUE == xQueueReceive(CAN_cfg.rx_queue, &received_frame, portMAX_DELAY))
    {
        Serial.write(received_frame.data.u8, Settings::Signal::BUFSIZE);
    }
}

#include <stdio.h>
#include "driver/gpio.h"
#include "driver/twai.h"

#include "esp_twai.h"
#include "esp_twai_onchip.h"
#include "esp_log.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define TAG_S "SENDER"

// https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/twai.html
void app_main(void)
{
    int tx_pin = 16;
    int rx_pin = 17;
    
    twai_node_handle_t node_hdl = NULL;
    twai_onchip_node_config_t node_config = {
        .io_cfg.tx = tx_pin,
        .io_cfg.rx = rx_pin,
        .bit_timing.bitrate = 200000,
        .tx_queue_depth = 5,
        .flags.enable_loopback =1, // enabled due to simulation issues (twai_node_transmit blocked due to lack of receiver)
    };
    
    ESP_ERROR_CHECK(twai_new_node_onchip(&node_config, &node_hdl));
    
    ESP_ERROR_CHECK(twai_node_enable(node_hdl));
    while(1) {
        ESP_LOGI(TAG_S,"start...\n");

        uint8_t temperature[1] = {25};
        twai_frame_t temp_msg = {
            .header.id = 0x10, .header.ide = false,
            .buffer = temperature, .buffer_len = 1,
        };
        twai_node_transmit(node_hdl, &temp_msg, 1000);
        ESP_LOGI(TAG_S,"start1...\n");

        uint8_t preasure[4] = {200};
        twai_frame_t pres_msg = {
            .header.id = 0x01, .header.ide = false,
            .buffer = preasure, .buffer_len = 4,
        };
        twai_node_transmit(node_hdl, &pres_msg, 0);
        ESP_LOGI(TAG_S,"start2...\n");


        uint8_t status[4] = {20, 30 ,40};
        twai_frame_t stat_msg = {
            .header.id = 0x100, .header.ide = false,
            .buffer = status, .buffer_len = 4,
        };
        twai_node_transmit(node_hdl, &stat_msg, 0);
        ESP_LOGI(TAG_S,"wait ...\n");

        twai_node_transmit_wait_all_done(node_hdl, 10);
        ESP_LOGI(TAG_S,"sended ...\n");
        vTaskDelay(pdMS_TO_TICKS(3000));
    }
}
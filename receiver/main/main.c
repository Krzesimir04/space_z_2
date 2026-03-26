#include <stdio.h>
#include "driver/gpio.h"
#include "driver/twai.h"

#include "esp_twai.h"
#include "esp_twai_onchip.h"
#include "esp_log.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define TAG_R "RECEIVER"

// https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/twai.html
static bool twai_rx_cb(twai_node_handle_t handle, const twai_rx_done_event_data_t *edata, void *user_ctx)
{
    uint8_t recv_buff[8];
    twai_frame_t rx_frame = {
        .buffer = recv_buff,
        .buffer_len = sizeof(recv_buff),
    };
    
    if (ESP_OK == twai_node_receive_from_isr(handle, &rx_frame)) {
        printf("Odebrano ID: 0x%lx, Dane: %d\n", rx_frame.header.id, rx_frame.buffer[0]);
    }
    return false;
}
void app_main(void)
{
    int tx_pin = 4;
    int rx_pin = 5;

    twai_node_handle_t node_hdl = NULL;
    twai_onchip_node_config_t node_config = {
        .io_cfg.tx = tx_pin,
        .io_cfg.rx = rx_pin,
        .bit_timing.bitrate = 200000,
        .tx_queue_depth = 5,
    };
    
    ESP_ERROR_CHECK(twai_new_node_onchip(&node_config, &node_hdl));

    ESP_LOGI(TAG_R,"Rola: ODBIORCA\n");

    twai_event_callbacks_t user_cbs = {
        .on_rx_done = twai_rx_cb,
    };
    ESP_ERROR_CHECK(twai_node_register_event_callbacks(node_hdl, &user_cbs, NULL));
    
    ESP_ERROR_CHECK(twai_node_enable(node_hdl));
    
    while(1) {
        // ESP_LOGI(TAG_R,"Rola: ODBIORCAdd\n");
        vTaskDelay(pdMS_TO_TICKS(1000)); }
}
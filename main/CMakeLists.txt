idf_component_register(
    SRCS "main.c"
    INCLUDE_DIRS "."
    REQUIRES lvgl esp_lcd usb_device nvs_flash spiffs freertos esp_timer
)

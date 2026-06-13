/**
 * @file sdio_protocol.h
 * @brief SDIO protocol definitions (Phase 3)
 */

#pragma once

/* Custom HCI commands over ESP-Hosted */
#define SDIO_CMD_SOFTAP_ENABLE    0x09
#define SDIO_CMD_SOFTAP_CONFIG    0x0A
#define SDIO_CMD_STA_CONNECT      0x0B
#define SDIO_CMD_STA_STATUS       0x0C
#define SDIO_CMD_HTTP_GET         0x0D
#define SDIO_CMD_STA_COUNT        0x0E

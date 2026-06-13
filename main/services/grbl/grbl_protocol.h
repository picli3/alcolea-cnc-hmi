/**
 * @file grbl_protocol.h
 * @brief GRBL protocol definitions
 */

#pragma once

/* GRBL command types */
#define GRBL_CMD_JOG           "$J="
#define GRBL_CMD_HOME         "$H"
#define GRBL_CMD_UNLOCK       "$X"
#define GRBL_CMD_RESET        "RESET"
#define GRBL_CMD_STATUS       "?"
#define GRBL_CMD_PARSER_STATE "$G"
#define GRBL_CMD_BUILD_INFO   "$I"
#define GRBL_CMD_FEED_HOLD    "!"
#define GRBL_CMD_CYCLE_START  "~"
#define GRBL_CMD_FEED_OVERRIDE "M50 P"
#define GRBL_CMD_SPINDLE_CW   "M3 S"
#define GRBL_CMD_SPINDLE_CCW  "M4 S"
#define GRBL_CMD_SPINDLE_OFF  "M5"
#define GRBL_CMD_COOLANT_MIST "M7"
#define GRBL_CMD_COOLANT_FLOOD "M8"
#define GRBL_CMD_COOLANT_OFF  "M9"
#define GRBL_CMD_SET_WCO      "G10 L20 P0 X0 Y0 Z0"

/* GRBL response patterns */
#define GRBL_STATUS_START      "<"
#define GRBL_STATUS_END        ">"
#define GRBL_RESPONSE_OK       "ok"
#define GRBL_RESPONSE_ERROR    "error:"
#define GRBL_MESSAGE_START     "[MSG:"
#define GRBL_PROBE_START       "[PRB:"
#define GRBL_GCODE_START       "[GC:"

/* GRBL status values */
#define GRBL_STATUS_STR_IDLE   "Idle"
#define GRBL_STATUS_STR_RUN    "Run"
#define GRBL_STATUS_STR_JOG    "Jog"
#define GRBL_STATUS_STR_HOLD   "Hold"
#define GRBL_STATUS_STR_ALARM  "Alarm"
#define GRBL_STATUS_STR_DOOR    "Door"
#define GRBL_STATUS_STR_CHECK  "Check"
#define GRBL_STATUS_STR_HOME   "Home"
#define GRBL_STATUS_STR_UNLOCK "Unlock"
#define GRBL_STATUS_STR_OFF    "Off"

/* Protocol timing */
#define GRBL_STATUS_QUERY_MS       100
#define GRBL_STATUS_QUERY_IDLE_MS  1000
#define GRBL_COMMAND_TIMEOUT_MS    5000
#define GRBL_WATCHDOG_TIMEOUT_MS   5000

/**
 * @file grbl_config.h
 * @brief GRBL configuration defaults
 */

#pragma once

/* Default GRBL settings */
#define GRBL_DEFAULT_BAUD_RATE     115200
#define GRBL_DEFAULT_STEPS_PER_MM_X  80.0f
#define GRBL_DEFAULT_STEPS_PER_MM_Y  80.0f
#define GRBL_DEFAULT_STEPS_PER_MM_Z  400.0f
#define GRBL_DEFAULT_MAX_FEED_X     1000.0f
#define GRBL_DEFAULT_MAX_FEED_Y     1000.0f
#define GRBL_DEFAULT_MAX_FEED_Z     200.0f
#define GRBL_DEFAULT_MAX_SPINDLE_RPM 12000
#define GRBL_DEFAULT_ACCELERATION   500.0f

/* Jog settings */
#define GRBL_JOG_RATE_LIMIT_HZ     20
#define GRBL_JOG_MIN_STEP_MM       0.1f
#define GRBL_JOG_MAX_STEP_MM       50.0f
#define GRBL_JOG_DEFAULT_STEP_MM   1.0f

/* Buffer sizes */
#define GRBL_PLANNER_BUFFER_SIZE    128
#define GRBL_RX_BUFFER_SIZE        256
#define GRBL_CMD_QUEUE_SIZE         32

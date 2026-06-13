# Alcolea CNC HMI

**CNC HMI Interface for LKS_TTC-450Ultra (TwoTrees)**  
**Hardware:** ESP32-P4 + 7" Touchscreen 1024x600 + ESP32-C6 (WiFi)  
**Framework:** ESP-IDF + LVGL 9.x

## Overview

This project implements a professional Human-Machine Interface (HMI) for CNC machines running GRBL 1.1h firmware. The interface is designed for industrial use with a dark theme, real-time status monitoring, and intuitive touch controls.

## Features

### Phase 1 (Foundation) - Current
- [x] ESP-IDF project structure with modular architecture
- [x] State machine: BOOT → IDLE → CONNECTED
- [x] GRBL Manager with USB CDC-ACM communication
- [x] GRBL response parser (status, ok, error)
- [x] LVGL 9.x UI with dark industrial theme
- [x] Home screen with status bar and position display
- [x] Axis position widgets (X, Y, Z)
- [x] Buffer usage monitoring

### Phase 2 (Jogging & Control) - Planned
- [ ] Jog pad with XYZ controls
- [ ] Step selector (0.1/1/10/50mm)
- [ ] Feed rate and spindle speed sliders
- [ ] HOME, ZERO, START, PAUSE, STOP buttons
- [ ] Emergency stop button

### Phase 3 (WiFi & File Upload) - Planned
- [ ] ESP-Hosted SDIO integration
- [ ] ESP32-C6 WiFi coprocessor
- [ ] Web-based G-code upload
- [ ] File browser with SPIFFS
- [ ] 2D toolpath preview

### Phase 4 (Streaming & Execution) - Planned
- [ ] G-code streaming with ring buffer
- [ ] Progress bar and ETA
- [ ] Pause/Resume/Abort
- [ ] Alarm detection and handling

## Hardware

| Component | Specification |
|-----------|---------------|
| Main MCU | ESP32-P4 Function EV Board |
| Display | 7" RGB LCD 1024x600 |
| Coprocessor | ESP32-C6 (WiFi) |
| CNC Controller | LKS_TTC-450Ultra (GRBL 1.1h) |

## Project Structure

```
alcolea-cnc-hmi/
├── main/
│   ├── main.c                 # Entry point
│   ├── app/                   # Application layer
│   ├── sm/                    # State machine
│   │   └── states/           # State implementations
│   ├── ui/                    # LVGL UI
│   │   ├── screens/           # Screen definitions
│   │   ├── widgets/           # Reusable widgets
│   │   └── styles/            # Theme definitions
│   └── services/
│       └── grbl/              # GRBL communication
├── components/                # External components
├── partitions.csv              # Partition table
└── sdkconfig.defaults         # ESP-IDF configuration
```

## Build & Flash

```bash
# Set up ESP-IDF environment
. /path/to/esp-idf/export.sh

# Build the project
idf.py build

# Flash to device
idf.py flash monitor
```

## UI Layout

```
┌─────────────────────────────────────────────────────────────┐
│  [Connection] [GRBL Status] X: 0.000 Y: 0.000 Z: 0.000     │
├─────────────────────────────────────────────┬───────────────┤
│                                             │   [JOG PAD]   │
│                                             │   X +/-       │
│           TOOLPATH PREVIEW                  │   Y +/-       │
│           (2D projection)                   │   Z +/-       │
│                                             │               │
│                                             │   [Step]      │
│                                             │   0.1/1/10/50 │
│                                             │               │
│                                             │   [Feed]      │
│                                             │   [Spindle]   │
├─────────────────────────────────────────────┼───────────────┤
│           [START] [PAUSE] [STOP] [HOME] [ZERO]             │
│                     [EMERGENCY STOP]                        │
└─────────────────────────────────────────────────────────────┘
```

## GRBL Protocol

The interface communicates with GRBL 1.1h via USB CDC-ACM at 115200 baud.

### Key Commands
- `?` - Status query
- `$G` - Parser state
- `$H` - Home cycle
- `$J=X...` - Jog
- `G10 L20 P0 X0 Y0 Z0` - Set work zero
- `!` - Feed hold
- `~` - Cycle start

### Status Response Format
```
<Idle|Run|Jog|Hold|Alarm,MPos:X,Y,Z,WPos:X,Y,Z,Buf:N,RX:N>
```

## License

MIT License

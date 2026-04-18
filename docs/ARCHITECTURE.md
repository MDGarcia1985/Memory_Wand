<!--
Author: Michael Garcia, M&E Design
License: MIT
Date: 2026-04-17
Contact: michael@mandedesign.studio
Website: https://mandedesign.studio
-->

# ARCHITECTURE.md

## Overview

This project is structured as a modular embedded system with a clear separation between:

* Input handling (buttons, debounce)
* Event processing (event queue, tasks)
* Output systems (LED, Audio)
* Hardware abstraction (pins, drivers)
* Data-driven subsystems (audio patterns via builder pipeline)

The system is designed to be:

* Hardware agnostic
* Non-blocking
* Scalable across microcontrollers
* Maintainable through strict module boundaries

---

## High-Level Flow

```
[ ButtonDebounce (lib) ]
            â†“
      btn_debounce
            â†“
        btn_event
            â†“
       event_queue
            â†“
          tasks
        /       \
     led       audio
                  â†“
           AudioOutput
                  â†“
           IAudioDriver
                  â†“
             Hardware
```

---

## Directory Structure

```
project/
â”œâ”€ docs/
â”‚  â”œâ”€ API.md
â”‚  â”œâ”€ ARCHITECTURE.md
â”‚  â”œâ”€ NM_CON.md
â”‚  â””â”€ COMMENTS.md
â”‚
â”œâ”€ include/
â”‚  â”œâ”€ headers.h
â”‚  â”œâ”€ config.h
â”‚  â”œâ”€ pins.h
â”‚  â”œâ”€ glbl_context.h
â”‚  â”œâ”€ event_queue.h
â”‚  â”œâ”€ tasks.h
â”‚  â”œâ”€ led.h
â”‚  â”œâ”€ btn_event.h
â”‚  â”œâ”€ btn_debounce.h
â”‚  â”‚
â”‚  â””â”€ audio/
â”‚     â”œâ”€ AudioTypes.h
â”‚     â”œâ”€ AudioConfig.h
â”‚     â”œâ”€ IAudioDriver.h
â”‚     â”œâ”€ AudioOutput.h
â”‚     â”œâ”€ GeneratedSoundData.h
â”‚     â””â”€ drivers/
â”‚        â”œâ”€ AudioDriver_ESP32.h
â”‚        â”œâ”€ AudioDriver_RP2350.h
â”‚        â””â”€ AudioDriver_Null.h
â”‚
â”œâ”€ src/
â”‚  â”œâ”€ main.cpp
â”‚  â”œâ”€ config.cpp
â”‚  â”œâ”€ pins.cpp
â”‚  â”œâ”€ glbl_context.cpp
â”‚  â”œâ”€ event_queue.cpp
â”‚  â”œâ”€ tasks.cpp
â”‚  â”œâ”€ led.cpp
â”‚  â”œâ”€ btn_event.cpp
â”‚  â”œâ”€ btn_debounce.cpp
â”‚  â”‚
â”‚  â””â”€ audio/
â”‚     â”œâ”€ AudioOutput.cpp
â”‚     â”œâ”€ GeneratedSoundData.cpp
â”‚     â””â”€ drivers/
â”‚        â”œâ”€ AudioDriver_ESP32.cpp
â”‚        â”œâ”€ AudioDriver_RP2350.cpp
â”‚        â””â”€ AudioDriver_Null.cpp
â”‚
â”œâ”€ audio_builder/
â”‚  â”œâ”€ data/
â”‚  â”‚  â”œâ”€ note_map.json
â”‚  â”‚  â”œâ”€ melodies.json
â”‚  â”‚  â””â”€ sounds.json
â”‚  â””â”€ scripts/
â”‚     â””â”€ build_audio.py
â”‚
â””â”€ lib/
   â””â”€ ButtonDebounce/
```

---

## Core Modules

### main.cpp

* System entry point
* Initializes all subsystems
* Runs main loop
* Calls update functions (non-blocking)

---

### headers.h

* Aggregated include entry point
* Provides a single import for project modules
* Does not contain logic

---

### config / pins

* Centralized hardware configuration
* Defines pin mappings and board-specific values
* No behavioral logic

---

### glbl_context

* Stores shared system state
* Used by tasks and subsystems
* Avoids uncontrolled global variables

---

### btn_debounce (Wrapper)

* Integrates external ButtonDebounce library
* Normalizes input behavior
* Converts raw input into stable signals

**NOTE:**
ButtonDebounce is an external dependency:
https://github.com/MDGarcia1985/buttonDebounce

---

### btn_event

* Converts button state into events
* Example:

  * PRESS
  * RELEASE
  * HOLD

---

### event_queue

* FIFO queue for system events
* Decouples input from processing
* Enables deterministic behavior

---

### tasks

* Core system logic
* Consumes events from event_queue
* Updates system state
* Triggers outputs (LED, Audio)

---

### led

* Handles LED outputs and patterns
* Non-blocking
* Stateless or lightly stateful

---

## Audio Subsystem

### Design Goals

* Hardware agnostic
* Data-driven
* Non-blocking
* Multi-channel (logical)
* Single or multi-output capable

---

### Architecture

```
SoundID â†’ SoundDef â†’ AudioOutput â†’ IAudioDriver â†’ Hardware
```

---

### AudioTypes

Defines core data structures:

* NoteEvent
* SoundDef
* AudioChannel

No hardware knowledge exists in these types.

---

### GeneratedSoundData

* Auto-generated from JSON via builder pipeline
* Contains:

  * sound tables
  * note patterns
  * metadata

No runtime parsing required.

---

### AudioOutput

* Core audio engine
* Manages:

  * channel state
  * timing
  * priority arbitration
* Non-blocking update loop
* Selects final output frequency

---

### IAudioDriver

Hardware abstraction interface:

* begin()
* writeFrequency()
* stop()
* setVolume() (optional)

No platform-specific logic above this layer.

---

### Audio Drivers

Platform-specific implementations:

* AudioDriver_ESP32 â†’ PWM via LEDC
* AudioDriver_RP2350 â†’ PWM slice control
* AudioDriver_Null â†’ no-op (testing)

---

### AudioConfig

Defines per-board configuration:

* speaker pin
* PWM channel
* resolution
* amp enable behavior

---

## Audio Builder Pipeline

### Purpose

Separates editable sound data from embedded runtime.

---

### Flow

```
JSON â†’ Python Builder â†’ Generated C++ â†’ Firmware
```

---

### Data Files

#### note_map.json

Defines note â†’ frequency mapping

#### melodies.json

Reusable note sequences

#### sounds.json

Maps:

* SoundID
* pattern
* channel
* priority
* loop behavior

---

### Output

* GeneratedSoundData.h
* GeneratedSoundData.cpp

---

## System Design Principles

### 1. Separation of Concerns

* Input, processing, and output are isolated
* Hardware is abstracted from logic

---

### 2. Data-Driven Behavior

* Sounds defined in data, not code
* Easily extendable without rewriting logic

---

### 3. Non-Blocking Execution

* No delays in main loop
* All timing handled via state + millis()

---

### 4. Hardware Abstraction

* Audio uses driver interface
* Board swap requires config/driver change only

---

### 5. Scalability

* Supports:

  * additional outputs
  * more channels
  * new hardware targets

---

### 6. Deterministic Behavior

* Event queue ensures predictable execution order
* No hidden side effects

---

## Future Extensions

* Multi-channel audio mixing
* Volume envelopes (ADSR)
* MIDI import â†’ builder pipeline
* SD card sound loading
* RTOS task scheduling
* Visual debugging tools

---

## Summary

This architecture provides a clean, scalable foundation for:

* Embedded interaction systems
* Data-driven audio playback
* Cross-platform portability

The system is intentionally designed to evolve from:

* simple tone playback
  to
* structured, multi-channel audio behavior

without requiring architectural rewrites.
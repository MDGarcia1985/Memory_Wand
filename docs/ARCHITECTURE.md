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
            ↓
      btn_debounce
            ↓
        btn_event
            ↓
       event_queue
            ↓
          tasks
        /       \
     led       audio
                  ↓
           AudioOutput
                  ↓
           IAudioDriver
                  ↓
             Hardware
```

---

## Directory Structure

```
project/
├─ docs/
│  ├─ API.md
│  ├─ ARCHITECTURE.md
│  ├─ NM_CON.md
│  └─ COMMENTS.md
│
├─ include/
│  ├─ headers.h
│  ├─ config.h
│  ├─ pins.h
│  ├─ glbl_context.h
│  ├─ event_queue.h
│  ├─ tasks.h
│  ├─ led.h
│  ├─ btn_event.h
│  ├─ btn_debounce.h
│  │
│  └─ audio/
│     ├─ AudioTypes.h
│     ├─ AudioConfig.h
│     ├─ IAudioDriver.h
│     ├─ AudioOutput.h
│     ├─ GeneratedSoundData.h
│     └─ drivers/
│        ├─ AudioDriver_ESP32.h
│        ├─ AudioDriver_RP2350.h
│        └─ AudioDriver_Null.h
│
├─ src/
│  ├─ main.cpp
│  ├─ config.cpp
│  ├─ pins.cpp
│  ├─ glbl_context.cpp
│  ├─ event_queue.cpp
│  ├─ tasks.cpp
│  ├─ led.cpp
│  ├─ btn_event.cpp
│  ├─ btn_debounce.cpp
│  │
│  └─ audio/
│     ├─ AudioOutput.cpp
│     ├─ GeneratedSoundData.cpp
│     └─ drivers/
│        ├─ AudioDriver_ESP32.cpp
│        ├─ AudioDriver_RP2350.cpp
│        └─ AudioDriver_Null.cpp
│
├─ audio_builder/
│  ├─ data/
│  │  ├─ note_map.json
│  │  ├─ melodies.json
│  │  └─ sounds.json
│  └─ scripts/
│     └─ build_audio.py
│
└─ lib/
   └─ ButtonDebounce/
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
SoundID → SoundDef → AudioOutput → IAudioDriver → Hardware
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

* AudioDriver_ESP32 → PWM via LEDC
* AudioDriver_RP2350 → PWM slice control
* AudioDriver_Null → no-op (testing)

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
JSON → Python Builder → Generated C++ → Firmware
```

---

### Data Files

#### note_map.json

Defines note → frequency mapping

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
* MIDI import → builder pipeline
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
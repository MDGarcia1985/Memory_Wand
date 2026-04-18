<!--
Author: Michael Garcia, M&E Design
License: MIT
Date: 2026-04-17
Contact: michael@mandedesign.studio
Website: https://mandedesign.studio
-->

# Memory Wand

Memory Wand is a modular embedded firmware project for a light-and-sound memory toy.

## Repository Role

This repository contains:

- Firmware modules for input, event processing, LED output, and audio output
- Board pin and behavior configuration
- A simple audio data pipeline for generated sound definitions
- Supporting project documentation for architecture, coding standards, and commenting policy

## Architecture Summary

The firmware follows a non-blocking event-driven flow:

`button GPIO -> debounce -> button events -> event queue -> tasks -> LED/audio outputs`

That separation is intentional so the project can evolve without tightly coupling hardware details to gameplay behavior.

## Core Directories

- `include/`: Public project headers and subsystem contracts
- `src/`: Runtime implementations and firmware entry point
- `include/audio/` and `src/audio/`: Audio abstraction, generated data access, and platform drivers
- `include/drivers/` and `src/drivers/`: Hardware-facing output drivers
- `lib/ButtonDebounce/`: Vendored button debounce dependency used by the input wrapper
- `audio_builder/`: Source data and scripts for generated sound content
- `docs/`: Architecture, standards, and development notes
- `archive/`: Older experimental or reference code

## Build

This project uses PlatformIO.

```bash
platformio run
```

## Versioning

The repository version is tracked in the root `VERSION` file.

Releases follow semantic versioning:

- Major: incompatible architectural or API changes
- Minor: new features added in a backward-compatible way
- Patch: fixes and non-breaking cleanup

The current major release line is defined by the first number in `VERSION`.

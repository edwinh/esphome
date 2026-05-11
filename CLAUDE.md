# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is an ESPHome smart home automation project containing multiple ESP32-based devices configured via YAML. The project integrates with Home Assistant and includes custom C++ and Python components for controlling KlikAanKlikUit (Dutch RF protocol) devices.

## Device Configurations

- **box.yaml** — ESP32-S3-Box with touchscreen display, audio input/output, and Home Assistant integration. Uses boxdisplay.h module for UI rendering.
- **watermeter.yaml** — Water meter reading device with sensor integration
- **kaku.yaml** — KlikAanKlikUit RF remote control configuration
- **boilerroom.yaml** — Boiler room monitoring device
- **living.yaml** — Living room smart home device

Each configuration contains:
- WiFi and Home Assistant API setup
- Sensor/text_sensor definitions (pulling from Home Assistant)
- Device-specific platform configurations
- OTA update support via `ota` section

## Custom Components

Located in `custom_components/`:

### kaku_old_switch
- **switch.py** — Python component schema and code generation for legacy KlikAanKlikUit devices
- **kaku_old_switch.cpp/.h** — C++ implementation for RF switch control
- **kaku_old.h** — Protocol telegram generation (ternary encoding)
- Config includes: address (A-D), unit number (1-16), pin assignment, pulse length

### kaku_dimmable_light
- **light.py** — Python component for dimmable light variant
- **light.cpp/.h** — C++ implementation with brightness control

## Key Modules

### modules/
- **kaku.h** — Generates KlikAanKlikUit bit sequences using ternary encoding (LOW/HIGH/FLOAT). Includes `KlikAanKlikUitOld` class with `on()`, `off()`, `buildSequence()` methods.
- **boxdisplay.h** — Display rendering for ESP32-S3-Box. Contains page setup, header rendering with time/icons, and touch event handling. Uses ESPHome's display drawing primitives.

## Development Setup

```bash
# Activate virtual environment
source venv/bin/activate

# Build configuration for a device
esphome compile box.yaml
esphome compile watermeter.yaml

# Upload to device (requires device connected)
esphome upload box.yaml
esphome upload watermeter.yaml

# Monitor device logs
esphome logs box.yaml

# Full workflow: validate → compile → upload → logs
esphome run box.yaml
```

## Build and Deployment

- Configurations compile to C++/binary for ESP32 boards via the ESPHome build system
- Custom components use ESPHome's Python config validation and C++ code generation APIs
- External components referenced via GitHub PR/branch sources (e.g., es8311 audio codec)
- OTA updates enable wireless firmware updates after initial deployment
- The `.esphome/` directory contains cached builds, manifests, and external component sources

## Configuration Patterns

- **Substitutions** at top of YAML for device name, friendly name reuse
- **External components** pull custom hardware support (audio codecs, sensors) from ESPHome repos
- **Home Assistant integration** via `homeassistant:` platform to read entity states (temperature, power consumption, timer status, light states)
- **Secrets** stored in `secrets.yaml` (not version controlled) for WiFi credentials, API passwords
- **Custom types** (C++ classes) instantiated via Python config `to_code()` functions

## Architecture Notes

- Custom switch/light components extend ESPHome base classes (`switch.Switch`, `light.Light`) and expose config validation via `CONFIG_SCHEMA`
- C++ includes in YAML (`includes:` key) allow direct header files for complex logic (KlikAanKlikUit encoding, display rendering)
- Pin definitions use ESPHome's pin schema validation and GPIO expression generation
- Time periods and durations validate via `cv.positive_time_period()` and convert to microseconds for C++

## Important Files to Know

- `.gitignore` — Ignores `__pycache__/`, `/.esphome/`, `secrets.yaml`, `venv`
- `pyvenv.cfg` — Python virtual environment configuration
- `docker-compose.yaml` — For containerized ESPHome development (if needed)

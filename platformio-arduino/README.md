# ESP32 + ILI9488 Display & XPT2046 Touch – Proof of Concept (POC)

This project is a **practical proof-of-concept** for working with an **ESP32 DevKit**, an **ILI9488 SPI TFT display**, and an **XPT2046 resistive touch controller**.

The goal is **not a fancy UI**, but a **reliable, reusable reference project** that proves:

- Display rendering works correctly
- Touch input works correctly
- Touch coordinates map accurately to the display
- Display + touch can share the SPI bus without conflicts

This project is intended to be reused as a **base display layer** for future projects (BLE, WiFi, MQTT, Firebase, dashboards, media, etc.).

---

## Hardware Used

- **ESP32 DevKit**
- **ILI9488 3.5" SPI TFT Display**
- **XPT2046 Touch Controller (on the display module)**
- 3.3V power (NO 5V)

---

## Power & Voltage (IMPORTANT)

- **VCC → 3.3V only**
- **LED (Backlight) → 3.3V**
- GND → GND

⚠️ **Do NOT use 5V** on this display. Many ILI9488 modules are NOT 5V tolerant.

---

## SPI Architecture

- **VSPI is used**
- Display and touch **share MOSI, MISO, and SCK**
- Each device has its **own CS pin**

This setup is intentional and proven to work.

---

## Pinout Mapping

### Display → ESP32

| Display Pin | ESP32 Pin             |
| ----------- | --------------------- |
| VCC         | 3.3V                  |
| GND         | GND                   |
| CS          | GPIO15                |
| RESET       | GPIO4                 |
| DC          | GPIO2                 |
| MOSI        | GPIO23                |
| SCK         | GPIO18                |
| MISO        | **NC** (not required) |
| LED         | 3.3V                  |

> MISO is intentionally left disconnected for the display to avoid SPI bus conflicts.

---

### Touch (XPT2046) → ESP32

| Touch Pin | ESP32 Pin |
| --------- | --------- |
| T_CS      | GPIO22    |
| T_CLK     | GPIO18    |
| T_DIN     | GPIO23    |
| T_DO      | GPIO19    |
| T_IRQ     | GPIO21    |
| VCC       | 3.3V      |
| GND       | GND       |

---

## Why This Pinout Works

- ESP32 VSPI default pins are used where possible
- Display MISO is not needed → prevents bus noise
- Touch MISO (GPIO19) is required and stable
- Separate CS lines guarantee no SPI collision

This is a **deliberate, tested configuration**, not guesswork.

---

## Software Stack

- **Framework:** Arduino (PlatformIO)
- **Display Library:** TFT_eSPI
- **Touch Library:** XPT2046_Touchscreen

---

## Project Structure

```
.
├── platformio.ini
├── src
│   └── main.cpp
├── lib
├── include
├── User_Setup.h   (TFT_eSPI configuration)
└── README.md
```

---

## Touch Calibration

This project includes a **manual 4-point calibration** process:

1. Red dots appear at screen corners
2. User touches each dot
3. Raw touch values are recorded
4. Linear scaling + offset is calculated
5. Touch coordinates map 1:1 with display pixels

This avoids hardcoded `map()` hacks and works across the full screen.

---

## What This Demo Does

- Initializes ILI9488 in **portrait mode**
- Initializes XPT2046 touch controller
- Performs **interactive calibration**
- Displays real-time **touch coordinates**
- Draws touch points exactly under the finger

If this works, your hardware, wiring, SPI bus, and libraries are correct.

---

## Known Edge Cases & Notes

- ❌ Using 5V WILL damage the display
- ❌ Forgetting `INPUT_PULL_UP` on T_IRQ causes ghost touches
- ❌ Sharing MISO from display may break touch readings
- ✔ Touch IRQ is optional but strongly recommended
- ✔ Rotation must match display rotation

---

---

## Next Steps (Optional)

- Button widgets
- Screen navigation
- BLE/WiFi status pages
- MQTT dashboards
- Sensor graphs

---

## License

MIT License — use it, fork it, improve it.

---

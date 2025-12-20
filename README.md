# Mork Bot

Mork Bot is an interactive, expressive robot built with an ESP32 Super Mini, an OLED display, a buzzer, and a variety of sensors. Mork displays animated eyes, plays cute sounds, and reacts to the environment and user interactions, making it feel alive and engaging. 

Youtube Demo: https://youtube.com/shorts/6Psl2-D04oY?si=jbgeNS3dgydOk-Tg

## Features

- **OLED Animated Eyes:** Mork displays a range of emotions and playful eye animations.
- **Touch Sensor:** Pet Mork to make it show love (with a cute melody).
- **MPU6050 (Accelerometer/Gyro):** Shake Mork to make it angry (with a harsh sound).
- **LDR (Light Sensor):** Mork sleeps when it's dark and wakes up when light returns.
- **Idle Animations:** Neutral and playful eye roll animations when idle.
- **Boredom Detection:** Mork gets bored if left alone for too long.
- **Buzzer:** Mork plays cute, interactive sounds for love, anger, giggle, sleep, wake, and dizzy states.
- **Giggle Animation:** Touch and hold to trigger a giggle animation with a playful sound.

## Hardware Used

- ESP32 Super Mini
- 0.96" I2C OLED Display (SSD1306)
- TTP223 Touch Module (or similar)
- MPU6050 Accelerometer/Gyro Module
- LDR (Light Dependent Resistor) + 10kΩ resistor
- Buzzer (passive or active)
- Jumper wires, breadboard, or custom PCB

## Wiring Overview

- **OLED Display:**
  - SDA → GPIO6
  - SCL → GPIO7
- **Touch Module:**
  - I/O → GPIO4
  - VCC → 3.3V
  - GND → GND
- **MPU6050:**
  - SDA → GPIO6 (shared with OLED)
  - SCL → GPIO7 (shared with OLED)
  - VCC → 3.3V
  - GND → GND
- **LDR:**
  - One end → 3.3V
  - Other end → GPIO1 (ADC1_CH0) + 10kΩ resistor to GND
- **Buzzer:**
  - + (long leg) → GPIO5
  - - (short leg) → GND

## Connection Diagram

![Connection Diagram](connection-diagram.png)

## How It Works

- **Idle:** Mork cycles through neutral and eye roll animations.
- **Touch:** Mork shows a love emotion and plays a cute melody.
- **Shake:** Mork shows an angry emotion and plays a harsh sound.
- **Darkness:** Mork gets dizzy, then goes to sleep and ignores other interactions until light returns.
- **Giggle:** Touch and hold to trigger a giggle animation and sound.
- **Boredom:** If not interacted with for a while, Mork shows a bored face.

## Customization

- You can customize the eye graphics in `eyes.h`.
- Adjust thresholds, timings, and sound melodies in the `.ino` file to suit your environment and preferences.

## Getting Started

1. Wire up the hardware as described above and in the connection diagram.
2. Install the required Arduino libraries:
  - Adafruit SSD1306
  - Adafruit GFX
  - Adafruit MPU6050
3. Open `mork-bot.ino` in Arduino IDE.
4. Select the correct ESP32 board and port.
5. Upload the code.
6. Enjoy your interactive, expressive Mork Bot!

## License

MIT License

---

Feel free to modify and expand Mork's capabilities with more sensors, sounds, or connectivity features!
# Mork-Bot

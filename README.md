# Ethernet Environment Sensor

Simple environment sensor connected via Ethernet, optionally powered using PoE 802.3af.

## Hardware

- Development board:
  - **WIZnet 55RP20-EVB-PICO** - Raspberry Pi Pico RP2040 and WIZnet W5500 in one chip on development board.
- PoE Module:
  - **WIZnet WIZPoE-P1** (not tested).
  - **WIZnet WIZPoE-S1** - Caution! Do not use stock module. This is a faulty product and it generates voltage spikes up to 10V-12V. WIZnet is aware of this problem. More on [WIZnet Forums](https://maker.wiznet.io/forum/15852). This module works after following modification:
    - Add 100Ω load resistor increase minimal load of the module (piggy-backed 3 x 300Ω in parallel to ceramic caps is a good option).
    - Replace 47µF electrolytic output capacitor with larger, low ESR capacitor, i.e. 330µF/10V/ESR 17mΩ.
    - Add 5.6V Zener diode on output (BZV55C5V6). Could not find any TVS for such a low voltage, and SGM62112-3.3 has absolute maximum input supply voltage at 6V.
- Main PCB - Coming soon. It still requires some work and tests.
- Sensors:
  - **Sensirion SHT31** - precise relative humidity and temperature sensor RH ±2%, T ±0.2°C. Compatible sensors: SHT30, SHT31, SHT33, SHT35, SHT85.
  - **Bosch Sensortec BMP388**. Compatible sensors: BMP388, BMP390.
  - **Sensirion SEN50**. Compatible sensors: SEN50, SEN54, SEN55.
- Mechanical parts:
  - **Kradex Z123A** - ventilated enclosure

## Software

- VSCode + Platformio
- Framework: Arduino
- [Raspberry Pi Pico Arduino core](https://github.com/earlephilhower/arduino-pico)
- Libraries:
  - [Sensirion Core](https://github.com/Sensirion/arduino-core)
  - [Sensirion I2C SHT3x](https://github.com/Sensirion/arduino-i2c-sht3x)
  - [Sensirion I2C SEN5X](https://github.com/Sensirion/arduino-i2c-sen5x)
  - [Adafruit BusIO](https://github.com/adafruit/Adafruit_BusIO)
  - [Adafruit Unified Sensor](https://github.com/adafruit/Adafruit_Sensor)
  - [Adafruit BMP3XX Library](https://github.com/adafruit/Adafruit_BMP3XX)

## Data

Data is available in jason format via TCP/IP.
# Ethernet Environment Sensor

Simple environment sensor connected via Ethernet (preferable using PoE)

## Hardware

- Development board:
  - **WIZnet 55RP20-EVB-PICO** - Raspberry Pi Pico RP2040 and WIZnet W5500 in one chip on development board.
  - **WIZnet WIZPoE-P1** (optional PoE PD module).
- Sensors:
  - **Sensirion SHT31** - precise relative humidity and temperature sensor RH ±2%, T  ±0.2°C. Compatible sensors: SHT30, SHT31, SHT33, SHT35, SHT85.
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

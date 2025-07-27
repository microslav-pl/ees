#pragma once

// Log levels:  ERROR, WARNING, INFO, DEBUG
#define CURRENT_LOG_LEVEL       INFO

#define SERIAL_BAUD_RATE        115200
#define I2C_SDA                 4u
#define I2C_SCL                 5u

// Bosch Sensortec BMP388 I²C Address: 0x76 or 0x77
#define I2C_ADD_BMP388          0x77
// Sensirion SEN5x I²C Address: 0x69
#define I2C_ADD_SEN5x           0x69

// Sensors read time intervals (in ms)
#define SHT3X_READ_INTERVAL     1000
#define BMP388_READ_INTERVAL    1000
#define SEN3X_READ_INTERVAL     1000

// Median array size (how many readings in buffer)
#define SHT3X_MEDIAN_ARRAY      7
#define BMP388_MEDIAN_ARRAY     7
#define SEN5X_MEDIAN_ARRAY      7

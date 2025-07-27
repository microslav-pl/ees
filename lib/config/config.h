#pragma once

// Log levels:  ERROR, WARNING, INFO, DEBUG
#define CURRENT_LOG_LEVEL       INFO

#define SERIAL_BAUD_RATE        115200

#define I2C_SDA                 4u
#define I2C_SCL                 5u

// Sensirion SEN5x I²C Address: 0x69
#define I2C_ADD_SEN5x           0x69

#define SHT3X_READ_INTERVAL     1000
#define SHT3X_MEDIAN_ARRAY      7
#define SHT3X_READ_RETRY        3

#define BMP3XX_READ_INTERVAL    1000
#define BMP3XX_MEDIAN_ARRAY     7
#define BMP3XX_READ_RETRY       3

#define SEN5X_READ_INTERVAL     1000
#define SEN5X_MEDIAN_ARRAY      7
#define SEN5X_READ_RETRY        3

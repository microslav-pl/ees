#pragma once

/**
 * @brief Searches for known I²C addressess of Bosch Sensortec BMP3xx (0x76, 0x77).
 * @note If device is not found, function logs error and returns 0x77 address as fallback.
 * @return Address of BMP3xx connected to I²C bus.
 */
uint8_t findBmp3xxAddress();

/**
 * @brief Initializes Sensirion BMP3xx.
 * @return Status of initialization.
 * @retval true Initialization was successful.
 * @retval false Initialization failed.
 */
bool sensorInitBmp3xx();

/**
 * @brief Reads data from Sensirion BMP3xx.
 * @return Status of operation.
 * @retval true Read was successful.
 * @retval false Read failed.
 */
bool sensorReadBmp3xx();

/**
 * @brief Simple scheduler for reading Sensirion BMP3xx.
 */
void scheduleReadBmp3xx();

/**
 * @brief Returns median temperature form ring buffer.
 * @return Temperature (°C).
 */
float medianTemperatureBmp3xx();

/**
 * @brief Returns median atmospheric pressure form ring buffer.
 * @return Atmospheric pressure (Pa).
 */
float medianPressureBmp3xx();

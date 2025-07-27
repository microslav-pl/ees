#pragma once

/**
 * @brief Searches for known I²C addressess of Sensirion SHT3x (0x44, 0x45).
 * @note If device is not found, function logs error and returns 0x44 address as fallback.
 * @return Address of SHT3x connected to I²C bus.
 */
uint8_t findSht3xAddress();

/**
 * @brief Initializes Sensirion SHT3x.
 * @return Status of initialization.
 * @retval true Initialization was successful.
 * @retval false Initialization failed.
 */
bool sensorInitSht3x();

/**
 * @brief Reads data from Sensirion SHT3x.
 * @return Status of operation.
 * @retval true Read was successful.
 * @retval false Read failed.
 */
bool sensorReadSht3x();

/**
 * @brief Simple scheduler for reading Sensirion SHT3x.
 */
void scheduleReadSht3x();

/**
 * @brief Returns median temperature form ring buffer.
 * @return Temperature (°C).
 */
float medianTemperatureSht3x();

/**
 * @brief Returns median relative humidity form ring buffer.
 * @return Relative humidity (RH%).
 */
float medianRelHumiditySht3x();

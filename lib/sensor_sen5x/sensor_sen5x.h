#pragma once

#include <StreamString.h>

/**
 * @brief Initializes Sensirion SEN5x.
 * @return Status of initialization.
 * @retval true Initialization was successful.
 * @retval false Initialization failed.
 */
bool sensorInitSen5x();

/**
 * @brief Reads data from Sensirion SEN5x.
 * @return Status of operation.
 * @retval true Read was successful.
 * @retval false Read failed.
 */
bool sensorReadSen5x();

/**
 * @brief Simple scheduler for reading Sensirion SEN5x.
 */
void scheduleReadSen5x();

/**
 * @brief Returns median PM 1.0 form ring buffer.
 * @return PM 1.0 (µg/m³).
 */
float medianPM1Sen5x();

/**
 * @brief Returns median PM 2.5 form ring buffer.
 * @return PM 2.5 (µg/m³).
 */
float medianPM2p5Sen5x();

/**
 * @brief Returns median PM 4.0 form ring buffer.
 * @return PM 4.0 (µg/m³).
 */
float medianPM4Sen5x();

/**
 * @brief Returns median PM 10.0 form ring buffer.
 * @return PM 10.0 (µg/m³).
 */
float medianPM10Sen5x();

/**
 * @brief Returns median relative humidity form ring buffer.
 * @return Relative humidity (RH%).
 */
float medianRelHumiditySen5x();

/**
 * @brief Returns median temperature form ring buffer.
 * @return Temperature (°C).
 */
float medianTemperatureSen5x();

/**
 * @brief Returns median VOC Index form ring buffer.
 * @return VOC Index.
 */
float medianVocIndexSen5x();

/**
 * @brief Returns median NOx Index form ring buffer.
 * @return NOx Index.
 */
float medianNoxIndexSen5x();

/**
 * @brief Appends string with json data from sensor.
 * @param[in] json String to append.
 */
void appendJsonSen5x(StreamString& json);

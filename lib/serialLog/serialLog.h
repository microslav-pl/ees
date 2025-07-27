#pragma once

enum SerialLogLevel {
    ERROR   = 1u,
    WARNING = 2u,
    INFO    = 3u,
    DEBUG   = 4u
};

/**
 * @brief Return string representing log level.
 * @param[in] level     Log level
 * @return              String with log level name
 */
const char* serialLogLevelToString(SerialLogLevel level);

/**
 * @brief Logs message to serial port.
 * @note  If you need new line, don't forget to add one.
 * @param[in] level     Log level of logged message
 * @param[in] format    Formatted text and arguments (printf style)
 */
void serialLog(SerialLogLevel level, const char* format, ...);

/**
 * @brief Logs simple dot to serial
 * @param[in] level     Log level of logged message
 */
void serialLogProgress(SerialLogLevel level);

/**
 * @brief Logs new line
 * @param[in] level     Log level of logged message
 */
void serialLogNewLine(SerialLogLevel level);

/**
 * @brief Prints message of the day to serial.
 */
void serialLogDisplayMotd();

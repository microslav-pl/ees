#include <Arduino.h>
#include <cstdarg>

#include <config.h>
#include "serialLog.h"

static SerialLogLevel currentLogLevel = CURRENT_LOG_LEVEL;

const char* serialLogLevelToString(SerialLogLevel level) {
    switch (level) {
        case ERROR:   return "[ ERROR   ]";
        case WARNING: return "[ WARNING ]";
        case INFO:    return "[ INFO    ]";
        case DEBUG:   return "[ DEBUG   ]";
        default:      return "[ UNKNOWN ]";
    }
}

void serialLog(SerialLogLevel level, const char* format, ...) {
    if (level <= currentLogLevel) {
        char buffer[128];
        va_list args;
        va_start(args, format);
        vsnprintf(buffer, sizeof(buffer), format, args);
        va_end(args);
        Serial.printf("%s %s", serialLogLevelToString(level), buffer);
    }
}

void serialLogProgress(SerialLogLevel level) {
    if (level <= currentLogLevel) {
        Serial.print(".");
    }
}

void serialLogNewLine(SerialLogLevel level) {
    if (level <= currentLogLevel) {
        Serial.println("");
    }
}

void serialLogDisplayMotd() {
    Serial.println("");
    Serial.println("===========================================");
    Serial.println("        Ethernet Environment Sensor");
    Serial.println("===========================================");
    Serial.println("        github.com/microslav-pl/ees");
    Serial.println("===========================================");
    Serial.println("");
}

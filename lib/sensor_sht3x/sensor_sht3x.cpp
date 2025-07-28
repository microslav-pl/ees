#include <Arduino.h>
#include <SensirionI2cSht3x.h>
#include <Wire.h>

#include <config.h>
#include <median.h>
#include <serialLog.h>

#include "sensor_sht3x.h"

SensirionI2cSht3x sht;

static char errorMessage[64];
static uint16_t error;

static float sht3x_T_values[SHT3X_MEDIAN_ARRAY] = {0.0};
static float sht3x_RH_values[SHT3X_MEDIAN_ARRAY] = {0.0};
static uint32_t sht3x_T_index = 0;
static uint32_t sht3x_RH_index = 0;

static uint32_t sht3x_lastMeasurementTime = 0;

uint8_t findSht3xAddress() {
    serialLog(INFO, "Looking for SHT3x...\n");
    Wire.beginTransmission(0x44);
    if (Wire.endTransmission() == 0) {
        serialLog(INFO, "Found I²C device at address 0x44.\n");
        return 0x44;
    }
    Wire.beginTransmission(0x45);
    if (Wire.endTransmission() == 0) {
        serialLog(INFO, "Found I²C device at address 0x45.\n");
        return 0x45;
    }
    serialLog(ERROR, "No I²C device was found at 0x44 or 0x45.\n");
    // Fallback to default
    return 0x44;
}

bool sensorInitSht3x() {
    uint16_t statusRegister = 0u;

    sht.begin(Wire, findSht3xAddress());
    sht.stopMeasurement();
    delay(1);
    sht.softReset();
    delay(100);
  
    error = sht.readStatusRegister(statusRegister);
    if (error != NO_ERROR) {
        serialLog(ERROR, "Error trying to execute readStatusRegister():\n");
        errorToString(error, errorMessage, sizeof errorMessage);
        serialLog(ERROR, "%s\n", errorMessage);
        return false;
    }

    serialLog(DEBUG, "SHT Status register: 0x%04X\n", statusRegister);
    error = sht.startPeriodicMeasurement(REPEATABILITY_HIGH, MPS_TWO_PER_SECOND);

    if (error != NO_ERROR) {
        serialLog(ERROR, "Error trying to execute startPeriodicMeasurement():\n");
        errorToString(error, errorMessage, sizeof errorMessage);
        serialLog(ERROR, "%s\n", errorMessage);
        return false;
    }
    serialLog(INFO, "Sensirion SHT3x Initialized.\n");
    return true;
}

bool sensorReadSht3x() {
    float temperature = 0.0;
    float relHumidity = 0.0;

    error = sht.blockingReadMeasurement(temperature, relHumidity);
    if (error != NO_ERROR) {
        serialLog(ERROR, "Error trying to execute blockingReadMeasurement():\n");
        errorToString(error, errorMessage, sizeof errorMessage);
        serialLog(ERROR, "%s\n", errorMessage);
        return false;
    }
    serialLog(DEBUG, "Read from SHT3x: T: %6.2f, RH: %6.2f\n", temperature, relHumidity);

    sht3x_T_values[(sht3x_T_index++ % SHT3X_MEDIAN_ARRAY)] = temperature;
    sht3x_RH_values[(sht3x_RH_index++ % SHT3X_MEDIAN_ARRAY)] = relHumidity;

    return true;
}

void scheduleReadSht3x() {
    if (millis() - sht3x_lastMeasurementTime < SHT3X_READ_INTERVAL) {
        return;
    }
    sht3x_lastMeasurementTime = millis();    
    for (uint8_t i=0; i < SHT3X_READ_RETRY; i++) {
        if (sensorReadSht3x()) {
            break;
        }
    }
}

float medianTemperatureSht3x() {
    return medianFromArray(sht3x_T_values, SHT3X_MEDIAN_ARRAY);
}

float medianRelHumiditySht3x() {
    return medianFromArray(sht3x_RH_values, SHT3X_MEDIAN_ARRAY);
}

void appendJsonSht3x(StreamString& json) {
    json.printf(
        "  \"sht3x\": {\n"
        "    \"temperature\": %.1f,\n"
        "    \"humidity\": %.1f\n"
        "  }",
    medianTemperatureSht3x(), medianRelHumiditySht3x());
}

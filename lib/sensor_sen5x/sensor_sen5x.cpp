#include <Arduino.h>
#include <SensirionI2CSen5x.h>
#include <Wire.h>

#include <config.h>
#include <median.h>
#include <serialLog.h>

#include "sensor_sen5x.h"

SensirionI2CSen5x sen5x;

static char errorMessage[256];
static uint16_t error;

static float sen5x_pm1_values[SEN5X_MEDIAN_ARRAY] = {0.0};
static float sen5x_pm2p5_values[SEN5X_MEDIAN_ARRAY] = {0.0};
static float sen5x_pm4_values[SEN5X_MEDIAN_ARRAY] = {0.0};
static float sen5x_pm10_values[SEN5X_MEDIAN_ARRAY] = {0.0};
static float sen5x_rh_values[SEN5X_MEDIAN_ARRAY] = {0.0};
static float sen5x_t_values[SEN5X_MEDIAN_ARRAY] = {0.0};
static float sen5x_voc_values[SEN5X_MEDIAN_ARRAY] = {0.0};
static float sen5x_nox_values[SEN5X_MEDIAN_ARRAY] = {0.0};

static uint32_t sen5x_pm1_index = 0;
static uint32_t sen5x_pm2p5_index = 0;
static uint32_t sen5x_pm4_index = 0;
static uint32_t sen5x_pm10_index = 0;
static uint32_t sen5x_rh_index = 0;
static uint32_t sen5x_t_index = 0;
static uint32_t sen5x_voc_index = 0;
static uint32_t sen5x_nox_index = 0;

static uint32_t sen5x_lastMeasurementTime = 0;
static uint32_t sen5x_lastMeasurementDuration = 0;
static uint32_t sen5x_readsTotal = 0;
static uint32_t sen5x_readsFailed = 0;

bool sensorInitSen5x() {
    sen5x.begin(Wire);
    error = sen5x.deviceReset();
    if (error) {
        serialLog(ERROR, "Error trying to execute deviceReset()\n");
        errorToString(error, errorMessage, sizeof errorMessage);
        serialLog(ERROR, "%s\n", errorMessage);
        return false;
    }
    error = sen5x.startMeasurement();
    if (error) {
        serialLog(ERROR, "Error trying to execute startMeasurement()\n");
        errorToString(error, errorMessage, sizeof errorMessage);
        serialLog(ERROR, "%s\n", errorMessage);
        return false;
    }
    serialLog(INFO, "Sensirion SEN5x Initialized.\n");
    return true;
}

bool sensorReadSen5x() {
    float massConcentrationPm1p0;
    float massConcentrationPm2p5;
    float massConcentrationPm4p0;
    float massConcentrationPm10p0;
    float ambientHumidity;
    float ambientTemperature;
    float vocIndex;
    float noxIndex;
    uint32_t sen5x_measurementStartTime;

    sen5x_readsTotal++;
    sen5x_measurementStartTime = millis();
    error = sen5x.readMeasuredValues(
        massConcentrationPm1p0, massConcentrationPm2p5, massConcentrationPm4p0,
        massConcentrationPm10p0, ambientHumidity, ambientTemperature, vocIndex,
        noxIndex);
    sen5x_lastMeasurementDuration = millis() - sen5x_measurementStartTime;

    if (error) {
        serialLog(ERROR, "Error trying to execute readMeasuredValues().\n");
        errorToString(error, errorMessage, sizeof errorMessage);
        serialLog(ERROR, "%s\n", errorMessage);
        sen5x_readsFailed++;
        return false;
    }

    serialLog(DEBUG, "Read from SEN5x: PM1: %6.2f, PM2.5: %6.2f, PM4; %6.2f, PM10: %6.2f\n",
        massConcentrationPm1p0, massConcentrationPm2p5, massConcentrationPm4p0, massConcentrationPm10p0);
    serialLog(DEBUG, "Read from SEN5x: RH: %6.2f, T: %6.2f, VOC; %6.2f, NOx: %6.2f\n",
        ambientHumidity, ambientTemperature, vocIndex, noxIndex);

    sen5x_pm1_values[(sen5x_pm1_index++ % SEN5X_MEDIAN_ARRAY)] = massConcentrationPm1p0;
    sen5x_pm2p5_values[(sen5x_pm2p5_index++ % SEN5X_MEDIAN_ARRAY)] = massConcentrationPm2p5;
    sen5x_pm4_values[(sen5x_pm4_index++ % SEN5X_MEDIAN_ARRAY)] = massConcentrationPm4p0;
    sen5x_pm10_values[(sen5x_pm10_index++ % SEN5X_MEDIAN_ARRAY)] = massConcentrationPm10p0;

    // RH, T and VOC Index present only in SEN54 and SEN55
    if (!isnan(ambientHumidity)) {
        sen5x_rh_values[(sen5x_rh_index++ % SEN5X_MEDIAN_ARRAY)] = ambientHumidity;
    }
    if (!isnan(ambientTemperature)) {
        sen5x_t_values[(sen5x_t_index++ % SEN5X_MEDIAN_ARRAY)] = ambientTemperature;
    }
    if (!isnan(vocIndex)) {
        sen5x_voc_values[(sen5x_voc_index++ % SEN5X_MEDIAN_ARRAY)] = vocIndex;
    }
    // NOx Index present only in SEN55
    if (!isnan(noxIndex)) {
        sen5x_nox_values[(sen5x_nox_index++ % SEN5X_MEDIAN_ARRAY)] = noxIndex;
    }
    return true;
}

void scheduleReadSen5x() {
    if (millis() - sen5x_lastMeasurementTime < SEN5X_READ_INTERVAL) {
        return;
    }
    sen5x_lastMeasurementTime = millis();    
    for (uint8_t i=0; i < SEN5X_READ_RETRY; i++) {
        if (sensorReadSen5x()) {
            break;
        }
    }
}

float medianPM1Sen5x() {
    return medianFromArray(sen5x_pm1_values, SEN5X_MEDIAN_ARRAY);
}

float medianPM2p5Sen5x() {
    return medianFromArray(sen5x_pm2p5_values, SEN5X_MEDIAN_ARRAY);
}

float medianPM4Sen5x() {
    return medianFromArray(sen5x_pm4_values, SEN5X_MEDIAN_ARRAY);
}

float medianPM10Sen5x() {
    return medianFromArray(sen5x_pm10_values, SEN5X_MEDIAN_ARRAY);
}

float medianRelHumiditySen5x() {
    return medianFromArray(sen5x_rh_values, SEN5X_MEDIAN_ARRAY);
}

float medianTemperatureSen5x() {
    return medianFromArray(sen5x_t_values, SEN5X_MEDIAN_ARRAY);
}

float medianVocIndexSen5x() {
    return medianFromArray(sen5x_voc_values, SEN5X_MEDIAN_ARRAY);
}

float medianNoxIndexSen5x() {
    return medianFromArray(sen5x_nox_values, SEN5X_MEDIAN_ARRAY);
}

void appendJsonSen5x(StreamString& json) {
    json.printf(
        "  \"sen5x\": {\n"
        "    \"pm1_0\": %.1f,\n"
        "    \"pm2_5\": %.1f,\n"
        "    \"pm4_0\": %.1f,\n"
        "    \"pm10_0\": %.1f,\n"
        "    \"humidity\": %.1f,\n"
        "    \"temperature\": %.1f,\n"
        "    \"voc\": %.1f,\n"
        "    \"nox\": %.1f,\n"
        "    \"metadata\": {\n"
        "      \"timestamp\": %d,\n"
        "      \"duration\": %d,\n"
        "      \"reads\": %d,\n"
        "      \"failed\": %d\n"
        "    }\n"
        "  }",
        medianPM1Sen5x(), medianPM2p5Sen5x(), medianPM4Sen5x(), medianPM10Sen5x(),
        medianRelHumiditySen5x(), medianTemperatureSen5x(), medianVocIndexSen5x(),
        medianNoxIndexSen5x(), sen5x_lastMeasurementTime, sen5x_lastMeasurementDuration, sen5x_readsTotal, sen5x_readsFailed);
}

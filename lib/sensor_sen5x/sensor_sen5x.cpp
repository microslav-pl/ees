#include <Arduino.h>
#include <SensirionI2CSen5x.h>
#include <Wire.h>

#include <config.h>
#include <median.h>
#include <serialLog.h>

#include "sensor_sen5x.h"

static float sen5x_pm1_values[SEN5X_MEDIAN_ARRAY] = {0.0};
static float sen5x_pm2p5_values[SEN5X_MEDIAN_ARRAY] = {0.0};
static float sen5x_pm4_values[SEN5X_MEDIAN_ARRAY] = {0.0};
static float sen5x_pm10_values[SEN5X_MEDIAN_ARRAY] = {0.0};

static uint32_t sen5x_pm1_index = 0;
static uint32_t sen5x_pm2p5_index = 0;
static uint32_t sen5x_pm4_index = 0;
static uint32_t sen5x_pm10_index = 0;

static uint32_t sen5x_lastMeasurementTime = 0;

bool sensorInitSen5x() {
    return true;
}

bool sensorReadSen5x() {
    sen5x_pm1_values[(sen5x_pm1_index++ % SEN5X_MEDIAN_ARRAY)] = 1.0;
    sen5x_pm2p5_values[(sen5x_pm2p5_index++ % SEN5X_MEDIAN_ARRAY)] = 2.5;
    sen5x_pm4_values[(sen5x_pm4_index++ % SEN5X_MEDIAN_ARRAY)] = 4.0;
    sen5x_pm10_values[(sen5x_pm10_index++ % SEN5X_MEDIAN_ARRAY)] = 10.0;
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
    return medianFromArray(sen5x_pm1_values, SHT3X_MEDIAN_ARRAY);
}

float medianPM2_5Sen5x() {
    return medianFromArray(sen5x_pm2p5_values, SHT3X_MEDIAN_ARRAY);
}

float medianPM4Sen5x() {
    return medianFromArray(sen5x_pm4_values, SHT3X_MEDIAN_ARRAY);
}

float medianPM10Sen5x() {
    return medianFromArray(sen5x_pm10_values, SHT3X_MEDIAN_ARRAY);
}
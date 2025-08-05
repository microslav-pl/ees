#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP3XX.h>
#include <Wire.h>

#include <config.h>
#include <median.h>
#include <serialLog.h>

#include "sensor_bmp3xx.h"

Adafruit_BMP3XX bmp3xx;

static float bmp3xx_T_values[BMP3XX_MEDIAN_ARRAY] = {0.0};
static float bmp3xx_P_values[BMP3XX_MEDIAN_ARRAY] = {0.0};
static uint32_t bmp3xx_T_index = 0;
static uint32_t bmp3xx_P_index = 0;

static uint32_t bmp3xx_lastMeasurementTime = 0;
static uint32_t bmp3xx_lastMeasurementDuration = 0;
static uint32_t bmp3xx_readsTotal = 0;
static uint32_t bmp3xx_readsFailed = 0;

uint8_t findBmp3xxAddress() {
    serialLog(DEBUG, "Looking for BMP3xx...\n");
    Wire.beginTransmission(0x77);
    if (Wire.endTransmission() == 0) {
        serialLog(DEBUG, "Found I²C device at address 0x77.\n");
        return 0x77;
    }
    Wire.beginTransmission(0x76);
    if (Wire.endTransmission() == 0) {
        serialLog(DEBUG, "Found I²C device at address 0x76.\n");
        return 0x76;
    }
    serialLog(ERROR, "No I²C device was found at 0x77 or 0x76.\n");
    // Fallback to default
    return 0x77;
}

bool sensorInitBmp3xx() {
    if (!bmp3xx.begin_I2C(findBmp3xxAddress())) {
        serialLog(ERROR, "Could not initialize BMP3xx.\n");
        return false;
    }
    bmp3xx.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
    bmp3xx.setPressureOversampling(BMP3_OVERSAMPLING_4X);
    bmp3xx.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
    bmp3xx.setOutputDataRate(BMP3_ODR_50_HZ);
    serialLog(INFO, "Bosch Sensortec BMP3xx Initialized.\n");
    return true;
}

bool sensorReadBmp3xx() {
    float temperature = 0.0;
    float pressure = 0.0;
    bool status;
    uint32_t bmp3xx_measurementStartTime;

    bmp3xx_readsTotal++;
    bmp3xx_measurementStartTime = millis();
    status = bmp3xx.performReading();
    bmp3xx_lastMeasurementDuration = millis() - bmp3xx_measurementStartTime;

    if (status == false) {
        serialLog(ERROR, "Could not read values from BMP3xx.\n");
        bmp3xx_readsFailed++;
        return false;
    }
    temperature = (float)bmp3xx.temperature;
    pressure = (float)bmp3xx.pressure;
    serialLog(DEBUG, "Read from BMP3xx: T: %6.2f, P: %6.2f\n", temperature, pressure);
    bmp3xx_T_values[(bmp3xx_T_index++ % BMP3XX_MEDIAN_ARRAY)] = temperature;
    bmp3xx_P_values[(bmp3xx_P_index++ % BMP3XX_MEDIAN_ARRAY)] = pressure;
    return true;
}

void scheduleReadBmp3xx() {
    if (millis() - bmp3xx_lastMeasurementTime < BMP3XX_READ_INTERVAL) {
        return;
    }
    bmp3xx_lastMeasurementTime = millis();    
    for (uint8_t i=0; i < BMP3XX_READ_RETRY; i++) {
        if (sensorReadBmp3xx()) {
            break;
        }
    }
}

float medianTemperatureBmp3xx() {
    return medianFromArray(bmp3xx_T_values, BMP3XX_MEDIAN_ARRAY);
}

float medianPressureBmp3xx() {
    return medianFromArray(bmp3xx_P_values, BMP3XX_MEDIAN_ARRAY);
}


void appendJsonBmp3xx(StreamString& json) {
    json.printf(
        "  \"bmp3xx\": {\n"
        "    \"temperature\": %.1f,\n"
        "    \"pressure\": %.1f,\n"
        "    \"metadata\": {\n"
        "      \"timestamp\": %d,\n"
        "      \"duration\": %d,\n"
        "      \"reads\": %d,\n"
        "      \"failed\": %d\n"
        "    }\n"
        "  }",
    medianTemperatureBmp3xx(), medianPressureBmp3xx(), bmp3xx_lastMeasurementTime, bmp3xx_lastMeasurementDuration, bmp3xx_readsTotal, bmp3xx_readsFailed);
}

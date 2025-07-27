#pragma once

uint8_t findSht3xAddress();

bool sensorInitSht3x();

bool sensorReadSht3x();

void scheduleReadSht3x();

float medianTemperatureSht3x();

float medianRelHumiditySht3x();

#include <Arduino.h>
#include <W55RP20lwIP.h>
#include <Wire.h>

#include <median.h>
#include <sensor_sht3x.h>
#include <sensor_sen5x.h>
#include <sensor_bmp3xx.h>
#include <serialLog.h>

#include <config.h>

Wiznet55rp20lwIP eth(20);

void connect_to_ethernet() {
    if (!eth.begin()) {
        serialLog(ERROR, "No wired Ethernet hardware detected.\n");
        while (1) {
        delay(1000);
        }
    }
    serialLog(INFO, "Connecting");
    while (!eth.connected()) {
        serialLogProgress(INFO);
        delay(500);
    }
    serialLogNewLine(INFO);

    serialLog(INFO, "Ethernet connected\n");
    serialLog(INFO, "IP address: %s\n", eth.localIP().toString().c_str());

    // After connecting, you can check uplink: (eth.linkStatus() == 1)
}

void setup() {
    bool setupFailed = false;
    
    Serial.begin(SERIAL_BAUD_RATE);
    delay(2000);
    
    Wire.setSDA(I2C_SDA);
    Wire.setSCL(I2C_SCL);
    Wire.begin();

    serialLogDisplayMotd();
    connect_to_ethernet();

    if (!sensorInitSht3x()) setupFailed = true;
    if (!sensorInitSen5x()) setupFailed = true;
    if (!sensorInitBmp3xx()) setupFailed = true;

    if (setupFailed) {
      serialLog(ERROR, "Setup failed! Reboot in 10 seconds: ");
      for (uint8_t i = 10; i > 0; i--) {
          serialLogProgress(ERROR);
          delay(1000);
      }
      rp2040.reboot();
    }
}

void loop() {
    scheduleReadSht3x();
    scheduleReadSen5x();
    scheduleReadBmp3xx();

    serialLog(INFO, "Median T: %6.2f, RH: %6.2f\n", medianTemperatureSht3x(), medianRelHumiditySht3x());
    serialLog(INFO, "Median T: %6.2f, P:  %6.2f\n", medianTemperatureBmp3xx(), medianPressureBmp3xx());
    //serialLog(INFO, "Median PM1: %5.1f, PM2.5: %5.1f, PM4; %5.1f, PM10: %5.1f\n", medianPM1Sen5x(), medianPM2_5Sen5x(), medianPM4Sen5x(), medianPM10Sen5x());
    delay(2000);
}









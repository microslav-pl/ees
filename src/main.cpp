#include <Arduino.h>
//#include <W55RP20lwIP.h>
#include <Wire.h>

#include <config.h>
#include <ethernet_webserver.h>
#include <median.h>
#include <sensor_sht3x.h>
#include <sensor_sen5x.h>
#include <sensor_bmp3xx.h>
#include <serialLog.h>

void setup() {
    bool setupFailed = false;
    
    Serial.begin(SERIAL_BAUD_RATE);
    delay(2000);
    
    Wire.setSDA(I2C_SDA);
    Wire.setSCL(I2C_SCL);
    Wire.begin();

    serialLogDisplayMotd();
    connectToEthernet();

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
    
    startWebServer();
}

void loop() {
    scheduleReadSht3x();
    scheduleReadSen5x();
    scheduleReadBmp3xx();
    handleWebServerRequests();
    // serialLog(INFO, "Median T: %6.2f, RH: %6.2f\n", medianTemperatureSht3x(), medianRelHumiditySht3x());
    // serialLog(INFO, "Median T: %6.2f, P:  %6.2f\n", medianTemperatureBmp3xx(), medianPressureBmp3xx());
    // serialLog(INFO, "Median PM1: %6.2f, PM2.5: %6.2f, PM4; %6.2f, PM10: %6.2f\n", medianPM1Sen5x(), medianPM2p5Sen5x(), medianPM4Sen5x(), medianPM10Sen5x());
    // serialLog(INFO, "Median T: %6.2f, RH: %6.2f, VOC: %6.2f, NOx: %6.2f\n", medianTemperatureSen5x(), medianRelHumiditySen5x(), medianVocIndexSen5x(), medianNoxIndexSen5x());
}

#include <Arduino.h>
#include <Wire.h>

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
}

#include <Arduino.h>
#include <W55RP20lwIP.h>
#include <WebServer.h>
#include <StreamString.h>

#include <sensor_sht3x.h>
#include <sensor_sen5x.h>
#include <sensor_bmp3xx.h>
#include <serialLog.h>

#include "ethernet_webserver.h"

Wiznet55rp20lwIP eth(20);
WebServer server(80);

void connectToEthernet() {
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
    serialLog(INFO, "Ethernet connected.\n");
    serialLog(INFO, "IP address: %s\n", eth.localIP().toString().c_str());
    // After connecting, you can check uplink: (eth.linkStatus() == 1)
}

void startWebServer() {
    server.on("/", handleRootPage);
    server.on("/debug", handleDebugPage);
    server.on("/data.json", handleDataJson);
    server.onNotFound(handleNotFoundPage);
    server.begin();
    serialLog(INFO, "Webserver started.\n");
}

void handleWebServerRequests() {
    server.handleClient();
}

void handleRootPage() {
    StreamString page = "<html>\n";
    page += "<head>\n<title>EES</title>\n</head>\n<body>\n";
    page += "<h1>Hello, world!</h1>\n</body>\n";
    page += "</html>";
    server.send(200, "text/html", page);
    serialLog(DEBUG, "Webserver: Root page was served.\n");
}

void handleNotFoundPage() {
    String message = "File Not Found\n\n";
    message += "URI: ";
    message += server.uri();
    message += "\nMethod: ";
    message += (server.method() == HTTP_GET) ? "GET" : "POST";
    message += "\nArguments: ";
    message += server.args();
    message += "\n";
    for (uint8_t i = 0; i < server.args(); i++) {
        message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    }
    server.send(404, "text/plain", message);
    serialLog(DEBUG, "Webserver: 404 page was served.\n");
}

void handleDebugPage() {
    uint32_t sec = millis() / 1000;
    uint32_t hr = sec / 3600;
    uint32_t min = (sec / 60) % 60;
    sec = sec % 60;

    StreamString page;
    page.reserve(500);
    page.printf("<html>\
    <head>\
        <title>EES Debug Page</title>\
    </head>\
    <body>\
        <h1>EES Debug Page</h1>\
        <p>Uptime: %02d:%02d:%02d</p>\
        <p>Free Memory: %d</p>\
    </body>\
    </html>", hr, min, sec, rp2040.getFreeHeap());
    server.send(200, "text/html", page);
    serialLog(DEBUG, "Webserver: Debug page was served.\n");
}

void handleDataJson() {
    StreamString json;
    json.reserve(500);
    json.printf("{\n"
        "  \"sht3x\": {\n"
        "    \"temperature\": %.1f,\n"
        "    \"humidity\": %.1f\n"
        "  },\n"
        "  \"bmp3xx\": {\n"
        "    \"temperature\": %.1f,\n"
        "    \"pressure\": %.1f\n"
        "  },\n"
        "  \"sen5x\": {\n"
        "    \"pm1_0\": %.1f,\n"
        "    \"pm2_5\": %.1f,\n"
        "    \"pm4_0\": %.1f,\n"
        "    \"pm10_0\": %.1f,\n"
        "    \"humidity\": %.1f,\n"
        "    \"temperature\": %.1f,\n"
        "    \"voc\": %.1f,\n"
        "    \"nox\": %.1f\n"
        "  }\n"
        "}",
        medianTemperatureSht3x(), medianRelHumiditySht3x(),
        medianTemperatureBmp3xx(), medianPressureBmp3xx(),
        medianPM1Sen5x(), medianPM2p5Sen5x(), medianPM4Sen5x(), medianPM10Sen5x(),
        medianRelHumiditySen5x(), medianTemperatureSen5x(), medianVocIndexSen5x(), medianNoxIndexSen5x());
    server.send(200, "application/json", json);
    serialLog(DEBUG, "Webserver: data.json was served.\n");
}
#include <Arduino.h>
#include <W55RP20lwIP.h>
#include <WebServer.h>
#include <LEAmDNS.h>
#include <StreamString.h>

#include <sensor_sht3x.h>
#include <sensor_sen5x.h>
#include <sensor_bmp3xx.h>
#include <serialLog.h>
#include <timestamp.h>

#include "ethernet_webserver.h"

Wiznet55rp20lwIP eth(20);
WebServer server(80);

void connectToEthernet() {
    uint8_t mac[6];
    StreamString mdnsHostname;

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
    serialLog(INFO, "IP address:    %s\n", eth.localIP().toString().c_str());
    eth.macAddress(mac);
    serialLog(INFO, "MAC address:   %02X:%02X:%02X:%02X:%02X:%02X\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    // After connecting, you can check uplink: (eth.linkStatus() == 1)

    mdnsHostname.printf("ees-%02x%02x%02x",mac[3], mac[4], mac[5]);
    if (MDNS.begin(mdnsHostname.c_str())) {
        serialLog(INFO, "mDNS hostname: %s.local\n", mdnsHostname.c_str());
    }
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
    MDNS.update();
}

void handleRootPage() {
    StreamString page;
    page.print("<html>\n<head>\n");
    page.print("<title>EES</title>\n");
    page.print("</head>\n<body>\n");
    page.print("<h1>Hello, world!</h1>\n");
    page.print("<p><a href=\"data.json\">data.json</a></p>\n");
    page.print("<p><a href=\"debug\">debug</a></p>\n");
    page.printf("<p>IP: %s</p>", eth.localIP().toString().c_str());
    page.print("<p>Uptime: ");
    appendTimestamp(page);
    page.print("</p>\n");
    page.print("</body>\n</html>");
    server.send(200, "text/html", page);
    serialLog(DEBUG, "Webserver: Root page was served.\n");
    //serialLog(DEBUG, "Webserver: length of page buffer: %d.\n", page.length());
}

void handleNotFoundPage() {
    StreamString page = "Error 404 - File Not Found\n";
    server.send(404, "text/plain", page);
    serialLog(DEBUG, "Webserver: 404 page was served.\n");
    //serialLog(DEBUG, "Webserver: length of page buffer: %d.\n", page.length());
}

void handleDebugPage() {
    uint32_t sec = millis() / 1000;
    uint32_t hr = sec / 3600;
    uint32_t min = (sec / 60) % 60;
    sec = sec % 60;

    StreamString page;
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
    //serialLog(DEBUG, "Webserver: length of page buffer: %d.\n", page.length());
}

void handleDataJson() {
    StreamString json;
    json.print("{\n");
    appendJsonSht3x(json);
    json.print(",\n");
    appendJsonBmp3xx(json);
    json.print(",\n");
    appendJsonSen5x(json);
    json.print("\n}");
    server.send(200, "application/json", json);
    serialLog(DEBUG, "Webserver: data.json was served.\n");
    //serialLog(DEBUG, "Webserver: length of json buffer: %d.\n", json.length());
}

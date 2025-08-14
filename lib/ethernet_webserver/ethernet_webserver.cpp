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
#include <config.h>

#include "ethernet_webserver.h"

Wiznet55rp20lwIP eth(20);
WebServer server(80);

void connectToEthernet() {
    uint8_t mac[6] = BOARD_MAC_ADDRESS;
    StreamString hostname;
    hostname.printf("ees-%02x%02x%02x",mac[3], mac[4], mac[5]);
    eth.setHostname(hostname.c_str());

    if (!eth.begin(mac)) {
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
    if (MDNS.begin(hostname.c_str())) {
        serialLog(INFO, "mDNS hostname: %s.local\n", hostname.c_str());
    }
}

void startWebServer() {
    server.on("/", handleRootPage);
    server.on("/data.json", handleDataJson);
    server.on("/logo.svg", handleLogo);
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
    page.print("<img src=\"logo.svg\">\n");
    page.print("<h1>Hello, world!</h1>\n");
    page.print("<p><a href=\"data.json\">data.json</a></p>\n");
    page.print("<br>\n");
    page.printf("<p>IP: %s</p>", eth.localIP().toString().c_str());
    page.print("<p>Uptime: ");
    appendTimestamp(page);
    page.print("</p>\n");
    page.printf("<p>Free Heap memory: %dB<p>\n", rp2040.getFreeHeap());
    page.printf("<p>Free Stack memory: %dB<p>\n", rp2040.getFreeStack());
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

void handleLogo() {
    StreamString logo;
    logo.print("<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"100\" height=\"100\" version=\"1.1\" viewBox=\"0 0 2000 2000\">\n");
    logo.print("<g>\n<path fill=\"#424242\" d=\"M1045 871l75 -130c10,-18 9,-39 -3,-56 -61,-86 11,-212 117,-202 21,");
    logo.print("2 39,-8 50,-26l74 -129c20,-35 -4,-78 -44,-78l-150 0c-21,0 -38,11 -47,30 -44,97 -190,97 -234,0 -9,");
    logo.print("-19 -26,-30 -47,-30l-150 0c-40,0 -64,43 -44,78l313 543c20,34 70,34 90,0zm89 103l150 0c21,0 38,-11 47,");
    logo.print("-30 44,-96 190,-97 234,0 9,19 26,30 47,30l150 0c40,0 64,-43 44,-77l-74 -130c-11,-18 -30,-28 -50,");
    logo.print("-26 -106,10 -179,-115 -117,-202 12,-17 13,-38 3,-56l-75 -130c-20,-34 -70,-34 -90,0l-314 544c-20,34 5,");
    logo.print("77 45,77zm-45 129l75 130c11,18 29,28 50,26 106,-10 179,115 117,202 -12,17 -13,38 -3,56l75 130c20,34 70,");
    logo.print("34 90,0l75 -130c10,-18 9,-39 -3,-56 -62,-87 11,-212 117,-202 20,2 39,-8 50,-26l74 -130c20,-34 -5,");
    logo.print("-77 -44,-77l-628 0c-40,0 -65,43 -45,77zm-134 26l-75 130c-10,18 -9,39 3,56 61,86 -11,212 -117,202 -21,");
    logo.print("-2 -39,8 -50,26l-74 129c-20,35 4,78 44,78l150 0c21,0 38,-11 47,-30 44,-97 190,-97 234,0 9,19 26,30 47,");
    logo.print("30l150 0c40,0 64,-43 44,-78l-313 -543c-20,-34 -70,-34 -90,0zm-89 -103l-150 0c-21,0 -38,11 -47,30 -44,");
    logo.print("96 -190,97 -234,0 -9,-19 -26,-30 -47,-30l-150 0c-39,0 -64,43 -44,77l74 130c11,18 30,28 50,26 106,-10 179,");
    logo.print("115 117,202 -12,17 -13,38 -3,56l75 130c20,34 70,34 90,0l314 -544c20,-34 -5,-77 -45,");
    logo.print("-77zm45 -129l-75 -130c-11,-18 -29,-28 -50,-26 -106,10 -179,-115 -117,-202 12,-17 13,-38 3,-56l-75 -130c-20,");
    logo.print("-34 -70,-34 -90,0l-75 130c-10,18 -9,39 3,56 62,87 -11,212 -117,202 -20,-2 -39,8 -50,26l-74 130c-20,34 5,");
    logo.print("77 44,77l628 0c40,0 65,-43 45,-77z\"/></g>\n</svg>\n");
    server.send(200, "image/svg+xml", logo);
}

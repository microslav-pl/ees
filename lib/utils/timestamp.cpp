#include <Arduino.h>

#include "timestamp.h"

void appendTimestamp(StreamString& timestamp) {
    uint32_t mil = millis();
    uint32_t sec = mil / 1000;
    uint32_t day = sec / 86400;
    uint32_t hr = (sec / 3600) % 24;
    uint32_t min = (sec / 60) % 60;
    sec = sec % 60;
    mil = mil % 1000;
    // Format DD:HH:MM:SS:mmm
    // 4294967295 ms (max uint32_t) / 86400000 (ms/day) ≈ 49.71 days
    timestamp.printf("%2d:%02d:%02d:%02d.%03d", day, hr, min, sec, mil);
}

#include "TrackRay/TrackRay.h"

const uint8_t CONTROL_PERIOD = 50;
uint32_t prevControlTime = 0;

const uint8_t ENC_SW = 23;

void setup() {
    TrackRay.begin();
    TrackRay.startWiFiCaptain("TrackRay", "12345678");    // password length minimally 8 characters
    pinMode(ENC_SW, INPUT_PULLDOWN);
}

void loop() {
    if(millis() > prevControlTime + CONTROL_PERIOD) {
        prevControlTime = millis();
    }
}

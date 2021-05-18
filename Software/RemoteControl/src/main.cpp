#include "TrackJet/TrackJet.h"

const uint8_t CONTROL_PERIOD = 200;
uint32_t prevControlTime = 0;

void setup() {
    TrackJet.begin();
    TrackJet.startWiFiCaptain("TrackJet", "12345678");    // password length minimally 8 characters
    TrackJet.displayText(ipToDisp());
}

void loop() {
    if(millis() > prevControlTime + CONTROL_PERIOD) {
        prevControlTime = millis();
        //TrackJet.displayDigit(TrackJet.getEncoder()%10);
        Serial.printf("Button %d, Enc %d, %d, %d\n", TrackJet.getButton(), TrackJet.getEncoder(), TrackJet.getEncoderSW(), TrackJet.getEncoderSWPulse());
    }
}

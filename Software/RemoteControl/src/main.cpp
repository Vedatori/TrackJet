#include "TrackJet/TrackJet.h"

const uint8_t CONTROL_PERIOD = 100;
uint32_t prevControlTime = 0;

void setup() {
    TrackJet.begin();
    TrackJet.startWiFiCaptain("TrackJet", "12345678");    // password length minimally 8 characters
    TrackJet.displayText(ipToDisp());
}

void loop() {
    if(millis() > prevControlTime + CONTROL_PERIOD) {
        prevControlTime = millis();
        //TrackJet.displayDigit(TrackJet.encoderRead()%10);
        Serial.printf("Button %d, Enc %d, %d, %d\n", TrackJet.buttonRead(), TrackJet.encoderRead(), TrackJet.encoderReadButton(), TrackJet.encoderReadButtonPulse());

        if(TrackJet.commandGetIndexed(0) == "servo") {
            TrackJet.servoSetPosition(0, TrackJet.commandGetIndexed(1).toInt());
            TrackJet.commandClear();
        }
    }
}

#include "TrackJet/TrackJet.h"

const uint8_t CONTROL_PERIOD = 100;
uint32_t prevControlTime = 0;

void setup() {
    TrackJet.begin();
    TrackJet.startWiFiCaptain("TrackJet", "12345678");    // password length minimally 8 characters
    TrackJet.displayText(ipToDisp());
    TrackJet.servoSetSpeed(0, 10);
}

void loop() {
    if(millis() > prevControlTime + CONTROL_PERIOD) {
        prevControlTime = millis();
        if(TrackJet.displayIsBusy() == false)
            TrackJet.displayDigit(TrackJet.encoderRead()%10);
        //Serial.printf("Button %d, Enc %d, %d, %d\n", TrackJet.buttonRead(), TrackJet.encoderRead(), TrackJet.encoderReadButton(), TrackJet.encoderReadButtonPulse());
        Serial.printf("%f %f %f\t", TrackJet.gyroAngleYPR(0), TrackJet.gyroAngleYPR(1), TrackJet.gyroAngleYPR(2));
        TrackJet.printOffsets();

        if(TrackJet.commandGetIndexed(0) == "servo") {
            TrackJet.servoSetPosition(0, TrackJet.commandGetIndexed(1).toInt());
            TrackJet.commandClear();
        }
        else if(TrackJet.commandGetIndexed(0) == "gyro") {
            if(TrackJet.commandGetIndexed(1) == "calibrate") {
                TrackJet.gyroCalibrate();
                TrackJet.commandClear();
            }
        }
        else if(TrackJet.commandGetIndexed(0) == "restart") {
            ESP.restart();
        }
    }
}

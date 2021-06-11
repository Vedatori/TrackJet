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
        if(TrackJet.displayIsBusy() == false) {
            TrackJet.displayDigit(TrackJet.encoderRead()%10);
            for(uint8_t i = 0; i < 8; ++i) {
                TrackJet.displaySingle(7, i, 0);
                TrackJet.displaySingle(i, 7, 0);
                TrackJet.displaySingle(i, 6, 0);
            }
            TrackJet.displaySingle(7, 3 - ((int)TrackJet.gyroAngleYPR(2) - 10)/20, 12);
            TrackJet.displaySingle(7, 4 - ((int)TrackJet.gyroAngleYPR(2) + 10)/20, 12);
            TrackJet.displaySingle(3 + ((int)TrackJet.gyroAngleYPR(1) + 10)/20, 7, 12);
            TrackJet.displaySingle(4 + ((int)TrackJet.gyroAngleYPR(1) - 10)/20, 7, 12);
            TrackJet.displaySingle(TrackJet.lidarDistance()/100, 6, 12);
        }
        //Serial.printf("Button %d, Enc %d, %d, %d\n", TrackJet.buttonRead(), TrackJet.encoderRead(), TrackJet.encoderReadButton(), TrackJet.encoderReadButtonPulse());
        //Serial.printf("%f %f %f %d\n", TrackJet.gyroAngleYPR(0), TrackJet.gyroAngleYPR(1), TrackJet.gyroAngleYPR(2), TrackJet.lidarDistance());
        
        Serial.printf("adc %d\n", TrackJet.analogRead(2));

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

#include "TrackJet/TrackJet.h"

const uint8_t CONTROL_PERIOD = 100;
uint32_t prevControlTime = 0;

void setup() {
    TrackJet.begin();
    TrackJet.startWiFiCaptain("<your_name>");
    TrackJet.displayText(ipToDisp());
    TrackJet.servoSetSpeed(1, 60);
}

void loop() {
    if(millis() > prevControlTime + CONTROL_PERIOD) {
        prevControlTime = millis();
        if(TrackJet.displayIsBusy() == false) {
            TrackJet.displayDigit(TrackJet.encoderRead()%10);
            for(uint8_t i = 0; i < 8; ++i) {
                TrackJet.displaySingle(i, 7, 0);
                TrackJet.displaySingle(i, 6, 0);
            }
            for(uint8_t i = 0; i < TrackJet.battPercent()/12; ++i) {
                TrackJet.displaySingle(7 - i, 7, 1);
            }
            TrackJet.displaySingle(TrackJet.lidarDistance()/100, 6, 1);
        }

        if(TrackJet.commandGetIndexed(0) == "blade") {
            TrackJet.servoSetPosition(1, TrackJet.commandGetIndexed(1).toInt());
            TrackJet.commandClear();
        }
        else if(TrackJet.commandGetIndexed(0) == "head") {
            TrackJet.servoSetPosition(2, TrackJet.commandGetIndexed(1).toInt());
            TrackJet.commandClear();
        }
        else if(TrackJet.commandGetIndexed(0) == "beep") {
            TrackJet.soundTone();
            delay(500);
            TrackJet.soundEnd();
            TrackJet.commandClear();
        }
        else if(TrackJet.commandGet() == "take on me") {
            TrackJet.playMelody(takeonme, sizeof(takeonme), takeonme_tempo);
            TrackJet.commandClear();
        }
        else if(TrackJet.commandGetIndexed(0) == "silence") {
            TrackJet.stopMelody();
            TrackJet.commandClear();
        }
    }
}

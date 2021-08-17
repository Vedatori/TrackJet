#include "TrackJet/TrackJet.h"

const uint8_t CONTROL_PERIOD = 100;
uint32_t prevControlTime = 0;

uint16_t prevEnc1 = 0, prevEnc2 = 0;

void setup() {
    TrackJet.begin();
    TrackJet.startWiFiCaptain("Honza V");
    TrackJet.displayText(ipToDisp());
    TrackJet.servoSetSpeed(0, 60);
}

void loop() {
    if(millis() > prevControlTime + CONTROL_PERIOD) {
        prevControlTime = millis();
        if(TrackJet.displayIsBusy() == false) {
            /*TrackJet.displayDigit(TrackJet.encoderRead()%10);
            for(uint8_t i = 0; i < 8; ++i) {
                TrackJet.displaySingle(i, 7, 0);
                TrackJet.displaySingle(i, 6, 0);
            }
            for(uint8_t i = 0; i < TrackJet.battPercent()/12; ++i) {
                TrackJet.displaySingle(7 - i, 7, 1);
            }
            TrackJet.displaySingle(TrackJet.lidarDistance()/100, 6, 1);*/
            TrackJet.displayText(String(TrackJet.battPercent())+"   ", false);
        }
        //printf("Button %d, Enc %d, %d, %d\n", TrackJet.buttonRead(), TrackJet.encoderRead(), TrackJet.encoderReadButton(), TrackJet.encoderReadButtonPulse());
        printf("%d\n", TrackJet.battPercent());

        TrackJet.msgSend("battery",String(TrackJet.battPercent()));

        //printf("FL %d RL %d FR %d RR %d enc0 %d enc1 %d speed0 %d speed1 %d\n", adc1_get_raw(TJ::ADC_CH_ENC_FL), adc1_get_raw(TJ::ADC_CH_ENC_RL), adc1_get_raw(TJ::ADC_CH_ENC_FR), adc1_get_raw(TJ::ADC_CH_ENC_RR), TrackJet.encoderGetSteps(1), TrackJet.encoderGetSteps(2), TrackJet.encoderGetSteps(1) - prevEnc1, TrackJet.encoderGetSteps(2) - prevEnc2);
        prevEnc1 = TrackJet.encoderGetSteps(1);
        prevEnc2 = TrackJet.encoderGetSteps(2);

        //Serial.printf("pot %d battV %f battP %d lineL %d lineR %d\n", TrackJet.potentiometerRead(), TrackJet.battVolt(), TrackJet.battPercent(), TrackJet.lineLeft(), TrackJet.lineRight());

        if(TrackJet.commandGetIndexed(0) == "blade") {
            TrackJet.servoSetPosition(0, TrackJet.commandGetIndexed(1).toInt());
            TrackJet.commandClear();
        }
        else if(TrackJet.commandGetIndexed(0) == "lidar") {
            TrackJet.servoSetPosition(1, TrackJet.commandGetIndexed(1).toInt());
            TrackJet.commandClear();
        }
        else if(TrackJet.commandGetIndexed(0) == "beep") {
            TrackJet.soundTone();
            delay(500);
            TrackJet.soundEnd();
            TrackJet.commandClear();
        }
        TrackJet.ledWrite(1, TrackJet.buttonRead());
    }
}

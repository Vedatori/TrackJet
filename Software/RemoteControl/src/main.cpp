#include "TrackJet/TrackJet.h"
#include "TrackJet/Music.h"

const uint8_t CONTROL_PERIOD = 100;
uint32_t prevControlTime = 0;

uint16_t prevEnc1 = 0, prevEnc2 = 0;



void setup() {
    TrackJet.begin();
    TrackJet.startWiFiCaptain("Honza V");
    TrackJet.displayText(ipToDisp());
    TrackJet.servoSetSpeed(1, 60);
}


void loop() {
    if(millis() > prevControlTime + CONTROL_PERIOD) {
        prevControlTime = millis();
        if(TrackJet.displayIsBusy() == false) {

        }
        //printf("Button %d, Enc %d, %d, %d\n", TrackJet.buttonRead(), TrackJet.encoderRead(), TrackJet.encoderReadButton(), TrackJet.encoderReadButtonPulse());
        //printf("%d\n", TrackJet.lidarDistance());

        //printf("FL %d RL %d FR %d RR %d enc0 %d enc1 %d speed0 %d speed1 %d\n", adc1_get_raw(TJ::ADC_CH_ENC_FL), adc1_get_raw(TJ::ADC_CH_ENC_RL), adc1_get_raw(TJ::ADC_CH_ENC_FR), adc1_get_raw(TJ::ADC_CH_ENC_RR), TrackJet.encoderGetSteps(1), TrackJet.encoderGetSteps(2), TrackJet.encoderGetSteps(1) - prevEnc1, TrackJet.encoderGetSteps(2) - prevEnc2);
        prevEnc1 = TrackJet.encoderGetSteps(1);
        prevEnc2 = TrackJet.encoderGetSteps(2);

        //Serial.printf("pot %d battV %f battP %d lineL %d lineR %d\n", TrackJet.potentiometerRead(), TrackJet.battVolt(), TrackJet.battPercent(), TrackJet.lineRead(1), TrackJet.lineRead(2));

        if(TrackJet.commandGetIndexed(0) == "blade") {
            TrackJet.servoSetPosition(1, TrackJet.commandGetIndexed(1).toInt());
            TrackJet.commandClear();
        }
        else if(TrackJet.commandGetIndexed(0) == "lidar") {
            TrackJet.servoSetPosition(2, TrackJet.commandGetIndexed(1).toInt());
            TrackJet.commandClear();
        }
        else if(TrackJet.commandGetIndexed(0) == "beep") {
            TrackJet.soundTone();
            delay(500);
            TrackJet.soundEnd();
            TrackJet.commandClear();
        }else if(TrackJet.commandGetIndexed(0) == "starwars"){
            TrackJet.playMelody(starwars,sizeof(starwars),starwars_tempo);
            TrackJet.commandClear();
        }else if(TrackJet.commandGetIndexed(0) == "rick"){
            TrackJet.playMelody(nevergonnagiveyouup,sizeof(nevergonnagiveyouup),nevergonnagiveyouup_tempo);
            TrackJet.commandClear();
        }else if(TrackJet.commandGetIndexed(0) == "nokia"){
            TrackJet.playMelody(nokia,sizeof(nokia),nokia_tempo);
            TrackJet.commandClear();
        }else if(TrackJet.commandGetIndexed(0) == "mii"){
            TrackJet.playMelody(miichannel,sizeof(miichannel),miichannel_tempo);
            TrackJet.commandClear();
        }else if(TrackJet.commandGetIndexed(0) == "mario"){
            TrackJet.playMelody(supermariobros,sizeof(supermariobros),supermariobros_tempo);
            TrackJet.commandClear();
        }else if(TrackJet.commandGetIndexed(0) == "takeonme"){
            TrackJet.playMelody(takeonme,sizeof(takeonme),takeonme_tempo);
            TrackJet.commandClear();
        }else if(TrackJet.commandGetIndexed(0) == "tetris"){
            TrackJet.playMelody(tetris,sizeof(tetris),tetris_tempo);
            TrackJet.commandClear();
        }else if(TrackJet.commandGetIndexed(0) == "stop"){
            TrackJet.stopMelody();
            TrackJet.commandClear();
        }


        TrackJet.ledWrite(1, TrackJet.buttonRead());


    }
}

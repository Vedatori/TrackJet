#include "TrackJet/TrackJet.h"
#include "TrackJet/music.h"

const uint8_t CONTROL_PERIOD = 100;
uint32_t prevControlTime = 0;

uint16_t prevEnc1 = 0, prevEnc2 = 0;

void setup() {
    TrackJet.begin();
    TrackJet.startWiFiCaptain("Honza V");
    TrackJet.displayText(ipToDisp());
    TrackJet.servoSetSpeed(1, 60);
}

String songDisplay = "";
bool prevEncButton = false;


void loop() {
    if(millis() > prevControlTime + CONTROL_PERIOD) {
        prevControlTime = millis();
        if(TrackJet.displayIsBusy() == false) {
            TrackJet.displayText(songDisplay, false);
        }
        //printf("Button %d, Enc %d, %d, %d\n", TrackJet.buttonRead(), TrackJet.encoderRead(), TrackJet.encoderReadButton(), TrackJet.encoderReadButtonPulse());
        //printf("%d\n", TrackJet.lidarDistance());

        //printf("FL %d RL %d FR %d RR %d enc0 %d enc1 %d speed0 %d speed1 %d\n", adc1_get_raw(TJ::ADC_CH_ENC_FL), adc1_get_raw(TJ::ADC_CH_ENC_RL), adc1_get_raw(TJ::ADC_CH_ENC_FR), adc1_get_raw(TJ::ADC_CH_ENC_RR), TrackJet.encoderGetSteps(1), TrackJet.encoderGetSteps(2), TrackJet.encoderGetSteps(1) - prevEnc1, TrackJet.encoderGetSteps(2) - prevEnc2);
        prevEnc1 = TrackJet.encoderGetSteps(1);
        prevEnc2 = TrackJet.encoderGetSteps(2);

        //Serial.printf("pot %d battV %f battP %d lineL %d lineR %d\n", TrackJet.potentiometerRead(), TrackJet.battVolt(), TrackJet.battPercent(), TrackJet.lineRead(1), TrackJet.lineRead(2));
        
        TrackJet.msgSend("battery",String(TrackJet.battPercent())+","+String(((float)((int)(TrackJet.battVolt()*100)))/100));

        int song = TrackJet.encoderRead() % 7;

        switch (song)
        {
        case 0:
            songDisplay = "0";
            break;
        case 1:
            songDisplay = "1";
            break;
        case 2:
            songDisplay = "2";
            break;
        case 3:
            songDisplay = "3";
            break;
        case 4:
            songDisplay = "4";
            break;
        case 5:
            songDisplay = "5";
            break;
        case 6:
            songDisplay = "6";
            break;
        
        default:
            break;
        }

        if(TrackJet.encoderReadButton() && !prevEncButton){
            switch (song)
            {
            case 0:
                TrackJet.playMelody(starwars, sizeof(starwars)/sizeof(starwars[0]), starwars_tempo);
                break;
            case 1:
                TrackJet.playMelody(nevergonnagiveyouup, sizeof(nevergonnagiveyouup)/sizeof(nevergonnagiveyouup[0]), nevergonnagiveyouup_tempo);
                break;
            case 2:
                TrackJet.playMelody(miichannel, sizeof(miichannel)/sizeof(miichannel[0]), miichannel_tempo);
                break;
            case 3:
                TrackJet.playMelody(nokia, sizeof(nokia)/sizeof(nokia[0]), nokia_tempo);
                break;
            case 4:
                TrackJet.playMelody(supermariobros, sizeof(supermariobros)/sizeof(supermariobros[0]), supermariobros_tempo);
                break;
            case 5:
                TrackJet.playMelody(takeonme, sizeof(takeonme)/sizeof(takeonme[0]), takeonme_tempo);
                break;
            case 6:
                TrackJet.playMelody(tetris, sizeof(tetris)/sizeof(tetris[0]), tetris_tempo);
                break;
            
            
            default:
                break;
            }
        }

        prevEncButton = TrackJet.encoderReadButton();

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
        }
        TrackJet.ledWrite(1, TrackJet.buttonRead());


    }
}

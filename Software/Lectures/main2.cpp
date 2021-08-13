// Lekce 2 - přečteme stav tlačítka, ukážeme si větvící konstrukci 'if'
#include "TrackJet/TrackJet.h"

void setup() {
    TrackJet.begin();
}

void loop() {
    // Rozsvícení LED1 pomocí konstrukce 'if'
    if(TrackJet.buttonRead()) {
        TrackJet.ledWrite(1, true);
    }
    else {
        TrackJet.ledWrite(1, false);
    }

    //______________________________________________
    // Rozsvícení LED2 úsporněji bez konstrukce 'if'
    TrackJet.ledWrite(2, TrackJet.buttonRead());

    delay(100);
}
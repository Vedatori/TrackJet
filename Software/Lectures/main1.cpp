// Lekce 1 - rozblíkáme LED pomocí "delay"
#include "TrackJet/TrackJet.h"

void setup() {
    TrackJet.begin();
}

void loop() {
    // Rozsvícení LED číslo 1
    TrackJet.ledWrite(1, true);
    // Čekání 1 s (1000ms)
    delay(1000);
    // Zhasnutí LED číslo 1
    TrackJet.ledWrite(1, false);
    // Čekání 1 s
    delay(1000);
}
# Lekce 7 - Rozblikáme LED bez použití příkazu delay(). Vytvoříme vlastní funkci.
#include "TrackJet/TrackJet.h"

int predchoziHodnotaMillis = 0;
int interval = 1000;
bool stavLed = false;

void setup() {
    TrackJet.begin();
}

void loop() {
    int aktualniHodnotaMillis = millis();

    if (aktualniHodnotaMillis - predchoziHodnotaMillis >= interval) {
        predchoziHodnotaMillis = aktualniHodnotaMillis;

        stavLed = !stavLed;

		TrackJet.ledWrite(1, stavLed);		
    }
}

#include "TrackJet/TrackJet.h"

int predchoziHodnotaMillis = 0;
int interval = 1000;
bool stavLed = false;

// alternativní realizace funkcí
void ledFunkce() {
    int aktualniHodnotaMillis = millis();

    if (aktualniHodnotaMillis - predchoziHodnotaMillis >= interval) {
        predchoziHodnotaMillis = aktualniHodnotaMillis;

        stavLed = !stavLed;
        
		TrackJet.ledWrite(1, stavLed);     
    }
}

void setup() {
    TrackJet.begin();
}

void loop() {
    int aktualniHodnotaMillis = millis();

    if (aktualniHodnotaMillis - predchoziHodnotaMillis >= interval) {
        // uložit hodnotu přepnutí z false na true
        predchoziHodnotaMillis = aktualniHodnotaMillis;

        // prohodit stav LED
        stavLed = !stavLed;
        // alternativní prohození stavu LED
        // if (stavLed == false) {
        //     stavLed = true;
        // } else {
        //     stavLed = false;
        // }

        // nastavit stav LED1 hodnotou proměnné stavLed
		TrackJet.ledWrite(1, stavLed);
		
		// Cyklus for pro nastavení stavu LED v horním řádku LED panelu
		// for (int i = 0; i < 8; i++)
        //     displaySingle(0, i, stavLed); 
    }

    // Alternativní realizace funkcí
    // ledFunkce();
}

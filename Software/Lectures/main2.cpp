// Lekce 2 - přečteme stav tlačítka, ukážeme si binární proměnnou
#include "TrackJet/TrackJet.h"

void setup() {
    TrackJet.begin();
}

void loop() {
	// Vytvoření binární proměnné
	bool button = false;
	
	// Přečtení stavu tlačítka a uložení do proměnné
	button = TrackJet.buttonRead();
	
    // Rozsvícení LED1 podle stavu tlačítka
	TrackJet.ledWrite(1, button);

    //______________________________________________
    // Rozsvícení LED2 úsporněji bez zavedení proměnné
    TrackJet.ledWrite(2, TrackJet.buttonRead());

    delay(100);
}
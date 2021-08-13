// Lekce 3 - přečteme stav potenciometru, ukážeme si číselnou proměnnou
// a nastavíme jas LED
#include "TrackJet/TrackJet.h"

void setup() {
    TrackJet.begin();
}

void loop() {
	// Vytvoření celočíselné proměnné
	int jas = 0;
	
	// Přečtení stavu potenciometru (0-4095) a uložení do proměnné
	jas = TrackJet.potentiometerRead();
	
	// Nastavení jasu LED1 podle stavu proměnné v rozsahu (0-100)
	TrackJet.ledWriteAnalog(1, jas/40);

	//______________________________________________
    // Nastavení jasu LED2 úsporněji bez zavedení proměnné
    TrackJet.ledWriteAnalog(2, TrackJet.potentiometerRead()/40);
	
    delay(100);
}
// Lekce 4 - roztočíme motory a ukážeme si podmínku "if"
#include "TrackJet/TrackJet.h"

void setup() {
    TrackJet.begin();
	
	// Roztočení motoru 0 (levý pás) na 50% výkonu
	TrackJet.motorsSetSpeed(50, 0);
	delay(1000);
}

void loop() {
    if(TrackJet.buttonRead()) {
		// Když je tláčítko zmáčknuto, roztočí se oba motory
		TrackJet.motorsSetSpeed(50, 0);
		TrackJet.motorsSetSpeed(50, 1);
	}
	else {
		// Když není tláčítko zmáčknuto, oba motory se zastaví
		TrackJet.motorsSetSpeed(0, 0);
		TrackJet.motorsSetSpeed(0, 1);
	}

    delay(100);
}
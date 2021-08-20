// Lekce 4 - roztočíme motory a ukážeme si podmínku "if"
#include "TrackJet/TrackJet.h"

void setup() {
    TrackJet.begin();
	
	// Roztočení motoru 0 (levý pás) na 50% výkonu
	TrackJet.motorsSetSpeed(1, 50);
	delay(1000);
}

void loop() {
    if(TrackJet.buttonRead()) {
		// Když je tláčítko zmáčknuto, roztočí se oba motory
		TrackJet.motorsSetSpeed(1, 50);
		TrackJet.motorsSetSpeed(2, 50);
	}
	else {
		// Když není tláčítko zmáčknuto, oba motory se zastaví
		TrackJet.motorsSetSpeed(1, 0);
		TrackJet.motorsSetSpeed(2, 0);
	}

    delay(100);
}
// Lekce 6 - přečteme stav rotačního enkodéru a rozpohybujeme servomotory
#include "TrackJet/TrackJet.h"

void setup() {
    TrackJet.begin();
}

void loop() {
    int uhel = TrackJet.encoderRead();
	printf("%d\n", uhel);
	TrackJet.servoSetPosition(1, uhel);

    delay(100);
}
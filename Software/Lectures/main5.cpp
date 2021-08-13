// Lekce 5 - TrackJet začne komunikovat s počítačem
#include "TrackJet/TrackJet.h"

void setup() {
    TrackJet.begin();
    printf("Hello World!\n");

    int cele_cislo = 3;
    printf("%d\n", cele_cislo);

    float desetinne_cislo = 3.14;
    printf("%f\n", desetinne_cislo);

    char slovo[] = "ahoj";
    printf("%s\n", slovo);
}

void loop() {
    printf("%d %d %d\n", millis(), TrackJet.buttonRead(), TrackJet.potentiometerRead());

    delay(100);
}
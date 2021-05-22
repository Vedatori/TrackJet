#include <Arduino.h>

#include "SemiIntelligentServo.h"

SemiIntelligentServo::SemiIntelligentServo(uint8_t aPwmPin, uint8_t aPwmChannel) {
    pwmPin = aPwmPin;
    pwmChannel = aPwmChannel;
    ledcSetup(pwmChannel, servoFreqPWM, servoResolPWM);
    ledcAttachPin(pwmPin, pwmChannel);
}
void SemiIntelligentServo::setPosition(float aPosition) {
    if(aPosition < 0)
        aPosition = 0;
    else if(aPosition > 180)
        aPosition = 180;
    position = aPosition;
    uint16_t dutyCycleServo = position/180*(servoCountHigh - servoCountLow) + servoCountLow;
    ledcWrite(pwmChannel, dutyCycleServo);
}
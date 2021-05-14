#include <Arduino.h>
#include "RBControl_serialPWM.hpp"


uint16_t loopDelay = 200;

const uint8_t PWM_CHANNELS = 32;
const uint16_t PWM_FREQUENCY = 1000;  //330
extern const uint8_t PWM_MAX = rb::SerialPWM::resolution();

const uint8_t REG_DAT = 13;
const uint8_t REG_CLK = 14;
const uint8_t REG_LATCH = 12;
const uint8_t REG_OE = 25;

const uint8_t ENC_SW = 23;

rb::SerialPWM serialPWM(PWM_CHANNELS, REG_DAT, REG_LATCH, REG_CLK, PWM_FREQUENCY);
int8_t pwm_index[] = {0, 3, 2, 29, 28, 31, 30, 25, 24, 27, 26, 21, 20, 23, 22, 17, 16, 19, 18, 13, 12, 15, 14, 9, 8, 11, 10, 5, 4, 7, 6, 1, 0};

void setPWM(rb::SerialPWM::value_type& channel, int8_t power) {
    if(power > PWM_MAX)
        power = PWM_MAX;
    else if(power < 0)
        power = 0;

    channel = power;
}

void taskOne( void * parameter ) {
    for(;;) {
        serialPWM.update();
        delay(20);
    }
}

void setup() {
    Serial.begin(115200);
    pinMode(ENC_SW, INPUT_PULLDOWN);
    pinMode(REG_OE, OUTPUT);

    xTaskCreate(
        taskOne,          /* Task function. */
        "TaskOne",        /* String with name of task. */
        10000,            /* Stack size in bytes. */
        NULL,             /* Parameter passed as input of the task */
        1,                /* Priority of the task. */
        NULL);            /* Task handle. */
    
    serialPWM.setPWM(pwm_index[12], 10);
    serialPWM.setPWM(pwm_index[13], 50);
}

void loop() {
    digitalWrite(REG_OE, !digitalRead(ENC_SW));
    Serial.printf("ENC_SW %d\n", digitalRead(ENC_SW));
    delay(loopDelay);
}
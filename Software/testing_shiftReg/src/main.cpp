#include <Arduino.h>
#include "SerialPWM.h"


uint16_t loopDelay = 200;

const uint8_t PWM_CHANNELS = 32;
const uint16_t PWM_FREQUENCY = 1000;  //330

const uint8_t REG_DAT = 13;
const uint8_t REG_CLK = 14;
const uint8_t REG_LATCH = 12;
const uint8_t REG_OE = 25;

const uint8_t ENC_SW = 23;

SerialPWM serialPWM(PWM_CHANNELS, REG_DAT, REG_LATCH, REG_CLK, REG_OE, PWM_FREQUENCY);

void taskOne( void * parameter ) {
    for(;;) {
        serialPWM.update();
        delay(20);
    }
}

void setup() {
    Serial.begin(115200);
    pinMode(ENC_SW, INPUT_PULLDOWN);
    //pinMode(REG_OE, OUTPUT);

    xTaskCreate(
        taskOne,          /* Task function. */
        "TaskOne",        /* String with name of task. */
        10000,            /* Stack size in bytes. */
        NULL,             /* Parameter passed as input of the task */
        1,                /* Priority of the task. */
        NULL);            /* Task handle. */
    
    serialPWM.setPWM(12, 10);
    serialPWM.setPWM(13, 50);
}

void loop() {
    //digitalWrite(REG_OE, !digitalRead(ENC_SW));
    serialPWM.set_output(digitalRead(ENC_SW));
    Serial.printf("ENC_SW %d\n", digitalRead(ENC_SW));
    delay(loopDelay);
}
#include <Arduino.h>
#include "SerialPWM.h"


uint16_t loopDelay = 200;

const uint16_t PWM_FREQUENCY = 1000;  //330

const uint8_t REG_DAT = 13;
const uint8_t REG_CLK = 14;
const uint8_t REG_LATCH = 12;
const uint8_t REG_OE = 25;

const uint8_t ENC_SW = 23;

SerialPWM serialPWM(REG_DAT, REG_LATCH, REG_CLK, REG_OE, PWM_FREQUENCY);

void taskOne( void * parameter ) {
    for(;;) {
        serialPWM.update();
        delay(20);
    }
}

void setup() {
    Serial.begin(115200);
    pinMode(ENC_SW, INPUT_PULLDOWN);

    xTaskCreate(
        taskOne,          /* Task function. */
        "TaskOne",        /* String with name of task. */
        10000,            /* Stack size in bytes. */
        NULL,             /* Parameter passed as input of the task */
        1,                /* Priority of the task. */
        NULL);            /* Task handle. */
    
    serialPWM.setPWM(11, 10);
    serialPWM.setPWM(12, 50);
}

void loop() {
    serialPWM.set_output(digitalRead(ENC_SW));
    Serial.printf("ENC_SW %d\n", digitalRead(ENC_SW));
    delay(loopDelay);
}
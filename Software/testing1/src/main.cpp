#include <Arduino.h>
#include <Wire.h>
#include <VL53L0X.h>

const uint8_t motorA = 25, motorB = 26, analogIn = 36, servoOut = 13, encA = 16, encB = 17, encAnalogA = 34, encAnalogB = 35;
const uint16_t freqMotor = 5000, freqServo = 50;
const uint8_t channel0 = 0, channel1 = 1, channel2 = 2;
const uint8_t resolutionMotor = 8, resolutionServo = 16;
const uint16_t countLow = 1634, countHigh = 8187;
const float minServo = 0, maxServo = 180, stepServo = 18;

volatile int16_t encSteps = 0;
uint16_t loopDelay = 100;

VL53L0X sensor;

void IRAM_ATTR encARising() {
    encSteps += (digitalRead(encB)) ? 1 : -1;
}
void IRAM_ATTR IRAM_ATTR encAFalling() {
    encSteps += (!digitalRead(encB)) ? 1 : -1;
}
void IRAM_ATTR encBRising() {
    encSteps += (!digitalRead(encA)) ? 1 : -1;
}
void IRAM_ATTR IRAM_ATTR encBFalling() {
    encSteps += (digitalRead(encA)) ? 1 : -1;
}

int16_t encSteps2 = 0;
uint16_t encThreshold = 1400;

uint8_t getState() {
    bool encA = (analogRead(encAnalogA) > encThreshold);
    bool encB = (analogRead(encAnalogB) > encThreshold);
    if(encA && encB) {
        return 0;
    }
    if(encA && !encB) {
        return 1;
    }
    if(!encA && !encB) {
        return 2;
    }
    if(!encA && encB) {
        return 3;
    }
    return 0;
}

void taskOne( void * parameter ) {
    for(;;) {
        static uint8_t prevEncState = getState();
        uint8_t encState = getState();

        if((encState == (prevEncState + 1)) || (encState == (prevEncState - 3))) {
            encSteps2++;
        }
        else if((encState == (prevEncState - 1)) || (encState == (prevEncState + 3))){
            encSteps2--;
        }
        prevEncState = encState;
        
        delay(2);
    }
}

void setup() {
    Serial.begin(115200);
    ledcSetup(channel0, freqMotor, resolutionMotor);
    ledcSetup(channel1, freqMotor, resolutionMotor);
    ledcSetup(channel2, freqServo, resolutionServo);
    ledcAttachPin(motorA, channel0);
    ledcAttachPin(motorB, channel1);
    ledcAttachPin(servoOut, channel2);

    pinMode(encA, INPUT_PULLUP);
    pinMode(encB, INPUT_PULLUP);

    attachInterrupt(encA, encARising, RISING);
    attachInterrupt(encA, encAFalling, FALLING);
    attachInterrupt(encB, encBRising, RISING);
    attachInterrupt(encB, encBFalling, FALLING);

    Wire.begin();

    sensor.setTimeout(500);
    if (!sensor.init())
    {
        Serial.println("Failed to detect and initialize sensor!");
        while (1) {}
    }

    // Start continuous back-to-back mode (take readings as
    // fast as possible).  To use continuous timed mode
    // instead, provide a desired inter-measurement period in
    // ms (e.g. sensor.startContinuous(100)).
    sensor.startContinuous();

    xTaskCreate(
        taskOne,          /* Task function. */
        "TaskOne",        /* String with name of task. */
        10000,            /* Stack size in bytes. */
        NULL,             /* Parameter passed as input of the task */
        1,                /* Priority of the task. */
        NULL);            /* Task handle. */
}

void loop() {
    uint16_t speed = analogRead(analogIn);  // 0-4095

    if(speed > 2048) {
        ledcWrite(channel0, (speed - 2048)/8);
        ledcWrite(channel1, 0);
    }
    else {
        ledcWrite(channel1, (2048 - speed)/8);
        ledcWrite(channel0, 0);
    }

    static float positionServo = minServo;
    static int16_t prevEncSteps2 = 0;
    float encSpeed = (encSteps2 - prevEncSteps2) / (loopDelay * 0.001) * 2.75; // [mm/s]
    prevEncSteps2 = encSteps2;

    //printf("%u, %u\n", uint16_t(positionServo), sensor.readRangeContinuousMillimeters());
    //printf("%d, %d, %d, %d\n", digitalRead(encAnalogA), digitalRead(encAnalogB), encSteps, encSteps2);
    printf("%.0f mm/s\n", encSpeed);

    static bool directionServo = 0;
    if(directionServo) {
        if(positionServo < maxServo) {
        positionServo += stepServo;
        }
        else {
        directionServo = !directionServo;
        }
    }
    else {
        if(positionServo > minServo) {
        positionServo -= stepServo;
        }
        else {
        directionServo = !directionServo;
        }
    }
    if(positionServo > maxServo) {
        positionServo = maxServo;
    }
    else if(positionServo < minServo) {
        positionServo = minServo;
    }

    uint16_t dutyCycleServo = positionServo/maxServo*(countHigh - countLow) + countLow;
    ledcWrite(channel2, dutyCycleServo);

    //printf("%d, %d\n", analogRead(encAnalogA), analogRead(encAnalogB));

    delay(loopDelay);
}
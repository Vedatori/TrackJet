#pragma once

#include <Arduino.h>
#include <Wire.h>
#include "Preferences.h"

#include "VL53L0X.h"
#include "MPU6050_light.h"

#include "SerialPWM.h"
#include "WiFiCaptain.h"
#include "QuadEncoder.h"
#include "SemiIntelligentServo.h"

namespace TJ {

const uint16_t PWM_FREQUENCY = 1000;

const uint8_t REG_DAT = 13;
const uint8_t REG_CLK = 14;
const uint8_t REG_LATCH = 12;
const uint8_t REG_OE = 25;
const uint8_t I2C_SDA = 21;
const uint8_t I2C_SCL = 22;

const uint8_t ENC_A = 15;
const uint8_t ENC_B = 5;
const uint8_t ENC_SW = 23;
const uint8_t BUTTON = 18;
const uint8_t LIDAR = 19;
const uint8_t ADC_MUX = 39;
const adc1_channel_t ADC_CH = ADC1_CHANNEL_3;

#define SERVO_COUNT 3
const uint8_t SERVO[SERVO_COUNT] = {27, 32, 33};

const float MOTOR_SPEED_FILTER_UPDATE_COEF = 0.15;

const uint8_t FREQ_PWM_THRESHOLD = 40;
const uint16_t controlPeriod = 10;  // [ms]
const char STORAGE_NAMESPACE[] = "TrackJet";
const uint16_t communicationTimeout = 1000;
const uint16_t lettersSwapTimeout = 500;
const uint16_t lettersBlankTimeout = 50;
const uint16_t lettersSweepTimeout =  100;

extern SerialPWM serialPWM;
extern QuadEncoder quadEnc;
extern SemiIntelligentServo servo[SERVO_COUNT];
extern MPU6050 mpu;
extern VL53L0X lidar;
void updatePWM(void * param);
void handleRot();
void handleSW();
}

enum shiftRegPins {
    MOTA1, MOTA2, MOTB1, MOTB2, STEP_EN, STEP_MODE, MOT1, MOT2, MOT3, MOT4, 
    XSHUT, LED2, LED1, MUXA, MUXB, MUXC
};

class TrackJetClass {
    bool beginCalled = false;
    int8_t motorsSpeed[3];
    float motorsSpeedFiltered[3];
    uint8_t gyroStatus = 0; // 0-disabled, 1-running, 2-calibrating
    float gyroYPR[3];
    float gyroOffsets[3];
    float accelOffsets[3];
    uint8_t lidarStatus = 0;    // 0-disabled, 1-running
    uint16_t lidarDist = 0;
    Preferences preferences;
    uint32_t prevCommunicationTime = 0;
    bool connectionEnabled = false;
    bool connectionActive = false;
    uint32_t beepingEnd = 0;
    String displayTextBuffer;

public:
    TrackJetClass();
    void begin();

    bool buttonRead();
    uint16_t potentiometerRead();
    bool encoderReadButton();
    bool encoderReadButtonPulse();
    int16_t encoderRead();
    void encoderReset();

    void motorsSetSpeed(const int8_t speed, const int8_t index);
    void motorsUpdateSpeed();
    void controlMovement(const int8_t joystickX, const int8_t joystickY);

    void servoSetPosition(uint8_t servoID, float position);     // servoID 0, 1, 2; position 0-180 [°]
    void servoSetSpeed(uint8_t servoID, float speed);    // speed 0-600 [°/s]

    void buzzerBeep(const uint16_t length);
    
    uint8_t gyroGetStatus();
    float gyroAngleYPR(uint8_t index) ;
    void gyroCalibrate();
    void gyroUpdate();
    void printOffsets() {
        printf("offsets: %f %f %f %f %f %f\n", gyroOffsets[0], gyroOffsets[1], gyroOffsets[2], accelOffsets[0], accelOffsets[1], accelOffsets[2]);
    }

    uint16_t analogRead(uint8_t pin);

    uint16_t lidarDistance();
    void lidarUpdate();

    void displaySingle(uint8_t row, uint8_t col, int8_t value);
    void displayAll(int8_t value);
    void display(uint8_t state[][DISP_COLS]);
    void displayDigit(const uint8_t digit);
    void displayChar(const char letter, int8_t sweepRight = 0, int8_t sweepDown = 0);
    void displayText(String text = "", bool sweep = true);
    bool displayIsBusy();

    void startWiFiCaptain(String ssid, String password);
    void checkConnection();
    String commandGet();
    String commandGetIndexed(uint8_t index);
    void commandClear();
    void commandSend(String command);
};

extern TrackJetClass TrackJet;

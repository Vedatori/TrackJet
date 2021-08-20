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
#include "Music.h"


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
const uint8_t BUZZER = 17;
const uint8_t ADC_MUX = 39;
const adc1_channel_t ADC_CH_COM = ADC1_CHANNEL_3;
const adc1_channel_t ADC_CH_ENC_FR = ADC1_CHANNEL_4;
const adc1_channel_t ADC_CH_ENC_RL = ADC1_CHANNEL_5;
const adc1_channel_t ADC_CH_ENC_RR = ADC1_CHANNEL_6;
const adc1_channel_t ADC_CH_ENC_FL = ADC1_CHANNEL_7;

#define SERVO_COUNT 3
const uint8_t SERVO[SERVO_COUNT] = {27, 16, 36};
const uint8_t SERVO_CHANNEL[SERVO_COUNT] = {0, 1, 2};
const uint8_t BUZZER_CHANNEL = 3;

const float MOTOR_SPEED_FILTER_UPDATE_COEF = 0.15;
const float BATT_VOLTAGE_UPDATE_COEF = 0.05;
const float BATT_VOLTAGE_CUTOFF = 3.4;    // [V]
const float BATT_VOLTAGE_RESET = 3.6;     // [V]

const uint16_t encThresholdInit = 1600;

const uint8_t FREQ_PWM_THRESHOLD = 40;
const uint16_t controlPeriod = 10;  // [ms]
const uint16_t statusSendPeriod = 500; // [ms]
const uint16_t melodyHandlePeriod = 40; //[ms]
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
void updateEnc(void * param);
uint8_t encGetState(uint8_t encID);
void handleRot();
void handleSW();
}

enum shiftRegPins {
    MOT1A, MOT1B, MOT2A, MOT2B, STEP_EN, STEP_MODE, MOT3, MOT4, MOT5, MOT6, 
    XSHUT, LED2, LED1, MUXA, MUXB, MUXC
};

enum adcMuxPins {
    LINE_SENSOR_RIGHT, NC, LINE_SENSOR_LEFT, SERVO_CURR_1, SERVO_CURR_2, SERVO_CURR_3, BAT_VOLT, POTENTIOMETER
};

class TrackJetClass {
    bool beginCalled = false;
    int8_t motorsSpeed[2];
    //float motorsSpeedFiltered[2];
    uint8_t gyroStatus = 0; // 0-disabled, 1-running, 2-calibrating
    float gyroYPR[3];
    float gyroOffsets[3];
    float accelOffsets[3];
    uint16_t analogReadData[8];
    uint8_t lidarStatus = 0;    // 0-disabled, 1-running
    uint16_t lidarDist = 0;
    Preferences preferences;
    uint32_t prevCommunicationTime = 0;
    bool connectionEnabled = false;
    bool connectionActive = false;
    String displayTextBuffer;
    float battVoltageFiltered = 3.7;
    float battPercentFiltered = 50;
    bool battCutoff = false;    // false-high V., true-low V.

    bool melodyPlaying = false;
    bool melodyPause = false;
    int melodyTempo = 180;
    int * melody;
    int melodythisNote = 0;
    int melodySize = 0;
    unsigned long melodyLastMillis = 0;

public:
    uint16_t encThreshold[4];   // 0-FL, 1-RL, 2-FR, 3-RR
    int16_t encSteps[2];    // 0-Left, 1-right
    TrackJetClass();
    void begin();

    bool buttonRead();
    uint16_t potentiometerRead();   // 0-100
    bool encoderReadButton();
    bool encoderReadButtonPulse();
    int16_t encoderRead();
    void encoderReset();

    void motorSetSpeed(const int8_t index, int8_t speed);
    void motorsUpdateSpeed();
    void controlMovement(const int8_t joystickX, const int8_t joystickY);

    int16_t encoderGetSteps(uint8_t encID);
    float encoderGetDistance(uint8_t encID);
    float encoderGetSpeed(uint8_t encID);

    void servoSetPosition(uint8_t servoID, float position);     // servoID 1, 2, 3; position 0-180 [°]
    void servoSetSpeed(uint8_t servoID, float speed);    // speed 0-600 [°/s]
    float servoGetPosition(uint8_t servoID);
    bool servoMoving(uint8_t servoID);

    void soundTone(float freq = 1000);
    void soundEnd();
    void playMelody(int * aMelody, int size, int tempo = 180);
    void stopMelody();
    void handleMelody();

    uint8_t gyroGetStatus();
    float gyroAngleYPR(uint8_t index) ;
    void gyroCalibrate();
    void gyroUpdate();
    void printOffsets() {
        printf("offsets: %f %f %f %f %f %f\n", gyroOffsets[0], gyroOffsets[1], gyroOffsets[2], accelOffsets[0], accelOffsets[1], accelOffsets[2]);
    }

    void updateAnalogMux();

    float battVolt();
    float battPercentCalc(float voltage);
    uint8_t battPercent();
    void handleLowBatt();

    uint16_t lineRead(uint8_t index);

    uint16_t lidarDistance();
    void lidarUpdate();

    void ledWrite(uint8_t id, bool state);
    void ledWriteAnalog(uint8_t id, uint8_t brightness);    // brightness 0-100

    void displaySingle(uint8_t row, uint8_t col, bool state);
    void displaySingleAnalog(uint8_t row, uint8_t col, int8_t brightness);   // brightness 0-12
    void displayAll(int8_t value);
    void display(uint8_t state[][DISP_COLS]);
    void displayDigit(const uint8_t digit);
    void displayChar(const char letter, int8_t sweepRight = 0, int8_t sweepDown = 0);
    void displayText(String text = "", bool sweep = true);
    bool displayIsBusy();

    void startWiFiCaptain(String name="", String password="");
    void checkConnection();
    String commandGet();
    String commandGetIndexed(uint8_t index);
    void commandClear();
    void commandSend(String command);
    void msgSend(String type, String msg);
    void internCommandHandle();
    void sendStatus();

    void encoderCalibrate(uint16_t duration);
};

extern TrackJetClass TrackJet;

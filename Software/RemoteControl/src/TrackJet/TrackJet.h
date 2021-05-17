#pragma once

#include "SerialPWM.h"
#include <Arduino.h>
#include "Preferences.h"

#include "WiFiCaptain.h"

namespace TJ {

const uint16_t PWM_FREQUENCY = 1000;  //330
extern const uint8_t PWM_MAX;

const uint8_t REG_DAT = 13;
const uint8_t REG_CLK = 14;
const uint8_t REG_LATCH = 12;
const uint8_t REG_OE = 25;
const uint8_t I2C_SDA = 21;
const uint8_t I2C_SCL = 22;
const uint8_t ENC_SW = 23;

const float MOTOR_SPEED_FILTER_UPDATE_COEF = 0.15;
const uint8_t FREQ_PWM_THRESHOLD = 40;
const char STORAGE_NAMESPACE[] = "TrackJet";
const uint16_t communicationTimeout = 1000;
const uint16_t lettersSwapTimeout = 500;
const uint16_t lettersBlankTimeout = 50;
const uint16_t lettersSweepTimeout =  150;

extern SerialPWM serialPWM;
void updatePWM(void * param);
}

enum shiftRegPins {
    D0, D1, D2, D3, D4, D5, D6, D7, D8, D9, D10, D11, D12, D13, D14, D15, D16, 
    D17, D18, D19, D20, D21, D22, D23, D24, D25, 
    TR_OUT26, TR_OUT27, TR_OUT28, TR_OUT29, TR_OUT30, TR_OUT31, TR_OUT32,
};

class TrackJetClass {
    bool beginCalled = false;
    bool buttonPressed = false;
    int8_t motorsSpeed[3];
    float motorsSpeedFiltered[3];
    bool gyroEnabled = false;
    float gyroYPR[3];
    int16_t gyroOffsets[3];
    Preferences preferences;
    uint32_t prevCommunicationTime = 0;
    bool connectionEnabled = false;
    bool connectionActive = false;
    uint32_t shootingEnd = 0;
    uint32_t beepingEnd = 0;
    String displayTextBuffer;

public:
    TrackJetClass();
    bool getButton();
    void setButton(bool pressed);
    void setFlashLight(int16_t brightness);
    void setMotorsSpeed(const int8_t speed, const int8_t index);
    void updateMotorsSpeed();
    void controlMovement(const int8_t joystickX, const int8_t joystickY);
    void canonShoot(const uint16_t length);
    void buzzerBeep(const uint16_t length);
    void begin();
    
    bool gyroGetEnabled();
    float gyroData(uint8_t index) ;
    void gyroCalibrate();
    void gyroUpdate();
    void printOffsets() {
        printf("offsets: %d %d %d\n", gyroOffsets[0], gyroOffsets[1], gyroOffsets[2]);
    }

    void displayDigit(const uint8_t digit);
    void displayChar(const char letter, int8_t sweepRight = 0, int8_t sweepDown = 0);
    void displayText(String text = "", bool sweep = true);
    bool isDisplayingText();

    void startWiFiCaptain(String ssid, String password);
    void checkConnection();
    String commandGet();
    String commandGetIndexed(uint8_t index);
    void commandClear();
    void commandSend(String command);
};

extern TrackJetClass TrackJet;

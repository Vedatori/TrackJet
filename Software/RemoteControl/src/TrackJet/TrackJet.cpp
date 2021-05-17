#include "TrackJet.h"
#include "Wire.h"
#include "Preferences.h"

#include "DisplayCharactersSet.h"
#include "WiFiCaptain.h"

const uint8_t TR::PWM_MAX = rb::SerialPWM::resolution();
rb::SerialPWM TR::serialPWM(TR::PWM_CHANNELS, { TR::REG_DAT }, TR::REG_LATCH, TR::REG_CLK, -1, TR::PWM_FREQUENCY);
int8_t TR::pwm_index[] = {0, 3, 2, 29, 28, 31, 30, 25, 24, 27, 26, 21, 20, 23, 22, 17, 16, 19, 18, 13, 12, 15, 14, 9, 8, 11, 10, 5, 4, 7, 6, 1, 0};

void TR::setPWM(rb::SerialPWM::value_type& channel, int8_t power) {
    if(power > TR::PWM_MAX)
        power = TR::PWM_MAX;
    else if(power < 0)
        power = 0;

    channel = power;
}

void TR::updatePWM(void * param) {
    for(;;) {
        TR::serialPWM.update();

        TrackJet.checkConnection();
        TrackJet.updateMotorsSpeed();
        TrackJet.displayText();

        vTaskDelay(5);
    }
}

TrackJetClass::TrackJetClass(void) {
    for(uint8_t i = 0; i < 3; ++i) {
        motorsSpeed[i] = 0;
        motorsSpeedFiltered[i] = 0;
        gyroYPR[i] = 0;
        gyroOffsets[i] = 0;
    }
}

bool TrackJetClass::getButton() {
    return buttonPressed;
}
void TrackJetClass::setButton(bool pressed) {
    TrackJet.buttonPressed = pressed;
}
void TrackJetClass::setMotorsSpeed(const int8_t speed, const int8_t index) {
    if(index == 0 || index == 1) {
        if(speed < -100)
            motorsSpeed[index] = -100;
        else if(speed > 100)
            motorsSpeed[index] = 100;
        else
            motorsSpeed[index] = speed;
    }
    else if(index == 2) {
        if(speed < 0)
            motorsSpeed[index] = 0;
        else if(speed > 100)
            motorsSpeed[index] = 100;
        else
            motorsSpeed[index] = speed;
    }
    
}
void TrackJetClass::updateMotorsSpeed() {
    if(connectionEnabled == true && connectionActive == false) {
        for(uint8_t i = 0; i < 3; ++i) {
            motorsSpeed[i] = 0;
        }
    }
    if(shootingEnd != 0 && millis() > shootingEnd) {
        motorsSpeed[2] = 0;
        shootingEnd = 0;
    }
    if(beepingEnd != 0 && millis() > beepingEnd) {
        TR::setPWM(TR::serialPWM[TR::pwm_index[TR_OUT27]], 0);
        beepingEnd = 0;
    }

    // Switch PFM/PWM step up converter mode
    if(motorsSpeed[0] != 0 || motorsSpeed[1] != 0 || motorsSpeed[2]) {
        TR::setPWM(TR::serialPWM[TR::pwm_index[6]], 0);     // PWM mode - High power
    }
    else {
        TR::setPWM(TR::serialPWM[TR::pwm_index[6]], 100);   // PFM mode - Low power, high efficiency
    }

    // Filter motor speeds and turn off when connection not active
    for(uint8_t i = 0; i < 3; ++i) {
        motorsSpeedFiltered[i] = motorsSpeed[i] * TR::MOTOR_SPEED_FILTER_UPDATE_COEF + motorsSpeedFiltered[i] * (1 - TR::MOTOR_SPEED_FILTER_UPDATE_COEF);
    }
    
    // Left motor
    if(motorsSpeed[0] > 0) {
        TR::setPWM(TR::serialPWM[TR::pwm_index[3]], (int)motorsSpeedFiltered[0]);
        TR::setPWM(TR::serialPWM[TR::pwm_index[4]], 0);
    }
    else {
        TR::setPWM(TR::serialPWM[TR::pwm_index[3]], 0);
        TR::setPWM(TR::serialPWM[TR::pwm_index[4]], -(int)motorsSpeedFiltered[0]);
    }

    // Right motor
    if(motorsSpeed[1] > 0) {
        TR::setPWM(TR::serialPWM[TR::pwm_index[1]], 0);
        TR::setPWM(TR::serialPWM[TR::pwm_index[2]], (int)motorsSpeedFiltered[1]);
    }
    else {
        TR::setPWM(TR::serialPWM[TR::pwm_index[1]], -(int)motorsSpeedFiltered[1]);
        TR::setPWM(TR::serialPWM[TR::pwm_index[2]], 0);
    }
}

void TrackJetClass::controlMovement(const int8_t joystickX, const int8_t joystickY) {
    prevCommunicationTime = millis();
    int16_t engineLeftSpeed = 0;
    int16_t engineRightSpeed = 0;

    engineLeftSpeed = (joystickY + ((joystickY >= 0) ? 1 : -1) * joystickX);
    engineRightSpeed = (joystickY - ((joystickY >= 0) ? 1 : -1) * joystickX);

    engineLeftSpeed = constrain(engineLeftSpeed, -100, 100);
    engineRightSpeed = constrain(engineRightSpeed, -100, 100);

    setMotorsSpeed(engineLeftSpeed, 0);
    setMotorsSpeed(engineRightSpeed, 1);
}

void TrackJetClass::canonShoot(const uint16_t length) {
    setMotorsSpeed(100, 2);
    shootingEnd = millis() + length;
}

void TrackJetClass::buzzerBeep(const uint16_t length) {
    TR::setPWM(TR::serialPWM[TR::pwm_index[TR_OUT27]], 100);
    beepingEnd = millis() + length;
}

void TrackJetClass::begin() {
    beginCalled = true;
    Serial.begin(115200);
    
    preferences.begin("TrackJet", false);
    gyroOffsets[0] = preferences.getInt("counter", 0);
    gyroOffsets[1] = preferences.getInt("gyroOffPitch", 0);
    gyroOffsets[2] = preferences.getInt("gyroOffRoll", 0);
    bool prefsPresent = preferences.getBool("prefsPresent", false);
    preferences.end();

    //Wire.begin(TR::I2C_SDA, TR::I2C_SCL);
    
    TR::setPWM(TR::serialPWM[TR::pwm_index[5]], 100);   // Turn on motor step up
    xTaskCreate(TR::updatePWM, "updatePWM", 10000 , (void*) 0, 1, NULL);
    pinMode(TR::REG_OE, OUTPUT);
    digitalWrite(TR::REG_OE, 0);    // Enable shift registers output
}

bool TrackJetClass::gyroGetEnabled() {
    return gyroEnabled;
}
float TrackJetClass::gyroData(uint8_t index) {
    float output = gyroYPR[index];
    if(index == 1) {
        output -= 20;
        if(output < -180) {
            output += 360;
        }
        output = -output;
    }
    return output;
}
void TrackJetClass::gyroCalibrate() {
    //gyroCalibration(gyroOffsets);

    preferences.begin("TrackJet", false);
    preferences.putInt("counter", gyroOffsets[0]);
    preferences.putInt("gyroOffPitch", gyroOffsets[1]);
    preferences.putInt("gyroOffRoll", gyroOffsets[2]);
    preferences.putBool("prefsPresent", true);
    preferences.end();
}
void TrackJetClass::gyroUpdate() {
    //updateGyroData(gyroYPR);
}

void TrackJetClass::displayDigit(const uint8_t digit) {
    if(digit > 9) {
        return;
    }
    displayChar(digit + 48);
}

void TrackJetClass::displayChar(const char letter, int8_t sweepRight, int8_t sweepDown) {
    /*
    uint8_t letterID = letter;
    if(letterID >= 97 && letterID <= 122) {
        letterID = letterID - 32;   // change to upper case letters
    }
    if(letterID < 40 || letterID > 90) {
        return;     // out of defined letters
    }
    letterID = letterID - 40;   // move to beginning of character set array
    for(uint8_t i = 0; i < 5; ++i) {
        for(uint8_t j = 0; j < 5; ++j) {
            int16_t ledIndex = (i + sweepDown)*5 + j + sweepRight + 1;
            if((i + sweepDown) >= 0 && (i + sweepDown) < 5 && (j + sweepRight) >= 0 && (j + sweepRight) < 5) {
                trrSetLedDigital(ledIndex, characterSet[letterID][i][j]);
            }
        }
    }*/
    ;
}

void TrackJetClass::displayText(String text, bool sweep) {
    /*static uint8_t letterIndex;
    static uint32_t prevMoveTime = 0;
    static bool sweeping;
    if(text.length() > 0) {
        displayTextBuffer = text;
        letterIndex = 0;
        prevMoveTime = millis();
        sweeping = sweep;
    }
    if(prevMoveTime == 0) {
        return;
    }

    if(sweeping) {
        if((millis() > prevMoveTime + TR::lettersSweepTimeout) && letterIndex <= (displayTextBuffer.length()*5)) {
            prevMoveTime = millis();

            uint8_t letterID1 = displayTextBuffer[letterIndex / 5];
            uint8_t letterID2 = displayTextBuffer[letterIndex / 5 + 1];

            trrSetLedAllDigital(0);
            displayChar(letterID1, -(letterIndex % 5));
            displayChar(letterID2, (5 - (letterIndex % 5)));

            if(letterIndex / 5 >= displayTextBuffer.length()) {
                displayTextBuffer = "";
                prevMoveTime = 0;
            }
            ++letterIndex;
        }
    }
    else {
        if(millis() > prevMoveTime + TR::lettersSwapTimeout - TR::lettersBlankTimeout) {
            trrSetLedAllDigital(0);
        }
        if((millis() > prevMoveTime + TR::lettersSwapTimeout) && letterIndex <= displayTextBuffer.length()) {
            prevMoveTime = millis();

            trrSetLedAllDigital(0);
            displayChar(displayTextBuffer[letterIndex]);

            if(letterIndex >= displayTextBuffer.length()) {
                displayTextBuffer = "";
                prevMoveTime = 0;
            }
            ++letterIndex;
        }
    }
    */
   ;
}

bool TrackJetClass::isDisplayingText() {
    return !displayTextBuffer.isEmpty();
}

void TrackJetClass::startWiFiCaptain(String ssid, String password) {
    if(!beginCalled) {
        begin();
    }
    setApCredentials(ssid, password);
    wifiCaptInit();
    connectionEnabled = true;
}

void TrackJetClass::checkConnection() {
    if(!connectionEnabled) {
        return;
    }
    if(millis() > prevCommunicationTime + TR::communicationTimeout) {
        connectionActive = false;
    }
    else {
        connectionActive = true;
    }
}

String TrackJetClass::commandGet() {
    String command = String(commandGetCaptain());
    command.toLowerCase();
    return command;
}

String TrackJetClass::commandGetIndexed(uint8_t index) {
    char commandBuffer[64];
    sprintf(commandBuffer, commandGetCaptain());
    const char delimiter[2] = " ";
    char *token;
    token = strtok((char *)commandBuffer, delimiter);
    for(uint8_t i = 0; i < index; ++i) {
        token = strtok(NULL, delimiter);
    }
    String command = String(token);
    command.toLowerCase();
    return command;
}

void TrackJetClass::commandClear() {
    commandClearCaptain();
}

void TrackJetClass::commandSend(String command) {
    commandSendCaptain(command);
}

TrackJetClass TrackJet;

#include "TrackJet.h"
#include "Wire.h"
#include "Preferences.h"

#include "DisplayCharactersSet.h"
#include "WiFiCaptain.h"
#include "SemiIntelligentServo.h"

SerialPWM TJ::serialPWM(TJ::REG_DAT, TJ::REG_LATCH, TJ::REG_CLK, TJ::REG_OE, TJ::PWM_FREQUENCY);
QuadEncoder TJ::quadEnc(TJ::ENC_A, TJ::ENC_B, TJ::ENC_SW);
SemiIntelligentServo TJ::servo[] = {SemiIntelligentServo(TJ::SERVO[0], 0),
                                    SemiIntelligentServo(TJ::SERVO[1], 1), 
                                    SemiIntelligentServo(TJ::SERVO[2], 2)};


void TJ::updatePWM(void * param) {
    for(;;) {
        serialPWM.update();

        TrackJet.checkConnection();
        TrackJet.motorsUpdateSpeed();
        TrackJet.displayText();

        vTaskDelay(5);
    }
}
void TJ::handleRot(){
    quadEnc.updatePosition();
}
void TJ::handleSW(){
    quadEnc.lastPressed = millis();
}

TrackJetClass::TrackJetClass(void) {
    for(uint8_t i = 0; i < 3; ++i) {
        motorsSpeed[i] = 0;
        motorsSpeedFiltered[i] = 0;
        gyroYPR[i] = 0;
        gyroOffsets[i] = 0;
    }
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

    //Wire.begin(TJ::I2C_SDA, TJ::I2C_SCL);

    TJ::serialPWM.setPWM(STEP_EN, 100);   // Turn on motor step up
    display(dispWelcome);
    xTaskCreate(TJ::updatePWM, "updatePWM", 10000 , (void*) 0, 1, NULL);
    TJ::serialPWM.set_output(true);

    pinMode(TJ::BUTTON, INPUT_PULLUP);
    attachInterrupt(TJ::ENC_A, TJ::handleRot, CHANGE);
    attachInterrupt(TJ::ENC_B, TJ::handleRot, CHANGE);
    attachInterrupt(TJ::ENC_SW, TJ::handleSW, RISING);
}

bool TrackJetClass::buttonRead() {
    // 0-not pressed, 1-pressed
    return !digitalRead(TJ::BUTTON);
}
uint16_t TrackJetClass::potentiometerRead() {
    return 0;
}
bool TrackJetClass::encoderReadButton() {
    return TJ::quadEnc.getSWPressed();
}
bool TrackJetClass::encoderReadButtonPulse() {
    return TJ::quadEnc.getSW();
}
int16_t TrackJetClass::encoderRead() {
    return TJ::quadEnc.getPosition();
}
void TrackJetClass::encoderReset() {
    TJ::quadEnc.clear();
}
void TrackJetClass::motorsSetSpeed(const int8_t speed, const int8_t index) {
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
void TrackJetClass::motorsUpdateSpeed() {
    if(connectionEnabled == true && connectionActive == false) {
        for(uint8_t i = 0; i < 3; ++i) {
            motorsSpeed[i] = 0;
        }
    }
    if(beepingEnd != 0 && millis() > beepingEnd) {
        //TJ::setPWM(TJ::serialPWM[TJ::pwm_index[TJ_OUT27]], 0);
        beepingEnd = 0;
    }

    // Switch PFM/PWM step up converter mode
    if(motorsSpeed[0] != 0 || motorsSpeed[1] != 0 || motorsSpeed[2]) {
        TJ::serialPWM.setPWM(STEP_MODE, 0);     // PWM mode - High power
        
    }
    else {
        TJ::serialPWM.setPWM(STEP_MODE, 100);   // PFM mode - Low power, high efficiency
    }

    // Filter motor speeds and turn off when connection not active
    for(uint8_t i = 0; i < 3; ++i) {
        motorsSpeedFiltered[i] = motorsSpeed[i] * TJ::MOTOR_SPEED_FILTER_UPDATE_COEF + motorsSpeedFiltered[i] * (1 - TJ::MOTOR_SPEED_FILTER_UPDATE_COEF);
    }
    
    // Left motor
    if(motorsSpeedFiltered[0] > 0) {
        TJ::serialPWM.setPWM(MOTB1, (int)motorsSpeedFiltered[0]);
        TJ::serialPWM.setPWM(MOTB2, 0);
    }
    else {
        TJ::serialPWM.setPWM(MOTB1, 0);
        TJ::serialPWM.setPWM(MOTB2, -(int)motorsSpeedFiltered[0]);
    }

    // Right motor
    if(motorsSpeedFiltered[1] > 0) {
        TJ::serialPWM.setPWM(MOTA1, 0);
        TJ::serialPWM.setPWM(MOTA2, (int)motorsSpeedFiltered[1]);
    }
    else {
        TJ::serialPWM.setPWM(MOTA1, -(int)motorsSpeedFiltered[1]);
        TJ::serialPWM.setPWM(MOTA2, 0);
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

    motorsSetSpeed(engineLeftSpeed, 0);
    motorsSetSpeed(engineRightSpeed, 1);
}

void TrackJetClass::servoSetPosition(uint8_t servoID, float position) {
    TJ::servo[0].setPosition(position);
}
void TrackJetClass::servoDriveToPosition(uint8_t servoID, float position, float speed) {
    ;
}

void TrackJetClass::buzzerBeep(const uint16_t length) {
    //TJ::setPWM(TJ::serialPWM[TJ::pwm_index[TJ_OUT27]], 100);
    beepingEnd = millis() + length;
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

void TrackJetClass::displaySingle(uint8_t row, uint8_t col, int8_t value) {
    TJ::serialPWM.setDispSingle(row, col, value);
}
void TrackJetClass::displayAll(int8_t value) {
    TJ::serialPWM.setDispAll(value);
}
void TrackJetClass::display(uint8_t state[][DISP_COLS]) {
    TJ::serialPWM.setDisp(state);
}
void TrackJetClass::displayDigit(const uint8_t digit) {
    if(digit > 9) {
        return;
    }
    displayChar(digit + 48);
}

void TrackJetClass::displayChar(const char letter, int8_t sweepRight, int8_t sweepDown) {
    
    uint8_t letterID = letter;
    if(letterID >= 97 && letterID <= 122) {
        letterID = letterID - 32;   // change to upper case letters
    }
    if(letterID < 40 || letterID > 90) {
        return;     // out of defined letters
    }
    letterID = letterID - 40;   // move to beginning of character set array
    for(uint8_t row = 0; row < 5; ++row) {
        for(uint8_t col = 0; col < 5; ++col) {
            if((row + sweepDown) >= 0 && (row + sweepDown) < DISP_ROWS && (col + sweepRight) >= 0 && (col + sweepRight) < DISP_COLS) {
                TJ::serialPWM.setDispSingle(row + sweepDown, col + sweepRight, DISP_PWM_RESOLUTION*characterSet[letterID][row][col]);
            }
        }
    }
}

void TrackJetClass::displayText(String text, bool sweep) {
    static uint8_t letterIndex;
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
        if((millis() > prevMoveTime + TJ::lettersSweepTimeout) && letterIndex <= (displayTextBuffer.length()*5)) {
            prevMoveTime = millis();

            uint8_t letterID1 = displayTextBuffer[letterIndex / 5];
            uint8_t letterID2 = displayTextBuffer[letterIndex / 5 + 1];

            displayAll(0);
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
        if(millis() > prevMoveTime + TJ::lettersSwapTimeout - TJ::lettersBlankTimeout) {
            displayAll(0);
        }
        if((millis() > prevMoveTime + TJ::lettersSwapTimeout) && letterIndex <= displayTextBuffer.length()) {
            prevMoveTime = millis();

            displayAll(0);
            displayChar(displayTextBuffer[letterIndex]);

            if(letterIndex >= displayTextBuffer.length()) {
                displayTextBuffer = "";
                prevMoveTime = 0;
            }
            ++letterIndex;
        }
    }
}

bool TrackJetClass::displayIsBusy() {
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
    if(millis() > prevCommunicationTime + TJ::communicationTimeout) {
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

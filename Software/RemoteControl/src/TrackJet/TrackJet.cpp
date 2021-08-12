#include "TrackJet.h"
#include "Wire.h"
#include "Preferences.h"

#include "VL53L0X.h"
#include "MPU6050_light.h"

#include "DisplayCharactersSet.h"
#include "WiFiCaptain.h"
#include "SemiIntelligentServo.h"

SerialPWM TJ::serialPWM(TJ::REG_DAT, TJ::REG_LATCH, TJ::REG_CLK, TJ::REG_OE, TJ::PWM_FREQUENCY);
QuadEncoder TJ::quadEnc(TJ::ENC_A, TJ::ENC_B, TJ::ENC_SW);
SemiIntelligentServo TJ::servo[] = {SemiIntelligentServo(TJ::SERVO[0], SERVO_CHANNEL[0]),
                                    SemiIntelligentServo(TJ::SERVO[1], SERVO_CHANNEL[1]), 
                                    SemiIntelligentServo(TJ::SERVO[2], SERVO_CHANNEL[2])};
MPU6050 TJ::mpu(Wire);
VL53L0X TJ::lidar;

void TJ::updatePWM(void * param) {
    for(;;) {
        TrackJet.updateAnalogMux();
        serialPWM.update();

        TrackJet.checkConnection();
        TrackJet.motorsUpdateSpeed();
        TrackJet.displayText();

        for(uint8_t servoID = 0; servoID < SERVO_COUNT; ++servoID) {
            TJ::servo[servoID].updatePWM();
        }

        TrackJet.gyroUpdate();
        TrackJet.lidarUpdate();

        TrackJet.internCommandHandle();

        vTaskDelay(TJ::controlPeriod);
    }
}
void TJ::updateEnc(void * parameter) {
    for(;;) {
        static uint8_t prevEncState = encGetState();
        uint8_t encState = encGetState();

        if((encState == (prevEncState + 1)) || (encState == (prevEncState - 3))) {
            TrackJet.encSteps2++;
        }
        else if((encState == (prevEncState - 1)) || (encState == (prevEncState + 3))){
            TrackJet.encSteps2--;
        }
        prevEncState = encState;
        
        delay(2);
    }
}
uint8_t TJ::encGetState() {
    bool encA = (adc1_get_raw(TJ::ADC_CH_ENC_FL) > TJ::encThreshold);
    bool encB = (adc1_get_raw(TJ::ADC_CH_ENC_RL) > TJ::encThreshold);
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
        accelOffsets[i] = 0;
        analogReadData[i] = 0;
        analogReadData[i + 4] = 0;
    }
}

void TrackJetClass::begin() {
    beginCalled = true;
    Serial.begin(115200);

    Wire.begin(TJ::I2C_SDA, TJ::I2C_SCL);

    if(TJ::mpu.begin() == 0) {
        gyroStatus = 1;

        preferences.begin("TrackJet", false);
        gyroOffsets[0] = preferences.getFloat("gyroOffYaw", 0);
        gyroOffsets[1] = preferences.getFloat("gyroOffPitch", 0);
        gyroOffsets[2] = preferences.getFloat("gyroOffRoll", 0);
        
        accelOffsets[0] = preferences.getFloat("accOffX", 0);
        accelOffsets[1] = preferences.getFloat("accOffY", 0);
        accelOffsets[2] = preferences.getFloat("accOffZ", 0);
        preferences.end();

        TJ::mpu.setGyroOffsets(gyroYPR[1], gyroYPR[2], gyroYPR[0]);
        TJ::mpu.setAccOffsets(accelOffsets[0], accelOffsets[1], accelOffsets[2]);
    }
    else
        Serial.printf("Gyroscope MPU6050 not connected.\n");

    pinMode(TJ::LIDAR, OUTPUT);
    digitalWrite(TJ::LIDAR, 1);
    TJ::lidar.setTimeout(500);
    if (TJ::lidar.init()) {
        lidarStatus = 1;
        TJ::lidar.startContinuous(TJ::controlPeriod);
    }
    else
        Serial.printf("Lidar VL53L0X not connected.\n");

    TJ::serialPWM.setPWM(STEP_EN, 100);   // Turn on motor step up
    display(dispWelcome);
    xTaskCreatePinnedToCore(TJ::updatePWM, "updatePWM", 10000 , (void*) 0, 1, NULL, 1);
    xTaskCreate(TJ::updateEnc, "updateEnc", 10000 , (void*) 0, 1, NULL);
    TJ::serialPWM.set_output(true);

    pinMode(TJ::BUTTON, INPUT_PULLUP);
    attachInterrupt(TJ::ENC_A, TJ::handleRot, CHANGE);
    attachInterrupt(TJ::ENC_B, TJ::handleRot, CHANGE);
    attachInterrupt(TJ::ENC_SW, TJ::handleSW, RISING);
    pinMode(TJ::BUZZER, OUTPUT);

    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(TJ::ADC_CH_COM, ADC_ATTEN_DB_11);
    adc1_config_channel_atten(TJ::ADC_CH_ENC_FR, ADC_ATTEN_DB_11);
    adc1_config_channel_atten(TJ::ADC_CH_ENC_RL, ADC_ATTEN_DB_11);
    adc1_config_channel_atten(TJ::ADC_CH_ENC_RR, ADC_ATTEN_DB_11);
    adc1_config_channel_atten(TJ::ADC_CH_ENC_FL, ADC_ATTEN_DB_11);

    ledcSetup(TJ::BUZZER_CHANNEL, 1000, 10);
}

bool TrackJetClass::buttonRead() {
    // 0-not pressed, 1-pressed
    return !digitalRead(TJ::BUTTON);
}
uint16_t TrackJetClass::potentiometerRead() {
    return analogReadData[POTENTIOMETER];
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

float TrackJetClass::encoderGetSpeed() {
    static int16_t prevEncSteps2 = 0;
    static uint32_t prevTime = 0;
    float encSpeed = (encSteps2 - prevEncSteps2) / ((millis() - prevTime) * 0.001) * 2.75; // [mm/s]
    prevTime = millis();
    prevEncSteps2 = encSteps2;
    return encSpeed;
}

void TrackJetClass::servoSetPosition(uint8_t servoID, float position) {
    TJ::servo[servoID].setPosition(position);
}
void TrackJetClass::servoSetSpeed(uint8_t servoID, float speed) {
    TJ::servo[servoID].setSpeed(speed);
}

void TrackJetClass::soundNote(note_t note, uint8_t octave) {
    ledcAttachPin(TJ::BUZZER, TJ::BUZZER_CHANNEL);
    ledcWriteNote(TJ::BUZZER_CHANNEL, note, octave);
}
void TrackJetClass::soundTone(float freq) {
    ledcAttachPin(TJ::BUZZER, TJ::BUZZER_CHANNEL);
    ledcWriteTone(TJ::BUZZER_CHANNEL, freq);
}
void TrackJetClass::soundEnd() {
    ledcDetachPin(TJ::BUZZER);
}

uint8_t TrackJetClass::gyroGetStatus() {
    return gyroStatus;
}
float TrackJetClass::gyroAngleYPR(uint8_t index) {
    float output = gyroYPR[index];
    return output;
}
void TrackJetClass::gyroCalibrate() {
    if(gyroStatus == 0)
        return;
    Serial.printf("Calibration started.\n");
    gyroStatus = 2;
    TJ::mpu.calcOffsets();
    gyroStatus = 1;
    gyroOffsets[0] = TJ::mpu.getGyroZoffset();
    gyroOffsets[1] = TJ::mpu.getGyroXoffset();
    gyroOffsets[2] = TJ::mpu.getGyroYoffset();
    accelOffsets[0] = TJ::mpu.getAccXoffset();
    accelOffsets[1] = TJ::mpu.getAccYoffset();
    accelOffsets[2] = TJ::mpu.getAccZoffset();

    preferences.begin("TrackJet", false);
    preferences.putFloat("gyroOffYaw", gyroOffsets[0]);
    preferences.putFloat("gyroOffPitch", gyroOffsets[1]);
    preferences.putFloat("gyroOffRoll", gyroOffsets[2]);
    preferences.putFloat("accOffX", accelOffsets[0]);
    preferences.putFloat("accOffY", accelOffsets[1]);
    preferences.putFloat("accOffZ", accelOffsets[2]);
    preferences.end();
}
void TrackJetClass::gyroUpdate() {
    if(gyroStatus != 1)
        return;
    TJ::mpu.update();
    gyroYPR[0] = -TJ::mpu.getAngleZ();   // yaw
    gyroYPR[1] = TJ::mpu.getAngleX();   // pitch
    gyroYPR[2] = -TJ::mpu.getAngleY();   // roll
}

void TrackJetClass::updateAnalogMux() {
    static uint8_t analogReadIndex = 0;
    analogReadData[analogReadIndex] = adc1_get_raw(TJ::ADC_CH_COM);
    if(analogReadIndex == BAT_VOLT) {
        float newPercent = battPercentCalc(battVolt());
        battPercentFiltered = (1 - TJ::BATT_PERCENT_UPDATE_COEF)*battPercentFiltered + TJ::BATT_PERCENT_UPDATE_COEF*newPercent;
    }
    if(++analogReadIndex >= 8) {
        analogReadIndex = 0;
    }

    // Set analog mux for next ADC reading
    TJ::serialPWM.setPWM(MUXA, uint8_t(((analogReadIndex >> 0) & 0x01)*100));
    TJ::serialPWM.setPWM(MUXB, uint8_t(((analogReadIndex >> 1) & 0x01)*100));
    TJ::serialPWM.setPWM(MUXC, uint8_t(((analogReadIndex >> 2) & 0x01)*100));
}

float TrackJetClass::battVolt() {
    return float(analogReadData[BAT_VOLT])/4095*4/3*3.3;
}
float TrackJetClass::battPercentCalc(float voltage) {
    float percent = -740 + 200*voltage;; // Linear between 3,7~0% and 4.2~100%
    if(percent < 0)
        percent = 0;
    else if(percent > 100)
        percent = 100;
    return percent;
}
uint8_t TrackJetClass::battPercent() {
    return uint8_t(battPercentFiltered + 0.5);  // Add 0.5 to round correctly
}

uint16_t TrackJetClass::lineLeft() {
    return analogReadData[LINE_SENSOR_LEFT];
}
uint16_t TrackJetClass::lineRight() {
    return analogReadData[LINE_SENSOR_RIGHT];
}

uint16_t TrackJetClass::lidarDistance() {
    return lidarDist;
}
void TrackJetClass::lidarUpdate() {
    if(lidarStatus != 1)
        return;
    lidarDist = TJ::lidar.readRangeContinuousMillimeters();
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

void TrackJetClass::startWiFiCaptain(String name, String password) {
    if(!beginCalled) {
        begin();
    }

    String ssid_final = "TrackJet-";
    if(name.isEmpty() || name == "<your_name>") {
        ssid_final += WiFi.macAddress();
    }
    else {
        ssid_final += name;
    }
    setApCredentials(ssid_final, password);
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

void TrackJetClass::internCommandHandle() {
    if(TrackJet.commandGetIndexed(0) == "reset") {
        ESP.restart();
    }
}

TrackJetClass TrackJet;

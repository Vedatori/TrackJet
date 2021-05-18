#ifndef _QUAD_ENCODER_
#define _QUAD_ENCODER_

//#include <PetitSerial.h>

//PetitSerial PS;
//#define Serial PS   // Use PetitSerial instead of Serial.

#define ENC_DEBOUNCE_T 100  // [milliseconds]
#define ENC_REVERSE_DIR_T 300	// [milliseconds]
#define SW_DEAD_T 500	// [milliseconds]

class QuadEncoder{
	uint8_t chA;
	uint8_t chB;
	uint8_t sw;
	volatile int16_t virtualPosition;
	uint32_t encPrevTime = 0;
	uint32_t swPrevTime = 0;
	bool encEnabled = true;
	uint32_t disableBeginTime = 0;
	uint32_t encChangePrevTime = 0;
	bool currentStateCLK = 0;
	bool lastStateCLK = 0;
public:
 	uint32_t lastPressed = 0;
	QuadEncoder(uint8_t aChA, uint8_t aChB, uint8_t aSW){
		chA = aChA;
		chB = aChB;
		sw = aSW;
		pinMode(chA, INPUT_PULLUP);
		pinMode(chB, INPUT_PULLUP);
		pinMode(sw, INPUT_PULLDOWN);
		virtualPosition = 0;
	};
	int16_t getPosition(){
		return virtualPosition;
	};
	void setPosition(int16_t pos){
		virtualPosition = pos;
	}
	void disableEnc(uint16_t disableTime){
		encEnabled = false;
		disableBeginTime = millis() + disableTime;
	};
	void updatePosition(){
		if(encEnabled == false){
			if(millis() > disableBeginTime){
				disableBeginTime = 0;
				encEnabled = true;
			}
		}
		else {
			currentStateCLK = digitalRead(chA);

			// If last and current state of CLK are different, then pulse occurred
			// React to only 1 state change to avoid double count
			if (currentStateCLK != lastStateCLK  && currentStateCLK == 1){

				// If the DT state is different than the CLK state then
				// the encoder is rotating CCW so decrement
				if (digitalRead(chB) != currentStateCLK) {
					virtualPosition++;
				} else {
					// Encoder is rotating CW so increment
					virtualPosition--;
				}
			}

			// Remember last CLK state
			lastStateCLK = currentStateCLK;
		}
		
	};
	bool getSW(){
		/*if(swPrevTime == 0){
			if(digitalRead(sw)){
				swPrevTime = millis();
				return true;
			}
			else
				return false;
		}
		else{
			if(millis() > swPrevTime + SW_DEAD_T)
				swPrevTime = 0;
			return false;
		}*/
		if(lastPressed && lastPressed > (swPrevTime + SW_DEAD_T)) {
			swPrevTime = lastPressed;
			return 1;
		}
		return 0;
	};
	bool getSWPressed(){
		
		return digitalRead(sw);
	};
	void clear(){
		virtualPosition = 0;
	};
};
#endif /*_QUAD_ENCODER_*/
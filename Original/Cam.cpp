#include "Cam.h"

Cam::Cam(uint8_t get_X, uint8_t get_P_ONOFF, uint8_t get_CENTER_OPP_GOAL, uint8_t get_CENTER_OWN_GOAL,
	uint16_t get_SLOPE_RG, uint16_t get_INTERCEPT_RG) {
	//copy
	switch(get_X) {
		case 1: SerialX = &Serial1; break;
		case 2: SerialX = &Serial2; break;
		case 3: SerialX = &Serial3; break;
		case 4: SerialX = &Serial4; break;
		case 5: SerialX = &Serial5; break;
		case 6: SerialX = &Serial6; break;
	}
	P_ONOFF = get_P_ONOFF;
	CENTER_OPP_GOAL = get_CENTER_OPP_GOAL;
	CENTER_OWN_GOAL = get_CENTER_OWN_GOAL;
	SLOPE_RG = get_SLOPE_RG;
	INTERCEPT_RG = get_INTERCEPT_RG;

	//init
	SerialX->begin(9600);

	pinMode(P_ONOFF, INPUT);
}

cam_t Cam::get() {
	if(!digitalRead(P_ONOFF)) {
		if(SerialX->available()) {
			uint8_t value = SerialX->read();
			if(extractBit(value, 6, 7) == 0b11) {
				goal.isWide = extractBit(value, 5, 5) == 1;
				goal.distFW = extractBit(value, 3, 4);
				goal.rotOpp = CENTER_OPP_GOAL - extractBit(value, 0, 2);
			}else if(extractBit(value, 6, 7) == 0b00){
				goal.isInCorner = extractBit(value, 5, 5) == 1;
				goal.distGK = extractBit(value, 3, 4);
				goal.rot = CENTER_OWN_GOAL - extractBit(value, 0, 2);
			}
		}
	}else {
		goal = {0, 0, 2, 0, true, false};
	}
	return goal;
}

int16_t Cam::multiRotGoal(int16_t rotGoal) {
	return rotGoal * SLOPE_RG + signum(rotGoal) * INTERCEPT_RG;
}

bool Cam::getCanUse() {
	return !digitalRead(P_ONOFF);
}
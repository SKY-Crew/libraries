#include "Cam.h"

Cam::Cam(uint8_t given_X, uint8_t given_P_ONOFF, uint8_t given_CENTER_OPP_GOAL, uint8_t given_CENTER_OWN_GOAL,
	uint16_t given_SLOPE_RG, uint16_t given_INTERCEPT_RG) {
	//copy
	sCam.set(given_X);
	P_ONOFF = given_P_ONOFF;
	CENTER_OPP_GOAL = given_CENTER_OPP_GOAL;
	CENTER_OWN_GOAL = given_CENTER_OWN_GOAL;
	SLOPE_RG = given_SLOPE_RG;
	INTERCEPT_RG = given_INTERCEPT_RG;

	//init
	sCam.get()->begin(9600);

	pinMode(P_ONOFF, INPUT);
}

cam_t Cam::get() {
	if(!digitalRead(P_ONOFF)) {
		if(sCam.get()->available()) {
			uint8_t value = sCam.get()->read();
			if(extractBit(value, 7, 7) == 0b1) {
				goal.isWide = extractBit(value, 6, 6) == 1;
				goal.distFW = extractBit(value, 4, 5);
				goal.rotOpp = CENTER_OPP_GOAL - extractBit(value, 0, 3);
				// if(goal.rotOpp >= 0) {
				// 	goal.rotOpp ++;
				// }
				// Serial.println(goal.rotOpp);
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
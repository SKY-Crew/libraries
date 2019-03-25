#include "Cam.h"

Cam::Cam(uint8_t P_SERIAL, uint8_t P_ONOFF, uint8_t CENTER_OPP_GOAL, uint8_t CENTER_OWN_GOAL,
	uint16_t SLOPE_RG, uint16_t INTERCEPT_RG) {
	//copy
	sCam.set(P_SERIAL);
	this->P_ONOFF = P_ONOFF;
	this->CENTER_OPP_GOAL = CENTER_OPP_GOAL;
	this->CENTER_OWN_GOAL = CENTER_OWN_GOAL;
	this->SLOPE_RG = SLOPE_RG;
	this->INTERCEPT_RG = INTERCEPT_RG;

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
				goal.distFW = Dist(extractBit(value, 4, 5) + TOO_CLOSE);
				goal.rotOpp = CENTER_OPP_GOAL - extractBit(value, 0, 3);
				// if(goal.rotOpp >= 0) {
				// 	goal.rotOpp ++;
				// }
				// Serial.println(goal.rotOpp);
			}else if(extractBit(value, 6, 7) == 0b00){
				goal.isInCorner = extractBit(value, 5, 5) == 1;
				goal.distGK = Dist(extractBit(value, 3, 4) + PROPER);
				goal.rot = CENTER_OWN_GOAL - extractBit(value, 0, 2);
			}
		}
	}else {
		goal = {PROPER, 0, PROPER, 0, true, false};
	}
	return goal;
}

int16_t Cam::multiRotGoal(int16_t rotGoal) {
	return rotGoal * SLOPE_RG + signum(rotGoal) * INTERCEPT_RG;
}

bool Cam::getCanUse() {
	return !digitalRead(P_ONOFF);
}
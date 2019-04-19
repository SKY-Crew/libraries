#include "Cam.h"

Cam::Cam(uint8_t P_SERIAL, uint8_t P_ONOFF, uint8_t CENTER_OWN_GOAL,
	uint16_t SLOPE_RG, uint16_t INTERCEPT_RG) {
	// copy
	sCam.set(P_SERIAL);
	this->P_ONOFF = P_ONOFF;
	this->CENTER_OWN_GOAL = CENTER_OWN_GOAL;
	this->SLOPE_RG = SLOPE_RG;
	this->INTERCEPT_RG = INTERCEPT_RG;

	// init
	sCam.get()->begin(9600);

	pinMode(P_ONOFF, INPUT);
}

cam_t Cam::get() {
	if(!digitalRead(P_ONOFF)) {
		if(sCam.get()->available()) {
			uint8_t val = sCam.get()->read();
			switch(extractBit(val, 6, 7)) {
				case 0b00: // GK
					goal.isInCorner = extractBit(val, 5, 5) == 1;
					goal.distGK = Dist(extractBit(val, 3, 4) + PROPER);
					goal.posOwn = CENTER_OWN_GOAL - extractBit(val, 0, 2);
					goal.rotOwn = RotPos(signum(goal.posOwn));
					goal.diffOwn = DiffPos(abs(goal.posOwn));
					break;
				case 0b01: // FW
					goal.isWide = extractBit(val, 6, 6) == 1;
					goal.distFW = Dist(extractBit(val, 4, 5) + TOO_CLOSE);
					break;
				case 0b10: // rotOpp
					goal.rotOpp = map(extractBit(val, 0, 5), 0, 60, -180, 180);
					break;
			}
		}
	}else {
		goal = {PROPER, 0, CENTER, NONE, PROPER, 0, true, false};
	}
	return goal;
}

int16_t Cam::multiRotGoal(Angle rotGoal) {
	return double(rotGoal) * SLOPE_RG + signum(rotGoal) * INTERCEPT_RG;
}

bool Cam::getCanUse() {
	return !digitalRead(P_ONOFF);
}

void Cam::send(double gyro) {
	sCam.get()->write((uint8_t) map(gyro, -180, 180, 0, 256));
}
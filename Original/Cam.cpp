#include "Cam.h"

Cam::Cam(uint8_t P_SERIAL, uint8_t P_ONOFF,
	uint16_t SLOPE_RG, uint16_t INTERCEPT_RG) {
	// copy
	sCam.set(P_SERIAL);
	this->P_ONOFF = P_ONOFF;
	this->SLOPE_RG = SLOPE_RG;
	this->INTERCEPT_RG = INTERCEPT_RG;

	// init
	sCam.get()->begin(9600);

	pinMode(P_ONOFF, INPUT);
}

cam_t Cam::get() {
	if(!digitalRead(P_ONOFF)) {
		if(sCam.get()->available()) {
			int16_t val[4] = {-1, -1, -1, -1};
			while(sCam.get()->available()) {
				uint8_t crt = sCam.get()->read();
				val[extractBit(crt, 6, 7)] = crt;
			}

			if(val[0] >= 0) { // rotOpp
				Angle crtRot = extractBit(val[0], 0, 5) > 60 ? Angle(false) : 
						map(extractBit(val[0], 0, 5), 0, 60, 90, -90);
				goal.rotOpp = bool(goal.rotOpp) ? filterAngle(crtRot, goal.rotOpp, 0.2) : crtRot;
			}
			if(val[1] >= 0) { // rotOwn
				goal.rotOwn = extractBit(val[1], 0, 5) > 60 ? Angle(false) : 
						map(extractBit(val[1], 0, 5), 0, 60, 90, -90) + 180;
			}
			if(val[2] >= 0) { // distOwn
				double crtDist = extractBit(val[2], 0, 5);
				goal.distOwn = filter(crtDist, goal.distOwn, 0.2);
			}
			if(val[3] >= 0) { // others
				goal.isOppWide = extractBit(val[3], 0, 0) == 1;
				int8_t posOwn = extractBit(val[3], 1, 3) - SIZE_DIFF + 1;
				goal.sideOwn = Side(signum(posOwn));
				goal.diffOwn = Diff(abs(posOwn));
				goal.isInCorner = Side(extractBit(val[3], 4, 5) - 1);
			}

			trace(8) { Serial.println(str("rotOpp:")+str(goal.rotOpp)
							+str("\trotOwn:")+str(goal.rotOwn)
							+str("\tdistOwn:")+str(goal.distOwn)); }
			trace(9) { Serial.println(str("isOppWide:")+str(goal.isOppWide)
							+str("\tposOwn:")+str(goal.sideOwn * goal.diffOwn)
							+str("\tisInCorner:")+str(goal.isInCorner)); }

		}
	}else {
		goal = {0, true, 0, 64, CENTER, NONE, CENTER};
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
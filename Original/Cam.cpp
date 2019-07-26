#include "Cam.h"

Cam::Cam(uint8_t P_SERIAL, uint8_t P_ONOFF,
	uint8_t SIZE_POINT, double *POINT, double *ROT, double Kd) {
	// copy
	sCam.set(P_SERIAL);
	this->P_ONOFF = P_ONOFF;
	pld.set(SIZE_POINT, POINT, ROT, Kd);

	// init
	sCam.get()->begin(38400);
	pinMode(P_ONOFF, INPUT);

	cBallExists.set_MAX(20);
	cBallExists.set_COUNT_UP(false);
}

cam_t Cam::get(bool isInAir) {
	if(!digitalRead(P_ONOFF)) {
		if(sCam.get()->available()) {
			int16_t val[4] = {-1, -1, -1, -1};
			while(sCam.get()->available()) {
				uint8_t crt = sCam.get()->read();
				val[extractBit(crt, 6, 7)] = crt;
			}

			if(val[0] >= 0) { // rotOpp
				crtRot = extractBit(val[0], 0, 5) >= 59 || extractBit(val[0], 0, 5) <= 0
						? Angle(false) : map(extractBit(val[0], 0, 5), 0, 60, 90, -90);
			}
			if(val[1] >= 0) { // rotOwn
				goal.rotOwn = extractBit(val[1], 0, 5) > 60 ?
						Angle(false) : map(extractBit(val[1], 0, 5), 0, 60, 90, -90) + 180;
				cBallExists.increase(extractBit(val[1], 0, 5) <= 60);
				Angle crtBall_t = extractBit(val[1], 0, 5) <= 60 ? map(extractBit(val[1], 0, 5), 0, 60, 0, -360) : Angle(false);
				ball_t = bool(crtBall_t) ? crtBall_t :
						bool(cBallExists) ? ball_t : Angle(false);
			}
			if(val[2] >= 0) { // distOwn
				crtDist = extractBit(val[2], 0, 5);
			}
			if(val[3] >= 0) { // others
				goal.isOppWide = extractBit(val[3], 0, 0) == 1;
				int8_t posOwn;
				if(extractBit(val[3], 1, 3) == 7) {
					posOwn = isInAir ? 0 : goal.sideOwn * 3;
				}else {
					posOwn = extractBit(val[3], 1, 3) - SIZE_DIFF + 1;
				}
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
		goal.rotOpp = bool(goal.rotOpp) && bool(crtRot) ? filterAngle(crtRot, goal.rotOpp, 0.35) : crtRot;
		goal.distOwn = crtDist != 0 ? filter(crtDist, goal.distOwn, 0.9) : goal.distOwn;
	}else {
		goal = {0, true, 0, 64, CENTER, NONE, CENTER};
	}
	return goal;
}

int16_t Cam::calRot(Angle crt) {
	return pld.cal(crt);
}

bool Cam::getCanUse() {
	return !digitalRead(P_ONOFF);
}

void Cam::snd(uint8_t snd) {
	sCam.get()->write(snd);
}
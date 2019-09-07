#include "Comc.h"

Comc::Comc(uint8_t P_SERIAL, uint8_t P_ONOFF, uint16_t MAX_C_SND, uint16_t MAX_C_NR) {
	// copy
	sComc.set(P_SERIAL);
	this->P_ONOFF = P_ONOFF;
	this->MAX_C_SND = MAX_C_SND;
	this->MAX_C_NR = MAX_C_NR;

	// init
	sComc.get()->begin(9600);
	pinMode(P_ONOFF, INPUT);
}

comc_t Comc::rcv(bool isFW) {
	comc_t fellow = prvFellow;

	countNoRcv ++;
	fellow = rcvWireless();
	if(countNoRcv >= MAX_C_NR) {
		countNoRcv = MAX_C_NR;
		fellow = {false, !isFW, 512, 64, false, false};
	}

	prvFellow = fellow;

	trace(7) { Serial.println("Fellow:"+str(fellow.exists)+" "+str(fellow.isFW)
				+" "+str(fellow.ball_r)+" "+str(fellow.allowChangeRole)+" "+str(fellow.distOwn)+" "+str(fellow.isInAir)); }

	return fellow;
}

void Comc::snd(bool canRun, bool isFW, double ball_r, double distOwn, bool allowChangeRole, bool isInAir) {
	countSnd ++;
	if(countSnd >= MAX_C_SND) {
		sndWireless(canRun,
				(isFW << 6)
				+ (isInAir << 5)
				+ (uint8_t) conMap(ball_r, 0, 512, 0, 32, 0, 31));
		sndWireless(canRun,
				(1 << 7)
				+ (allowChangeRole << 6)
				+ (uint8_t) constrain(distOwn, 0, 63));
		countSnd = 0;
	}
}

void Comc::sndWireless(bool canRun, uint8_t sndData) {
	if(digitalRead(P_ONOFF) && canRun) {
		sComc.get()->write(sndData);
	}
}

comc_t Comc::rcvWireless() {
	comc_t rcvData = prvFellow;
	if(digitalRead(P_ONOFF) && sComc.get()->available()) {
		while (sComc.get()->available()) {
			uint8_t rcv = sComc.get()->read();
			rcvData.exists = true;
			switch(extractBit(rcv, 7, 7)) {
				case 0:
				rcvData.isFW = extractBit(rcv, 6, 6) == 1;
				rcvData.isInAir = extractBit(rcv, 5, 5) == 1;
				rcvData.ball_r = map(extractBit(rcv, 0, 4), 0, 32, 0, 512);
					break;
				case 1:
				rcvData.allowChangeRole = extractBit(rcv, 6, 6) == 1;
				rcvData.distOwn = extractBit(rcv, 0, 5);
					break;
			}
		}
		countNoRcv = 0;
	}
	return rcvData;
}

int8_t Comc::simplyRcv() {
	if(sComc.get()->available()) {
		return sComc.get()->read();
	}
	return -1;
}

void Comc::simplySnd(uint8_t snd) {
	sComc.get()->write(snd);
}

bool Comc::getCanUse() {
	return digitalRead(P_ONOFF);
}
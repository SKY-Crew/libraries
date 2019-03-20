#include "Comc.h"

Comc::Comc(uint8_t P_SERIAL, uint8_t P_ONOFF, uint16_t MAX_C_SND, uint16_t MAX_C_NR) {
	//copy
	sComc.set(P_SERIAL);
	this->P_ONOFF = P_ONOFF;
	this->MAX_C_SND = MAX_C_SND;
	this->MAX_C_NR = MAX_C_NR;

	//init
	sComc.get()->begin(9600);
	pinMode(P_ONOFF, INPUT);
}

comc_t Comc::communicate(bool canRun, bool isFW) {
	comc_t fellow = prvFellow;

	countSnd ++;
	if(countSnd >= MAX_C_SND) {
		sndWireless(canRun, isFW);
		countSnd = 0;
	}

	countNoRcv ++;
	fellow.exists = true;
	fellow.isFW = rcvWireless();
	if(countNoRcv >= MAX_C_NR) {
		countNoRcv = MAX_C_NR;
		fellow.exists = false;
		fellow.isFW = !isFW;
	}

	prvFellow = fellow;
	return fellow;
}

void Comc::sndWireless(bool canRun, bool isFW) {
	uint8_t sndData;
	if(digitalRead(P_ONOFF) && canRun) {
		sndData = isFW ? 1 : 0;
		sComc.get()->write(sndData);
	}
}

bool Comc::rcvWireless() {
	bool rcvData = prvFellow.isFW;
	if(digitalRead(P_ONOFF) && sComc.get()->available()) {
		while (sComc.get()->available()) {
			rcvData = sComc.get()->read() == 1;
		}
		countNoRcv = 0;
	}
	return rcvData;
}

bool Comc::getCanUse() {
	return digitalRead(P_ONOFF);
}
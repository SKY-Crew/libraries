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

comc_t Comc::communicate(bool canRun, bool isFW, double ball_r) {
	comc_t fellow = prvFellow;

	countSnd ++;
	if(countSnd >= MAX_C_SND) {
		sndWireless(canRun,
				((uint8_t) constrain(map(ball_r, 0, 512, 0, 0x80), 0, 0x80 - 1) << 1)
				+ (isFW ? 1 : 0));
		countSnd = 0;
	}

	countNoRcv ++;
	fellow = rcvWireless();
	if(countNoRcv >= MAX_C_NR) {
		countNoRcv = MAX_C_NR;
		fellow = {false, !isFW, 512};
	}

	prvFellow = fellow;
	return fellow;
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
			rcvData = {true, extractBit(rcv, 0, 0) == 1, (double) extractBit(rcv, 1, 7)};
		}
		countNoRcv = 0;
	}
	return rcvData;
}

bool Comc::getCanUse() {
	return digitalRead(P_ONOFF);
}
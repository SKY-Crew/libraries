#include "Comc.h"

Comc::Comc(uint8_t get_X, uint8_t get_P_ONOFF, uint16_t get_MAX_C_SND, uint16_t get_MAX_C_NR) {
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
	MAX_C_SND = get_MAX_C_SND;
	MAX_C_NR = get_MAX_C_NR;

	//init
	SerialX->begin(9600);
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
		SerialX->write(sndData);
	}
}

bool Comc::rcvWireless() {
	bool rcvData = prvFellow.isFW;
	if(digitalRead(P_ONOFF) && SerialX->available()) {
		while (SerialX->available()) {
			rcvData = SerialX->read() == 1;
		}
		countNoRcv = 0;
	}
	return rcvData;
}

bool Comc::getCanUse() {
	return digitalRead(P_ONOFF);
}
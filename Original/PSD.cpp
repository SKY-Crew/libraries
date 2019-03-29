#include "PSD.h"

PSD::PSD(uint8_t P_WIRE, double CHANGE_RATE, uint16_t THRE_IS_CLOSE, uint8_t MAX_CC) {
	//copy
	wPSD.set(P_WIRE);

	this->CHANGE_RATE = CHANGE_RATE;
	this->THRE_IS_CLOSE = THRE_IS_CLOSE;

	//init
	wPSD.get()->begin();
	cClose = Count(MAX_CC, false);
}

void PSD::get() {
	wPSD.get()->beginTransmission(0x40);
	wPSD.get()->write(0x5E);
	wPSD.get()->endTransmission();

	wPSD.get()->requestFrom(0x40, 2);
	value = wPSD.get()->read() * 16 + wPSD.get()->read();
	if(value <= 0) {
		value = 4094;
	}
	value = filter(value, prv, CHANGE_RATE);
	prv = value;
}

bool PSD::getBool(bool haveGot) {
	if(!haveGot) {
		get();
	}
	cClose.increase(value < THRE_IS_CLOSE);
	return bool(cClose);
}

double PSD::getValue(bool haveGot) {
	if(!haveGot) {
		get();
	}
	return value;
}
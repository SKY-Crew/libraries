#include "PSD.h"

PSD::PSD(uint8_t given_X, double given_MULTI_AVG, uint16_t given_BORDER_IS_CLOSE, uint8_t given_MAX_CC) {
	//copy
	wPSD.set(given_X);

	MULTI_AVG = given_MULTI_AVG;
	BORDER_IS_CLOSE = given_BORDER_IS_CLOSE;

	//init
	wPSD.get()->begin();
	cClose = Count(given_MAX_CC, false);
}

bool PSD::get() {
	wPSD.get()->beginTransmission(0x40);
	wPSD.get()->write(0x5E);
	wPSD.get()->endTransmission();

	wPSD.get()->requestFrom(0x40, 2);
	value = wPSD.get()->read() * 16 + wPSD.get()->read();
	if(value <= 0) {
		value = 4094;
	}
	value = value * (1 - MULTI_AVG) + prv * MULTI_AVG;
	prv = value;
	cClose.increase(value < BORDER_IS_CLOSE);
	return bool(cClose);
}

double PSD::getValue() {
	return value;
}
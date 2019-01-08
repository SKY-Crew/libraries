#include "PSD.h"

PSD::PSD(uint8_t given_X, double given_MULTI_AVG, uint16_t given_BORDER_IS_CLOSE, uint8_t given_MAX_CC) {
	//copy
	switch(given_X) {
		case 0: WireX = &Wire; break;
		case 1: WireX = &Wire1; break;
		case 2: WireX = &Wire2; break;
	}

	MULTI_AVG = given_MULTI_AVG;
	BORDER_IS_CLOSE = given_BORDER_IS_CLOSE;
	MAX_CC = given_MAX_CC;

	//init
	WireX->begin();
}

bool PSD::get() {
	WireX->beginTransmission(0x40);
	WireX->write(0x5E);
	WireX->endTransmission();

	WireX->requestFrom(0x40, 2);
	value = WireX->read() * 16 + WireX->read();
	if(value <= 0) {
		value = 4094;
	}
	value = value * (1 - MULTI_AVG) + prv * MULTI_AVG;
	prv = value;
	countClose = value <= BORDER_IS_CLOSE ? MAX_CC : max(countClose - 1, 0);
	return countClose > 0;
}

double PSD::getValue() {
	return value;
}
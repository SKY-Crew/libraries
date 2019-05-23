#include "PSD.h"

PSD::PSD(uint8_t P_WIRE, double CHANGE_RATE, uint16_t THRE_IS_CLOSE, uint8_t MAX_CC) {
	// copy
	wPSD.set(P_WIRE);

	this->CHANGE_RATE = CHANGE_RATE;
	this->THRE_IS_CLOSE = THRE_IS_CLOSE;

	// init
	wPSD.get()->begin();
	wPSD.get()->setClock(4*1000*1000);
	cClose = Count(MAX_CC, false);
}

void PSD::get() {
	wPSD.get()->beginTransmission(0x40);
	wPSD.get()->write(0x5E);
	wPSD.get()->endTransmission();

	wPSD.get()->requestFrom(0x40, 2);
	val = wPSD.get()->read() * 16 + wPSD.get()->read();
	if(val <= 0) {
		val = 4094;
	}
	val = filter(val, prv, CHANGE_RATE);
	prv = val;

	trace(6) { Serial.println("PSD:"+str(val)); }
}

bool PSD::getBool(bool haveGot) {
	if(!haveGot) {
		get();
	}
	cClose.increase(val < THRE_IS_CLOSE);
	return bool(cClose);
}

double PSD::getVal(bool haveGot) {
	if(!haveGot) {
		get();
	}
	return val;
}
#include "PSD.h"

PSD::PSD(uint8_t PORT, double CHANGE_RATE, uint16_t THRE_IS_CLOSE, uint8_t MAX_CCS, uint8_t MAX_CC) {
	// copy
	this->PORT = PORT;

	this->CHANGE_RATE = CHANGE_RATE;
	this->THRE_IS_CLOSE = THRE_IS_CLOSE;

	// init
	cCannotSee = Count(MAX_CCS, true);
	cClose = Count(MAX_CC, false);
}

void PSD::get() {
	val = analogRead(PORT);
	cCannotSee.increase(val <= 2);
	val = bool(cCannotSee) ? 0 : cCannotSee.compare(0) ? prv
			: filter(analogRead(PORT), prv, CHANGE_RATE);
	prv = val;

	trace(6) { Serial.println("PSD:"+str(val)); }
}

bool PSD::getBool(bool haveGot) {
	if(!haveGot) {
		get();
	}
	cClose.increase(val >= THRE_IS_CLOSE);
	return bool(cClose);
}

double PSD::getVal(bool haveGot) {
	if(!haveGot) {
		get();
	}
	return val;
}
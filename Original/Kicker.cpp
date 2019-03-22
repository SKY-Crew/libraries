#include "Kicker.h"

Kicker::Kicker(uint8_t P_KICKER, uint8_t P_ONOFF_KICKER, uint8_t P_RUN_KICKER,
	uint16_t MAX_CK, uint16_t MAX_CKW) {
	//copy
	this->P_KICKER = P_KICKER;
	this->P_ONOFF_KICKER = P_ONOFF_KICKER;
	this->P_RUN_KICKER = P_RUN_KICKER;
	this->MAX_CK = MAX_CK;
	this->MAX_CKW = MAX_CKW;

	//init
	pinMode(P_KICKER, OUTPUT);
	pinMode(P_ONOFF_KICKER, INPUT);
	pinMode(P_RUN_KICKER, INPUT);

	cKicking.set_MAX(MAX_CK);
	cWait.set_MAX(MAX_CKW);
}


void Kicker::kick(bool start) {
	if(!haveChecked) {
		cKicking.increase(kicking);
		if(bool(cKicking)) {
			digitalWrite(P_KICKER, LOW);
			kicking = false;
		}
		cWait.increase(!kicking);
		if(start && bool(cWait)) {
			digitalWrite(P_KICKER, digitalRead(P_ONOFF_KICKER));
			kicking = true;
		}
	}
	haveChecked = true;
}

void Kicker::check() {
	kick(digitalRead(P_RUN_KICKER) && !prvStart);
	prvStart = digitalRead(P_RUN_KICKER);
}

bool Kicker::getCanUse() {
	return digitalRead(P_ONOFF_KICKER);
}

bool Kicker::getIsKicking() {
	return digitalRead(P_ONOFF_KICKER) && kicking;
}

void Kicker::setHaveChecked(bool haveChecked) {
	this->haveChecked = haveChecked;
}
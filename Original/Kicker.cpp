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
}


void Kicker::kick(bool startKick) {
	if(!haveCheckKick) {
		countKick = min(countKick + 1, max(MAX_CK, MAX_CKW));
		if(kicking && countKick >= MAX_CK) {
			digitalWrite(P_KICKER, LOW);
			kicking = false;
			countKick = 0;
		}else if(startKick && !kicking && countKick >= MAX_CKW) {
			digitalWrite(P_KICKER, digitalRead(P_ONOFF_KICKER));
			kicking = true;
			countKick = 0;
		}
	}
	haveCheckKick = true;
}

void Kicker::checkKick() {
	kick(digitalRead(P_RUN_KICKER) && !prvRK);
	prvRK = digitalRead(P_RUN_KICKER);
}

bool Kicker::getCanUseKicker() {
	return digitalRead(P_ONOFF_KICKER);
}

bool Kicker::getIsKicking() {
	return digitalRead(P_ONOFF_KICKER) && kicking;
}

void Kicker::setHaveCheckKick(bool haveCheckKick) {
	haveCheckKick = haveCheckKick;
}
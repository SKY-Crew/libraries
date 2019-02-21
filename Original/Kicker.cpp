#include "Kicker.h"

Kicker::Kicker(uint8_t given_P_KICKER, uint8_t given_P_ONOFF_KICKER, uint8_t given_P_RUN_KICKER,
	uint16_t given_MAX_CK, uint16_t given_MAX_CKW) {
	//copy
	P_KICKER = given_P_KICKER;
	P_ONOFF_KICKER = given_P_ONOFF_KICKER;
	P_RUN_KICKER = given_P_RUN_KICKER;
	MAX_CK = given_MAX_CK;
	MAX_CKW = given_MAX_CKW;

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
	if(digitalRead(P_RUN_KICKER)) {
		kick(true);
		while(digitalRead(P_RUN_KICKER)){
			kick(false);
			delay(10);
		}
	}
	kick(false);
}

bool Kicker::getCanUseKicker() {
	return digitalRead(P_ONOFF_KICKER);
}

bool Kicker::getIsKicking() {
	return digitalRead(P_ONOFF_KICKER) && kicking;
}

void Kicker::setHaveCheckKick(bool given_haveCheckKick) {
	haveCheckKick = given_haveCheckKick;
}
#include "Actuator.h"

Actuator::Actuator(bool given_CAN_MOVE, uint8_t given_QTY, uint8_t *given_P_DIR, uint8_t *given_P_PWR,
	int16_t firstRM, double given_SLOPE_POWER, double given_INTERCEPT_POWER,
	uint8_t given_P_KICKER, uint8_t given_P_ONOFF_KICKER, uint8_t given_P_RUN_KICKER, uint16_t given_MAX_CK, uint16_t given_MAX_CKW) {
	//copy
	CAN_MOVE = given_CAN_MOVE;
	QTY = given_QTY;
	P_DIR = new uint8_t[QTY];
	copyArray(P_DIR, given_P_DIR, QTY);
	P_PWR = new uint8_t[QTY];
	copyArray(P_PWR, given_P_PWR, QTY);
	ROT_MOTOR = new int16_t[QTY];
	ROT_WHEEL = new int16_t[QTY];
	SIN_RW = new double[QTY];
	COS_RW = new double[QTY];
	MAX_DVP = abs(cos(toRadians(firstRM)) * QTY);
	SLOPE_POWER = given_SLOPE_POWER;
	INTERCEPT_POWER = given_INTERCEPT_POWER;

	P_KICKER = given_P_KICKER;
	P_ONOFF_KICKER =  given_P_ONOFF_KICKER;
	P_RUN_KICKER =  given_P_RUN_KICKER;
	MAX_CK = given_MAX_CK;
	MAX_CKW = given_MAX_CKW;

	for(uint8_t numM = 0; numM < QTY; numM ++) {
		ROT_MOTOR[numM] = firstRM * ((numM % 2) == 0 ? 1 : -1)
			+ 180 * ((numM % 3) == 0 ? 0 : 1);
		ROT_WHEEL[numM] = ROT_MOTOR[numM] + 90;
		SIN_RW[numM] = sin(toRadians(ROT_WHEEL[numM]));
		COS_RW[numM] = cos(toRadians(ROT_WHEEL[numM]));
	}

	//init
	for(uint8_t numM = 0; numM < QTY; numM ++) {
		pinMode(P_DIR[numM], OUTPUT);
		pinMode(P_PWR[numM], OUTPUT);
		analogWriteFrequency(P_PWR[numM], 250 * 1000);
	}

	pinMode(P_KICKER, OUTPUT);
	pinMode(P_ONOFF_KICKER, INPUT);
	pinMode(P_RUN_KICKER, INPUT);
}

void Actuator::run(int16_t moveAngle, int16_t rotPower, uint16_t maxPower) {
	if(!haveRun) {
		int16_t power[QTY];
		//モーターパワー計算
		if(moveAngle < 0) {
			//回転移動のみ
			for(uint8_t numM = 0; numM < QTY; numM ++) {
				power[numM] = rotPower;
			}
		}else {
			//平行+回転移動
			float ratePower[QTY];
			vectorXY_t vxyPower = {0, 0};
			for(uint8_t numM = 0; numM < QTY; numM ++) {
				ratePower[numM] =  cos(toRadians(ROT_WHEEL[numM] - moveAngle));
				vxyPower.x += COS_RW[numM] * ratePower[numM];
				vxyPower.y += SIN_RW[numM] * ratePower[numM];
			}
			float distVP = sqrt(vxyPower.x * vxyPower.x + vxyPower.y * vxyPower.y);
			for(uint8_t numM = 0; numM < QTY; numM ++) {
				power[numM] = maxPower * ratePower[numM] / distVP * MAX_DVP + rotPower;
			}
		}
		//パワー変換
		for(uint8_t numM = 0; numM < QTY; numM ++) {
			power[numM] = power[numM] == 0 ? 0 : power[numM] * SLOPE_POWER + signum(power[numM]) * INTERCEPT_POWER;
		}
		//モーター制御
		for(uint8_t numM = 0; numM < QTY; numM ++) {
			spin(numM, power[numM]);
		}
	}
	haveRun = true;
}

void Actuator::spin(uint8_t port, int16_t power) {
	if(CAN_MOVE) {
		power = absConstrain(power, 255);
		digitalWrite(P_DIR[port], 0.5 - signum(power) * 0.5);
		analogWrite(P_PWR[port], abs(power));
	}
}

void Actuator::setHaveRun(bool given_haveRun) {
	haveRun = given_haveRun;
}

void Actuator::kick(bool startKick) {
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

void Actuator::checkKick() {
	if(digitalRead(P_RUN_KICKER)) {
		kick(true);
		while(digitalRead(P_RUN_KICKER)){
			kick(false);
			delay(10);
		}
	}
	kick(false);
}

bool Actuator::getCanUseKicker() {
	return digitalRead(P_ONOFF_KICKER);
}

bool Actuator::getIsKicking() {
	return digitalRead(P_ONOFF_KICKER) && kicking;
}
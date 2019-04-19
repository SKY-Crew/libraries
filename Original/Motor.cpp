#include "Motor.h"

Motor::Motor(bool CAN_MOVE, uint8_t QTY, uint8_t *P_DIR, uint8_t *P_PWR,
	int16_t firstRM, double SLOPE_POWER, double INTERCEPT_POWER) {
	// copy
	this->CAN_MOVE = CAN_MOVE;
	this->QTY = QTY;
	this->P_DIR = new uint8_t[QTY];
	copyArray(this->P_DIR, P_DIR, QTY);
	this->P_PWR = new uint8_t[QTY];
	copyArray(this->P_PWR, P_PWR, QTY);
	ROT_MOTOR = new Angle[QTY];
	ROT_WHEEL = new Angle[QTY];
	SIN_RW = new double[QTY];
	COS_RW = new double[QTY];
	MAX_DVP = abs(cos(toRadians(firstRM)) * QTY);
	this->SLOPE_POWER = SLOPE_POWER;
	this->INTERCEPT_POWER = INTERCEPT_POWER;

	// init
	for(uint8_t i = 0; i < QTY; i ++) {
		ROT_MOTOR[i] = new Angle();
		if(QTY == 4) {
			ROT_MOTOR[i] = firstRM * ((i % 2) == 0 ? 1 : -1)
				+ 180 * ((i % 3) == 0 ? 0 : 1);
		}else {
			ROT_MOTOR[i] = - firstRM * (i + 0.5);
		}
		ROT_WHEEL[i] = new Angle();
		ROT_WHEEL[i] = ROT_MOTOR[i] + 90;
		COS_RW[i] = cos(ROT_WHEEL[i]);
		SIN_RW[i] = sin(ROT_WHEEL[i]);
	}

	for(uint8_t i = 0; i < QTY; i ++) {
		pinMode(P_DIR[i], OUTPUT);
		pinMode(P_PWR[i], OUTPUT);
		#ifdef CORE_TEENSY
			analogWriteFrequency(P_PWR[i], 70 * 1000);
		#else
			TCCR3B = (TCCR3B & 0b11111000) | 0x01;
		#endif
	}
}

void Motor::run(Angle moveAngle, int16_t rotPower, uint16_t maxPower) {
	if(!haveRun) {
		trace(1) { Serial.println("Motor:"+str(moveAngle)+
				" "+str(rotPower)+" "+str(maxPower)); }

		int16_t power[QTY];
		// モーターパワー計算
		if(!bool(moveAngle)) {
			// 回転移動のみ
			for(uint8_t i = 0; i < QTY; i ++) {
				power[i] = rotPower;
			}
		}else {
			// 平行+回転移動
			float ratePower[QTY];
			vectorXY_t xyPower = {0, 0};
			for(uint8_t i = 0; i < QTY; i ++) {
				ratePower[i] =  cos(ROT_WHEEL[i] - moveAngle);
				xyPower.x += COS_RW[i] * ratePower[i];
				xyPower.y += SIN_RW[i] * ratePower[i];
			}
			float distVP = sqrt(xyPower.x * xyPower.x + xyPower.y * xyPower.y);
			for(uint8_t i = 0; i < QTY; i ++) {
				power[i] = maxPower * ratePower[i] / distVP * MAX_DVP + rotPower;
			}
		}
		// パワー変換
		for(uint8_t i = 0; i < QTY; i ++) {
			power[i] = power[i] == 0 ? 0 : power[i] * SLOPE_POWER + signum(power[i]) * INTERCEPT_POWER;
		}
		// モーター制御
		for(uint8_t i = 0; i < QTY; i ++) {
			spin(i, power[i]);
		}
	}
	haveRun = true;
}

void Motor::spin(uint8_t port, int16_t power) {
	if(CAN_MOVE) {
		power = absConstrain(power, 255);
		// 回転方向
		digitalWrite(P_DIR[port], 0.5 - signum(power) * 0.5);
		// パワー
		analogWrite(P_PWR[port], abs(power));
	}
}

void Motor::setHaveRun(bool haveRun) {
	this->haveRun = haveRun;
}
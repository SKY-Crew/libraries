#include "Ball.h"

Ball::Ball(uint8_t given_QTY, uint8_t *given_PORT, uint16_t *given_MAX_IR, uint16_t *given_AVG_IR, double given_MULTI_AVG,
	uint8_t given_SIZE_SLOPE_DIR, double (*given_SLOPE_DIR)[2], double (*given_INTERCEPT_DIR)[2], double (*given_POINT_DIR)[2],
	uint8_t given_P_CATCH, uint16_t given_BORDER_CATCH, uint8_t given_MAX_C_CATCH) {
	//copy
	QTY = given_QTY;
	PORT = new uint8_t[QTY];
	copyArray(PORT, given_PORT, QTY);

	COS_IR = new double[QTY];
	SIN_IR = new double[QTY];
	for(int i = 0; i < QTY; i ++) {
		COS_IR[i] = cos(toRadians(i * 360.0 / QTY));
		SIN_IR[i] = sin(toRadians(i * 360.0 / QTY));
	}

	MAX_IR = new uint16_t[QTY];
	copyArray(MAX_IR, given_MAX_IR, QTY);
	AVG_IR = new uint16_t[QTY];
	copyArray(AVG_IR, given_AVG_IR, QTY);
	for(uint8_t numBall = 0; numBall < QTY; numBall ++) {
		avg_MAX_IR += MAX_IR[numBall];
		avg_AVG_IR += AVG_IR[numBall];
	}
	avg_MAX_IR /= QTY;
	avg_AVG_IR /= QTY;

	MULTI_AVG = given_MULTI_AVG;
	SIZE_SLOPE_DIR = given_SIZE_SLOPE_DIR;
	SLOPE_DIR = new double[SIZE_SLOPE_DIR][2];
	copyArray(&SLOPE_DIR[0][0], &given_SLOPE_DIR[0][0], SIZE_SLOPE_DIR, 2);
	INTERCEPT_DIR = new double[SIZE_SLOPE_DIR][2];
	copyArray(&INTERCEPT_DIR[0][0], &given_INTERCEPT_DIR[0][0], SIZE_SLOPE_DIR, 2);
	POINT_DIR = new double[SIZE_SLOPE_DIR - 1][2];
	copyArray(&POINT_DIR[0][0], &given_POINT_DIR[0][0], SIZE_SLOPE_DIR - 1, 2);

	value = new uint16_t[QTY];
	prv = new uint16_t[QTY];
	crt = new uint16_t[QTY];

	P_CATCH = given_P_CATCH;
	BORDER_CATCH = given_BORDER_CATCH;

	for(uint8_t numBall = 0; numBall < QTY; numBall ++) {
		prv[numBall] = 0;
	}

	//init
	for(uint8_t numBall = 0; numBall < QTY; numBall ++) {
		pinMode(PORT[numBall], INPUT);
	}

	cCatch.set_MAX(given_MAX_C_CATCH);
}


vectorRT_t Ball::get(bool hasFilter) {
	vectorRT_t vRT = {0, 0};
	bool canSeeBall = true;
	//初期化
	for(int numBall = 0; numBall < QTY; numBall ++) {
		value[numBall] = 0;
	}
	//計測
	uint64_t time = micros();
	uint16_t countMax = 0;
	while(micros() - time < 837 * 2) {
		countMax ++;
		for(uint8_t numBall = 0; numBall < QTY; numBall ++) {
			value[numBall] += !digitalRead(PORT[numBall]);
		}
	}
	// for(uint8_t numBall = 0; numBall < QTY; numBall ++) {
	// 	Serial.print(value[numBall]);
	// 	Serial.print("\t");
	// }
	// Serial.println();
	// 平均値計算
	for(uint8_t numBall = 0; numBall < QTY; numBall ++) {
		value[numBall] *= 1000.0 / (double) countMax;
		if(value[numBall] > 0) {
			canSeeBall = false;
			if(hasFilter) {
				value[numBall] = map(value[numBall], avg_AVG_IR, MAX_IR[numBall], avg_AVG_IR, avg_MAX_IR);
			}
			if(prv[numBall] != 0) {
				value[numBall] = prv[numBall] * MULTI_AVG + value[numBall] * (1 - MULTI_AVG);
			}
		}
		prv[numBall] = value[numBall];
	}
	//ベクトル合成 距離計算
	vectorXY_t vXY = {0, 0};
	for(uint8_t numBall = 0; numBall < QTY; numBall ++) {
		vXY.x += value[numBall] * COS_IR[numBall];
		vXY.y += value[numBall] * SIN_IR[numBall];
		vRT.r += value[numBall];
	}
	vRT.t = toDegrees(atan2(vXY.y, vXY.x));
	vRT.r /= 1.0 * QTY;

	//ボールが遠すぎるか
	if(canSeeBall) {
		vRT.t = false;
	}
	return vRT;
}

uint16_t Ball::getForward() {
	return value[0];
}

Angle Ball::getDir(Angle theta, bool isClose) {
	Angle dir = theta;
	if(bool(theta)) {
		uint8_t key = 0;
		for(; key < SIZE_SLOPE_DIR - 1; key ++) {
			if(absAngle(dir) <= POINT_DIR[key][isClose]) { break; }
		}
		Angle plusDir = dir * SLOPE_DIR[key][isClose] + signum(dir) * INTERCEPT_DIR[key][isClose];
		dir = dir + plusDir;
	}

	return dir;
}

bool Ball::getCatch() {
	valueCatch = analogRead(P_CATCH);
	cCatch.increase(valueCatch < BORDER_CATCH);
	return bool(cCatch);
}

bool Ball::compareCatch(double rate) {
	return cCatch.compare(rate);
}

uint16_t *Ball::getValue() {
	for(int numBall = 0; numBall < QTY; numBall ++) {
		crt[numBall] = value[numBall];
	}
	return crt;
}

uint8_t Ball::getQTY() {
	return QTY;
}

uint16_t Ball::getValueCatch() {
	return valueCatch;
}
#include "Ball.h"

Ball::Ball(uint8_t given_QTY, uint8_t *given_PORT,
	uint8_t given_MEASURING_COUNT, uint16_t given_BORDER_WEAK, double given_MULTI_AVG,
	uint16_t *given_BORDER_DIST, uint8_t given_SIZE_SLOPE_DIR, double (*given_SLOPE_DIR)[2], double (*given_INTERCEPT_DIR)[2], double (*given_POINT_DIR)[2],
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

	MEASURING_COUNT = given_MEASURING_COUNT;
	BORDER_WEAK = given_BORDER_WEAK;
	MULTI_AVG = given_MULTI_AVG;

	BORDER_DIST[0] = given_BORDER_DIST[0];
	BORDER_DIST[1] = given_BORDER_DIST[1];
	SIZE_SLOPE_DIR = given_SIZE_SLOPE_DIR;
	SLOPE_DIR = new double[SIZE_SLOPE_DIR][2];
	copyArray(&SLOPE_DIR[0][0], &given_SLOPE_DIR[0][0], SIZE_SLOPE_DIR, 2);
	INTERCEPT_DIR = new double[SIZE_SLOPE_DIR][2];
	copyArray(&INTERCEPT_DIR[0][0], &given_INTERCEPT_DIR[0][0], SIZE_SLOPE_DIR, 2);
	POINT_DIR = new double[SIZE_SLOPE_DIR - 1][2];
	copyArray(&POINT_DIR[0][0], &given_POINT_DIR[0][0], SIZE_SLOPE_DIR - 1, 2);

	value = new uint16_t[QTY];
	weak = new uint16_t[QTY];
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
	bool findingBall = true;
	//初期化
	for(int numBall = 0; numBall < QTY; numBall ++) {
		value[numBall] = 0;
	}
	//計測
	uint64_t time = micros();
	uint16_t countMax = 0;
	while(micros() - time < CYCLE * MEASURING_COUNT) {
		countMax ++;
		for(uint8_t numBall = 0; numBall < QTY; numBall ++) {
			value[numBall] += !digitalRead(PORT[numBall]);
		}
	}

	//比率化
	for(uint8_t numBall = 0; numBall < QTY; numBall ++) {
		value[numBall] *= 1000.0 / (double) countMax;
	}

	for(uint8_t numBall = 0; numBall < QTY; numBall ++) {
		if(value[numBall] > 0) {
			findingBall = false;
			// 平均値計算
			if(prv[numBall] > 0) {
				value[numBall] = prv[numBall] * MULTI_AVG + value[numBall] * (1 - MULTI_AVG);
			}
		}
		//平均値保存
		prv[numBall] = value[numBall];
	}

	//距離計算
	for(uint8_t numBall = 0; numBall < QTY; numBall ++) {
		vRT.r += value[numBall];
	}
	vRT.r /= (double) QTY;
	//弱反応切り捨て
	bool isAllWeak = true;
	for(uint8_t numBall = 0; numBall < QTY; numBall ++) {
		if(value[numBall] > BORDER_WEAK) {
			isAllWeak = false;
			break;
		}
	}
	for(uint8_t numBall = 0; numBall < QTY; numBall ++) {
		weak[numBall] = isAllWeak ? value[numBall] : max(value[numBall] - BORDER_WEAK, 0);
	}
	//ベクトル合成
	vectorXY_t vXY = {0, 0};
	for(uint8_t numBall = 0; numBall < QTY; numBall ++) {
		vXY.x += weak[numBall] * COS_IR[numBall];
		vXY.y += weak[numBall] * SIN_IR[numBall];
	}
	vRT.t = toDegrees(atan2(vXY.y, vXY.x));

	//null判定
	if(findingBall) {
		vRT.t = false;
	}
	return vRT;
}

uint16_t Ball::getForward() {
	return value[0];
}

Angle Ball::getDir(vectorRT_t ball) {
	Angle dir = ball.t;
	if(bool(ball.t)) {
		Angle plusDir[2];
		for(bool isClose = true; isClose; isClose = !isClose) {
			uint8_t key = 0;
			for(; key < SIZE_SLOPE_DIR - 1; key ++) {
				if(absAngle(dir) <= POINT_DIR[key][isClose]) { break; }
			}
			plusDir[isClose] = absAngle(dir) * SLOPE_DIR[key][isClose] + INTERCEPT_DIR[key][isClose];
		}
		dir += signum(dir) * max(0, map(ball.r, BORDER_DIST[0], BORDER_DIST[1], plusDir[0], plusDir[1]));
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
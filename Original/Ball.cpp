#include "Ball.h"

Ball::Ball(uint8_t QTY, uint8_t *PORT,
	uint8_t MEASURING_COUNT, uint16_t THRE_WEAK, double MULTI_AVG,
	uint16_t *THRE_DIST, uint8_t SIZE_SLOPE_DIR, double (*SLOPE_DIR)[2], double (*INTERCEPT_DIR)[2], double (*POINT_DIR)[2],
	uint8_t P_CATCH, uint16_t THRE_CATCH, uint8_t MAX_C_CATCH) {
	//copy
	this->QTY = QTY;
	PORT = new uint8_t[QTY];
	copyArray(PORT, PORT, QTY);

	COS_IR = new double[QTY];
	SIN_IR = new double[QTY];
	for(int i = 0; i < QTY; i ++) {
		COS_IR[i] = cos(toRadians(i * 360.0 / QTY));
		SIN_IR[i] = sin(toRadians(i * 360.0 / QTY));
	}

	this->MEASURING_COUNT = MEASURING_COUNT;
	this->THRE_WEAK = THRE_WEAK;
	this->MULTI_AVG = MULTI_AVG;

	THRE_DIST[0] = THRE_DIST[0];
	THRE_DIST[1] = THRE_DIST[1];
	this->SIZE_SLOPE_DIR = SIZE_SLOPE_DIR;
	SLOPE_DIR = new double[SIZE_SLOPE_DIR][2];
	copyArray(&SLOPE_DIR[0][0], &SLOPE_DIR[0][0], SIZE_SLOPE_DIR, 2);
	INTERCEPT_DIR = new double[SIZE_SLOPE_DIR][2];
	copyArray(&INTERCEPT_DIR[0][0], &INTERCEPT_DIR[0][0], SIZE_SLOPE_DIR, 2);
	POINT_DIR = new double[SIZE_SLOPE_DIR - 1][2];
	copyArray(&POINT_DIR[0][0], &POINT_DIR[0][0], SIZE_SLOPE_DIR - 1, 2);

	value = new uint16_t[QTY];
	weak = new uint16_t[QTY];
	prv = new uint16_t[QTY];
	crt = new uint16_t[QTY];

	this->P_CATCH = P_CATCH;
	this->THRE_CATCH = THRE_CATCH;

	for(uint8_t numBall = 0; numBall < QTY; numBall ++) {
		prv[numBall] = 0;
	}

	//init
	for(uint8_t numBall = 0; numBall < QTY; numBall ++) {
		pinMode(PORT[numBall], INPUT);
	}

	cCatch.set_MAX(MAX_C_CATCH);
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
				value[numBall] = filter(value[numBall], prv[numBall], MULTI_AVG);
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
		if(value[numBall] > THRE_WEAK) {
			isAllWeak = false;
			break;
		}
	}
	for(uint8_t numBall = 0; numBall < QTY; numBall ++) {
		weak[numBall] = isAllWeak ? value[numBall] : max(value[numBall] - THRE_WEAK, 0);
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
		dir += signum(dir) * max(0, map(ball.r, THRE_DIST[0], THRE_DIST[1], plusDir[0], plusDir[1]));
	}
	return dir;
}

bool Ball::getCatch() {
	valueCatch = analogRead(P_CATCH);
	cCatch.increase(valueCatch < THRE_CATCH);
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
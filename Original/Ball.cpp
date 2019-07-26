#include "Ball.h"

Ball::Ball(uint8_t QTY, uint8_t *PORT,
	uint8_t MEASURING_COUNT, uint16_t THRE_WEAK, double CHANGE_RATE, double CHANGE_RATE_T, double PLUS_T,
	uint8_t SIZE_THRE_DIST, double *THRE_DIST, uint8_t SIZE_DIR, double **DIR, double **PLUS_DIR,
	uint8_t P_CATCH, uint16_t THRE_CATCH, uint16_t MAX_C_CATCH, uint16_t MAX_C_MAY_KICK) {
	// copy
	this->QTY = QTY;
	this->PORT = new uint8_t[QTY];
	copyArray(this->PORT, PORT, QTY);

	COS_IR = new double[QTY];
	SIN_IR = new double[QTY];
	for(int i = 0; i < QTY; i ++) {
		COS_IR[i] = cos(toRadians(i * 360.0 / QTY));
		SIN_IR[i] = sin(toRadians(i * 360.0 / QTY));
	}

	this->MEASURING_COUNT = MEASURING_COUNT;
	this->THRE_WEAK = THRE_WEAK;
	this->CHANGE_RATE = CHANGE_RATE;
	this->CHANGE_RATE_T = CHANGE_RATE_T;
	this->PLUS_T = PLUS_T;

	this->SIZE_THRE_DIST = SIZE_THRE_DIST;
	this->THRE_DIST = new double[SIZE_THRE_DIST];
	copyArray(this->THRE_DIST, THRE_DIST, SIZE_THRE_DIST);
	this->SIZE_DIR = SIZE_DIR;
	this->DIR = new double*[SIZE_THRE_DIST];
	copyArray(this->DIR, DIR, SIZE_THRE_DIST);
	this->PLUS_DIR = new double*[SIZE_THRE_DIST];
	copyArray(this->PLUS_DIR, PLUS_DIR, SIZE_THRE_DIST);

	val = new uint16_t[QTY];
	weak = new uint16_t[QTY];
	prv = new uint16_t[QTY];
	crt = new uint16_t[QTY];


	this->P_CATCH = P_CATCH;
	this->THRE_CATCH = THRE_CATCH;

	// init
	for(uint8_t numBall = 0; numBall < QTY; numBall ++) {
		prv[numBall] = 0;
	}

	for(uint8_t numBall = 0; numBall < QTY; numBall ++) {
		pinMode(PORT[numBall], INPUT);
	}

	cCatch.set_MAX(MAX_C_CATCH);
	cCatch.set_COUNT_UP(false);
	cMayKick.set_MAX(MAX_C_MAY_KICK);
}


vectorRT_t Ball::get() {
	vectorRT_t vRT = {0, 0};
	bool findingBall = true;
	// 初期化
	for(int numBall = 0; numBall < QTY; numBall ++) {
		val[numBall] = 0;
	}
	// 計測
	uint64_t time = micros();
	uint16_t countMax = 0;
	while(micros() - time < CYCLE * MEASURING_COUNT) {
		countMax ++;
		for(uint8_t numBall = 0; numBall < QTY; numBall ++) {
			val[numBall] += !digitalRead(PORT[numBall]);
		}
	}

	// 比率化
	for(uint8_t numBall = 0; numBall < QTY; numBall ++) {
		val[numBall] *= 1000.0 / (double) countMax;
	}

	for(uint8_t numBall = 0; numBall < QTY; numBall ++) {
		if(val[numBall] > 0) {
			findingBall = false;
			// 平均値計算
			if(prv[numBall] > 0) {
				val[numBall] = filter(val[numBall], prv[numBall], CHANGE_RATE);
			}
		}
		// 平均値保存
		prv[numBall] = val[numBall];
	}

	// 距離計算
	vRT.r = mean(val, QTY);
	// 弱反応切り捨て
	bool isAllWeak = true;
	for(uint8_t numBall = 0; numBall < QTY; numBall ++) {
		if(val[numBall] > THRE_WEAK) {
			isAllWeak = false;
			break;
		}
	}
	for(uint8_t numBall = 0; numBall < QTY; numBall ++) {
		weak[numBall] = isAllWeak ? val[numBall] : max(val[numBall] - THRE_WEAK, 0);
	}
	// ベクトル合成
	vectorXY_t vXY = {0, 0};
	for(uint8_t numBall = 0; numBall < QTY; numBall ++) {
		vXY.x += weak[numBall] * COS_IR[numBall];
		vXY.y += weak[numBall] * SIN_IR[numBall];
	}
	vRT.t = toDegrees(atan2(vXY.y, vXY.x));

	// null判定
	if(findingBall) {
		vRT.t = false;
	}else {
		vRT.t += PLUS_T;
	}
	vRT.r *= map(double(abs(vRT.t)), 0, 180, 2.5, 4.9);

	vRT.r = filter(vRT.r, prvBall.r, CHANGE_RATE_T);

	prvBall = vRT;

	trace(2) { Serial.println("Ball:"+str(val,QTY)); }

	return vRT;
}

uint16_t *Ball::getVal() {
	for(int numBall = 0; numBall < QTY; numBall ++) {
		crt[numBall] = val[numBall];
	}
	return crt;
}

uint8_t Ball::getQTY() {
	return QTY;
}


uint16_t Ball::getForward() {
	return val[0];
}


Angle Ball::getDir(vectorRT_t ball) {
	Angle dir = ball.t;
	if(bool(ball.t)) {
		double plusDir[SIZE_THRE_DIST];
		for(uint8_t distIndex = 0; distIndex < SIZE_THRE_DIST; distIndex ++) {
			plusDir[distIndex] = polyLine(double(dir), DIR[distIndex], PLUS_DIR[distIndex], SIZE_DIR);
		}
		dir += polyLine(ball.r, THRE_DIST, plusDir, SIZE_THRE_DIST);
	}
	return dir;
}


bool Ball::getCatch() {
	valCatch = analogRead(P_CATCH);
	cCatch.increase(valCatch < THRE_CATCH);
	return bool(cCatch);
}

bool Ball::getMayKick() {
	cMayKick.increase(valCatch < THRE_CATCH);
	return bool(cMayKick);
}

bool Ball::compareCatch(double rate) {
	return cCatch.compare(rate);
}

uint16_t Ball::getValCatch() {
	return valCatch;
}
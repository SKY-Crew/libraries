#include "Ball.h"

Ball::Ball(uint8_t QTY, uint8_t *PORT,
	uint8_t MEASURING_COUNT, uint16_t THRE_WEAK, double CHANGE_RATE,
	uint8_t SIZE_THRE_DIST, double *THRE_DIST, uint8_t SIZE_DIR, double **DIR, double **PLUS_DIR,
	uint8_t P_CATCH, uint16_t THRE_CATCH, uint8_t MAX_C_CATCH,
	uint8_t P_IN_AIR, uint16_t THRE_IN_AIR) {
	//copy
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

	this->SIZE_THRE_DIST = SIZE_THRE_DIST;
	this->THRE_DIST = new double[SIZE_THRE_DIST];
	copyArray(this->THRE_DIST, THRE_DIST, SIZE_THRE_DIST);
	this->SIZE_DIR = SIZE_DIR;
	this->DIR = new double*[SIZE_THRE_DIST];
	copyArray(this->DIR, DIR, SIZE_THRE_DIST);
	this->PLUS_DIR = new double*[SIZE_THRE_DIST];
	copyArray(this->PLUS_DIR, PLUS_DIR, SIZE_THRE_DIST);

	value = new uint16_t[QTY];
	weak = new uint16_t[QTY];
	prv = new uint16_t[QTY];
	crt = new uint16_t[QTY];


	this->P_CATCH = P_CATCH;
	this->THRE_CATCH = THRE_CATCH;


	this->P_IN_AIR = P_IN_AIR;
	this->THRE_IN_AIR = THRE_IN_AIR;


	for(uint8_t numBall = 0; numBall < QTY; numBall ++) {
		prv[numBall] = 0;
	}

	//init
	for(uint8_t numBall = 0; numBall < QTY; numBall ++) {
		pinMode(PORT[numBall], INPUT);
	}

	cCatch.set_MAX(MAX_C_CATCH);


	pinMode(P_IN_AIR, INPUT);
}


vectorRT_t Ball::get() {
	vectorRT_t vRT = {0, 0};
	bool findingBall = true;
	//初期化
	for(int numBall = 0; numBall < QTY; numBall ++) {
		value[numBall] = 0;
	}
	valueInAir = 0;
	//計測
	uint64_t time = micros();
	uint16_t countMax = 0;
	while(micros() - time < CYCLE * MEASURING_COUNT) {
		countMax ++;
		for(uint8_t numBall = 0; numBall <= QTY; numBall ++) {
			if(numBall == QTY) {
				valueInAir += !digitalRead(P_IN_AIR);
			}else {
				value[numBall] += !digitalRead(PORT[numBall]);
			}
		}
	}

	//比率化
	for(uint8_t numBall = 0; numBall < QTY; numBall ++) {
		value[numBall] *= 1000.0 / (double) countMax;
	}
	valueInAir *= 1000.0 / (double) countMax;

	for(uint8_t numBall = 0; numBall < QTY; numBall ++) {
		if(value[numBall] > 0) {
			findingBall = false;
			// 平均値計算
			if(prv[numBall] > 0) {
				value[numBall] = filter(value[numBall], prv[numBall], CHANGE_RATE);
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

	//diffInAir計算
	diffInAir = valueInAir - vRT.r;

	trace(2) {
		Serial.print(str("Ball:[ "));
		for(uint8_t numBall = 0; numBall < QTY; numBall ++) {
			Serial.print(str(value[numBall])+" ");
		}
		Serial.println(str("]"));
	}

	trace(3) { Serial.println("Ball(InAir):"+str(valueInAir)); }

	return vRT;
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


uint16_t Ball::getForward() {
	return value[0];
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
	valueCatch = analogRead(P_CATCH);
	cCatch.increase(valueCatch < THRE_CATCH);
	return bool(cCatch);
}

bool Ball::compareCatch(double rate) {
	return cCatch.compare(rate);
}

uint16_t Ball::getValueCatch() {
	return valueCatch;
}


bool Ball::getIsInAir() {
	return diffInAir >= THRE_IN_AIR;
}

uint16_t Ball::getValueInAir() {
	return valueInAir;
}
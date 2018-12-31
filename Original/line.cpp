#include "line.h"

Line::Line(bool get_CAN_LEAVE_LINE, uint8_t get_QTY, uint8_t *get_PORT, uint8_t get_MAX_CIIA,
	uint16_t get_BORDER_BLACK, uint16_t get_BORDER_WHITE, uint8_t get_BORDER_IS_IN_AIR, double get_MULTI_AVG) {
	//copy
	CAN_LEAVE_LINE = get_CAN_LEAVE_LINE;

	QTY = get_QTY;
	PORT = new uint8_t[QTY];
	copyArray(PORT, get_PORT, QTY);

	MAX_CIIA = get_MAX_CIIA;
	BORDER_BLACK = get_BORDER_BLACK;
	BORDER_WHITE = get_BORDER_WHITE;
	BORDER_IS_IN_AIR = get_BORDER_IS_IN_AIR;

	MULTI_AVG = get_MULTI_AVG;

	value = new uint16_t[QTY];
	crt = new uint16_t[QTY];
	state = new uint8_t[QTY];
	crtState = new uint8_t[QTY];
}

line_t Line::get(bool isFW, bool canUseGyro, int16_t gyro) {
	int qtyILW = 0;
	int qtyILB = 0;
	for(int numLine = 0; numLine < QTY; numLine ++) {
		//ライン読み取り
		value[numLine] = analogRead(PORT[numLine]);
		state[numLine] = value[numLine] <= BORDER_BLACK ? 2
			: value[numLine] >= BORDER_WHITE ? 1 : 0;
		switch(state[numLine]) {
			case 2: qtyILB ++; break;
			case 1: qtyILW ++; break;
			case 0: break;
		}
	}

	if(!CAN_LEAVE_LINE) {
		//ライン無効
		line = {false, false, false, false, -1, false}
	}else if(qtyILB >= BORDER_IS_IN_AIR) {
		//持ち上げられている
		countIIA ++;
		if(countIIA >= MAX_CIIA) {
			countIIA = MAX_CIIA;
			line = {false, false, false, true, -1, false}
		}
	}else if(qtyILW <= 1) {
		//ライン上でない
		countIIA = 0;
		line.isOutside = line.isHalfOut && prvDI >= 0;
		line.isWholeOut = line.isOutside;
		line.dirInside = line.isOutside ? prvDI : -1;
	}else {
		//ライン上
		countIIA = 0;
		//白の番号を調べる
		int posILW[qtyILW];
		int numILW = 0;
		for(int numLine = 0; numLine < QTY; numLine ++) {
			if(state[numLine] == 1) {
				posILW[numILW] = numLine;
				numILW ++;
			}
		}
		//白の間隔を調べる
		int intvLine[qtyILW];
		for(int numILW = 0; numILW < qtyILW - 1; numILW ++) {
			intvLine[numILW] = posILW[numILW + 1] - posILW[numILW];
		}
		intvLine[qtyILW - 1] = posILW[0] - posILW[qtyILW - 1] + QTY;
		//ラインの方向を調べる
		int maxIntvL = 0;
		int posMaxIntvL = 0;
		for(numILW = 0; numILW < qtyILW; numILW ++) {
			if(maxIntvL < intvLine[numILW]) {
				maxIntvL = intvLine[numILW];
				posMaxIntvL = numILW;
			}
		}
		double numDI = posILW[posMaxIntvL] + maxIntvL * 0.5;
		line.dirInside = simplifyDeg(numDI / QTY * 360);
		//前回と比較
		if(prvDI >= 0) {
			//半分以上外か
			line.isHalfOut = line.isWholeOut;
			if(line.isHalfOut || insideAngle(line.dirInside, prvDI + 110, prvDI + 250)) {
				line.dirInside = simplifyDeg(line.dirInside + 180);
				line.isHalfOut = true;
			}
			//平均値計算
			if(abs(line.dirInside - prvDI) <= 180) {
				line.dirInside = prvDI * MULTI_AVG + line.dirInside * (1 - MULTI_AVG);
			}else {
				line.dirInside = prvDI * MULTI_AVG + line.dirInside * (1 - MULTI_AVG)
								+ 360 * (line.dirInside >= prvDI ? MULTI_AVG : 1 - MULTI_AVG);
			}
			line.dirInside = simplifyDeg(line.dirInside);
		}

		line.isOutside = false;
		line.isWholeOut = false;

		//後ろのライン・前方の角にいるか
		if(canUseGyro) {
			line.isOutside |= insideAngle(line.dirInside - gyro + 360, 125, 235) || insideAngle(line.dirInside - gyro + 360, 305, 55);
			line.canPause = line.isHalfOut;
		}else {
			line.isOutside |= line.isHalfOut;
			line.canPause = true;
		}
		if(!isFW) {
			line.isOutside = true;
		}
	}

	prvDI = line.dirInside;
	return line;
}

uint16_t *Line::getValue() {
	for(int numLine = 0; numLine < QTY; numLine ++) {
		crt[numLine] = value[numLine];
	}
	return crt;
}

uint8_t *Line::getState() {
	for(int numLine = 0; numLine < QTY; numLine ++) {
		crtState[numLine] = state[numLine];
	}
	return crtState;
}

uint8_t Line::getQTY() {
	return QTY;
}
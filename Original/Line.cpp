#include "Line.h"

Line::Line(bool given_CAN_LEAVE_LINE, uint8_t given_QTY, uint8_t *given_PORT, uint8_t given_MAX_CIIA,
	uint16_t given_BORDER_BLACK, uint16_t given_BORDER_WHITE, uint8_t given_BORDER_IS_IN_AIR, double given_MULTI_AVG) {
	//copy
	CAN_LEAVE_LINE = given_CAN_LEAVE_LINE;

	QTY = given_QTY;
	PORT = new uint8_t[QTY];
	copyArray(PORT, given_PORT, QTY);

	MAX_CIIA = given_MAX_CIIA;
	BORDER_BLACK = given_BORDER_BLACK;
	BORDER_WHITE = given_BORDER_WHITE;
	BORDER_IS_IN_AIR = given_BORDER_IS_IN_AIR;

	MULTI_AVG = given_MULTI_AVG;

	value = new uint16_t[QTY];
	crt = new uint16_t[QTY];
	state = new Color[QTY];
	crtState = new Color[QTY];
}

line_t Line::get(bool isFW, bool canUseGyro, Angle gyro) {
	int qtyILW = 0;
	int qtyILB = 0;
	for(int numLine = 0; numLine < QTY; numLine ++) {
		//ライン読み取り
		value[numLine] = analogRead(PORT[numLine]);
		state[numLine] = value[numLine] <= BORDER_BLACK ? BLACK
			: value[numLine] >= BORDER_WHITE ? WHITE : GREEN;
		switch(state[numLine]) {
			case BLACK: qtyILB ++; break;
			case WHITE: qtyILW ++; break;
			case GREEN: break;
		}
	}

	if(!CAN_LEAVE_LINE) {
		//ライン無効
		line = {false, false, false, false, false, false};
	}else if(qtyILB >= BORDER_IS_IN_AIR) {
		//持ち上げられている
		countIIA ++;
		if(countIIA >= MAX_CIIA) {
			countIIA = MAX_CIIA;
			line = {false, false, false, true, false, false};
		}
	}else if(qtyILW <= 1) {
		//ライン上でない
		countIIA = 0;
		line.isOutside = line.isHalfOut && bool(prvDI);
		line.isWholeOut = line.isOutside;
		line.dirInside = line.isOutside ? prvDI : Angle(false);
	}else {
		//ライン上
		countIIA = 0;
		//白の番号を調べる
		int posILW[qtyILW];
		int numILW = 0;
		for(int numLine = 0; numLine < QTY; numLine ++) {
			if(state[numLine] == WHITE) {
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
		line.dirInside = numDI / QTY * 360;
		//前回と比較
		if(bool(prvDI)) {
			//半分以上外か
			line.isHalfOut = false;
			if(line.isWholeOut || line.dirInside.inside(prvDI + 110, prvDI + 250)) {
				line.dirInside += 180;
				line.isHalfOut = true;
			}
			//平均値計算
			if(!line.isWholeOut) {
				if(absAngle(line.dirInside - prvDI) <= 180) {
					line.dirInside = prvDI * MULTI_AVG + line.dirInside * (1 - MULTI_AVG);
				}else {
					line.dirInside = prvDI * MULTI_AVG + line.dirInside * (1 - MULTI_AVG)
									+ 360 * (line.dirInside >= prvDI ? MULTI_AVG : 1 - MULTI_AVG);
				}
			}
		}

		line.isOutside = false;
		line.isWholeOut = false;

		//後ろのライン・前方の角にいるか
		if(canUseGyro) {
			line.isOutside |= (line.dirInside - gyro).inside(125, 235)
				|| (line.dirInside - gyro).inside(305, 55);
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

Color *Line::getState() {
	for(int numLine = 0; numLine < QTY; numLine ++) {
		crtState[numLine] = state[numLine];
	}
	return crtState;
}

uint8_t Line::getQTY() {
	return QTY;
}
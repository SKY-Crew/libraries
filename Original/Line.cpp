#include "Line.h"

Line::Line(bool CAN_LEAVE_LINE, uint8_t QTY, uint8_t *PORT, uint8_t MAX_CIIA,
	uint16_t BORDER_BLACK, uint16_t BORDER_WHITE, uint8_t BORDER_IS_IN_AIR, double MULTI_AVG) {
	//copy
	this->CAN_LEAVE_LINE = CAN_LEAVE_LINE;

	this->QTY = QTY;
	PORT = new uint8_t[QTY];
	copyArray(PORT, PORT, QTY);

	this->MAX_CIIA = MAX_CIIA;
	this->BORDER_BLACK = BORDER_BLACK;
	this->BORDER_WHITE = BORDER_WHITE;
	this->BORDER_IS_IN_AIR = BORDER_IS_IN_AIR;

	this->MULTI_AVG = MULTI_AVG;

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
		line.dirInside = line.isOutside ? prvDI : false;
		if(bool(gyro) && bool(prvGyro) && bool(line.dirInside)) {
			line.dirInside += gyro - prvGyro;
		}
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
			if(!line.isWholeOut && bool(prvDI)) {
				line.dirInside = filter(line.dirInside, prvDI, MULTI_AVG);
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
	prvGyro = gyro;
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
#include "Line.h"

Line::Line(bool CAN_LEAVE_LINE, uint8_t QTY, uint8_t *PORT, uint8_t MAX_CIIA,
	uint16_t THRE_BLACK, uint16_t THRE_WHITE, uint8_t THRE_IS_IN_AIR, double CHANGE_RATE) {
	//copy
	this->CAN_LEAVE_LINE = CAN_LEAVE_LINE;

	this->QTY = QTY;
	this->PORT = new uint8_t[QTY];
	copyArray(this->PORT, PORT, QTY);

	this->MAX_CIIA = MAX_CIIA;
	this->THRE_BLACK = THRE_BLACK;
	this->THRE_WHITE = THRE_WHITE;
	this->THRE_IS_IN_AIR = THRE_IS_IN_AIR;

	this->CHANGE_RATE = CHANGE_RATE;

	val = new uint16_t[QTY];
	crt = new uint16_t[QTY];
	state = new Color[QTY];
	crtState = new Color[QTY];
}

line_t Line::get(bool isFW, Angle gyro, Angle diffGyro) {
	int qtyILW = 0;
	int qtyILB = 0;
	for(int numLine = 0; numLine < QTY; numLine ++) {
		//ライン読み取り
		val[numLine] = analogRead(PORT[numLine]);
		state[numLine] = val[numLine] <= THRE_BLACK ? BLACK
			: val[numLine] >= THRE_WHITE ? WHITE : GREEN;
		switch(state[numLine]) {
			case BLACK: qtyILB ++; break;
			case WHITE: qtyILW ++; break;
			case GREEN: break;
		}
	}

	if(!CAN_LEAVE_LINE) {
		//ライン無効
		line = {false, false, false, false, false, false};
	}else if(qtyILB >= THRE_IS_IN_AIR) {
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
		if(bool(gyro) && bool(line.dirInside)) {
			line.dirInside += diffGyro;
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
			if(line.isWholeOut || (line.dirInside - prvDI).isDown(70)) {
				line.dirInside += 180;
				line.isHalfOut = true;
			}
			//平均値計算
			if(!line.isWholeOut && bool(prvDI)) {
				line.dirInside = filterAngle(line.dirInside, prvDI, CHANGE_RATE);
			}
		}

		line.isOutside = false;
		line.isWholeOut = false;

		//後ろのライン・前方の角にいるか
		isLineFront = (line.dirInside - gyro).isDown(55);
		if(bool(gyro)) {
			line.isOutside |= (line.dirInside - gyro).isUp(55) || isLineFront;
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

	trace(4) {
		Serial.print(str("Line:[ "));
		for(uint8_t numLine = 0; numLine < QTY; numLine ++) {
			Serial.print(str(val[numLine])+" ");
		}
		Serial.println(str("]"));
	}

	return line;
}

bool Line::getIsLineFront() {
	return isLineFront;
}

uint16_t *Line::getVal() {
	for(int numLine = 0; numLine < QTY; numLine ++) {
		crt[numLine] = val[numLine];
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
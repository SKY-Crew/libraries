#ifndef ANGLE_ORIG
#define ANGLE_ORIG

#include "Arduino.h"

class Angle {
	private:
		bool hasValue;
		double angle;
	public:
		inline Angle(bool given_hasValue = false, double given_angle = 0) {
			hasValue = given_hasValue;
			angle = hasValue ? simplifyDeg(given_angle) : 0;
		}
		inline Angle(double given_angle) {
			hasValue = true;
			angle = simplifyDeg(given_angle);
		}

		inline String string() const{ return (hasValue ? String(angle) : "null"); }
		inline explicit operator bool() const{ return this->hasValue; }
		inline explicit operator double() const{ return this->angle; }

		inline Angle operator+() const{
			return Angle(bool(*this), + double(*this));
		}
		inline Angle operator-() const{
			return Angle(bool(*this), - double(*this));
		}
		inline Angle& operator+=(const Angle& b) {
			(*this) = Angle(bool(*this) || bool(b), double(*this) + double(b));
			return *this;
		}
		inline Angle& operator-=(const Angle& b) {
			(*this) = Angle(bool(*this) || bool(b), double(*this) - double(b));
			return *this;
		}
		inline Angle& operator*=(double multi) {
			(*this) = Angle(bool(*this), double(*this) * multi);
			return *this;
		}
		inline Angle& operator/=(double div) {
			(*this) = Angle(bool(*this), double(*this) / div);
			return *this;
		}

		bool inside(const Angle&, const Angle&) const;
};

const Angle operator+(const Angle& a, const Angle& b) { return Angle(a) += b; }
const Angle operator-(const Angle& a, const Angle& b) { return Angle(a) -= b; }
const Angle operator*(const Angle& a, double multi) { return Angle(a) *= multi; }
const Angle operator/(const Angle& a, double div) { return Angle(a) /= div; }

bool operator>(const Angle& a, const Angle& b) {
	return a && (!bool(b) || double(a) > double(b));
}
bool operator<(const Angle& a, const Angle& b) { return b > a; }
bool operator==(const Angle& a, const Angle& b) { return !(a > b) && !(a < b); }
bool operator!=(const Angle& a, const Angle& b) { return !(a == b); }
bool operator>=(const Angle& a, const Angle& b) { return a > b || a == b; }
bool operator<=(const Angle& a, const Angle& b) { return b >= a; }

bool Angle::inside(const Angle& A_min, const Angle& A_max) const{
	return bool(*this) && (*this - A_min - 180.) <= (A_max - A_min - 180.);
}

double diff(const Angle& a, const Angle& b) {
	return max(double(a - b), double(b - a));
}

double cos(const Angle& a) {
	return bool(a) ? advRound(sin(toRadians(double(a))), 5) : 0;
}
double sin(const Angle& a) {
	return bool(a) ? advRound(sin(toRadians(double(a))), 5) : 0;
}
double tan(const Angle& a) {
	return bool(a) ? advRound(tan(toRadians(double(a))), 5) : 0;
}

#endif
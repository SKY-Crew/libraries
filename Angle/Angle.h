#ifndef ANGLE_ORIG
#define ANGLE_ORIG

#include <Arduino.h>
#include "AdvMath.h"

class Angle {
	private:
		bool hasValue;
		double angle;
	public:
		inline Angle(bool hasValue = false, double angle = 0) {
			setAngle(hasValue, angle);
		}
		inline Angle(bool hasValue, int16_t angle) {
			setAngle(hasValue, (double)angle);
		}
		inline Angle(double angle) { setAngle(true, angle); }
		inline Angle(int angle) { setAngle(true, (double)angle); }
		inline void setAngle(bool hasValue = false, double angle = 0) {
			this->hasValue = hasValue;
			this->angle = hasValue ? simplifyDeg(angle) : 0;
		}

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
		bool isUp(const Angle&) const;
		bool isRight(const Angle&) const;
		bool isDown(const Angle&) const;
		bool isLeft(const Angle&) const;
};

inline String toString(const Angle& a) { return (bool(a) ? String((int)double(a)) : "null"); }

inline const Angle operator+(const Angle& a, const Angle& b) { return Angle(a) += b; }
inline const Angle operator-(const Angle& a, const Angle& b) { return Angle(a) -= b; }
inline const Angle operator*(const Angle& a, double multi) { return Angle(a) *= multi; }
inline const Angle operator/(const Angle& a, double div) { return Angle(a) /= div; }

inline bool operator>(const Angle& a, const Angle& b) {
	return a && (!bool(b) || double(a) > double(b));
}
inline bool operator<(const Angle& a, const Angle& b) { return b > a; }
inline bool operator==(const Angle& a, const Angle& b) { return !(a > b) && !(a < b); }
inline bool operator!=(const Angle& a, const Angle& b) { return !(a == b); }
inline bool operator>=(const Angle& a, const Angle& b) { return a > b || a == b; }
inline bool operator<=(const Angle& a, const Angle& b) { return b >= a; }

inline bool Angle::inside(const Angle& A_min, const Angle& A_max) const{
	return bool(*this) && (*this - A_min - 180) <= (A_max - A_min - 180);
}
inline bool Angle::isUp(const Angle& A_diff) const{
	return this->inside(0 - A_diff, 0 + A_diff);
}
inline bool Angle::isRight(const Angle& A_diff) const{
	return this->inside(90 - A_diff, 90 + A_diff);
}
inline bool Angle::isDown(const Angle& A_diff) const{
	return this->inside(180 - A_diff, 180 + A_diff);
}
inline bool Angle::isLeft(const Angle& A_diff) const{
	return this->inside(270 - A_diff, 270 + A_diff);
}

inline double absAngle(const Angle& a) {
	return bool(a) ? abs(double(a)) : 0;
}
inline int8_t signum(const Angle& a) {
	return bool(a) ? signum(double(a)) : 0;
}
inline Angle filterAngle(const Angle& a, const Angle& avg, double CHANGE_RATE) {
	return filter(double(avg), double(a), CHANGE_RATE)
	 + (abs(double(a) - double(avg)) > 180 ? 360 * (a >= avg ? CHANGE_RATE : 1 - CHANGE_RATE) : 0);
}

inline double diff(const Angle& a, const Angle& b) {
	return absAngle(a - b);
}

inline double cos(const Angle& a) {
	return bool(a) ? advRound(cos(toRadians(double(a))), 5) : 0;
}
inline double sin(const Angle& a) {
	return bool(a) ? advRound(sin(toRadians(double(a))), 5) : 0;
}
inline double tan(const Angle& a) {
	return bool(a) ? advRound(tan(toRadians(double(a))), 5) : 0;
}

typedef struct {
	double r;
	Angle t;
} vectorRT_t;

#endif
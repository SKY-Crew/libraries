#ifndef ANGLE_ORIG
#define ANGLE_ORIG

#include <Arduino.h>
#include "advMath.h"

class Angle {
	private:
		bool hasValue;
		double angle;
	public:
		inline Angle(bool given_hasValue = false, double given_angle = 0) {
			setAngle(given_hasValue, given_angle);
		}
		inline Angle(bool given_hasValue, int16_t given_angle) {
			setAngle(given_hasValue, (double)given_angle);
		}
		inline Angle(double given_angle) { setAngle(true, given_angle); }
		inline Angle(int given_angle) { setAngle(true, (double)given_angle); }
		inline void setAngle(bool given_hasValue = false, double given_angle = 0) {
			hasValue = given_hasValue;
			angle = hasValue ? simplifyDeg(given_angle) : 0;
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
};

inline String string(const Angle& a) { return (bool(a) ? String((int)double(a)) : "null"); }

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
	return bool(*this) && (*this - A_min - 180.) <= (A_max - A_min - 180.);
}

inline double diff(const Angle& a, const Angle& b) {
	return max(double(a - b), double(b - a));
}

inline double absAngle(const Angle& a) {
	return bool(a) ? abs(double(a)) : 0;
}
inline int8_t signum(const Angle& a) {
	return bool(a) ? signum(double(a)) : 0;
}
inline Angle avg(const Angle& a, const Angle& avg, double MULTI_AVG) {
	Angle ans;
	if(abs(double(a) - double(avg)) <= 180) {
		ans = avg * MULTI_AVG + a * (1 - MULTI_AVG);
	}else {
		ans = avg * MULTI_AVG + a * (1 - MULTI_AVG)
						+ 360 * (a >= avg ? MULTI_AVG : 1 - MULTI_AVG);
	}
	return ans;
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
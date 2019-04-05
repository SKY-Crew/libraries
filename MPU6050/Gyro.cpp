#include "Gyro.h"

void Gyro::_gyro_initialize() {
  _gyro_mpu->initialize();
  _gyro_mpu->dmpInitialize();
  _gyro_mpu->setDMPEnabled(true);
}

Gyro::Gyro(uint8_t P_WIRE, uint8_t PORT, uint8_t ONOFF_PIN, uint8_t RESET_PIN,
  uint8_t SIZE_POINT, double *POINT, double *ROT, double Kd,
  uint8_t BROKEN_THRE, uint8_t STOP_FRAMES, uint8_t STAY_THRE) {
  //copy
  wGyro.set(P_WIRE);

  _gyro_mpu = new MPU6050(PORT);

  this->ONOFF_PIN = ONOFF_PIN;
  this->RESET_PIN = RESET_PIN;
  this->SIZE_POINT = SIZE_POINT;
  this->POINT = new double[SIZE_POINT];
  copyArray(this->POINT, POINT, SIZE_POINT);
  this->ROT = new double[SIZE_POINT];
  copyArray(this->ROT, ROT, SIZE_POINT);

  this->Kd = Kd;

  this->BROKEN_THRE = BROKEN_THRE;

  //init
  wGyro.get()->begin();
  wGyro.get()->setClock(400000);

  pinMode(MISO, OUTPUT);

  _gyro_initialize();

  pinMode(RESET_PIN, INPUT);
  _gyro_oldPinState = digitalRead(RESET_PIN);

  pinMode(ONOFF_PIN, INPUT);

  stayCounter.set_MAX(STAY_THRE);
  brokenCounter.set_MAX(STOP_FRAMES);
  brokenCounter.set_COUNT_UP(false);
}

float Gyro::_gyro_calcYaw(uint8_t *fifoBuf) {
  Quaternion q;
  VectorFloat gravity;
  float ypr[3];

  _gyro_mpu->dmpGetQuaternion(&q, fifoBuf);
  _gyro_mpu->dmpGetGravity(&gravity, &q);
  _gyro_mpu->dmpGetYawPitchRoll(ypr, &q, &gravity);
  return ypr[0];
}

Angle Gyro::get() {
  prv = crt;
  if(!bool(brokenCounter) && !digitalRead(ONOFF_PIN)) {
    int fifoSize;
    const int packetSize = 42;
    uint8_t fifoBuf[64];
    float yaw;
    int16_t rounded;
    int newPinState;

    fifoSize = _gyro_mpu->getFIFOCount();
    while(fifoSize > 768) {
      _gyro_mpu->resetFIFO();
      fifoSize = _gyro_mpu->getFIFOCount();
    }

    if(fifoSize < 2 * packetSize) {// while(fifoSize < 2 * packetSize) {
      return toSend;//   fifoSize = _gyro_mpu->getFIFOCount();
    }

    _gyro_mpu->getFIFOBytes(fifoBuf, packetSize);
    fifoSize -= packetSize;

    yaw = _gyro_calcYaw(fifoBuf) * M_PI * -18.2369953125F;
    newPinState = digitalRead(RESET_PIN);

    rounded = (int16_t)round(yaw);
    toSend = rounded - _gyro_base;

    if (newPinState == HIGH && _gyro_oldPinState == LOW) _gyro_base = rounded;

    _gyro_oldPinState = newPinState;

    crt = toSend;
    brokenCounter.increase(abs(crt - prv) > BROKEN_THRE);
    stayCounter.increase(prv == crt);
    if (bool(stayCounter)) {
      _gyro_mpu->resetFIFO();
    }
    return crt;
  }else {
    brokenCounter.increase(false);
    return crt = false;
  }
}

Angle Gyro::getDiff() {
  return crt - prv;
}

int16_t Gyro::multiRot(Angle origin) {
  double error[] = {0, 0}; // P, D
  error[0] = absMinus(double(crt - origin), 1.5);
  error[1] = digitalRead(RESET_PIN) ? 0 : filter(double(getDiff()) * 10, error[1], 0.6);
  error[1] = absMinus(error[1], 1);
  return polyLine(error[0], POINT, ROT, SIZE_POINT) + error[1] * Kd;
}

bool Gyro::getCanUse() {
  return !digitalRead(ONOFF_PIN);
}
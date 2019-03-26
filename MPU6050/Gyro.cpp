#include "Gyro.h"

void Gyro::_gyro_initialize() {
  _gyro_mpu->initialize();
  _gyro_mpu->dmpInitialize();
  _gyro_mpu->setDMPEnabled(true);
  _gyro_mpu->initialize();
  _gyro_mpu->dmpInitialize();
  _gyro_mpu->setDMPEnabled(true);
}

Gyro::Gyro(uint8_t P_WIRE, uint8_t PORT, uint8_t ONOFF_PIN, uint8_t RESET_PIN,
  uint8_t SIZE_POINT, double *POINT, double *ROT, double Kd) {
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
  
  //init
  wGyro.get()->begin();
  wGyro.get()->setClock(400000);

  pinMode(MISO, OUTPUT);

  _gyro_initialize();

  pinMode(RESET_PIN, INPUT);
  _gyro_oldPinState = digitalRead(RESET_PIN);

  pinMode(ONOFF_PIN, INPUT);
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
  if(!digitalRead(ONOFF_PIN)) {
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
    return crt;
  }else {
    crt = false;
    return false;
  }
}

Angle Gyro::getDiff() {
  return crt - prv;
}

int16_t Gyro::multiRot(Angle origin) {
  int16_t rot = 0;
  double error[] = {0, 0};//P, D
  error[0] = signum(crt) * (max(1.5, abs((double)(crt - origin))) - 1.5);
  error[1] = digitalRead(RESET_PIN) ? 0 : filter((double)getDiff() * 10, error[1], 0.6);
  error[1] = signum(error[1]) * (max(1, abs(error[1])) - 1);

  uint8_t index = 0;
  for(; index < SIZE_POINT; index ++) {
    if(POINT[index] > absAngle(error[0])) { break; }
  }
  if(index == SIZE_POINT) {
    rot = ROT[index - 1];
  }else {
    rot = map(absAngle(error[0]),
      POINT[index - 1], POINT[index],
      ROT[index - 1], ROT[index]);
  }
  rot *= signum(error[0]);

  rot += error[1] * Kd;
  return rot;
}

bool Gyro::getCanUse() {
  return !digitalRead(ONOFF_PIN);
}
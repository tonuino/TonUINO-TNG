#include "mpu6050.hpp"

#include "constants.hpp"

#ifdef MPU6050_TAP_DETECTION
#include <Wire.h>

#include "logger.hpp"

namespace {

constexpr uint16_t avr_weight = 4;
constexpr int16_t  tabLimit   = 2000;
constexpr uint16_t blockTime  = 500;

constexpr uint8_t MPU6050_ADDR         = 0x68; // Alternatively set AD0 to HIGH  --> Address = 0x69
constexpr uint8_t MPU6050_GYRO_CONFIG  = 0x1B; ///< Gyro specfic configuration register
constexpr uint8_t MPU6050_ACCEL_CONFIG = 0x1C;
constexpr uint8_t MPU6050_ACCEL_XOUT_H = 0x3B;
constexpr uint8_t MPU6050_PWR_MGT_1    = 0x6B;
constexpr uint8_t MPU6050_SLEEP        = 0x06;

constexpr uint8_t MPU6050_INT_PIN_CFG       = 0x37; // Interrupt Pin / Bypass Enable Configuration Register
constexpr uint8_t MPU6050_INT_ENABLE        = 0x38; // Interrupt Enable Register
constexpr uint8_t MPU6050_LATCH_INT_EN      = 0x05; // Latch Enable Bit for Interrupt
constexpr uint8_t MPU6050_ACTL              = 0x07; // Active-Low Enable Bit
constexpr uint8_t MPU6050_WOM_EN            = 0x06; // Wake on Motion Enable bit
constexpr uint8_t MPU6050_WOM_THR           = 0x1F; // Wake on Motion Threshold Register
constexpr uint8_t MPU6050_MOT_DUR           = 0x20; // Motion Detection Duration Register
constexpr uint8_t MPU6050_ACCEL_INTEL_CTRL  = 0x69; // Accelaration Interrupt Control Register

#ifdef MPU6050_INT
volatile bool accEvent = false;
#else
constexpr bool accEvent = true;
#endif

enum mpu6050_acc_range {
  MPU6050_ACC_RANGE_2G,  // +/- 2g (default)
  MPU6050_ACC_RANGE_4G,  // +/- 4g
  MPU6050_ACC_RANGE_8G,  // +/- 8g
  MPU6050_ACC_RANGE_16G, // +/- 16g
};

enum mpu6050_gyr_range {
  MPU6050_GYR_RANGE_250,  // +/- 250 deg/s (default)
  MPU6050_GYR_RANGE_500,  // +/- 500 deg/s
  MPU6050_GYR_RANGE_1000, // +/- 1000 deg/s
  MPU6050_GYR_RANGE_2000, // +/- 2000 deg/s
};

void writeRegister(uint16_t reg, byte value){
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission(true);
}

void setAccRange(mpu6050_acc_range range){
  writeRegister(MPU6050_ACCEL_CONFIG, range<<3);
}

void setGyrRange(mpu6050_gyr_range range){
  writeRegister(MPU6050_GYRO_CONFIG, range<<3);
}

void MPU6050_wakeUp(){
  writeRegister(MPU6050_PWR_MGT_1, 0);
  delay(30); // give him time to wake up, gyro needs quite a while to stabilize;
}

void MPU6050_sleep(){
  writeRegister(MPU6050_PWR_MGT_1, 1<<MPU6050_SLEEP);
}

int16_t getAccX() {
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(MPU6050_ACCEL_XOUT_H);
  Wire.endTransmission(false);

  Wire.requestFrom(MPU6050_ADDR, 2u, true);

  const int16_t accX  = Wire.read()<<8 | Wire.read(); // reading registers: 0x3B (ACCEL_XOUT_H) and 0x3C (ACCEL_XOUT_L)

  return accX;
}

#ifdef MPU6050_INT
void setInterrupt(byte threshold){
//writeRegister(MPU6050_INT_PIN_CFG, 1<<MPU6050_ACTL); // 1<<MPU6050_LATCH_INT_EN
  writeRegister(MPU6050_ACCEL_CONFIG, 0b00000001);
  writeRegister(MPU6050_WOM_THR, threshold);
  writeRegister(MPU6050_MOT_DUR, 0b00000001);  // set duration (LSB = 1 ms)
//writeRegister(MPU6050_ACCEL_INTEL_CTRL, 0x15);  // not needed (?)
  writeRegister(MPU6050_INT_ENABLE, 1<<MPU6050_WOM_EN);
}

void motion(){
  accEvent = true;
  detachInterrupt(digitalPinToInterrupt(mpu6050InterruptPin));
}
#endif // MPU6050_INT
}

Mpu6050::Mpu6050(const
Settings& settings)
: CommandSource()
, settings{settings}
, avr{}
, block{}
{
}

void Mpu6050::init() {
  Wire.begin();
  MPU6050_wakeUp();
  setAccRange(MPU6050_ACC_RANGE_8G);
  setGyrRange(MPU6050_GYR_RANGE_250);

#ifdef MPU6050_INT
  setInterrupt(70); // set Wake on Motion Interrupt / Sensitivity; 1(highest sensitivity) - 255
  pinMode(mpu6050InterruptPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(mpu6050InterruptPin), motion, RISING);
#endif

  avr = getAccX();
}

commandRaw Mpu6050::getCommandRaw() {
  commandRaw ret = commandRaw::none;

  const int16_t accX  = getAccX();

  avr = (avr * (avr_weight-1)  + accX) / avr_weight;

  if (accEvent && (accX - avr) > tabLimit && not block) {
    LOG(button_log, s_info, F("Mpu6050: "), (accX - avr));
#ifdef FIVEBUTTONS
    ret = commandRaw::down;
#else
    ret = (settings.invertVolumeButtons==1)? commandRaw::downLong : commandRaw::down;
#endif
    block = blockTime/cycleTime;
  }

  if (accEvent && (accX - avr) < -tabLimit && not block) {
    LOG(button_log, s_info, F("Mpu6050: "), (accX - avr));
#ifdef FIVEBUTTONS
    ret = commandRaw::up;
#else
    ret = (settings.invertVolumeButtons==1)? commandRaw::upLong : commandRaw::up;
#endif
    block = blockTime/cycleTime;
  }
  if (block > 0)
    --block;

#ifdef  MPU6050_INT
  if (accEvent && not block) {
    LOG(button_log, s_debug, F("MPU6050: clear int"));
    accEvent = false;
    attachInterrupt(digitalPinToInterrupt(mpu6050InterruptPin), motion, RISING);
  }
#endif

  if (ret != commandRaw::none) {
    LOG(button_log, s_debug, F("Mpu6050 raw: "), static_cast<uint8_t>(ret));
  }
  return ret;
}

#endif // MPU6050_TAP_DETECTION

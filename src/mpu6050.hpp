#ifndef SRC_MPU6050_HPP_
#define SRC_MPU6050_HPP_

#include <Arduino.h>

#include "commands.hpp"
#include "constants.hpp"

class Mpu6050: public CommandSource {
public:
  Mpu6050(const Settings& settings);

  void init();

  commandRaw getCommandRaw() override;

private:

  int16_t getAccX();

  const Settings& settings;

  int32_t         avr;
  uint16_t        block;
};

#endif /* SRC_MPU6050_HPP_ */

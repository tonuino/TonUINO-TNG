#ifndef SRC_BATVOLTAGE_HPP_
#define SRC_BATVOLTAGE_HPP_

#include <Arduino.h>

#include "timer.hpp"

class Mp3;

class BatVoltage {
public:

  BatVoltage(Mp3& mp3);
  bool check();

  float getVoltage() { return voltage; }
  bool isLow      () { return low; }

private:
  void lowMessage();

  float voltage{};
  bool  low{};

  Timer logTimer{};
  Timer lowTimer{};
  Timer emptyTimer{};
  Mp3&  mp3;
};

#endif /* SRC_BATVOLTAGE_HPP_ */

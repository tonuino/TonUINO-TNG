#ifndef SRC_BATVOLTAGE_HPP_
#define SRC_BATVOLTAGE_HPP_

#include <Arduino.h>

#include "timer.hpp"

class Mp3;

class BatVoltage {
public:

  BatVoltage(Mp3& mp3);
  bool check();

private:
  void lowMessage();

  Timer logTimer{};
  Timer lowTimer{};
  Timer emptyTimer{};
  Mp3&  mp3;
};

#endif /* SRC_BATVOLTAGE_HPP_ */

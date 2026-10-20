#ifndef SRC_BATVOLTAGE_HPP_
#define SRC_BATVOLTAGE_HPP_

#include <Arduino.h>

#include "constants.hpp"
#include "timer.hpp"

class Mp3;
class Settings;

class BatVoltage {
public:

  BatVoltage(Mp3& mp3, Settings& settings);
  bool check();

  uint16_t getVoltage() { return voltage; }
  bool     isLow     () { return low; }

#ifdef BAT_VOLTAGE_MEASUREMENT_CALIBRATION
  void setVoltageMeasurementCorrection(uint16_t newVoltageMeasurementCorrection);
#endif


private:
  void lowMessage();

  uint16_t  voltage{};
  bool      low{};

  Timer     logTimer{};
  Timer     lowTimer{};
  Timer     emptyTimer{};
  Mp3&      mp3;
  Settings& settings;
};

#endif /* SRC_BATVOLTAGE_HPP_ */

#ifndef SRC_BUTTONS3X3_HPP_
#define SRC_BUTTONS3X3_HPP_

#include <Arduino.h>
#include "linearAnalogKeypad.h"


#include "commands.hpp"

class Buttons3x3: public CommandSource {
public:
  static constexpr uint16_t numLevels = 9;
#ifdef TonUINO_Classic
  static constexpr int16_t maxLevel =  900; // 1023
#endif
#ifdef TonUINO_Every
  static constexpr int16_t maxLevel =  900; // 1023
#endif
#ifdef TonUINO_Every_4808
  static constexpr int16_t maxLevel =  900; // 1023
#endif
#ifdef ALLinONE
  static constexpr int16_t maxLevel = 4300; // 4064;
#endif
#ifdef ALLinONE_Plus
  static constexpr int16_t maxLevel =  900; // 1023;
#endif
#ifdef TonUINO_Esp32
  static constexpr int16_t maxLevel = 2900; // 4095;
#endif

  Buttons3x3();
  void begin();
  commandRaw getCommandRaw() override;

private:
  linearAnalogKeypad buttons;
};

#endif /* SRC_BUTTONS3X3_HPP_ */

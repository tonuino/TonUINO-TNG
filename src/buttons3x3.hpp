#ifndef SRC_BUTTONS3X3_HPP_
#define SRC_BUTTONS3X3_HPP_

#include <Arduino.h>
#include "ezAnalogKeypad.h"


#include "commands.hpp"

class Buttons3x3: public CommandSource {
public:
  static constexpr uint8_t numLevels = 9;
  static constexpr int levels[numLevels+1] = {
                                           0
                                       ,  77
                                       , 148
                                       , 234
                                       , 306
                                       , 390
                                       , 465
                                       , 534
                                       , 593
                                       , 784
                                       };

  Buttons3x3();
  commandRaw getCommandRaw() override;

private:
  ezAnalogKeypad buttons;
};

#endif /* SRC_BUTTONS3X3_HPP_ */

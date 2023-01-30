#ifndef SRC_BUTTONS3X3_HPP_
#define SRC_BUTTONS3X3_HPP_

#include <Arduino.h>
#include "ezAnalogKeypad.h"


#include "commands.hpp"

class Buttons3x3: public CommandSource {
public:
  static constexpr uint8_t numLevels = 9;
  static constexpr int levels[numLevels+1] = {
#ifdef TonUINO_Classic
                                            0
                                       ,   92
                                       ,  194
                                       ,  307
                                       ,  405
                                       ,  512
                                       ,  608
                                       ,  698
                                       ,  773
                                       , 1023
#endif
#ifdef ALLinONE
                                            0
                                       ,  490
                                       ,  945
                                       , 1494
                                       , 1949
                                       , 2480
                                       , 2937
                                       , 3373
                                       , 3691
                                       , 4064
#endif
#ifdef ALLinONE_Plus
                                            0
                                       ,   77
                                       ,  148
                                       ,  234
                                       ,  306
                                       ,  390
                                       ,  465
                                       ,  534
                                       ,  593
                                       ,  784
#endif
                                       };

  Buttons3x3();
  commandRaw getCommandRaw() override;

private:
  ezAnalogKeypad buttons;
};

#endif /* SRC_BUTTONS3X3_HPP_ */

#ifndef SRC_BUTTONS3X3_HPP_
#define SRC_BUTTONS3X3_HPP_

#include <Arduino.h>
#include <ezAnalogKeypad.h>


#include "commands.hpp"

class Buttons3x3: public CommandSource {
public:
  Buttons3x3();
  commandRaw getCommandRaw() override;

private:
  ezAnalogKeypad buttons;
#ifdef BUTTONS3X3_PRESS_TWO
  uint8_t        first_button{0};
#endif
};

#endif /* SRC_BUTTONS3X3_HPP_ */

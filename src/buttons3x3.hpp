#ifndef SRC_BUTTONS3X3_HPP_
#define SRC_BUTTONS3X3_HPP_

#include <Arduino.h>
#include "ezAnalogKeypad.h"


#include "commands.hpp"

class Buttons3x3: public CommandSource {
public:
  Buttons3x3();
  commandRaw getCommandRaw() override;

private:
  ezAnalogKeypad buttons;
};

#endif /* SRC_BUTTONS3X3_HPP_ */

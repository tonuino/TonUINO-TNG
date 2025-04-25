#ifndef SRC_SERIAL_INPUT_HPP_
#define SRC_SERIAL_INPUT_HPP_

#include <Arduino.h>

#include "commands.hpp"
#include "constants.hpp"

class SerialInput: public CommandSource {
public:
  SerialInput();

  commandRaw getCommandRaw() override;

  uint8_t get_menu_jump() const { return menu_jump; }
private:
  uint8_t menu_jump{};
};

#endif /* SRC_SERIAL_INPUT_HPP_ */

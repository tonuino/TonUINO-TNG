#ifndef SRC_SERIAL_INPUT_HPP_
#define SRC_SERIAL_INPUT_HPP_

#include <Arduino.h>

#include "commands.hpp"
#include "constants.hpp"

class SerialInput: public CommandSource {
public:
  SerialInput();

  commandRaw getCommandRaw() override;

#ifdef SerialInputAsCommand
  uint8_t get_menu_jump() const { return menu_jump; }
#endif
#ifdef BAT_VOLTAGE_MEASUREMENT_CALIBRATION
  const uint16_t& getBatVoltageFromInput() const { return batVolt; }
#endif

private:
#ifdef SerialInputAsCommand
  uint8_t menu_jump{};
#endif
#ifdef BAT_VOLTAGE_MEASUREMENT_CALIBRATION
  uint16_t batVolt{};
#endif
};

#endif /* SRC_SERIAL_INPUT_HPP_ */

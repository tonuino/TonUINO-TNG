#ifndef SRC_ROTARY_ENCODER_HPP_
#define SRC_ROTARY_ENCODER_HPP_

#include <Arduino.h>

#include "commands.hpp"

class RotaryEncoder: public CommandSource {
public:

  RotaryEncoder(const Settings& settings);
  commandRaw getCommandRaw() override;

private:
  const Settings& settings;
  uint8_t old_clk = 0;
};

#endif /* SRC_ROTARY_ENCODER_HPP_ */

#ifndef SRC_ROTARY_ENCODER_HPP_
#define SRC_ROTARY_ENCODER_HPP_

#include <Arduino.h>

#include "commands.hpp"

class RotaryEncoder: public CommandSource {
public:

  RotaryEncoder(const Settings& settings);
  virtual ~RotaryEncoder() {}
  commandRaw getCommandRaw() override;

  static void changed();
private:
  const Settings& settings;
  static int pos;
  int old_pos = 0;
};

#endif /* SRC_ROTARY_ENCODER_HPP_ */

#ifndef SRC_ROTARY_ENCODER_HPP_
#define SRC_ROTARY_ENCODER_HPP_

#include "constants.hpp"

#ifdef ROTARY_ENCODER

#include <Arduino.h>

#include "commands.hpp"
#include "timer.hpp"

#if not defined(ALLinONE_Plus) and not defined(TonUINO_Every) and not defined(TonUINO_Every_4808)
#define USE_TIMER
#define ROTARY_ENCODER_USES_TIMER
#endif

class RotaryEncoder: public CommandSource {
public:

  RotaryEncoder(const Settings& settings);
  commandRaw getCommandRaw() override;

  static void changed();
#ifdef USE_TIMER
  static void timer_loop();
#endif
private:

  commandRaw getCommandRawFromCommand(const command& cmd);

  const Settings& settings;

  volatile static int8_t  pos;
#ifdef USE_TIMER
  volatile static uint8_t clk;
#endif

#ifdef ROTARY_ENCODER_LONGPRESS
  Timer vol_timer;
  Timer long_timer;
#endif
};

#endif // ROTARY_ENCODER

#endif /* SRC_ROTARY_ENCODER_HPP_ */

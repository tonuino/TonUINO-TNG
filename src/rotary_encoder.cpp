#include "rotary_encoder.hpp"

#include "constants.hpp"
#include "logger.hpp"

RotaryEncoder::RotaryEncoder(const Settings& settings)
: CommandSource()
, settings(settings)
{
  pinMode(rotaryEncoderClkPin, INPUT_PULLUP);
  pinMode(rotaryEncoderDtPin , INPUT_PULLUP);
  old_clk = digitalRead(rotaryEncoderClkPin);
}

commandRaw RotaryEncoder::getCommandRaw() {
  commandRaw ret = commandRaw::none;

  const uint8_t new_clk = digitalRead(rotaryEncoderClkPin);
  const uint8_t dt      = digitalRead(rotaryEncoderDtPin);
//  if (new_clk != old_clk) {
//    LOG(button_log, s_info, F("clk: "), new_clk, F("dt: "), dt);
//  }
  if ((new_clk == 1) && (old_clk ==0)) {
    if (dt == 1) {
#ifdef FIVEBUTTONS
      ret = commandRaw::four;

#else
      ret = settings.invertVolumeButtons? commandRaw::up : commandRaw::upLong;
#endif
    }
    else {
#ifdef FIVEBUTTONS
      ret = commandRaw::five;
#else
      ret = settings.invertVolumeButtons? commandRaw::down : commandRaw::downLong;
#endif
    }
  }
  old_clk = new_clk;

  return ret;
}

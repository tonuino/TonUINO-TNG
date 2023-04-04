#include "rotary_encoder.hpp"

#include "constants.hpp"
#include "logger.hpp"

int RotaryEncoder::pos = 0;

void RotaryEncoder::changed() {
  const uint8_t dt      = digitalRead(rotaryEncoderDtPin);
  LOG(button_log, s_debug, F("dt: "), dt);
  if (dt == 0)
    --pos;
  else
    ++pos;
}

RotaryEncoder::RotaryEncoder(const Settings& settings)
: CommandSource()
, settings(settings)
{
  pinMode(rotaryEncoderClkPin, INPUT_PULLUP);
  pinMode(rotaryEncoderDtPin , INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(rotaryEncoderClkPin), RotaryEncoder::changed, FALLING);
}

commandRaw RotaryEncoder::getCommandRaw() {
  commandRaw ret = commandRaw::none;

  if (pos > old_pos) {
#ifdef FIVEBUTTONS
    ret = commandRaw::four;

#else
    ret = settings.invertVolumeButtons? commandRaw::up : commandRaw::upLong;
#endif
  }
  else if (pos < old_pos) {
#ifdef FIVEBUTTONS
    ret = commandRaw::five;
#else
    ret = settings.invertVolumeButtons? commandRaw::down : commandRaw::downLong;
#endif
  }
  old_pos = pos;

  return ret;
}

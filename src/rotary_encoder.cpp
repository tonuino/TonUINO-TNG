#include "rotary_encoder.hpp"

#include "constants.hpp"

#ifdef ROTARY_ENCODER
#include "logger.hpp"

#ifndef ALLinONE_Plus
static_assert(false, "The rotary encoder only works with AiOplus");
#endif

int RotaryEncoder::pos = 0;

void RotaryEncoder::changed() {
  const uint8_t dt = digitalRead(rotaryEncoderDtPin);
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

  noInterrupts();
  const int new_pos = pos;
  interrupts();

  if (new_pos > old_pos) {
#ifdef FIVEBUTTONS
    ret = commandRaw::four;

#else
    ret = settings.invertVolumeButtons? commandRaw::up : commandRaw::upLong;
#endif
  }
  else if (new_pos < old_pos) {
#ifdef FIVEBUTTONS
    ret = commandRaw::five;
#else
    ret = settings.invertVolumeButtons? commandRaw::down : commandRaw::downLong;
#endif
  }
  old_pos = new_pos;

  return ret;
}

#endif // ROTARY_ENCODER

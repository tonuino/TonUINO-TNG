#include "rotary_encoder.hpp"

#include "constants.hpp"

#ifdef ROTARY_ENCODER
#include "logger.hpp"

#if not defined(ALLinONE_Plus) and not defined(TonUINO_Every)
static_assert(false, "The rotary encoder only works with AiOplus or Nano Every");
#endif

volatile int8_t RotaryEncoder::pos = 0;

void RotaryEncoder::changed() {
  const uint8_t dt = digitalRead(rotaryEncoderDtPin);
  if (dt == 0)
    --pos;
  else
    ++pos;
  LOG(button_log, s_debug, F("dt: "), dt, F(" pos: "), pos);
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

  const int8_t new_pos = pos;

  if (new_pos == old_pos)
    return ret;

  bool cw = (new_pos > old_pos);
  if (abs(new_pos - old_pos) > 200) // overflow
    cw = !cw;

  if (cw) {
#ifdef FIVEBUTTONS
    ret = commandRaw::four;

#else
    ret = (settings.invertVolumeButtons==1)? commandRaw::up : commandRaw::upLong;
#endif
  }
  else {
#ifdef FIVEBUTTONS
    ret = commandRaw::five;
#else
    ret = (settings.invertVolumeButtons==1)? commandRaw::down : commandRaw::downLong;
#endif
  }
  old_pos = new_pos;

  return ret;
}

#endif // ROTARY_ENCODER

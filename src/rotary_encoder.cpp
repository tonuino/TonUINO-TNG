#include "rotary_encoder.hpp"

#include "constants.hpp"

#ifdef ROTARY_ENCODER
#include "logger.hpp"

volatile int8_t  RotaryEncoder::pos = 0;

#ifdef ROTARY_ENCODER_USES_TIMER
volatile uint8_t RotaryEncoder::clk = 0;

void RotaryEncoder::timer_loop() {
  const uint8_t old_clk = clk;
  clk = digitalRead(rotaryEncoderClkPin);
  if (clk < old_clk){
    RotaryEncoder::changed();
  }
}
#endif

void IRAM_ATTR RotaryEncoder::changed() {
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
#ifdef ROTARY_ENCODER_LONGPRESS
, vol_timer()
, long_timer()
#endif
{}

void RotaryEncoder::init()
{
  pinMode(rotaryEncoderClkPin, INPUT_PULLUP);
  pinMode(rotaryEncoderDtPin , INPUT_PULLUP);

#ifndef ROTARY_ENCODER_USES_TIMER
  attachInterrupt(digitalPinToInterrupt(rotaryEncoderClkPin), RotaryEncoder::changed, FALLING);
#endif // ROTARY_ENCODER_USES_TIMER
}

commandRaw RotaryEncoder::getCommandRaw() {
  commandRaw ret = commandRaw::none;

  if (pos == 0)
    return ret;

#ifndef ROTARY_ENCODER_LONGPRESS

  if (pos > 0) {
    ret = getCommandRawFromCommand(command::volume_up);
  }
  else {
    ret = getCommandRawFromCommand(command::volume_down);
  }

#else

  if ((pos > 1) ||
      (pos > 0 && vol_timer.isActive() && not vol_timer.isExpired())) {
    ret = getCommandRawFromCommand(command::volume_up);
    vol_timer.start(buttonLongPress);
    long_timer.stop();
  }
  else if ((pos < -1) ||
           (pos < 0 && vol_timer.isActive() && not vol_timer.isExpired())){
    ret = getCommandRawFromCommand(command::volume_down);
    vol_timer.start(buttonLongPress);
    long_timer.stop();
  }
  else if (abs(pos) == 1 && not long_timer.isActive()) {
    long_timer.start(buttonLongPress);
  }
  else   if ((pos == 1) && long_timer.isActive() && long_timer.isExpired()) {
    ret = getCommandRawFromCommand(command::next);
  }
  else   if ((pos == -1) && long_timer.isActive() && long_timer.isExpired()) {
    ret = getCommandRawFromCommand(command::previous);
  }

#endif // ROTARY_ENCODER_LONGPRESS

  if (ret != commandRaw::none) {
    LOG(button_log, s_debug, F("Rot Env raw: "), static_cast<uint8_t>(ret));
    pos = 0;
  }

  return ret;
}

commandRaw RotaryEncoder::getCommandRawFromCommand(const command& cmd) {
  switch(cmd) {

#ifdef FIVEBUTTONS

  case command::next:
    return commandRaw::up;
  case command::previous:
    return commandRaw::down;
  case command::volume_up:
    return commandRaw::four;
  case command::volume_down:
    return commandRaw::five;

#else

  case command::next:
    return (settings.invertVolumeButtons==1)? commandRaw::upLong : commandRaw::up;
  case command::previous:
    return (settings.invertVolumeButtons==1)? commandRaw::downLong : commandRaw::down;
  case command::volume_up:
    return (settings.invertVolumeButtons==1)? commandRaw::up : commandRaw::upLong;
  case command::volume_down:
    return (settings.invertVolumeButtons==1)? commandRaw::down : commandRaw::downLong;

#endif
  default: return commandRaw::none;
  }
  return commandRaw::none;
}

#endif // ROTARY_ENCODER

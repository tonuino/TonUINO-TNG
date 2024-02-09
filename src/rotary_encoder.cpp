#include "rotary_encoder.hpp"

#include "constants.hpp"

#ifdef ROTARY_ENCODER
#include "logger.hpp"

#if not defined(ALLinONE_Plus) and not defined(TonUINO_Every)
volatile uint8_t clk_state = 0;
ISR(TIMER1_COMPA_vect){
  uint8_t new_state = digitalRead(rotaryEncoderClkPin);
  if (clk_state != new_state){
    if (new_state == 0 ){
      RotaryEncoder::changed();
    }
  clk_state = new_state;
  }
}
#endif

volatile int8_t RotaryEncoder::pos = 0;
volatile uint32_t RotaryEncoder::last_change = 0;

void RotaryEncoder::changed() {
  const uint8_t dt = digitalRead(rotaryEncoderDtPin);
  if (dt == 0)
    --pos;
  else
    ++pos;
#if defined(ROTARY_LONGPRESS)
  last_change = millis();
#endif
  LOG(button_log, s_debug, F("dt: "), dt, F(" pos: "), pos);
}

RotaryEncoder::RotaryEncoder(const Settings& settings)
: CommandSource()
, settings(settings)
{
  pinMode(rotaryEncoderClkPin, INPUT_PULLUP);
  pinMode(rotaryEncoderDtPin , INPUT_PULLUP);

#if not defined(ALLinONE_Plus) and not defined(TonUINO_Every)
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0;
  // set timer count for frequency
  OCR1A = 10000U;// = (16*10^6) / (F*8) - 1, F=.2kHz
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS11 bit for 8 prescaler
  TCCR1B |= (1 << CS11);   
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
#else
  attachInterrupt(digitalPinToInterrupt(rotaryEncoderClkPin), RotaryEncoder::changed, FALLING);
#endif

}

commandRaw RotaryEncoder::getCommandRaw() {
  commandRaw ret = commandRaw::none;


  if (pos == 0)
    return ret;

#if defined(ROTARY_LONGPRESS)
  if ((millis() - last_change) < buttonLongPress)
    return ret;
#endif

  if (pos > 0) {
#if defined(FIVEBUTTONS)
    ret = commandRaw::four;
#elif defined(ROTARY_LONGPRESS)
    ret = (pos > 1)? commandRaw::upLong : commandRaw::up;
    if(settings.invertVolumeButtons==1)
      ret = (ret==commandRaw::upLong)? commandRaw::up : commandRaw::upLong;
#else
    ret = (settings.invertVolumeButtons==1)? commandRaw::up : commandRaw::upLong;
#endif
  }
  else {
#if defined(FIVEBUTTONS)
    ret = commandRaw::five;
#elif defined(ROTARY_LONGPRESS)
    ret = (pos < -1)? commandRaw::downLong : commandRaw::down;
    if(settings.invertVolumeButtons==1)
        ret = (ret==commandRaw::downLong)? commandRaw::down : commandRaw::downLong;
#else
    ret = (settings.invertVolumeButtons==1)? commandRaw::down : commandRaw::downLong;
#endif
  }
  pos=0;
  last_change = 0;

  return ret;
}

#endif // ROTARY_ENCODER

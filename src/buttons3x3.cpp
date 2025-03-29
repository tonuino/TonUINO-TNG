#include "buttons3x3.hpp"

#include "constants.hpp"
#include "logger.hpp"

#ifdef BUTTONS3X3
//#define CALIBRATE3X3

Buttons3x3::Buttons3x3()
: CommandSource()
, buttons(button3x3Pin, numLevels, maxLevel, buttonLongPress)
{
}

void Buttons3x3::begin() {
  pinMode(button3x3Pin, INPUT);
}

commandRaw Buttons3x3::getCommandRaw() {
  commandRaw ret = commandRaw::none;

#ifdef CALIBRATE3X3
  static uint16_t t = 0;
  if (t++ % 10 == 0)
    LOG(button_log, s_info, F("Button3x3 analog value: "), static_cast<int>(analogRead(button3x3Pin)));
#else
  const uint8_t button = buttons.getKey();

  if (button >= 1 && button <= buttonExtSC_buttons) {
    ret = static_cast<commandRaw>(static_cast<uint8_t>(commandRaw::ext_begin) + button - 1);
  }

  if (ret != commandRaw::none) {
    LOG(button_log, s_debug, F("btn3x3: "), static_cast<uint8_t>(ret)-buttonExtSC_begin+1);
  }
#endif
  return ret;
}

#endif // BUTTONS3X3

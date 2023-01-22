#ifdef BUTTONS3X3
#include "buttons3x3.hpp"

#include "constants.hpp"
#include "logger.hpp"

//#define CALIBRATE3X3

Buttons3x3::Buttons3x3()
: CommandSource()
, buttons(button3x3Pin)
{
  buttons.setLongPressTime(buttonLongPress);
  for (uint8_t i = 0; i < numLevels; ++i)
    buttons.registerKey(i+1, levels[i], 9+i+1);
  buttons.setNoPressValue(levels[numLevels]);
}

commandRaw Buttons3x3::getCommandRaw() {
  commandRaw ret = commandRaw::none;

#ifdef CALIBRATE3X3
  static uint8_t t = 0;
  if (t % 20 == 0)
    LOG(button_log, s_info, F("Button3x3 analog value: "), static_cast<int>(analogRead(button3x3Pin)));
#else
  const uint8_t button = buttons.getKey();

  if (button >= 1 && button <= buttonExtSC_buttons) {
    ret = static_cast<commandRaw>(static_cast<uint8_t>(commandRaw::ext_begin) + button - 1);
  }

  if (ret != commandRaw::none) {
    LOG(button_log, s_debug, F("Button3x3 raw: "), static_cast<uint8_t>(ret));
  }
#endif
  return ret;
}

#endif // BUTTONS3X3

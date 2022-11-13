#include "buttons3x3.hpp"

#include "constants.hpp"
#include "logger.hpp"
#ifdef BUTTONS3X3

//#define CALIBRATE3X3

Buttons3x3::Buttons3x3()
: CommandSource()
, buttons(button3x3Pin)
{
  buttons.setDebounceTime(button3x3DbTime);
  buttons.setNoPressValue(784) ;
  buttons.registerKey(1,   0);
  buttons.registerKey(2,  77);
  buttons.registerKey(3, 148);
  buttons.registerKey(4, 234);
  buttons.registerKey(5, 306);
  buttons.registerKey(6, 390);
  buttons.registerKey(7, 465);
  buttons.registerKey(8, 534);
  buttons.registerKey(9, 593);
}

commandRaw Buttons3x3::getCommandRaw() {
  commandRaw ret = commandRaw::none;

#ifdef CALIBRATE3X3
  static uint8_t t = 0;
  if (t % 20 == 0)
    LOG(button_log, s_info, F("Button3x3 analog value: "), static_cast<int>(analogRead(button3x3Pin)));
#else
  const uint8_t button = buttons.getKey();

  if (button > 0 && button < 10) {
#ifdef BUTTONS3X3_PRESS_TWO
    if (first_button == 0)
      first_button = button;
    else {
      ret = static_cast<commandRaw>(static_cast<uint8_t>(commandRaw::ext_begin) + (first_button-1)*9 + button - 1);
      first_button = 0;
    }
#else
    ret = static_cast<commandRaw>(static_cast<uint8_t>(commandRaw::ext_begin) + button - 1);
#endif
  }

  if (ret != commandRaw::none) {
    LOG(button_log, s_debug, F("Button3x3 raw: "), static_cast<uint8_t>(ret));
  }
#endif
  return ret;
}

#endif // BUTTONS3X3

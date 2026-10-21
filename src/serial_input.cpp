#include "serial_input.hpp"

#include "constants.hpp"
#include "logger.hpp"

#if defined(SerialInputAsCommand) or defined(BAT_VOLTAGE_MEASUREMENT_CALIBRATION)
SerialInput::SerialInput()
: CommandSource()
{
}

commandRaw SerialInput::getCommandRaw() {
  commandRaw ret = commandRaw::none;
  if (Serial.available() > 0) {
#ifdef BAT_VOLTAGE_MEASUREMENT_CALIBRATION
    if (isAlpha(Serial.peek())) {
      char cmdWord[5]; cmdWord[4] = 0;
      Serial.readBytesUntil(' ', cmdWord, 4);
      if (strcmp(cmdWord, "BAT") == 0) {
        batVolt = Serial.parseInt();
        if (batVolt != 0 && batVolt < 10000) {
          return commandRaw::bat_volt_corr;
        }
      }
      return commandRaw::none;
    }
#endif
#ifdef SerialInputAsCommand
    long optionSerial = Serial.parseInt();
    switch (optionSerial) {
    case -2: ret = commandRaw::down      ; break;
    case -8: ret = commandRaw::up        ; break;
    case -3: ret = commandRaw::downLong  ; break;
    case -9: ret = commandRaw::upLong    ; break;
    case -5: ret = commandRaw::pause     ; break;
    case -6: ret = commandRaw::pauseLong ; break;
    case -4: ret = commandRaw::allLong   ; break;
    case -1: ret = commandRaw::updownLong; break;
    default:
      if (optionSerial > 0) {
        ret = commandRaw::menu_jump;
        menu_jump = optionSerial;
      }
      break;
    }
#else
    Serial.read();
#endif
  }
  return ret;
}
#endif


#ifndef SRC_BUTTONS_HPP_
#define SRC_BUTTONS_HPP_

#include <Arduino.h>
#include <JC_Button.h>

#include "commands.hpp"
#include "constants.hpp"

class Buttons: public CommandSource {
public:
  Buttons();
  void begin();

  commandRaw getCommandRaw() override;
  bool isNoButton();
  bool isReset();

private:

  void readButtons();

  Button buttonPause;
  Button buttonUp   ;
  Button buttonDown ;
#ifdef FIVEBUTTONS
  Button  buttonFour;
  Button  buttonFive;
#endif
  bool ignoreRelease     = false;
  bool ignoreAll         = false;

  uint8_t longPressFactor = 0;
};

#endif /* SRC_BUTTONS_HPP_ */

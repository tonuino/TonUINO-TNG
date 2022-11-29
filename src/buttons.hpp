#ifndef SRC_BUTTONS_HPP_
#define SRC_BUTTONS_HPP_

#include <Arduino.h>
#include <JC_Button.h>

#include "commands.hpp"
#include "constants.hpp"
/*
three Button
============
                                                      vol/next-prev inverted
           admin-menu  idle        pause       play        play
-------------------------------------------------------------------------
pause      select                  play        pause       pause
pauseLong  end adm     shortcut    shortcut    track#      track#
up         next                                next        vol++
upLong     next 10     shortcut    shortcut    vol++       next
down       prev                                prev        vol--
downLong   prev 10     shortcut    shortcut    vol--       prev

five Button
===========
                                                      vol/next-prev inverted
           admin-menu  idle        pause       play        play
-------------------------------------------------------------------------
pause      select                  play        pause       pause
pauseLong  end adm     shortcut    shortcut    track#      track#
up         next                                next        vol++
upLong     next 10     shortcut    shortcut    next 10     vol++ cont.
down       prev                                prev        vol--
downLong   prev 10     shortcut    shortcut    prev 10     vol-- cont.
four                                           vol++       next
fourLong                                       vol++ cont. next 10
five                                           vol--       prev
fiveLong                                       vol-- cont. prev 10
*/

class Buttons: public CommandSource {
public:
  Buttons(const Settings& settings);

  commandRaw getCommandRaw() override;
  bool isNoButton();
  bool isReset();

private:

  void readButtons();

  const Settings& settings;

  Button buttonPause;
  Button buttonUp   ;
  Button buttonDown ;
#ifdef FIVEBUTTONS
  Button  buttonFour;
  Button  buttonFive;
#endif
  bool ignoreRelease     = false;
  bool ignoreAll         = false;

  uint8_t longPressFactor = 1;
};

#endif /* SRC_BUTTONS_HPP_ */

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
           admin-menu  idle        pause       play         play
--------------------------------------------------------------------------
pause      select                  play        pause        pause
pauseLong  end adm     shutdown    shutdown    track#       track#
up         next                                next         vol++
upLong     next 10     shortcut2   shortcut2   vol++ cont.  next
down       prev                                prev         vol--
downLong   prev 10     shortcut3   shortcut3   vol-- cont.  prev
updownLong             shortcut1   shortcut1

five Button
===========
vol/next-prev inverted has no effect for 5 Buttons

           admin-menu  idle        pause       play
-----------------------------------------------------------
pause      select                  play        pause
pauseLong  end adm     shutdown    shutdown    track#
up         next                                next
upLong     next 10     shortcut2   shortcut2   next 10
down       prev                                prev
downLong   prev 10     shortcut2   shortcut3   prev 10
updownLong             shortcut1   shortcut1
four       next                                vol++
fourLong   next 10                             vol++ cont.
five       prev                                vol--
fiveLong   prev 10                             vol-- cont.
*/

class Buttons: public CommandSource {
public:
  Buttons();

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

  uint8_t longPressFactor = 1;
};

#endif /* SRC_BUTTONS_HPP_ */

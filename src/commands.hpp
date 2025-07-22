#ifndef SRC_COMMANDS_HPP_
#define SRC_COMMANDS_HPP_

#include <Arduino.h>

#include "settings.hpp"
#include "constants.hpp"
#include "array.hpp"

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

enum class commandRaw: uint8_t {
  none           =  0,
  start          =  1,
  allLong        =  2,
  pause          =  3,
  pauseLong      =  4,
  up             =  5,
  upLong         =  6,
  upLongRepeat   =  7,
  down           =  8,
  downLong       =  9,
  downLongRepeat = 10,
  updownLong     = 11,
#ifdef FIVEBUTTONS
  four           = 12,
  fourLong       = 13,
  fourLongRepeat = 14,
  five           = 15,
  fiveLong       = 16,
  fiveLongRepeat = 17,
#endif
  specialStart   = 18,
  menu_jump      = 19,
  card_from_web       = 20,
  write_card_from_web = 21,
  cmd_end        = 22,
#ifdef BUTTONS3X3
  ext_begin = buttonExtSC_begin,
  ext_end   = ext_begin +  buttonExtSC_buttons,
#endif
};

enum class command: uint8_t {
  none        =  0,
  // play/pause/idle
  admin       =  1,
  shutdown    =  2,
  shortcut1   =  3,
  shortcut2   =  4,
  shortcut3   =  5,
  start       =  6,
  pause       =  7,
  track       =  8,
  volume_up   =  9,
  volume_down = 10,
  bright_up   = 11,
  bright_down = 12,
  to_first    = 13,
  // play/pause/idle/adm
  next        = 14,
  next10      = 15,
  previous    = 16,
  previous10  = 17,
  // adm
  select      = 18,
#ifdef SerialInputAsCommand
  menu_jump   = 19,
#endif
  adm_end     = 20,
#ifdef TonUINO_Esp32
  card_from_web       = 21,
  write_card_from_web = 22,
#endif
  last        = 23,
#ifdef BUTTONS3X3
  ext_begin = buttonExtSC_begin,
  ext_end   = ext_begin +  buttonExtSC_buttons,
#endif
#ifdef SPECIAL_START_SHORTCUT
  specialStart,
#endif
};

enum class state_for_command: uint8_t {
  admin       = 0,
  idle_pause  = 1,
  play        = 2,
  play_invert = 3,
};

extern const command cmd_table[][4] PROGMEM;

class CommandSource {
public:
  virtual commandRaw getCommandRaw()=0;
};

class Commands {
public:
  Commands(const Settings& settings, CommandSource* source1, CommandSource* source2 = nullptr, CommandSource* source3 = nullptr, CommandSource* source4 = nullptr);

  commandRaw getCommandRaw();
  command    getCommand   (commandRaw b, state_for_command s);
  command    getLastCommand() { return last_command; }

  const __FlashStringHelper* getCommandRawStr(commandRaw cmd);
  const __FlashStringHelper* getCommandStr   (command    cmd);

  static bool isSelect(command cmd) {
    return cmd == command::select
#ifdef SerialInputAsCommand
        || cmd == command::menu_jump
#endif
        ;
  }

  static uint8_t   getButtonCode(commandRaw b);
#ifdef BUTTONS3X3
  static bool isExtButton(command c);
#endif

private:
  const Settings&           settings;
  array<CommandSource*, 4>  sources;
  command                   last_command{command::none};
};

#endif /* SRC_COMMANDS_HPP_ */

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
  none,
  start,
  allLong,
  pause,
  pauseLong,
  up,
  upLong,
  upLongRepeat,
  down,
  downLong,
  downLongRepeat,
  updownLong,
#ifdef FIVEBUTTONS
  four,
  fourLong,
  fourLongRepeat,
  five,
  fiveLong,
  fiveLongRepeat,
#endif
#ifdef SerialInputAsCommand
  menu_jump,
#endif
  cmd_end,
#ifdef BUTTONS3X3
  ext_begin = buttonExtSC_begin,
  ext_end   = ext_begin +  buttonExtSC_buttons,
#endif
};

enum class command: uint8_t {
  none,
  // play/pause/idle
  admin,
  shutdown,
  shortcut1,
  shortcut2,
  shortcut3,
  start,
  pause,
  track,
  volume_up,
  volume_down,
  bright_up,
  bright_down,
  // play/pause/idle/adm
  next,
  next10,
  previous,
  previous10,
  // adm
  select,
#ifdef SerialInputAsCommand
  menu_jump,
#endif
  adm_end,
#ifdef BUTTONS3X3
  ext_begin = buttonExtSC_begin,
  ext_end   = ext_begin +  buttonExtSC_buttons,
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
  Commands(const Settings& settings, CommandSource* source1, CommandSource* source2 = nullptr, CommandSource* source3 = nullptr);

  commandRaw getCommandRaw();
  command    getCommand   (commandRaw b, state_for_command s);

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
  array<CommandSource*, 3>  sources;
};

#endif /* SRC_COMMANDS_HPP_ */

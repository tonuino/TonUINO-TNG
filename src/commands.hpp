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
  // play/pause/idle/adm
  next,
  next10,
  previous,
  previous10,
  // adm
  select,
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

inline constexpr command cmd_table[][4] = {
/*  raw commands                   adm             idle/pause               play           play_invert       */
/*  none,           */  { command::none      , command::none     , command::none       , command::none        },
/*  start,          */  { command::none      , command::start    , command::none       , command::none        },
/*  allLong,        */  { command::none      , command::admin    , command::admin      , command::admin       },
/*  pause,          */  { command::select    , command::pause    , command::pause      , command::pause       },
/*  pauseLong,      */  { command::adm_end   , command::shutdown , command::track      , command::track       },
#ifdef FIVEBUTTONS
/*  up,             */  { command::next      , command::none     , command::next       , command::none        },
/*  upLong,         */  { command::next10    , command::shortcut2, command::next10     , command::none        },
/*  upLongRepeat,   */  { command::none      , command::none     , command::none       , command::none        },
/*  down,           */  { command::previous  , command::none     , command::previous   , command::none        },
/*  downLong,       */  { command::previous10, command::shortcut3, command::previous10 , command::none        },
/*  downLongRepeat, */  { command::none      , command::none     , command::none       , command::none        },
/*  updownLong,     */  { command::none      , command::shortcut1, command::none       , command::none        },
/*  four,           */  { command::next      , command::none     , command::volume_up  , command::none        },
/*  fourLong,       */  { command::next10    , command::none     , command::volume_up  , command::none        },
/*  fourLongRepeat, */  { command::none      , command::none     , command::volume_up  , command::none        },
/*  five,           */  { command::previous  , command::none     , command::volume_down, command::none        },
/*  fiveLong,       */  { command::previous10, command::none     , command::volume_down, command::none        },
/*  fiveLongRepeat, */  { command::none      , command::none     , command::volume_down, command::none        }
#else // Three Button
/*  up,             */  { command::next      , command::none     , command::next       , command::volume_up   },
/*  upLong,         */  { command::next10    , command::shortcut2, command::volume_up  , command::next        },
/*  upLongRepeat,   */  { command::none      , command::none     , command::volume_up  , command::none        },
/*  down,           */  { command::previous  , command::none     , command::previous   , command::volume_down },
/*  downLong,       */  { command::previous10, command::shortcut3, command::volume_down, command::previous    },
/*  downLongRepeat, */  { command::none      , command::none     , command::volume_down, command::none        },
/*  updownLong,     */  { command::none      , command::shortcut1, command::none       , command::none        }
#endif
};

class CommandSource {
public:
  virtual commandRaw getCommandRaw()=0;
};

class Commands {
public:
  Commands(const Settings& settings, CommandSource* source1, CommandSource* source2 = nullptr, CommandSource* source3 = nullptr);

  commandRaw getCommandRaw();
  command    getCommand   (commandRaw b, state_for_command s);

  static uint8_t   getButtonCode(commandRaw b);
#ifdef BUTTONS3X3
  static bool isExtButton(command c);
#endif

private:
  const Settings&           settings;
  array<CommandSource*, 3>  sources;
};

#endif /* SRC_COMMANDS_HPP_ */

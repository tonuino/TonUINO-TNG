#include "commands.hpp"

#include "constants.hpp"
#include "logger.hpp"

const command cmd_table[][4] PROGMEM = {
/*  raw commands                   adm             idle/pause               play           play_invert       */
/*  none,           */  { command::none      , command::none       , command::none       , command::none        }
/*  start,          */ ,{ command::none      , command::start      , command::none       , command::none        }
/*  allLong,        */ ,{ command::none      , command::admin      , command::admin      , command::admin       }
/*  pause,          */ ,{ command::select    , command::pause      , command::pause      , command::pause       }
/*  pauseLong,      */ ,{ command::adm_end   , command::shutdown   , command::track      , command::track       }
#ifdef FIVEBUTTONS
/*  up,             */ ,{ command::next      , command::bright_up  , command::next       , command::none        }
/*  upLong,         */ ,{ command::next10    , command::shortcut2  , command::next10     , command::none        }
/*  upLongRepeat,   */ ,{ command::none      , command::none       , command::none       , command::none        }
/*  down,           */ ,{ command::previous  , command::bright_down, command::previous   , command::none        }
/*  downLong,       */ ,{ command::previous10, command::shortcut3  , command::previous10 , command::none        }
/*  downLongRepeat, */ ,{ command::none      , command::none       , command::none       , command::none        }
/*  updownLong,     */ ,{ command::none      , command::shortcut1  , command::none       , command::none        }
/*  four,           */ ,{ command::next      , command::bright_up  , command::volume_up  , command::none        }
/*  fourLong,       */ ,{ command::next10    , command::none       , command::volume_up  , command::none        }
/*  fourLongRepeat, */ ,{ command::none      , command::none       , command::volume_up  , command::none        }
/*  five,           */ ,{ command::previous  , command::bright_down, command::volume_down, command::none        }
/*  fiveLong,       */ ,{ command::previous10, command::none       , command::volume_down, command::none        }
/*  fiveLongRepeat, */ ,{ command::none      , command::none       , command::volume_down, command::none        }
#else // Three Button
/*  up,             */ ,{ command::next      , command::bright_up  , command::next       , command::volume_up   }
/*  upLong,         */ ,{ command::next10    , command::shortcut2  , command::volume_up  , command::next        }
/*  upLongRepeat,   */ ,{ command::none      , command::none       , command::volume_up  , command::none        }
/*  down,           */ ,{ command::previous  , command::bright_down, command::previous   , command::volume_down }
/*  downLong,       */ ,{ command::previous10, command::shortcut3  , command::volume_down, command::previous    }
/*  downLongRepeat, */ ,{ command::none      , command::none       , command::volume_down, command::none        }
/*  updownLong,     */ ,{ command::none      , command::shortcut1  , command::none       , command::none        }
#endif
#ifdef SerialInputAsCommand
/*  menu_jump ,     */ ,{ command::menu_jump , command::none       , command::none       , command::none        }
#endif
};
Commands::Commands(const Settings& settings, CommandSource* source1, CommandSource* source2, CommandSource* source3)
: settings(settings)
, sources()
{
  sources[0] = source1;
  sources[1] = source2;
  sources[2] = source3;
}

commandRaw Commands::getCommandRaw() {
  for (auto source: sources) {
    if (source != nullptr) {
      const commandRaw ret = source->getCommandRaw();
      if (ret != commandRaw::none)
        return ret;
    }
  }
  return commandRaw::none;
}

command Commands::getCommand(commandRaw b, state_for_command s) {

  command ret = command::none;
#ifdef BUTTONS3X3
  if (b >= commandRaw::ext_begin && b < commandRaw::ext_end)
    ret =  static_cast<command>(b);
#endif

#ifndef FIVEBUTTONS
  if (s == state_for_command::play && (settings.invertVolumeButtons==1))
    s = state_for_command::play_invert;
#endif

  if (b < commandRaw::cmd_end) {
    PROGMEM_read(&cmd_table[static_cast<int>(b)][static_cast<int>(s)], ret);
  }

  if (ret != command::none) {
    LOG(button_log, s_info, F("Command: "), static_cast<uint8_t>(ret));
  }
  return ret;
}

uint8_t Commands::getButtonCode(commandRaw b) {
  switch (b) {
  case commandRaw::pause: return 1;
  case commandRaw::up   : return 2;
  case commandRaw::down : return 3;
  default               : return 0;
  }
}

#ifdef BUTTONS3X3
bool Commands::isExtButton(command c) {
  return c >= command::ext_begin && c < command::ext_end;
}
#endif


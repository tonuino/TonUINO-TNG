#include "commands.hpp"

#include "constants.hpp"
#include "logger.hpp"

const command cmd_table[][4] PROGMEM = {
/*  raw commands                   adm             idle/pause               play           play_invert       */
/*  0 none,           */  { command::none      , command::none       , command::none       , command::none        }
/*  1 start,          */ ,{ command::none      , command::start      , command::none       , command::none        }
/*  2 allLong,        */ ,{ command::none      , command::admin      , command::admin      , command::admin       }
/*  3 pause,          */ ,{ command::select    , command::pause      , command::pause      , command::pause       }
#ifndef DISABLE_SHUTDOWN_VIA_BUTTON
/*  4 pauseLong,      */ ,{ command::adm_end   , command::shutdown   , command::track      , command::track       }
#else
/*  4 pauseLong,      */ ,{ command::adm_end   , command::shortcut1  , command::track      , command::track       }
#endif
#ifdef FIVEBUTTONS
/*  5 up,             */ ,{ command::next      , command::bright_up  , command::next       , command::none        }
/*  6 upLong,         */ ,{ command::next10    , command::shortcut2  , command::next10     , command::none        }
/*  7 upLongRepeat,   */ ,{ command::none      , command::none       , command::none       , command::none        }
/*  8 down,           */ ,{ command::previous  , command::bright_down, command::previous   , command::none        }
/*  9 downLong,       */ ,{ command::previous10, command::shortcut3  , command::previous10 , command::none        }
/* 10 downLongRepeat, */ ,{ command::none      , command::none       , command::none       , command::none        }
/* 11 updownLong,     */ ,{ command::none      , command::shortcut1  , command::to_first   , command::none        }
/* 12 four,           */ ,{ command::next      , command::bright_up  , command::volume_up  , command::none        }
/* 13 fourLong,       */ ,{ command::next10    , command::none       , command::volume_up  , command::none        }
/* 14 fourLongRepeat, */ ,{ command::none      , command::none       , command::volume_up  , command::none        }
/* 15 five,           */ ,{ command::previous  , command::bright_down, command::volume_down, command::none        }
/* 16 fiveLong,       */ ,{ command::previous10, command::none       , command::volume_down, command::none        }
/* 17 fiveLongRepeat, */ ,{ command::none      , command::none       , command::volume_down, command::none        }
#else // Three Button
/*  5 up,             */ ,{ command::next      , command::bright_up  , command::next       , command::volume_up   }
/*  6 upLong,         */ ,{ command::next10    , command::shortcut2  , command::volume_up  , command::next        }
/*  7 upLongRepeat,   */ ,{ command::none      , command::none       , command::volume_up  , command::none        }
/*  8 down,           */ ,{ command::previous  , command::bright_down, command::previous   , command::volume_down }
/*  9 downLong,       */ ,{ command::previous10, command::shortcut3  , command::volume_down, command::previous    }
/* 10 downLongRepeat, */ ,{ command::none      , command::none       , command::volume_down, command::none        }
/* 11 updownLong,     */ ,{ command::none      , command::shortcut1  , command::to_first   , command::to_first    }
/* 12 four,           */ ,{ command::none      , command::none       , command::none       , command::none        }
/* 13 fourLong,       */ ,{ command::none      , command::none       , command::none       , command::none        }
/* 14 fourLongRepeat, */ ,{ command::none      , command::none       , command::none       , command::none        }
/* 15 five,           */ ,{ command::none      , command::none       , command::none       , command::none        }
/* 16 fiveLong,       */ ,{ command::none      , command::none       , command::none       , command::none        }
/* 17 fiveLongRepeat, */ ,{ command::none      , command::none       , command::none       , command::none        }
#endif
#ifdef SPECIAL_START_SHORTCUT
/* 18 specialStart,   */ ,{ command::none      , command::specialStart,command::none       , command::none        }
#else
/* 18 specialStart,   */ ,{ command::none      , command::none       ,command::none        , command::none        }
#endif
#ifdef SerialInputAsCommand
/* 19 menu_jump ,     */ ,{ command::menu_jump , command::none       , command::none       , command::none        }
#else
/* 19 menu_jump ,     */ ,{ command::none      , command::none       , command::none       , command::none        }
#endif
#ifdef TonUINO_Esp32
/* 20 card_from_web,  */ ,{ command::none , command::card_from_web, command::card_from_web , command::card_from_web}
/* 21 w_card_from_web,*/ ,{ command::none , command::write_card_from_web, command::none    , command::none}
#else
/* 20 card_from_web,  */ ,{ command::none      , command::none       , command::none       , command::none        }
/* 21 w_card_from_web,*/ ,{ command::none      , command::none       , command::none       , command::none        }
#endif
};
Commands::Commands(const Settings& settings, CommandSource* source1, CommandSource* source2, CommandSource* source3, CommandSource* source4)
: settings(settings)
, sources()
{
  sources[0] = source1;
  sources[1] = source2;
  sources[2] = source3;
  sources[3] = source4;
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

  last_command = ret;

  if (ret != command::none && ret < command::last)
#ifdef ALLinONE
    LOG(button_log, s_debug, F("btn/cmd: "), static_cast<uint8_t>(b), F("/"), static_cast<uint8_t>(ret));
#else
    LOG(button_log, s_info , F("btn/cmd: "), getCommandRawStr(b), F("/"), getCommandStr(ret));
#endif

#ifdef BUTTONS3X3
    if (ret != command::none && ret >= command::ext_begin)
      LOG(button_log, s_info , F("btn3x3: "), static_cast<uint8_t>(ret)-buttonExtSC_begin+1);
#endif

    return ret;
}

const __FlashStringHelper* Commands::getCommandRawStr(commandRaw cmd) {
  switch(cmd) {
  case commandRaw::none          : return(F("none"          )); break;
  case commandRaw::start         : return(F("start"         )); break;
  case commandRaw::allLong       : return(F("allLong"       )); break;
  case commandRaw::pause         : return(F("pause"         )); break;
  case commandRaw::pauseLong     : return(F("pauseLong"     )); break;
  case commandRaw::up            : return(F("up"            )); break;
  case commandRaw::upLong        : return(F("upLong"        )); break;
  case commandRaw::upLongRepeat  : return(F("upLongRepeat"  )); break;
  case commandRaw::down          : return(F("down"          )); break;
  case commandRaw::downLong      : return(F("downLong"      )); break;
  case commandRaw::downLongRepeat: return(F("downLongRepeat")); break;
  case commandRaw::updownLong    : return(F("updownLong"    )); break;
#ifdef FIVEBUTTONS
  case commandRaw::four          : return(F("four"          )); break;
  case commandRaw::fourLong      : return(F("fourLong"      )); break;
  case commandRaw::fourLongRepeat: return(F("fourLongRepeat")); break;
  case commandRaw::five          : return(F("five"          )); break;
  case commandRaw::fiveLong      : return(F("fiveLong"      )); break;
  case commandRaw::fiveLongRepeat: return(F("fiveLongRepeat")); break;
#endif
#ifdef TonUINO_Esp32
  case commandRaw::card_from_web : return(F("card_from_web" )); break;
#endif
  default                        : return(F(""              )); break;
  }
}

const __FlashStringHelper* Commands::getCommandStr   (command    cmd) {
  switch(cmd) {
  case command::none               : return(F("none"               )); break;
  case command::admin              : return(F("admin"              )); break;
  case command::shutdown           : return(F("shutdown"           )); break;
  case command::shortcut1          : return(F("shortcut1"          )); break;
  case command::shortcut2          : return(F("shortcut2"          )); break;
  case command::shortcut3          : return(F("shortcut3"          )); break;
  case command::start              : return(F("start"              )); break;
  case command::pause              : return(F("pause"              )); break;
  case command::track              : return(F("track"              )); break;
  case command::volume_up          : return(F("volume_up"          )); break;
  case command::volume_down        : return(F("volume_down"        )); break;
  case command::bright_up          : return(F("bright_up"          )); break;
  case command::bright_down        : return(F("bright_down"        )); break;
  case command::to_first           : return(F("to_first"           )); break;
  case command::next               : return(F("next"               )); break;
  case command::next10             : return(F("next10"             )); break;
  case command::previous           : return(F("previous"           )); break;
  case command::previous10         : return(F("previous10"         )); break;
  case command::select             : return(F("select"             )); break;
#ifdef SerialInputAsCommand
  case command::menu_jump          : return(F("menu_jump"          )); break;
#endif
  case command::adm_end            : return(F("adm_end"            )); break;
#ifdef TonUINO_Esp32
  case command::card_from_web      : return(F("card_from_web"      )); break;
  case command::write_card_from_web: return(F("write_card_from_web")); break;
#endif
  default                          : return(F(""                   )); break;
  }
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


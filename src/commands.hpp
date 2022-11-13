#ifndef SRC_COMMANDS_HPP_
#define SRC_COMMANDS_HPP_

#include <Arduino.h>

#include "settings.hpp"
#include "constants.hpp"
#include "array.hpp"

enum class commandRaw: uint8_t {
  none,
  pause,
  pauseLong,
  up,
  upLong,
  down,
  downLong,
  allLong,
#ifdef FIVEBUTTONS
  four,
  five,
#endif
  start,
#ifdef BUTTONS3X3
  ext_begin = buttonExtSC_begin,
  ext_end   = ext_begin +  buttonExtSC_buttons,
#endif
};

enum class command: uint8_t {
  none,
  admin,
  pause,
  track,
  volume_up,
  volume_down,
  next,
  previous,
  start,
};

class CommandSource {
public:
  virtual commandRaw getCommandRaw()=0;
};

class Commands {
public:
  Commands(const Settings& settings, CommandSource* source1, CommandSource* source2 = nullptr, CommandSource* source3 = nullptr);

  commandRaw getCommandRaw();
  command    getCommand   (commandRaw b);

  static uint8_t   getButtonCode(commandRaw b);
#ifdef BUTTONS3X3
  static bool isExtButton(commandRaw b);
#endif

private:
  const Settings&           settings;
  array<CommandSource*, 3>  sources;
};

#endif /* SRC_COMMANDS_HPP_ */

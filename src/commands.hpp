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
  upLongRepeat,
  down,
  downLong,
  downLongRepeat,
  allLong,
  updownLong,
#ifdef FIVEBUTTONS
  four,
  fourLong,
  fourLongRepeat,
  five,
  fiveLong,
  fiveLongRepeat,
#endif
  start,
};

enum class command: uint8_t {
  none,
  admin,
  pause,
  track,
  volume_up,
  volume_down,
  next,
  next10,
  previous,
  previous10,
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

private:
  const Settings&           settings;
  array<CommandSource*, 3>  sources;
};

#endif /* SRC_COMMANDS_HPP_ */

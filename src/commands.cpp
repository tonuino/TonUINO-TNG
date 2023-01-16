#include "commands.hpp"

#include "constants.hpp"
#include "logger.hpp"

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
      const commandRaw c = source->getCommandRaw();
      if (c != commandRaw::none)
        return c;
    }
  }
  return commandRaw::none;
}

command Commands::getCommand(commandRaw b, state_for_command s) {

#ifndef FIVEBUTTONS
  if (s == state_for_command::play && settings.invertVolumeButtons)
    s = state_for_command::play_invert;
#endif

  const uint8_t si = static_cast<uint8_t>(s);
  const uint8_t ci = static_cast<uint8_t>(b);

  const command ret = cmd_table[ci][si];

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


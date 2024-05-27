#include "poti.hpp"

#include "constants.hpp"

#ifdef POTI
#include "logger.hpp"
#include "mp3.hpp"

#ifdef ALLinONE
  static constexpr int16_t maxLevel = 4064;
#else
  static constexpr int16_t maxLevel = 1023;
#endif


Poti::Poti(const Settings& settings, Mp3& mp3)
: CommandSource()
, settings(settings)
, mp3(mp3)
{
  pinMode(potiPin, INPUT);
}

commandRaw Poti::getCommandRaw() {
  const uint8_t volume = map( analogRead(potiPin), 0, maxLevel, mp3.getMinVolume(), mp3.getMaxVolume());

  if (volume < mp3.getVolume()) {
    LOG(button_log, s_debug, F("poti volume: "), volume);
    mp3.setVolume(volume);
  }
  else if (volume > mp3.getVolume()+1) {
    LOG(button_log, s_debug, F("poti volume: "), volume-1);
    mp3.setVolume(volume-1);
  }

  return commandRaw::none;
}

#endif // POTI

#ifndef SRC_POTI_HPP_
#define SRC_POTI_HPP_

#include <Arduino.h>

#include "commands.hpp"

class Mp3;

class Poti: public CommandSource {
public:

  Poti(const Settings& settings, Mp3& mp3);
  virtual ~Poti() {}
  commandRaw getCommandRaw() override;

private:
  const Settings& settings;
  Mp3&            mp3;
};

#endif /* SRC_POTI_HPP_ */

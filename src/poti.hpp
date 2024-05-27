#ifndef SRC_POTI_HPP_
#define SRC_POTI_HPP_

#include <Arduino.h>

#include "commands.hpp"

class Mp3;

class Poti: public CommandSource {
public:

  Poti(Mp3& mp3);
  commandRaw getCommandRaw() override;

private:
  Mp3&            mp3;
};

#endif /* SRC_POTI_HPP_ */

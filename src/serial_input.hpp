#ifndef SRC_SERIAL_INPUT_HPP_
#define SRC_SERIAL_INPUT_HPP_

#include <Arduino.h>

#include "commands.hpp"
#include "constants.hpp"
#include "chip_card.hpp"

class SerialInput: public CommandSource {
public:
  SerialInput();

  commandRaw getCommandRaw() override;

#ifdef SerialInputAsCommand
  uint8_t get_menu_jump() const { return menu_jump; }
#endif
#ifdef SerialWriteCard
  const folderSettings& get_write_card() const { return writeCard; }
#endif
private:
#ifdef SerialWriteCard
  bool validateWriteCard(folderSettings& card);
  folderSettings writeCard{};
#endif
#ifdef SerialInputAsCommand
  uint8_t menu_jump{};
#endif
};

#endif /* SRC_SERIAL_INPUT_HPP_ */

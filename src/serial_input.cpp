#include "serial_input.hpp"

#include <string.h>

#include "constants.hpp"
#include "logger.hpp"

#if defined(SerialInputAsCommand) || defined(SerialWriteCard)
SerialInput::SerialInput()
: CommandSource()
{
}

#ifdef SerialWriteCard
bool SerialInput::validateWriteCard(folderSettings &card) {
  if (card.mode == pmode_t::none) {
    return false;
  }

  switch (card.mode) {
  case pmode_t::einzel:
    if (card.special < 1) {
      return false;
    }
    break;
  case pmode_t::hoerspiel_vb:
  case pmode_t::album_vb:
  case pmode_t::party_vb:
  case pmode_t::hoerbuch_vb:
    if (card.special < 1 || card.special > card.special2) {
      return false;
    }
    break;
  case pmode_t::hoerbuch_1:
    if (card.special >= 30) {
      return false;
    }
    break;
  case pmode_t::quiz_game:
    if ((card.special != 0 && card.special != 2 && card.special != 4) ||
        (card.special2 != 0 && card.special2 != 1)) {
      return false;
    }
    break;
  case pmode_t::repeat_last:
  case pmode_t::switch_bt:
    card.folder = 0xff; // dummy Wert, damit readCard() true liefert
    break;
  default:
    break;
  }
  return true;
}
#endif

commandRaw SerialInput::getCommandRaw() {
  commandRaw ret = commandRaw::none;
  if (Serial.available() > 0) {
#ifdef SerialWriteCard
    if (isAlpha(Serial.peek())) {
      char cmdWord[12];
      uint8_t len = 0;
      while (isAlpha(Serial.peek()) && len < sizeof(cmdWord) - 1) {
        cmdWord[len++] = Serial.read();
      }
      cmdWord[len] = '\0';
      if (strcmp(cmdWord, "WRITECARD") == 0 && Serial.read() == ' ') {
        if (isAlpha(Serial.peek())) {
          char subWord[8];
          uint8_t subLen = 0;
          while (isAlpha(Serial.peek()) && subLen < sizeof(subWord) - 1) {
            subWord[subLen++] = Serial.read();
          }
          subWord[subLen] = '\0';
          if (strcmp(subWord, "CANCEL") == 0) {
            return commandRaw::write_card_cancel_from_serial;
          }
          return commandRaw::none;
        }
        folderSettings card{};
        card.mode = static_cast<pmode_t>(Serial.parseInt());
        switch (card.mode) {
        case pmode_t::repeat_last:
        case pmode_t::switch_bt:
          break; // folder/special/special2 haben hier keine Funktion
        case pmode_t::einzel:
        case pmode_t::hoerbuch_1:
          card.folder  = Serial.parseInt();
          card.special = Serial.parseInt();
          break;
        case pmode_t::hoerspiel_vb:
        case pmode_t::album_vb:
        case pmode_t::party_vb:
        case pmode_t::hoerbuch_vb:
        case pmode_t::quiz_game:
          card.folder   = Serial.parseInt();
          card.special  = Serial.parseInt();
          card.special2 = Serial.parseInt();
          break;
        default:
          card.folder = Serial.parseInt();
          break;
        }
        if (validateWriteCard(card)) {
          writeCard = card;
          return commandRaw::write_card_from_serial;
        }
        Serial.println(F("WRITECARD: ERR"));
      }
      return commandRaw::none;
    }
#endif
#ifdef SerialInputAsCommand
    long optionSerial = Serial.parseInt();
    switch (optionSerial) {
    case -2: ret = commandRaw::down      ; break;
    case -8: ret = commandRaw::up        ; break;
    case -3: ret = commandRaw::downLong  ; break;
    case -9: ret = commandRaw::upLong    ; break;
    case -5: ret = commandRaw::pause     ; break;
    case -6: ret = commandRaw::pauseLong ; break;
    case -4: ret = commandRaw::allLong   ; break;
    case -1: ret = commandRaw::updownLong; break;
    default:
      if (optionSerial > 0) {
        ret = commandRaw::menu_jump;
        menu_jump = optionSerial;
      }
      break;
    }
#else
    Serial.read();
#endif
  }
  return ret;
}
#endif


#ifndef SRC_CHIP_CARD_HPP_
#define SRC_CHIP_CARD_HPP_

#include <stdint.h>

#include <MFRC522.h>

#include "constants.hpp"

enum class pmode_t: uint8_t {
  none          =   0,

  // folder modes
  hoerspiel     =   1,
  album         =   2,
  party         =   3,
  einzel        =   4,
  hoerbuch      =   5,
  admin         =   6,
  hoerspiel_vb  =   7,
  album_vb      =   8,
  party_vb      =   9,
  hoerbuch_1    =  10,
  repeat_last   =  11,
  quiz_game     =  12,
  memory_game   =  13,
  switch_bt     =  14,

  // modifier modes
  sleep_timer   =   1,
  freeze_dance  =   2,
  fi_wa_ai      =   3,
  toddler       =   4,
  kindergarden  =   5,
  repeat_single =   6,
  bt_module     =   7,
  jukebox       =   8,
  pause_aft_tr  =   9,
  stdb_timer_sw =  10,

  admin_card    = 0xff,
};

struct folderSettings {
  uint8_t folder;
  pmode_t mode;
  uint8_t special;
  uint8_t special2;
  bool operator==(const folderSettings& rhs) const {
    if (folder != rhs.folder || mode != rhs.mode)
      return false;
    if (mode == pmode_t::einzel && special != rhs.special)
      return false;
    if ((mode == pmode_t::hoerspiel_vb || mode == pmode_t::album_vb || mode == pmode_t::party_vb) &&
        (special != rhs.special || special2 != rhs.special2))
      return false;
    return true;
  }
};

enum class cardEvent : uint8_t {
  none    ,
  removed ,
  inserted,
};

class Mp3;     // forward declaration to not have to include it here

class delayedSwitchOn {
public:
  delayedSwitchOn(uint8_t delay)
  : delaySteps(delay)
  , counter(delay)
  {}
  delayedSwitchOn& operator++() { if (counter < delaySteps) ++counter; return *this; }
  void reset() { counter = 0; }
  bool on   () { return counter == delaySteps; }

private:
  const uint8_t delaySteps;
  uint8_t       counter;
};

class Chip_card {
public:
  Chip_card(Mp3 &mp3);

  enum class readCardEvent : uint8_t {
    none ,
    known,
    empty,
  };

  readCardEvent readCard (      folderSettings &nfcTag);
  bool writeCard         (const folderSettings &nfcTag);
  void sleepCard         ();
  void initCard          ();
  cardEvent getCardEvent ();
  bool isCardRemoved     () { return cardRemoved; }

private:
  friend class tonuino_fixture;
  friend class chip_card_test_fixture;

  void stopCrypto1();
  void stopCard   ();
  bool auth       (MFRC522::PICC_Type piccType);

  MFRC522             mfrc522;
  Mp3                 &mp3;

  delayedSwitchOn     cardRemovedSwitch;
  bool                cardRemoved = true;
};

#endif /* SRC_CHIP_CARD_HPP_ */

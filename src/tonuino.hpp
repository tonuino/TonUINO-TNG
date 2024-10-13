#ifndef SRC_TONUINO_HPP_
#define SRC_TONUINO_HPP_

#include "settings.hpp"
#include "commands.hpp"
#include "buttons.hpp"
#include "buttons3x3.hpp"
#include "rotary_encoder.hpp"
#include "poti.hpp"
#include "serial_input.hpp"
#include "mp3.hpp"
#include "modifier.hpp"
#include "timer.hpp"
#include "batVoltage.hpp"
#ifdef NEO_RING
#include "ring.hpp"
#endif

class Tonuino {
public:
  Tonuino() {}
  static Tonuino& getTonuino() { static Tonuino tonuino; return tonuino; }

  void setup          ();
  void loop           ();

  void playFolder     ();
  void playTrackNumber();

  void       nextTrack(uint8_t tracks = 1, bool fromOnPlayFinished = false);
  void   previousTrack(uint8_t tracks = 1);

  void resetActiveModifier   () { activeModifier = &noneModifier; }
  Modifier& getActiveModifier() { return *activeModifier; }

  void setStandbyTimer();
  void disableStandbyTimer();

  void setMyFolder(const folderSettings &newFolder, bool a_myFolderIsCard) {
#ifdef STORE_LAST_CARD
    if (not (myFolder == newFolder)) {
      LOG(init_log, s_debug, F("set last, folder: "), newFolder.folder, F(", mode: "), static_cast<uint8_t>(newFolder.mode));
      settings.writeExtShortCutToFlash(lastSortCut, newFolder);
    }
#endif
    myFolderIsCard = a_myFolderIsCard;
    myFolder = newFolder;
  }
  const folderSettings& getMyFolder() const     { return myFolder; }
  uint8_t getFolder()                           { return myFolder.folder; }
  bool playingCard()                            { return myFolderIsCard; }

  Mp3&      getMp3      () { return mp3      ; }
  Commands& getCommands () { return commands ; }
  Settings& getSettings () { return settings ; }
  Chip_card& getChipCard() { return chip_card; }
#ifdef NEO_RING
  Ring&     getRing     () { return ring     ; }
#endif
  static uint32_t generateRamdomSeed();

#ifdef SerialInputAsCommand
  uint8_t getMenuJump() const { return serialInput.get_menu_jump(); }
#endif

  void shutdown();

  uint16_t getNumTracksInFolder() const {return numTracksInFolder; }

#ifdef BT_MODULE
  bool isBtModuleOn() { return btModuleOn; }
  void switchBtModuleOnOff();
  void btModulePairing();
#endif

private:

  void setup_timer();
  void setup_adc();

  void checkStandby();

  bool specialCard(const folderSettings &nfcTag);

  Settings             settings            {};
  Mp3                  mp3                 {settings};
  Buttons              buttons             {};
#ifdef BAT_VOLTAGE_MEASUREMENT
       BatVoltage      batVoltage          {mp3};
#endif
#ifdef SerialInputAsCommand
  SerialInput          serialInput         {};
#endif
#ifdef BUTTONS3X3
  Buttons3x3           buttons3x3          {};
#endif
#ifdef ROTARY_ENCODER
  RotaryEncoder        rotaryEncoder       {settings};
#endif
#ifdef POTI
  Poti                 poti                {mp3};
#endif
  Commands             commands            {
                                            settings
                                          , &buttons
#ifdef SerialInputAsCommand
                                          , &serialInput
#endif
#ifdef BUTTONS3X3
                                          , &buttons3x3
#endif
#ifdef ROTARY_ENCODER
                                          , &rotaryEncoder
#endif
#ifdef POTI
                                          , &poti
#endif
                                           };
  Chip_card            chip_card           {mp3};
#ifdef NEO_RING
  Ring                 ring                {};
#endif

  friend class Base;

  Modifier             noneModifier        {};
  SleepTimer           sleepTimer          {};
  DanceGame            danceGame           {};
  ToddlerMode          toddlerMode         {};
  KindergardenMode     kindergardenMode    {};
  RepeatSingleModifier repeatSingleModifier{};

  Modifier*            activeModifier      {&noneModifier};

  Timer                standbyTimer        {};

  folderSettings       myFolder            {};
  bool                 myFolderIsCard      {};
  uint16_t             numTracksInFolder   {};

#ifdef BT_MODULE
  bool                 btModuleOn          {};
  Timer                btModulePairingTimer{};
#endif
};

#endif /* SRC_TONUINO_HPP_ */

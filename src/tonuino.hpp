#ifndef SRC_TONUINO_HPP_
#define SRC_TONUINO_HPP_

#include "settings.hpp"
#include "commands.hpp"
#include "buttons.hpp"
#include "buttons3x3.hpp"
#include "rotary_encoder.hpp"
#include "serial_input.hpp"
#include "mp3.hpp"
#include "modifier.hpp"
#include "timer.hpp"

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

  void setCard  (const nfcTagObject   &newCard) { myCard = newCard; setFolder(&myCard.nfcFolderSettings); }
  const nfcTagObject& getCard() const           { return myCard; }
  void setFolder(folderSettings *newFolder    ) { myFolder = newFolder; }
  uint8_t getFolder()                           { return myFolder->folder; }

  Mp3&      getMp3      () { return mp3      ; }
  Commands& getCommands () { return commands ; }
  Settings& getSettings () { return settings ; }
  Chip_card& getChipCard() { return chip_card; }
  static uint32_t generateRamdomSeed();

  void shutdown();

private:

  void checkStandby();

  bool specialCard(const nfcTagObject &nfcTag);

  Settings             settings            {};
  Mp3                  mp3                 {settings};
  Buttons              buttons             {};
#ifdef SerialInputAsCommand
  SerialInput          serialInput         {};
#endif
#ifdef BUTTONS3X3
  Buttons3x3           buttons3x3          {};
#endif
#ifdef ROTARY_ENCODER
  RotaryEncoder        rotaryEncoder       {settings};
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
                                           };
  Chip_card            chip_card           {mp3};

  friend class Base;

  Modifier             noneModifier        {*this, mp3, settings};
  SleepTimer           sleepTimer          {*this, mp3, settings};
  FreezeDance          freezeDance         {*this, mp3, settings};
  Locked               locked              {*this, mp3, settings};
  ToddlerMode          toddlerMode         {*this, mp3, settings};
  KindergardenMode     kindergardenMode    {*this, mp3, settings};
  RepeatSingleModifier repeatSingleModifier{*this, mp3, settings};
  //FeedbackModifier     feedbackModifier    {*this, mp3, settings};

  Modifier*            activeModifier      {&noneModifier};

  Timer                standbyTimer        {};

  nfcTagObject         myCard              {};
  folderSettings*      myFolder            {&myCard.nfcFolderSettings};
  uint16_t             numTracksInFolder   {};
};

#endif /* SRC_TONUINO_HPP_ */

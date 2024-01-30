#ifndef SRC_MODIFIER_HPP_
#define SRC_MODIFIER_HPP_

#include <Arduino.h>

#include "chip_card.hpp"
#include "logger.hpp"
#include "timer.hpp"
#include "commands.hpp"

class Tonuino;
class Mp3;
struct folderSettings;

class Modifier {
public:
  Modifier(Tonuino &tonuino, Mp3 &mp3): tonuino(tonuino), mp3(mp3) {}
  //virtual ~Modifier() {}
  virtual void loop                () {}
  virtual bool handleNext          () { return false; }
  virtual bool handlePrevious      () { return false; }
  virtual bool handleButton(command ) { return false; }
  virtual bool handleRFID(const folderSettings&)
                                      { return false; }
  virtual pmode_t getActive        () { return pmode_t::none; }
  virtual void init         (uint8_t) {}

  Modifier& operator=(const Modifier&) = delete;
protected:
  Tonuino        &tonuino;
  Mp3            &mp3;
};

class SleepTimer: public Modifier {
public:
  SleepTimer(Tonuino &tonuino, Mp3 &mp3): Modifier(tonuino, mp3) {}
  void   loop       () final;

  pmode_t getActive () final { return pmode_t::sleep_timer; }
  void   init(uint8_t) final;

private:
  Timer sleepTimer{};
};

class FreezeDance: public Modifier {
public:
  FreezeDance(Tonuino &tonuino, Mp3 &mp3): Modifier(tonuino, mp3) {}
  void   loop       () final;

  pmode_t getActive () final { return pmode_t::freeze_dance; }
  void   init(uint8_t) final { setNextStopAtMillis(); }

private:
  void setNextStopAtMillis();

  Timer stopTimer{};
  static constexpr uint8_t minSecondsBetweenStops =  5;
  static constexpr uint8_t maxSecondsBetweenStops = 30;
};

class Locked: public Modifier {
public:
  Locked(Tonuino &tonuino, Mp3 &mp3): Modifier(tonuino, mp3) {}
  bool handleButton(command) final { LOG(modifier_log, s_debug, F("Locked::Button -> LOCKED!"))    ; return true; }
  bool handleRFID(const folderSettings&)
                             final { LOG(modifier_log, s_debug, F("Locked::RFID -> LOCKED!"))      ; return true; }

  pmode_t getActive()        final { return pmode_t::locked; }
};

class ToddlerMode: public Modifier {
public:
  ToddlerMode(Tonuino &tonuino, Mp3 &mp3): Modifier(tonuino, mp3) {}
  bool handleButton(command) final { LOG(modifier_log, s_debug, F("ToddlerMode::Button -> LOCKED!")); return true; }

  pmode_t getActive()        final { return pmode_t::toddler; }
};

class KindergardenMode: public Modifier {
public:
  KindergardenMode(Tonuino &tonuino, Mp3 &mp3): Modifier(tonuino, mp3) {}
  bool handleNext  (                           ) final;
  bool handleButton(command cmd                ) final;
  bool handleRFID  (const folderSettings &newCard) final;

  pmode_t getActive (                          ) final { return pmode_t::kindergarden; }
  void   init       (uint8_t                   ) final { cardQueued = false; }

private:
  folderSettings nextCard{};
  bool cardQueued = false;
};

class RepeatSingleModifier: public Modifier {
public:
  RepeatSingleModifier(Tonuino &tonuino, Mp3 &mp3): Modifier(tonuino, mp3) {}
  bool   handleNext    () final;
  bool   handlePrevious() final;
  pmode_t getActive    () final { return pmode_t::repeat_single; }
};

// An modifier can also do somethings in addition to the modified action
// by returning false (not handled) at the end
// This simple FeedbackModifier will tell the volume before changing it and
// give some feedback once a RFID card is detected.
//class FeedbackModifier: public Modifier {
//public:
//  FeedbackModifier(Tonuino &tonuino, Mp3 &mp3, const Settings &settings): Modifier(tonuino, mp3, settings) {}
//  bool handleVolumeDown() final;
//  bool handleVolumeUp  () final;
//  bool handleRFID      (const folderSettings &newCard) final;
//};

#endif /* SRC_MODIFIER_HPP_ */

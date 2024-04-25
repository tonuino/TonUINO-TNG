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
  Modifier() {}
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
};

class SleepTimer: public Modifier {
public:
  SleepTimer() {}
  void   loop       () final;
  bool   handleNext () final;

  pmode_t getActive () final { return pmode_t::sleep_timer; }
  void   init(uint8_t) final;

private:
  Timer sleepTimer{};
  bool  stopAfterTrackFinished{};
  bool  stopAfterTrackFinished_active{};
};

class FreezeDance: public Modifier {
public:
  FreezeDance() {}
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
  Locked() {}
  bool handleButton(command) final { LOG(modifier_log, s_debug, F("Locked::Button -> LOCKED!"))    ; return true; }
  bool handleRFID(const folderSettings&)
                             final { LOG(modifier_log, s_debug, F("Locked::RFID -> LOCKED!"))      ; return true; }

  pmode_t getActive()        final { return pmode_t::locked; }
};

class ToddlerMode: public Modifier {
public:
  ToddlerMode() {}
  bool handleButton(command) final { LOG(modifier_log, s_debug, F("ToddlerMode::Button -> LOCKED!")); return true; }

  pmode_t getActive()        final { return pmode_t::toddler; }
};

class KindergardenMode: public Modifier {
public:
  KindergardenMode() {}
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
  RepeatSingleModifier() {}
  bool   handleNext    () final;
  bool   handlePrevious() final;
  pmode_t getActive    () final { return pmode_t::repeat_single; }
};

#endif /* SRC_MODIFIER_HPP_ */

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
  virtual void init         (pmode_t, uint8_t) {}

  Modifier& operator=(const Modifier&) = delete;
};

class SleepTimer: public Modifier {
public:
  SleepTimer() {}
  void   loop       () final;
  bool   handleNext () final;
  bool handleButton(command cmd                  ) final;
  bool handleRFID  (const folderSettings &newCard) final;

  pmode_t getActive () final { return pmode_t::sleep_timer; }
  void   init(pmode_t, uint8_t) final;

private:
  Timer sleepTimer{};
  bool  stopAfterTrackFinished{};
  bool  stopAfterTrackFinished_active{};
  bool  fired{};
};

class DanceGame: public Modifier {
public:
  DanceGame() {}
  void   loop       () final;

  pmode_t getActive ()        final { return mode; }
  void   init(pmode_t, uint8_t) final;

  static constexpr uint8_t minSecondsBetweenStops[]      = {15, 25, 35};
  static constexpr uint8_t maxSecondsBetweenStops[]      = {30, 40, 50};
  static constexpr uint8_t addSecondsBetweenStopsFreezeD =  6;
  static constexpr uint8_t addSecondsBetweenStopsFiWaAi  = 17;

private:
  void setNextStop(bool addAdvTime);

  Timer stopTimer{};
  pmode_t mode{};
  uint8_t lastFiWaAi{};
  uint8_t t{0};
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
  void   init       (pmode_t, uint8_t          ) final { cardQueued = false; }

private:
  folderSettings nextCard{};
  bool cardQueued{false};
};

class RepeatSingleModifier: public Modifier {
public:
  RepeatSingleModifier() {}
  bool   handleNext    () final;
  bool   handlePrevious() final;
  pmode_t getActive    () final { return pmode_t::repeat_single; }
};

#endif /* SRC_MODIFIER_HPP_ */

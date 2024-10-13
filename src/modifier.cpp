#include "modifier.hpp"

#include "mp3.hpp"
#include "tonuino.hpp"
#include "logger.hpp"
#include "state_machine.hpp"

namespace {

Tonuino        &tonuino   = Tonuino::getTonuino();
Mp3            &mp3       = tonuino.getMp3();

const __FlashStringHelper* str_SleepTimer          () { return F("SleepTimer")  ; }
const __FlashStringHelper* str_danceGame           () { return F("DanceGame") ; }
const __FlashStringHelper* str_KindergardenMode    () { return F("Kita")        ; }
const __FlashStringHelper* str_RepeatSingleModifier() { return F("RepeatSingle"); }

} // anonymous namespace

void SleepTimer::loop() {
  if (sleepTimer.isActive() && sleepTimer.isExpired()) {
    LOG(modifier_log, s_debug, str_SleepTimer(), F(" -> expired"));
    if (not stopAfterTrackFinished || stopAfterTrackFinished_active) {
      LOG(modifier_log, s_debug, str_SleepTimer(), F(" -> SLEEP!"));
      if (SM_tonuino::is_in_state<Play>())
        SM_tonuino::dispatch(command_e(commandRaw::pause));
      fired = true;
      //tonuino.resetActiveModifier();
    }
    else {
      stopAfterTrackFinished_active = true;
      sleepTimer.start(10 * 60000);
    }
  }
}
bool SleepTimer::handleNext() {
  if (stopAfterTrackFinished_active) {
    LOG(modifier_log, s_debug, str_SleepTimer(), F(" -> SLEEP!"));
    mp3.clearFolderQueue();
    stopAfterTrackFinished_active = false;
    sleepTimer.stop();
    fired = true;
    //tonuino.resetActiveModifier();
  }
  return false;
}

void SleepTimer::init(pmode_t, uint8_t special /* is minutes*/) {
  LOG(modifier_log, s_debug, str_SleepTimer(), F(" minutes: "), special);
  fired = false;
  stopAfterTrackFinished_active = false;
  if (special > 0x80) {
    stopAfterTrackFinished = true;
    special -= 0x80;
  } else {
    stopAfterTrackFinished = false;
  }
  sleepTimer.start(special * 60000);
}

bool SleepTimer::handleButton(command cmd) {
  if (cmd == command::pause && fired) {
    LOG(modifier_log, s_debug, F("SleepTimer::PauseButton -> LOCKED!"));
    return true;
  }
  return false;
}

bool SleepTimer::handleRFID(const folderSettings &/*newCard*/) {
  if (fired) {
    LOG(modifier_log, s_debug, F("SleepTimer::handleRFID -> LOCKED!"));
    return true;
  }
  return false;
}


void DanceGame::init(pmode_t a_mode, uint8_t a_t) {
  LOG(modifier_log, s_debug, str_danceGame(), F("t : "), a_t);
  mode = a_mode;
  if (mode == pmode_t::fi_wa_ai) lastFiWaAi = random(0, 3);
  setNextStop(true /*addAdvTime*/);
  t = a_t;
}


void DanceGame::loop() {
  if (SM_tonuino::is_in_state<Play>()) {
    if (not stopTimer.isActive()) {
      setNextStop(false /*addAdvTime*/);
    }
    if (stopTimer.isExpired()) {
      switch (mode) {
      case pmode_t::freeze_dance:
        LOG(modifier_log, s_debug, str_danceGame(), F(" -> FREEZE!"));
        mp3.playAdvertisement(advertTracks::t_301_freeze_freeze);
        setNextStop(true /*addAdvTime*/);
        break;
      case pmode_t::fi_wa_ai:
        LOG(modifier_log, s_debug, str_danceGame(), F(" -> Action! "));
        lastFiWaAi = (lastFiWaAi+random(1, 3))%3;
        mp3.playAdvertisement(static_cast<uint16_t>(advertTracks::t_306_fire)+lastFiWaAi);
        setNextStop(true /*addAdvTime*/);
        break;
      default:
        break;
      }
    }
  }
  else {
    stopTimer.stop();
  }
}

void DanceGame::setNextStop(bool addAdvTime) {
  uint16_t seconds = random(minSecondsBetweenStops[t], maxSecondsBetweenStops[t] + 1);
  if (addAdvTime) {
    switch (mode) {
      case pmode_t::fi_wa_ai    : seconds += addSecondsBetweenStopsFiWaAi ; break;
      case pmode_t::freeze_dance: seconds += addSecondsBetweenStopsFreezeD; break;
      default:                                                              break;
    }
  }
  LOG(modifier_log, s_debug, str_danceGame(), F(" next stop in "), seconds);
  stopTimer.start(seconds * 1000);
}

bool KindergardenMode::handleNext() {
  if (cardQueued) {
    LOG(modifier_log, s_debug, str_KindergardenMode(), F(" -> NEXT"));
    cardQueued = false;

    tonuino.setMyFolder(nextCard, true /*myFolderIsCard*/);
    LOG(modifier_log, s_debug, F("Folder: "), nextCard.folder, F(" Mode: "), static_cast<uint8_t>(nextCard.mode));
    tonuino.playFolder();
    mp3.loop(); // to start the new queue now and not going to Idle
    return true;
  }
  return false;
}
bool KindergardenMode::handleRFID(const folderSettings &newCard) {
  if (!mp3.isPlaying())
    return false;

  if (!cardQueued) {
    LOG(modifier_log, s_debug, str_KindergardenMode(), F(" -> queued!"));
    nextCard = newCard;
    cardQueued = true;
  }
  return true;
}

bool KindergardenMode::handleButton(command cmd) {
  if (cmd != command::pause) {
    LOG(modifier_log, s_debug, F("KindergardenMode::NextButton -> LOCKED!"));
    return true;
  }
  return false;
}


bool RepeatSingleModifier::handleNext() {
  LOG(modifier_log, s_debug, str_RepeatSingleModifier(), F(" -> REPEAT"));
  mp3.loop(); // WA: this will call again Mp3Notify::OnPlayFinished() (error in DFMiniMp3 lib)
              //     but will be blocked by lastTrackFinished
  Mp3Notify::ResetLastTrackFinished(); // unblock this track so that it can be repeated
  mp3.playCurrent();
  return true;
}
bool RepeatSingleModifier::handlePrevious() {
  return handleNext();
}

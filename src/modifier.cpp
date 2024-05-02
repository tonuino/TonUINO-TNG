#include "modifier.hpp"

#include "mp3.hpp"
#include "tonuino.hpp"
#include "logger.hpp"
#include "state_machine.hpp"

namespace {

Tonuino        &tonuino   = Tonuino::getTonuino();
Mp3            &mp3       = tonuino.getMp3();

const __FlashStringHelper* str_SleepTimer          () { return F("SleepTimer")  ; }
const __FlashStringHelper* str_FreezeDance         () { return F("FreezeDance") ; }
const __FlashStringHelper* str_KindergardenMode    () { return F("Kita")        ; }
const __FlashStringHelper* str_RepeatSingleModifier() { return F("RepeatSingle"); }

} // anonymous namespace

void SleepTimer::loop() {
  if (sleepTimer.isActive() && sleepTimer.isExpired()) {
    LOG(modifier_log, s_info, str_SleepTimer(), F(" -> expired"));
    if (not stopAfterTrackFinished || stopAfterTrackFinished_active) {
      LOG(modifier_log, s_info, str_SleepTimer(), F(" -> SLEEP!"));
      if (SM_tonuino::is_in_state<Play>())
        SM_tonuino::dispatch(command_e(commandRaw::pause));
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
    LOG(modifier_log, s_info, str_SleepTimer(), F(" -> SLEEP!"));
    mp3.clearFolderQueue();
    stopAfterTrackFinished_active = false;
    sleepTimer.stop();
    //tonuino.resetActiveModifier();
  }
  return false;
}

void SleepTimer::init(uint8_t special /* is minutes*/) {
  LOG(modifier_log, s_info, str_SleepTimer(), F(" minutes: "), special);
  stopAfterTrackFinished_active = false;
  if (special > 0x80) {
    stopAfterTrackFinished = true;
    special -= 0x80;
  } else {
    stopAfterTrackFinished = false;
  }
  sleepTimer.start(special * 60000);
}

void FreezeDance::loop() {
  if (stopTimer.isExpired()) {
    LOG(modifier_log, s_info, str_FreezeDance(), F(" -> FREEZE!"));
    mp3.playAdvertisement(advertTracks::t_301_freeze_freeze);
    setNextStopAtMillis();
  }
}

void FreezeDance::setNextStopAtMillis() {
  const uint16_t seconds = random(minSecondsBetweenStops, maxSecondsBetweenStops + 1);
  LOG(modifier_log, s_info, str_FreezeDance(), F(" next stop in "), seconds);
  stopTimer.start(seconds * 1000);
}

bool KindergardenMode::handleNext() {
  if (cardQueued) {
    LOG(modifier_log, s_info, str_KindergardenMode(), F(" -> NEXT"));
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
    LOG(modifier_log, s_info, str_KindergardenMode(), F(" -> queued!"));
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
  LOG(modifier_log, s_info, str_RepeatSingleModifier(), F(" -> REPEAT"));
  mp3.loop(); // WA: this will call again Mp3Notify::OnPlayFinished() (error in DFMiniMp3 lib)
              //     but will be blocked by lastTrackFinished
  Mp3Notify::ResetLastTrackFinished(); // unblock this track so that it can be repeated
  mp3.playCurrent();
  return true;
}
bool RepeatSingleModifier::handlePrevious() {
  return handleNext();
}

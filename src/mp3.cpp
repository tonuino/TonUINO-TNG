#include "mp3.hpp"

#include "tonuino.hpp"
#include "constants.hpp"

namespace {

const __FlashStringHelper* str_Space()  { return F(" ") ; }

}

uint16_t Mp3Notify::lastTrackFinished = 0;

void Mp3Notify::OnError(DfMp3&, uint16_t errorCode) {
  // see DfMp3_Error for code meaning
  LOG(mp3_log, s_error, F("DfPl Err: "), errorCode);
}
void Mp3Notify::OnPlaySourceOnline  (DfMp3&, DfMp3_PlaySources source) { PrintlnSourceAction(source, F("online"  )); }
void Mp3Notify::OnPlaySourceInserted(DfMp3&, DfMp3_PlaySources source) { PrintlnSourceAction(source, F("bereit"  )); }
void Mp3Notify::OnPlaySourceRemoved (DfMp3&, DfMp3_PlaySources source) { PrintlnSourceAction(source, F("entfernt")); }
void Mp3Notify::PrintlnSourceAction(DfMp3_PlaySources source, const __FlashStringHelper* action) {
  if (source & DfMp3_PlaySources_Sd   ) LOG(mp3_log, s_debug, F("SD Karte "), action);
  if (source & DfMp3_PlaySources_Usb  ) LOG(mp3_log, s_debug, F("USB "     ), action);
  if (source & DfMp3_PlaySources_Flash) LOG(mp3_log, s_debug, F("Flash "   ), action);
}

void Mp3Notify::OnPlayFinished(DfMp3&, DfMp3_PlaySources /*source*/, uint16_t track) {
  LOG(mp3_log, s_info, F("Track end: "), track);
  if (track == lastTrackFinished)
    return;
  else
    lastTrackFinished = track;
#ifdef DFMiniMp3_IGNORE_ONPLAYFINISHED_FOR_ADV
  if (Tonuino::getTonuino().getMp3().resetPlayingAdv())
    return;
#endif
  delay(1);
  Tonuino::getTonuino().nextTrack(1/*tracks*/, true/*fromOnPlayFinished*/);
}

#ifndef DFPlayerUsesHardwareSerial
Mp3::Mp3(Settings& settings)
: Base{softwareSerial}
, softwareSerial{dfPlayer_receivePin, dfPlayer_transmitPin}
, settings{settings}
#else
Mp3::Mp3(Settings &settings)
: Base{dfPlayer_serial}
, settings{settings}
#endif /* DFPlayerUsesHardwareSerial */
{
  // Busy Pin
  pinMode(dfPlayer_busyPin              , dfPlayer_busyPinType               == levelType::activeHigh ? INPUT : INPUT_PULLUP);

#ifdef HPJACKDETECT
  // Headphone Jack Detection
#ifdef ALLinONE_Plus
  pinMode(dfPlayer_noHeadphoneJackDetect, INPUT);
#else
  pinMode(dfPlayer_noHeadphoneJackDetect, INPUT_PULLUP);
#endif
#endif
}

void Mp3::init() {
  spkVolume = settings.spkInitVolume;
#ifdef HPJACKDETECT
  hpVolume  = settings.hpInitVolume;
#endif

  begin();
  loop();

  if (not setVolume())
    LOG(init_log, s_error, F("Com to DFPlayer broken"));

  setEq(static_cast<DfMp3_Eq>(settings.eq - 1));

  startTrackTimer.start(6000); // 6 seconds
  while (not startTrackTimer.isExpired() && (getTotalTrackCount() == 0)) {
    delay(100);
    loop();
  }
  startTrackTimer.stop();

  LOG(mp3_log, s_info, F("track_count: "), getTotalTrackCount());
  delay(1000);

  loop();
}

void Mp3::refreshIsPlaying() {
  is_playing_cache = !digitalRead(dfPlayer_busyPin);
}

void Mp3::waitForTrackToFinish() {
  LOG(mp3_log, s_debug, F("waitForTrackToFinish "), isPlaying());

  do {
    loop();
  } while (isPlaying());
  LOG(mp3_log, s_debug, F("waitForTrackToFinish End "), isPlaying());
}

void Mp3::waitForTrackToStart() {
  LOG(mp3_log, s_debug, F("waitForTrackToStart "), isPlaying());
  Timer timer;
  timer.start(dfPlayer_timeUntilStarts);

  do {
    loop();
  } while (!isPlaying() && not timer.isExpired());
  LOG(mp3_log, s_debug, F("waitForTrackToStart End "), isPlaying());
}

void Mp3::playAdvertisement(uint16_t track, bool olnyIfIsPlaying) {
  LOG(mp3_log, s_info, F("play adv: "), track);
#ifdef DFMiniMp3_IGNORE_ONPLAYFINISHED_FOR_ADV
  advPlaying = true;
#endif
  if (isPlaying()) {
    LOG(mp3_log, s_debug, F("playAdvertisement()"));
    Base::playAdvertisement(track);
  }
  else if (not olnyIfIsPlaying) {
    LOG(mp3_log, s_debug, F("playAdvertisement: "), track);
    if (isPause) {
      Base::start();
      LOG(mp3_log, s_debug, F("after start"));
    }
    else {
      Base::playFolderTrack(1, 1);
      LOG(mp3_log, s_debug, F("after playFolderTrack"));
      delay(dfPlayer_timeUntilStarts);
      LOG(mp3_log, s_debug, F("after delay"));
    }
    waitForTrackToStart();
    LOG(mp3_log, s_debug, F("after waitForTrackToStart"));

    Base::playAdvertisement(track);
    delay(dfPlayer_timeUntilStarts);
    LOG(mp3_log, s_debug, F("after delay"));

    waitForTrackToFinish(); // finish adv
    LOG(modifier_log, s_debug, "after waitForTrackToFinish");

    waitForTrackToStart();  // start folder track
    LOG(mp3_log, s_debug, F("after waitForTrackToStart()"));

#ifdef DFMiniMp3_T_CHIP_MH2024K24SS_MP3_TF_16P_V3_0
    waitForTrackToFinish();
    LOG(modifier_log, s_debug, "after waitForTrackToFinish");

    waitForTrackToStart();
    LOG(modifier_log, s_debug, "after waitForTrackToStart");
#endif

    delay(10);
    Base::pause();
    loop();
  }
}

void Mp3::playAdvertisement(advertTracks track, bool olnyIfIsPlaying) {
  playAdvertisement(static_cast<uint16_t>(track), olnyIfIsPlaying);
}

void Mp3::clearFolderQueue() {
  LOG(mp3_log, s_debug, F("clear folder"));
  if (playing == play_folder) playing = play_none;
  current_track  = 0;
  current_folder = 0;
  q.clear();
}
void Mp3::clearMp3Queue() {
  LOG(mp3_log, s_debug, F("clear mp3"));
  if (playing == play_mp3) playing = play_none;
  mp3_track      = 0;
  mp3_track_next = 0;
}
void Mp3::enqueueTrack(uint8_t folder, uint8_t firstTrack, uint8_t lastTrack, uint8_t currentTrack) {
#ifdef HPJACKDETECT
  if (tempSpkOn > 0)
    --tempSpkOn;
#endif
  clearAllQueue();
  current_folder = folder;
  endless = false;
  LOG(mp3_log, s_info, F("enqueue "), folder, F("-"), lf_no);
  for (uint8_t i = firstTrack; i<=lastTrack; ++i) {
    LOG(mp3_log, s_info, i, str_Space(), lf_no);
    q.push(i);
    if (i == 0xffu)
      break;
  }
  LOG(mp3_log, s_info, str_Space());
  current_track = currentTrack;
}
void Mp3::enqueueTrack(uint8_t folder, uint8_t track) {
  enqueueTrack(folder, track, track);
}
void Mp3::shuffleQueue() {
  q.shuffle();
  LOG(mp3_log, s_info, F("shuffled "), lf_no);
  for (uint8_t i = 0; i<q.size(); ++i)
    LOG(mp3_log, s_info, q.get(i), str_Space(), lf_no);
  LOG(mp3_log, s_info, str_Space());
}
void Mp3::enqueueMp3FolderTrack(uint16_t track, bool playAfter) {
  LOG(mp3_log, s_info, F("enqueue mp3 "), track, str_Space(), playAfter);
  clearFolderQueue();
  if (not playAfter)
    clearMp3Queue();
  if (mp3_track != 0)
    mp3_track_next = track;
  else
    mp3_track = track;
}
void Mp3::enqueueMp3FolderTrack(mp3Tracks track, bool playAfter) {
  enqueueMp3FolderTrack(static_cast<uint16_t>(track), playAfter);
}

void Mp3::playCurrent() {
  LOG(mp3_log, s_debug, F("play current"));
  if (current_folder == 0) { // maybe play mp3 track
    if (mp3_track != 0) {
      LOG(mp3_log, s_info, F("play mp3 "), mp3_track);
      Mp3Notify::ResetLastTrackFinished(); // maybe the same mp3 track is played twice
      LOG(mp3_log, s_debug, F("playMp3FolderTrack: "), mp3_track);
      Base::playMp3FolderTrack(mp3_track);
      isPause = false;
      startTrackTimer.start(dfPlayer_timeUntilStarts);
      playing = play_mp3;
      mp3_track = 0;
      swap(mp3_track, mp3_track_next);
    }
  }
  else { // play folder track
    uint8_t t = q.get(current_track);
    if (t != 0) {
      LOG(mp3_log, s_info, F("play "), current_folder, F("-"), t);
      Base::playFolderTrack(current_folder, t);
      isPause = false;
      startTrackTimer.start(dfPlayer_timeUntilStarts);
      playing = play_folder;
    }
  }
}
void Mp3::playNext(uint8_t tracks, bool fromOnPlayFinished) {
#ifdef HPJACKDETECT
  if (playing == play_folder)
    tempSpkOn = 0;
#endif
  if (playing == play_folder && (current_track+1 < q.size() || endless)) {
    current_track += tracks;
    if (current_track >= q.size())
      current_track = endless ? current_track % q.size() : q.size()-1;
    LOG(mp3_log, s_debug, F("playNext: "), current_track);
    playCurrent();
  }
  else if (playing == play_mp3 && mp3_track != 0) {
    LOG(mp3_log, s_debug, F("playNext mp3: "), mp3_track);
    playCurrent();
  }
  else if (fromOnPlayFinished) {
    LOG(mp3_log, s_debug, F("playNext: stop"));
    clearAllQueue();
    playing = play_none;
  }
}
void Mp3::playPrevious(uint8_t tracks) {
  if (playing == play_folder) {
#ifdef HPJACKDETECT
    tempSpkOn = 0;
#endif
    int current_track_tmp = static_cast<int>(current_track) - tracks;
    current_track = endless ? (current_track_tmp%q.size()+q.size()) % q.size() : max(current_track_tmp, 0);
    LOG(mp3_log, s_debug, F("playPrevious: "), current_track);
    playCurrent();
  }
}

void Mp3::jumpTo(uint8_t track) {
#ifdef HPJACKDETECT
  if (playing == play_folder)
    tempSpkOn = 0;
#endif
  if (playing == play_folder && (track < q.size())) {
    current_track = track;
    LOG(mp3_log, s_debug, F("jumpTo: "), current_track);
    playCurrent();
  }
}

uint16_t Mp3::getFolderTrackCount(uint16_t folder)
{
    uint16_t ret = 0;

#ifdef DFMiniMp3_T_CHIP_GD3200B
    Base::setVolume(0);
    delay(100);

    Base::playFolderTrack(folder, 1);
    delay(500);
#endif

    LOG(mp3_log, s_debug, F("getFolderTrackCount: "), folder);
    ret = Base::getFolderTrackCount(folder);
    LOG(mp3_log, s_debug, F("getFolderTrackCount return: "), ret);

#ifdef DFMiniMp3_T_CHIP_GD3200B
    Base::stop();
    Base::setVolume(*volume);
#endif

    return ret;
}

void Mp3::increaseVolume() {
  if (*volume < *maxVolume) {
    LOG(mp3_log, s_debug, F("setVolume: "), *volume+1);
    Base::setVolume(++*volume);
  }
#ifdef NEO_RING_EXT
  volumeChangedTimer.start(1000);
#endif // NEO_RING_EXT
  logVolume();
}

void Mp3::decreaseVolume() {
  if (*volume > *minVolume) {
    LOG(mp3_log, s_debug, F("setVolume: "), *volume-1);
    Base::setVolume(--*volume);
  }
#ifdef NEO_RING_EXT
  volumeChangedTimer.start(1000);
#endif // NEO_RING_EXT
  logVolume();
}

bool Mp3::setVolume() {
  LOG(mp3_log, s_debug, F("setVolume: "), volume);
  startTrackTimer.start(6000); // 6 seconds
  while(not startTrackTimer.isExpired() && (Base::getVolume() != *volume)) {
    delay(200);
    loop();
    Base::setVolume(*volume);
    delay(200);
    loop();
  }
  if (not startTrackTimer.isActive()) {
    return false;
  } else {
    startTrackTimer.stop();
  }
  logVolume();
  return true;
}

void Mp3::setVolume(uint8_t v) {
  *volume = v;
  LOG(mp3_log, s_debug, F("setVolume: "), *volume);
  Base::setVolume(*volume);
  logVolume();
}

void Mp3::logVolume() {
  LOG(mp3_log, s_info, F("Volume: "), *volume);
}

#ifdef HPJACKDETECT
void Mp3::hpjackdetect() {

  level noHeadphoneJackDetect_now = getLevel(dfPlayer_noHeadphoneJackDetectType, digitalRead(dfPlayer_noHeadphoneJackDetect));
  if (tempSpkOn)
    noHeadphoneJackDetect_now = level::active;

  if (noHeadphoneJackDetect != noHeadphoneJackDetect_now) {
    noHeadphoneJackDetect = noHeadphoneJackDetect_now;
    LOG(mp3_log, s_info, F("hpJackDetect: "), noHeadphoneJackDetect == level::active ? 0 : 1);
#ifdef SPKONOFF
    digitalWrite(ampEnablePin, getLevel(ampEnablePinType, noHeadphoneJackDetect));
#endif
    if (isHeadphoneJackDetect()) {
      volume     = &hpVolume;
      maxVolume  = &settings.hpMaxVolume;
      minVolume  = &settings.hpMinVolume;
      initVolume = &settings.hpInitVolume;
    } else {
      volume     = &spkVolume;
      maxVolume  = &settings.spkMaxVolume;
      minVolume  = &settings.spkMinVolume;
      initVolume = &settings.spkInitVolume;
    }
    Base::setVolume(*volume);
    logVolume();
  }
}
#endif

#ifdef TonUINO_Esp32
  String Mp3::getQueue() {
    String res;
    constexpr uint8_t additional = 4;

    if (isPlayingFolder()) {
      uint8_t first = (current_track < additional) ? 0 : current_track-additional;
      uint8_t last  = (current_track > q.size()-1-additional) ? q.size()-1 : current_track+additional;
      if (first > 0)
        res += "... ";
      for (uint8_t t = first; t <= last; ++t) {
        if (t == current_track)
          res += "<b>";
        res += String(q.get(t));
        if (t == current_track)
          res += "</b>";
        res += " ";
      }
      if (last < q.size()-1)
        res += "...";
    }

    return res;
  }
#endif


void Mp3::loop() {

  refreshIsPlaying();
  static bool is_playing = false;
  LOG_CODE(mp3_log, s_info, {
    if (is_playing != is_playing_cache) {
      is_playing = is_playing_cache;
      LOG(mp3_log, s_info, F("isPlaying: "), is_playing);
    }
  } );

  if (not isPause && playing != play_none && startTrackTimer.isExpired() && not isPlaying()) {
    if (not missingOnPlayFinishedTimer.isActive())
      missingOnPlayFinishedTimer.start(dfPlayer_timeUntilStarts);
  }
  else {
    missingOnPlayFinishedTimer.stop();
  }
  if (missingOnPlayFinishedTimer.isActive() && missingOnPlayFinishedTimer.isExpired()) {
    LOG(mp3_log, s_info, F("missing OnPlayFinished"));
    Tonuino::getTonuino().nextTrack(1/*tracks*/, true/*fromOnPlayFinished*/);
  }
  else
  if (playing == play_none && (current_folder != 0 || mp3_track != 0)) {
    playCurrent();
  }
  Base::loop();
}

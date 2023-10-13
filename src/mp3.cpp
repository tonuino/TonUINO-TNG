#include "mp3.hpp"

#include "tonuino.hpp"
#include "constants.hpp"

namespace {

const __FlashStringHelper* str_Space()  { return F(" ") ; }

}

uint16_t Mp3Notify::lastTrackFinished = 0;

void Mp3Notify::OnError(DfMp3&, uint16_t errorCode) {
  // see DfMp3_Error for code meaning
  LOG(mp3_log, s_error, F("DfPlayer Error: "), errorCode);
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
  LOG(mp3_log, s_debug, F("Track beendet: "), track);
  if (track == lastTrackFinished)
    return;
  else
    lastTrackFinished = track;
#ifdef DFMiniMp3_T_CHIP_LISP3  
  if (Tonuino::getTonuino().getMp3().resetPlayingAdv())
    return;
#endif
  delay(1);
  Tonuino::getTonuino().nextTrack(1/*tracks*/, true/*fromOnPlayFinished*/);
}

#ifdef DFPlayerUsesSoftwareSerial
Mp3::Mp3(const Settings& settings)
: Base{softwareSerial}
, softwareSerial{dfPlayer_receivePin, dfPlayer_transmitPin}
, settings{settings}
#endif /* DFPlayerUsesSoftwareSerial */
#ifdef DFPlayerUsesHardwareSerial
Mp3::Mp3(const Settings &settings)
: Base{dfPlayer_serial}
, settings{settings}
#endif /* DFPlayerUsesHardwareSerial */
{
  // Busy Pin
  pinMode(dfPlayer_busyPin, dfPlayer_busyPinType == levelType::activeHigh ? INPUT : INPUT_PULLUP);
}

bool Mp3::isPlaying() const {
  return !digitalRead(dfPlayer_busyPin);
}

void Mp3::waitForTrackToFinish() {

  // wait until track is started
  waitForTrackToStart();
  delay(1000);

  // wait until track is finished
  do {
    loop();
  } while (isPlaying());
}

void Mp3::waitForTrackToStart() {
  unsigned long currentTime = millis();
  const unsigned long maxStartTime = 1000;

  // wait until track is started
  do {
    loop();
  } while (!isPlaying() && millis() < currentTime + maxStartTime);
}

void Mp3::playAdvertisement(uint16_t track, bool olnyIfIsPlaying) {
  LOG(mp3_log, s_info, F("play adv: "), track);
#ifdef DFMiniMp3_T_CHIP_LISP3
  advPlaying = true;
#endif
  if (isPlaying()) {
    LOG(mp3_log, s_debug, F("playAdvertisement: "), track);
    Base::playAdvertisement(track);
  }
  else if (not olnyIfIsPlaying) {
    start();
    loop();
    LOG(mp3_log, s_debug, F("playAdvertisement: "), track);
    Base::playAdvertisement(track);
    waitForTrackToFinish(); // TODO remove waitForTrackToFinish
    pause();
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
#ifdef CHECK_MISSING_ONPLAYFINISHED
      isPause = false;
      startTrackTimer.start(dfPlayer_timeUntilStarts);
#endif
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
#ifdef CHECK_MISSING_ONPLAYFINISHED
      isPause = false;
      startTrackTimer.start(dfPlayer_timeUntilStarts);
#endif
      playing = play_folder;
    }
  }
}
void Mp3::playNext(uint8_t tracks) {
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
  else {
    LOG(mp3_log, s_debug, F("playNext: stop"));
    clearAllQueue();
    playing = play_none;
  }
}
void Mp3::playPrevious(uint8_t tracks) {
  if (playing == play_folder && (current_track > 0 || endless)) {
    int current_track_tmp = static_cast<int>(current_track) - tracks;
    current_track = endless ? (current_track_tmp%q.size()+q.size()) % q.size() : max(current_track_tmp, 0);
    LOG(mp3_log, s_debug, F("playPrevious: "), current_track);
    playCurrent();
  }
  else {
    LOG(mp3_log, s_debug, F("playPrevious: stop"));
    clearAllQueue();
    playing = play_none;
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
    Base::setVolume(volume);
#endif

    return ret;
}

void Mp3::increaseVolume() {
  if (volume < settings.maxVolume) {
    LOG(mp3_log, s_debug, F("setVolume: "), volume+1);
    Base::setVolume(++volume);
  }
  logVolume();
}

void Mp3::decreaseVolume() {
  if (volume > settings.minVolume) {
    LOG(mp3_log, s_debug, F("setVolume: "), volume-1);
    Base::setVolume(--volume);
  }
  logVolume();
}

void Mp3::setVolume() {
  volume = settings.initVolume;
  LOG(mp3_log, s_debug, F("setVolume: "), volume);
  uint8_t max_loop = 20; // 4 seconds
  while((--max_loop>0) && (Base::getVolume() != volume)) {
    delay(100);
    Base::setVolume(volume);
    delay(100);
  }
  LOG(mp3_log, s_debug, F("setVolume loops: "), 20-max_loop);
  logVolume();
}

void Mp3::setVolume(uint8_t v) {
  volume = v;
  LOG(mp3_log, s_debug, F("setVolume: "), volume);
  Base::setVolume(volume);
  logVolume();
}

void Mp3::logVolume() {
  LOG(mp3_log, s_info, F("Volume: "), volume);
}

void Mp3::loop() {
#ifdef CHECK_MISSING_ONPLAYFINISHED
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
#endif
  if (playing == play_none && (current_folder != 0 || mp3_track != 0)) {
    playCurrent();
  }
  Base::loop();
}

#ifndef SRC_MP3_HPP_
#define SRC_MP3_HPP_

#include <Arduino.h>
//#define DfMiniMp3Debug Serial
#include <DFMiniMp3.h>

#include "constants.hpp"
#include "settings.hpp"
#include "queue.hpp"
#include "timer.hpp"

#ifndef DFPlayerUsesHardwareSerial // make sure to include "constants.hpp" before this line!
#include <SoftwareSerial.h>
using SerialType = SoftwareSerial;
#else
using SerialType = HardwareSerial;
#endif // DFPlayerUsesHardwareSerial

// forward declare the notify class, just the name
class Mp3Notify;

#ifdef DFMiniMp3_T_CHIP_MH2024K16SS
#define DFMiniMp3_T_CHIP_VARIANT Mp3ChipMH2024K16SS
#endif
#ifdef DFMiniMp3_T_CHIP_Mp3ChipIncongruousNoAck
#define DFMiniMp3_T_CHIP_VARIANT Mp3ChipIncongruousNoAck
#endif

#if defined(DFMiniMp3_T_CHIP_GD3200B) or defined(DFMiniMp3_T_CHIP_LISP3) or defined(DFMiniMp3_T_CHIP_MH2024K24SS_MP3_TF_16P_V3_0)
#define DFMiniMp3_IGNORE_ONPLAYFINISHED_FOR_ADV
#endif

// define a handy type using serial and our notify class
#ifdef DFMiniMp3_T_CHIP_VARIANT
using DfMp3 = DFMiniMp3<SerialType, Mp3Notify, DFMiniMp3_T_CHIP_VARIANT, 4000>;
#else
using DfMp3 = DFMiniMp3<SerialType, Mp3Notify, Mp3ChipOriginal         , 4000>;
#endif

enum class mp3Tracks: uint16_t {
  t_0                          =   0,
  t_262_pling                  = 262,
  t_300_new_tag                = 300,
  t_301_select_folder          = 301,
  t_310_select_mode            = 310,
  t_311_mode_random_episode    = 311,
  t_312_mode_album             = 312,
  t_313_mode_party             = 313,
  t_314_mode_single_track      = 314,
  t_315_mode_audio_book        = 315,
  t_316_admin                  = 316,
  t_317_special_random         = 317,
  t_318_special_album          = 318,
  t_319_special_party          = 319,
  t_320_mode_audio_book_single = 320,
  t_321_mode_repeat_last_card  = 321,
  t_322_mode_quiz_game         = 322,
  t_323_mode_memory_game       = 323,
  t_324_mode_switch_bt         = 324,
  t_327_select_file            = 327,
  t_328_select_first_file      = 328,
  t_329_select_last_file       = 329,
  t_330_select_say_number      = 330,
  t_331_do_not_say_number      = 331,
  t_332_say_number             = 332,
  t_333_num_answer             = 333,
  t_334_num_answer_2           = 334,
  t_335_num_answer_4           = 335,
  t_336_num_answer_2_1         = 336,
  t_337_num_answer_4_1         = 337,
  t_338_num_answer_0_1         = 338,
  t_340_num_tracks             = 340,
  t_400_ok                     = 400,
  t_401_error                  = 401,
  t_402_ok_settings            = 402,
  t_500_quiz_game_intro        = 500,
  t_501_quiz_game_ok           = 501,
  t_502_quiz_game_bad          = 502,
  t_503_quiz_game_ok_with_sol  = 503,
  t_504_quiz_game_bad_with_sol = 504,
  t_505_quiz_game_buzzer_down  = 505,
  t_506_quiz_game_buzzer_up    = 506,
  t_507_quiz_game_buzzer_vold  = 507,
  t_508_quiz_game_buzzer_volu  = 508,
  t_509_quiz_game_buzzer_intro = 509,
  t_510_quiz_game_continue     = 510,
  t_520_memory_game_intro      = 520,
  t_521_memory_game_ok         = 521,
  t_522_memory_game_bad        = 522,
  t_523_memory_game_1          = 523,
  t_524_memory_game_2          = 524,
  t_800_waiting_for_card       = 800,
  t_801_remove_card            = 801,
  t_802_reset_aborted          = 802,
  t_900_admin                  = 900,
  t_901_card_reset             = 901,
  t_902_max_volume             = 902,
  t_903_min_volume             = 903,
  t_904_init_volume            = 904,
  t_905_eq                     = 905,
  t_906_modifiers              = 906,
  t_907_shortcut               = 907,
  t_908_standbytimer           = 908,
  t_909_batch_cards            = 909,
  t_910_switch_volume          = 910,
  t_911_reset                  = 911,
  t_912_admin_lock             = 912,
  t_913_pause_on_card_removed  = 913,
  t_914_memory_game_cards      = 914,
  t_919_continue_admin         = 919,
  t_920_eq_intro               = 920,
  t_921_normal                 = 921,
  t_922_pop                    = 922,
  t_923_rock                   = 923,
  t_924_jazz                   = 924,
  t_925_classic                = 925,
  t_926_bass                   = 926,
  t_930_max_volume_intro       = 930,
  t_931_min_volume_into        = 931,
  t_932_init_volume_into       = 932,
  t_933_switch_volume_intro    = 933,
  t_934_no                     = 934,
  t_935_yes                    = 935,
  t_936_batch_cards_intro      = 936,
  t_937_memory_game_cards_intro= 937,
  t_938_modifier_sleep_mode    = 938,
  t_940_shortcut_into          = 940,
  t_941_pause                  = 941,
  t_942_up                     = 942,
  t_943_down                   = 943,
  t_944_startup                = 944,
  t_960_timer_intro            = 960,
  t_961_timer_5                = 961,
  t_962_timer_15               = 962,
  t_963_timer_30               = 963,
  t_964_timer_60               = 964,
  t_965_timer_disabled         = 965,
  t_966_dance_pause_intro      = 966,
  t_967_dance_pause_15_30      = 967,
  t_968_dance_pause_25_40      = 968,
  t_969_dance_pause_35_50      = 969,
  t_970_modifier_Intro         = 970,
  t_971_modifier_SleepTimer    = 971,
  t_972_modifier_FreezeDance   = 972,
  t_973_modifier_Locked        = 973,
  t_974_modifier_Toddler       = 974,
  t_975_modifier_KinderGarden  = 975,
  t_976_modifier_repeat1       = 976,
  t_977_modifier_bluetooth     = 977,
  t_978_modifier_jukebox       = 978,
  t_979_modifier_pauseAftTr    = 979,
  t_980_modifier_disableStandby= 980,
  t_985_admin_lock_intro       = 985,
  t_986_admin_lock_disabled    = 986,
  t_987_admin_lock_card        = 987,
  t_988_admin_lock_pin         = 988,
  t_989_admin_lock_calc        = 989,
  t_991_admin_pin              = 991,
  t_992_admin_calc             = 992,
  t_993_admin_calc             = 993,
  t_994_admin_calc             = 994,
  t_999_reset_ok               = 999,
};

inline mp3Tracks operator+(mp3Tracks lhs, uint16_t  rhs) { return static_cast<mp3Tracks>(static_cast<uint16_t>(lhs)+rhs); }

enum class advertTracks: uint16_t {
  t_260_activate_mod_card      = 260,
  t_261_deactivate_mod_card    = 261,
  t_262_pling                  = 262,
  t_300_freeze_into            = 300,
  t_301_freeze_freeze          = 301,
  t_302_sleep                  = 302,
  t_303_fi_wa_ai               = 303,
  t_304_buttonslocked          = 304,
  t_305_kindergarden           = 305,
  t_306_fire                   = 306,
  t_307_water                  = 307,
  t_308_air                    = 308,
  t_309_jukebox                = 309,
  t_320_bt_on                  = 320,
  t_321_bt_off                 = 321,
  t_322_bt_pairing             = 322,
  t_323_standby_timer_off      = 323,
  t_324_standby_timer_on       = 324,
};

// implement a notification class,
// its member methods will get called
//
class Mp3Notify {
public:
  static void OnError             (DfMp3&, uint16_t errorCode);
  static void OnPlayFinished      (DfMp3&, DfMp3_PlaySources source, uint16_t track);
  static void OnPlaySourceOnline  (DfMp3&, DfMp3_PlaySources source);
  static void OnPlaySourceInserted(DfMp3&, DfMp3_PlaySources source);
  static void OnPlaySourceRemoved (DfMp3&, DfMp3_PlaySources source);

  static void ResetLastTrackFinished() { lastTrackFinished = 0; }
private:
  static void PrintlnSourceAction (DfMp3_PlaySources source, const __FlashStringHelper* action);
  static uint16_t lastTrackFinished;
};

class Mp3: public DfMp3 {
public:
  using Base = DfMp3;

  Mp3(Settings& settings);

  void init();
  bool isPlaying() const { return is_playing_cache; }
  void waitForTrackToFinish();
  void waitForTrackToStart();
  void playAdvertisement(uint16_t     track, bool olnyIfIsPlaying = true);
  void playAdvertisement(advertTracks track, bool olnyIfIsPlaying = true);

  void clearFolderQueue();
  void clearMp3Queue();
  void clearAllQueue() { clearFolderQueue(); clearMp3Queue(); }
  bool isPlayingFolder() { return playing == play_folder; }
  bool isPlayingMp3   () { return playing == play_mp3   ; }
#ifdef DFMiniMp3_IGNORE_ONPLAYFINISHED_FOR_ADV
  bool resetPlayingAdv() { bool ret = advPlaying; advPlaying = false; return ret; }
#endif
  // firstTrack and lastTrack -> index in folder starting with 1
  // currentTrack             -> index in queue starting with 0
  void enqueueTrack(uint8_t folder, uint8_t firstTrack, uint8_t lastTrack, uint8_t currentTrack = 0);
  void enqueueTrack(uint8_t folder, uint8_t track);
  void setEndless(bool v = true) { endless = v; }
  void shuffleQueue();
  void enqueueMp3FolderTrack(uint16_t  track, bool playAfter = false);
  void enqueueMp3FolderTrack(mp3Tracks track, bool playAfter = false);
  void playCurrent();
  void playNext(uint8_t tracks, bool fromOnPlayFinished);
  void playPrevious(uint8_t tracks = 1);
  void jumpTo(uint8_t track);
  uint8_t getCurrentTrack() { return playing ? q.get(current_track) : 0; }
  uint16_t getFolderTrackCount(uint16_t folder);
  uint8_t getCurrentFolder() { return current_folder; }
  bool isLastTrack() { return current_track+1 >= q.size(); }

  void start() { if (isPause) { isPause = false; Base::start();} }
  void stop () { isPause = false; Base::stop (); }
  void pause() { isPause = true ; Base::pause(); }

  void increaseVolume();
  void decreaseVolume();
  bool setVolume     ();
  void setVolume     (uint8_t);
#ifdef NEO_RING_EXT
  uint8_t getVolumeRel() const { return static_cast<uint16_t>(*volume-*minVolume)*0xff/(*maxVolume-*minVolume); }
  bool volumeChanged () { return not volumeChangedTimer.isExpired(); }
#endif // NEO_RING_EXT
  void loop          ();

  uint8_t& getVolume    () { return *volume    ; }
  uint8_t& getMaxVolume () { return *maxVolume ; }
  uint8_t& getMinVolume () { return *minVolume ; }
  uint8_t& getInitVolume() { return *initVolume; }

#ifdef HPJACKDETECT
  void hpjackdetect         ();
  bool isHeadphoneJackDetect() { return noHeadphoneJackDetect == level::inactive; }
  void setTempSpkOn         () { tempSpkOn = 2; }
#endif

#ifdef TonUINO_Esp32
  String getQueue();
#endif

private:
  friend class tonuino_fixture;

  void logVolume();
  void refreshIsPlaying();

  typedef queue<uint8_t, maxTracksInFolder> track_queue;

#ifndef DFPlayerUsesHardwareSerial
  SoftwareSerial       softwareSerial;
#endif /* not DFPlayerUsesHardwareSerial */
  Settings&            settings;

  uint8_t              spkVolume{};
#ifdef HPJACKDETECT
  uint8_t              hpVolume{};
#endif
  uint8_t*             volume    {&spkVolume};
  uint8_t*             maxVolume {&settings.spkMaxVolume};
  uint8_t*             minVolume {&settings.spkMinVolume};
  uint8_t*             initVolume{&settings.spkInitVolume};
#ifdef NEO_RING_EXT
  Timer                volumeChangedTimer{};
#endif // NEO_RING_EXT

  // folder queue
  track_queue          q{};
  uint8_t              current_folder{};
  uint16_t             current_track{};
  bool                 endless{false};

  // mp3 queue
  uint16_t             mp3_track{};
  uint16_t             mp3_track_next{};

  bool                 is_playing_cache{};

  enum play_type: uint8_t {
    play_none,
    play_folder,
    play_mp3,
  };
  play_type            playing{play_none};
  Timer                startTrackTimer{};
  Timer                missingOnPlayFinishedTimer{};
  bool                 isPause{};
#ifdef DFMiniMp3_IGNORE_ONPLAYFINISHED_FOR_ADV
  bool                 advPlaying{false};
#endif

#ifdef HPJACKDETECT
  level                noHeadphoneJackDetect{level::unknown};
  uint8_t              tempSpkOn{};
#endif

};

#endif /* SRC_MP3_HPP_ */

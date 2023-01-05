
#include <gtest/gtest.h>

#include <tonuino.hpp>
#include <state_machine.hpp>
#include <chip_card.hpp>

class tonuino_test_fixture: public ::testing::Test {
public:
  tonuino_test_fixture()
  : tonuino(Tonuino::getTonuino())
  {
    tonuino.setup();
  }

  void execute_cycle() {
    tonuino.loop();
  }

  Mp3&       getMp3     () { return tonuino.getMp3     (); }
  Commands&  getCommands() { return tonuino.getCommands(); }
  Settings&  getSettings() { return tonuino.getSettings(); }
  Chip_card& getChipCard() { return tonuino.getChipCard(); }
  MFRC522&   getMFRC522 () { return getChipCard().mfrc522; }

  Tonuino& tonuino;
};


TEST_F(tonuino_test_fixture, initial_state) {
  EXPECT_TRUE(SM_tonuino::is_in_state<Idle>());
  EXPECT_EQ(pin_mode[shutdownPin], OUTPUT);
  EXPECT_EQ(pin_value[shutdownPin], HIGH);

#if defined ALLinONE || defined ALLinONE_Plus
  EXPECT_EQ(pin_mode[ampEnablePin], OUTPUT);
  EXPECT_EQ(pin_value[ampEnablePin], LOW);

  EXPECT_EQ(pin_mode[usbAccessPin], OUTPUT);
  EXPECT_EQ(pin_value[usbAccessPin], LOW);
#endif

  EXPECT_EQ(getSettings().cookie              ,  cardCookie);
  EXPECT_EQ(getSettings().version             , cardVersion);
  EXPECT_EQ(getSettings().maxVolume           ,          25);
  EXPECT_EQ(getSettings().minVolume           ,           5);
  EXPECT_EQ(getSettings().initVolume          ,          15);
  EXPECT_EQ(getSettings().eq                  ,           1);
  EXPECT_EQ(getSettings().locked              ,       false);
  EXPECT_EQ(getSettings().standbyTimer        ,           0);
  EXPECT_EQ(getSettings().invertVolumeButtons ,           1);
  EXPECT_EQ(getSettings().shortCuts[0].folder ,           0);
  EXPECT_EQ(getSettings().shortCuts[1].folder ,           0);
  EXPECT_EQ(getSettings().shortCuts[2].folder ,           0);
  EXPECT_EQ(getSettings().shortCuts[3].folder ,           0);
  EXPECT_EQ(getSettings().adminMenuLocked     ,           0);
  EXPECT_EQ(getSettings().adminMenuPin[0]     ,           1);
  EXPECT_EQ(getSettings().adminMenuPin[1]     ,           1);
  EXPECT_EQ(getSettings().adminMenuPin[2]     ,           1);
  EXPECT_EQ(getSettings().adminMenuPin[3]     ,           1);
  EXPECT_EQ(getSettings().pauseWhenCardRemoved,       false);

  EXPECT_TRUE(getMp3().called_begin);
  EXPECT_EQ(getMp3().current_volume, getSettings().initVolume);
  EXPECT_EQ(getMp3().current_eq    , getSettings().eq-1      );

  EXPECT_TRUE(getMFRC522().called_Init);


  //EXPECT_TRUE(false) << "log: " << Print::get_output();
}

TEST_F(tonuino_test_fixture, loop_takes_cycleTime) {
  unsigned long t = current_time;
  execute_cycle();
  EXPECT_EQ(current_time - t, cycleTime);
}

TEST_F(tonuino_test_fixture, sunny_day) {
  const uint8_t play_folder = 1;
  const uint8_t play_folder_track_count = 10;

  EXPECT_TRUE(SM_tonuino::is_in_state<Idle>());
  Print::clear_output();

  // set sd-card and insert card
  getMp3().set_folder_track_count(play_folder, play_folder_track_count);
  getMFRC522().card_in(cardCookie, cardVersion, 1, static_cast<uint8_t>(pmode_t::album), 0, 0);
  execute_cycle();
  EXPECT_TRUE(SM_tonuino::is_in_state<StartPlay>());

  // play t_262_pling
  execute_cycle();
  EXPECT_TRUE(SM_tonuino::is_in_state<StartPlay>());
  EXPECT_TRUE(getMp3().is_playing_mp3());
  EXPECT_EQ(getMp3().df_mp3_track, static_cast<uint16_t>(mp3Tracks::t_262_pling));

  // end t_262_pling
  getMp3().end_track();
  execute_cycle();
  EXPECT_TRUE(SM_tonuino::is_in_state<Play>());
  EXPECT_TRUE(getMp3().is_stopped());

  // play 1-1
  execute_cycle();
  EXPECT_TRUE(getMp3().is_playing_folder());
  EXPECT_EQ(getMp3().df_folder, play_folder);
  EXPECT_EQ(getMp3().df_folder_track, 1);

  // button pause
  press_button(buttonPausePin);
  execute_cycle();
  release_button(buttonPausePin);
  execute_cycle();
  EXPECT_TRUE(SM_tonuino::is_in_state<Pause>());
  execute_cycle();
  EXPECT_TRUE(getMp3().is_pause());

  // button play
  press_button(buttonPausePin);
  execute_cycle();
  release_button(buttonPausePin);
  execute_cycle();
  EXPECT_TRUE(SM_tonuino::is_in_state<Play>());
  execute_cycle();
  EXPECT_TRUE(getMp3().is_playing_folder());
  EXPECT_EQ(getMp3().df_folder, play_folder);
  EXPECT_EQ(getMp3().df_folder_track, 1);

  // button next
  press_button(buttonUpPin);
  execute_cycle();
//#ifndef FIVEBUTTONS
  current_time += buttonLongPress;
  execute_cycle();
  release_button(buttonUpPin);
  execute_cycle();
//#else
//  release_button(buttonUpPin);
//  execute_cycle();
//  execute_cycle();
//#endif
  execute_cycle();
  EXPECT_TRUE(getMp3().is_playing_folder());
  EXPECT_EQ(getMp3().df_folder, play_folder);
  EXPECT_EQ(getMp3().df_folder_track, 2);

  // button prev
  press_button(buttonDownPin);
  execute_cycle();
  //#ifndef FIVEBUTTONS
    current_time += buttonLongPress;
    execute_cycle();
    release_button(buttonDownPin);
    execute_cycle();
  //#else
  //  release_button(buttonDownPin);
  //  execute_cycle();
  //  execute_cycle();
  //#endif
  execute_cycle();
  EXPECT_TRUE(getMp3().is_playing_folder());
  EXPECT_EQ(getMp3().df_folder, play_folder);
  EXPECT_EQ(getMp3().df_folder_track, 1);

  // loop for all tracks
  for (uint8_t i = 1; i < play_folder_track_count; ++i) {
    getMp3().end_track();
    execute_cycle();
    EXPECT_TRUE(getMp3().is_pause());

    execute_cycle();
    EXPECT_TRUE(getMp3().is_playing_folder());
    EXPECT_EQ(getMp3().df_folder, play_folder);
    EXPECT_EQ(getMp3().df_folder_track, i+1);
  }

  // end --> Idle
  getMp3().end_track();
  execute_cycle();
  EXPECT_TRUE(getMp3().is_stopped());
  EXPECT_TRUE(SM_tonuino::is_in_state<Idle>());

//  EXPECT_TRUE(false) << "log: " << Print::get_output();
}

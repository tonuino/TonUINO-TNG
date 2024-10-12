
#include <gtest/gtest.h>

#include <tonuino.hpp>
#include <state_machine.hpp>
#include <chip_card.hpp>
#include <commands.hpp>

#include "tonuino_fixture.hpp"

class tonuino_test_fixture: public tonuino_fixture {

};

// =======================================================
// Test SleepTimer
// =======================================================

TEST_F(tonuino_test_fixture, SleepTimer_in_play) {

  goto_play({ 2, pmode_t::album, 0, 0 });
  card_out();
  Print::clear_output();

  const uint8_t timeout_minutes = 5;
  const uint32_t timeout_ms     = timeout_minutes * 60 * 1000;

  card_in({ 0, pmode_t::sleep_timer, 5, 0 });
  uint32_t start_time = current_time;

  EXPECT_EQ(getModifier().getActive(), pmode_t::sleep_timer);
  EXPECT_TRUE(getMp3().is_playing_adv());
  EXPECT_EQ(getMp3().df_adv_track, static_cast<uint16_t>(advertTracks::t_302_sleep));
  card_out();
  EXPECT_TRUE(SM_tonuino::is_in_state<Play>());

  execute_cycle_for_ms(1000);
  EXPECT_TRUE(SM_tonuino::is_in_state<Play>());

  execute_cycle_for_ms(timeout_ms - cycleTime - (current_time - start_time));
  EXPECT_TRUE(SM_tonuino::is_in_state<Play>());

  execute_cycle();
  EXPECT_TRUE(SM_tonuino::is_in_state<Pause>());

  execute_cycle_for_ms(1000);
  EXPECT_TRUE(SM_tonuino::is_in_state<Pause>());

  tonuino.resetActiveModifier();
  goto_idle();
  //EXPECT_TRUE(false) << "log: " << Print::get_output();
}

TEST_F(tonuino_test_fixture, SleepTimer_in_idle) {

  goto_idle();
  Print::clear_output();

  const uint8_t timeout_minutes = 5;
  const uint32_t timeout_ms     = timeout_minutes * 60 * 1000;

  card_in({ 0, pmode_t::sleep_timer, 5, 0 });
  uint32_t start_time = current_time;

  EXPECT_EQ(getModifier().getActive(), pmode_t::sleep_timer);
  //EXPECT_TRUE(getMp3().is_playing_adv()); --> already stopped in waitForTrackToFinish()
  //EXPECT_EQ(getMp3().df_adv_track, static_cast<uint16_t>(advertTracks::t_302_sleep));
  card_out();
  EXPECT_TRUE(SM_tonuino::is_in_state<Idle>());

  goto_play({ 2, pmode_t::album, 0, 0 });
  card_out();

  execute_cycle_for_ms(1000);
  EXPECT_TRUE(SM_tonuino::is_in_state<Play>());

  execute_cycle_for_ms(timeout_ms - cycleTime - (current_time - start_time));
  EXPECT_TRUE(SM_tonuino::is_in_state<Play>());

  execute_cycle();
  execute_cycle(); // Hmm, no idea, why it takes one cycle to long
  EXPECT_TRUE(SM_tonuino::is_in_state<Pause>());

  execute_cycle_for_ms(1000);
  EXPECT_TRUE(SM_tonuino::is_in_state<Pause>());

  tonuino.resetActiveModifier();
  goto_idle();
  //EXPECT_TRUE(false) << "log: " << Print::get_output();
}

// =======================================================
// Test FreezeDance
// =======================================================

constexpr uint8_t maxSecondsBetweenStops        = DanceGame::maxSecondsBetweenStops[0];
constexpr uint8_t addSecondsBetweenStopsFreezeD = DanceGame::addSecondsBetweenStopsFreezeD;
constexpr uint8_t addSecondsBetweenStopsFiWaAi  = DanceGame::addSecondsBetweenStopsFiWaAi;

TEST_F(tonuino_test_fixture, FreezeDance) {

  goto_play({ 2, pmode_t::album, 0, 0 });
  card_out();
  Print::clear_output();

  card_in({ 0, pmode_t::freeze_dance, 0, 0 });

  EXPECT_EQ(getModifier().getActive(), pmode_t::freeze_dance);
  EXPECT_TRUE(getMp3().is_playing_adv());
  EXPECT_EQ(getMp3().df_adv_track, static_cast<uint16_t>(advertTracks::t_300_freeze_into));
  card_out();
  EXPECT_TRUE(SM_tonuino::is_in_state<Play>());
  EXPECT_TRUE(getMp3().is_playing_folder());
  EXPECT_EQ(getMp3().df_folder, 2);
  EXPECT_EQ(getMp3().df_folder_track, 1);

  int loop_count = 0;
  while (getMp3().is_playing_folder() && ++loop_count < (maxSecondsBetweenStops+addSecondsBetweenStopsFreezeD) * 1000 / cycleTime) {
    execute_cycle();
  }
  EXPECT_TRUE(getMp3().is_playing_adv());
  EXPECT_EQ(getMp3().df_adv_track, static_cast<uint16_t>(advertTracks::t_301_freeze_freeze));

  execute_cycle();
  execute_cycle();
  execute_cycle();
  EXPECT_TRUE(SM_tonuino::is_in_state<Play>());
  EXPECT_TRUE(getMp3().is_playing_folder());
  EXPECT_EQ(getMp3().df_folder, 2);
  EXPECT_EQ(getMp3().df_folder_track, 1);

  card_in({ 0, pmode_t::freeze_dance, 0, 0 });
  card_out();

  EXPECT_EQ(getModifier().getActive(), pmode_t::none);

  goto_idle();
  //EXPECT_TRUE(false) << "log: " << Print::get_output();
}

// =======================================================
// Test Fire, Water, Air
// =======================================================

TEST_F(tonuino_test_fixture, FiWaAi) {

  goto_play({ 2, pmode_t::album, 0, 0 });
  card_out();
  Print::clear_output();

  card_in({ 0, pmode_t::fi_wa_ai, 0, 0 });

  EXPECT_EQ(getModifier().getActive(), pmode_t::fi_wa_ai);
  EXPECT_TRUE(getMp3().is_playing_adv());
  EXPECT_EQ(getMp3().df_adv_track, static_cast<uint16_t>(advertTracks::t_303_fi_wa_ai));
  card_out();
  EXPECT_TRUE(SM_tonuino::is_in_state<Play>());
  EXPECT_TRUE(getMp3().is_playing_folder());
  EXPECT_EQ(getMp3().df_folder, 2);
  EXPECT_EQ(getMp3().df_folder_track, 1);

  int loop_count = 0;
  while (getMp3().is_playing_folder() && ++loop_count < (maxSecondsBetweenStops+addSecondsBetweenStopsFiWaAi) * 1000 / cycleTime) {
    execute_cycle();
  }
  EXPECT_TRUE(getMp3().is_playing_adv());
  EXPECT_GE(getMp3().df_adv_track, static_cast<uint16_t>(advertTracks::t_306_fire));
  EXPECT_LE(getMp3().df_adv_track, static_cast<uint16_t>(advertTracks::t_308_air));

  execute_cycle();
  execute_cycle();
  execute_cycle();
  EXPECT_TRUE(SM_tonuino::is_in_state<Play>());
  EXPECT_TRUE(getMp3().is_playing_folder());
  EXPECT_EQ(getMp3().df_folder, 2);
  EXPECT_EQ(getMp3().df_folder_track, 1);

  card_in({ 0, pmode_t::fi_wa_ai, 0, 0 });
  card_out();

  EXPECT_EQ(getModifier().getActive(), pmode_t::none);

  goto_idle();
  //EXPECT_TRUE(false) << "log: " << Print::get_output();
}

// =======================================================
// Test ToddlerMode
// =======================================================

TEST_F(tonuino_test_fixture, ToddlerMode_in_idle) {

  goto_idle();
  card_out();
  Print::clear_output();

  card_in({ 0, pmode_t::toddler, 0, 0 });

  EXPECT_EQ(getModifier().getActive(), pmode_t::toddler);
//  EXPECT_TRUE(getMp3().is_playing_adv());
//  EXPECT_EQ(getMp3().df_adv_track, static_cast<uint16_t>(advertTracks::t_304_buttonslocked));
  card_out();
  EXPECT_TRUE(SM_tonuino::is_in_state<Idle>());
  EXPECT_TRUE(getMp3().is_stopped() || getMp3().is_pause());

  button_for_command(command::shortcut1, state_for_command::idle_pause);
  EXPECT_TRUE(SM_tonuino::is_in_state<Idle>());

  button_for_command(command::shortcut2, state_for_command::idle_pause);
  EXPECT_TRUE(SM_tonuino::is_in_state<Idle>());

  button_for_command(command::shortcut3, state_for_command::idle_pause);
  EXPECT_TRUE(SM_tonuino::is_in_state<Idle>());

  button_for_command(command::admin, state_for_command::idle_pause);
  EXPECT_TRUE(SM_tonuino::is_in_state<Idle>());

  card_in({ 3, pmode_t::einzel, 4, 0 });
  EXPECT_TRUE(SM_tonuino::is_in_state<StartPlay<Play>>());
  leave_start_play();
  EXPECT_TRUE(SM_tonuino::is_in_state<Play>());
  EXPECT_TRUE(getMp3().is_playing_folder());
  EXPECT_EQ(getMp3().df_folder, 3);
  EXPECT_EQ(getMp3().df_folder_track, 4);
  card_out();

  card_in({ 0, pmode_t::toddler, 0, 0 });
  card_out();

  EXPECT_EQ(getModifier().getActive(), pmode_t::none);

  goto_idle();
  //EXPECT_TRUE(false) << "log: " << Print::get_output();
}

TEST_F(tonuino_test_fixture, ToddlerMode_in_play) {

  goto_play({ 2, pmode_t::album, 0, 0 });
  card_out();
  Print::clear_output();

  card_in({ 0, pmode_t::toddler, 0, 0 });

  EXPECT_EQ(getModifier().getActive(), pmode_t::toddler);
  EXPECT_TRUE(getMp3().is_playing_adv());
  EXPECT_EQ(getMp3().df_adv_track, static_cast<uint16_t>(advertTracks::t_304_buttonslocked));
  card_out();
  EXPECT_TRUE(SM_tonuino::is_in_state<Play>());
  EXPECT_TRUE(getMp3().is_playing_folder());
  EXPECT_EQ(getMp3().df_folder, 2);
  EXPECT_EQ(getMp3().df_folder_track, 1);

  button_for_command(command::pause, state_for_command::play);
  EXPECT_TRUE(SM_tonuino::is_in_state<Play>());

  button_for_command(command::next, state_for_command::play);
  EXPECT_TRUE(SM_tonuino::is_in_state<Play>());
  EXPECT_TRUE(getMp3().is_playing_folder());
  EXPECT_EQ(getMp3().df_folder, 2);
  EXPECT_EQ(getMp3().df_folder_track, 1);

  button_for_command(command::previous, state_for_command::play);
  EXPECT_TRUE(SM_tonuino::is_in_state<Play>());
  EXPECT_TRUE(getMp3().is_playing_folder());
  EXPECT_EQ(getMp3().df_folder, 2);
  EXPECT_EQ(getMp3().df_folder_track, 1);

#ifdef FIVEBUTTONS
  button_for_command(command::next10, state_for_command::play);
  EXPECT_TRUE(SM_tonuino::is_in_state<Play>());
  EXPECT_TRUE(getMp3().is_playing_folder());
  EXPECT_EQ(getMp3().df_folder, 2);
  EXPECT_EQ(getMp3().df_folder_track, 1);

  button_for_command(command::previous10, state_for_command::play);
  EXPECT_TRUE(SM_tonuino::is_in_state<Play>());
  EXPECT_TRUE(getMp3().is_playing_folder());
  EXPECT_EQ(getMp3().df_folder, 2);
  EXPECT_EQ(getMp3().df_folder_track, 1);
#endif

  const uint8_t old_volume = getMp3().current_volume;

  button_for_command(command::volume_up, state_for_command::play);
  EXPECT_EQ(getMp3().current_volume, old_volume);

  button_for_command(command::volume_down, state_for_command::play);
  EXPECT_EQ(getMp3().current_volume, old_volume);

  button_for_command(command::admin, state_for_command::play);
  EXPECT_TRUE(SM_tonuino::is_in_state<Play>());

  card_in({ 3, pmode_t::einzel, 4, 0 });
  EXPECT_TRUE(SM_tonuino::is_in_state<StartPlay<Play>>());
  leave_start_play();
  EXPECT_TRUE(SM_tonuino::is_in_state<Play>());
  EXPECT_TRUE(getMp3().is_playing_folder());
  EXPECT_EQ(getMp3().df_folder, 3);
  EXPECT_EQ(getMp3().df_folder_track, 4);
  card_out();

  card_in({ 0, pmode_t::toddler, 0, 0 });
  card_out();

  EXPECT_EQ(getModifier().getActive(), pmode_t::none);

  goto_idle();
  //EXPECT_TRUE(false) << "log: " << Print::get_output();
}

// =======================================================
// Test KindergardenMode
// =======================================================

TEST_F(tonuino_test_fixture, KindergardenMode) {

  goto_play({ 2, pmode_t::album, 0, 0 });
  card_out();
  Print::clear_output();

  card_in({ 0, pmode_t::kindergarden, 0, 0 });

  EXPECT_EQ(getModifier().getActive(), pmode_t::kindergarden);
  EXPECT_TRUE(getMp3().is_playing_adv());
  EXPECT_EQ(getMp3().df_adv_track, static_cast<uint16_t>(advertTracks::t_305_kindergarden));
  card_out();
  EXPECT_TRUE(SM_tonuino::is_in_state<Play>());
  EXPECT_TRUE(getMp3().is_playing_folder());
  EXPECT_EQ(getMp3().df_folder, 2);
  EXPECT_EQ(getMp3().df_folder_track, 1);

  button_for_command(command::pause, state_for_command::play);
  EXPECT_TRUE(SM_tonuino::is_in_state<Pause>());

  button_for_command(command::pause, state_for_command::play);
  EXPECT_TRUE(SM_tonuino::is_in_state<Play>());

  button_for_command(command::next, state_for_command::play);
  EXPECT_TRUE(SM_tonuino::is_in_state<Play>());
  EXPECT_TRUE(getMp3().is_playing_folder());
  EXPECT_EQ(getMp3().df_folder, 2);
  EXPECT_EQ(getMp3().df_folder_track, 1);

  button_for_command(command::previous, state_for_command::play);
  EXPECT_TRUE(SM_tonuino::is_in_state<Play>());
  EXPECT_TRUE(getMp3().is_playing_folder());
  EXPECT_EQ(getMp3().df_folder, 2);
  EXPECT_EQ(getMp3().df_folder_track, 1);

#ifdef FIVEBUTTONS
  button_for_command(command::next10, state_for_command::play);
  EXPECT_TRUE(SM_tonuino::is_in_state<Play>());
  EXPECT_TRUE(getMp3().is_playing_folder());
  EXPECT_EQ(getMp3().df_folder, 2);
  EXPECT_EQ(getMp3().df_folder_track, 1);

  button_for_command(command::previous10, state_for_command::play);
  EXPECT_TRUE(SM_tonuino::is_in_state<Play>());
  EXPECT_TRUE(getMp3().is_playing_folder());
  EXPECT_EQ(getMp3().df_folder, 2);
  EXPECT_EQ(getMp3().df_folder_track, 1);
#endif

  const uint8_t old_volume = getMp3().current_volume;

  button_for_command(command::volume_up, state_for_command::play);
  EXPECT_EQ(getMp3().current_volume, old_volume);

  button_for_command(command::volume_down, state_for_command::play);
  EXPECT_EQ(getMp3().current_volume, old_volume);

  button_for_command(command::admin, state_for_command::play);
  EXPECT_TRUE(SM_tonuino::is_in_state<Play>());

  card_in({ 3, pmode_t::einzel, 4, 0 });
  EXPECT_TRUE(SM_tonuino::is_in_state<Play>());
  EXPECT_TRUE(getMp3().is_playing_folder());
  EXPECT_EQ(getMp3().df_folder, 2);
  EXPECT_EQ(getMp3().df_folder_track, 1);

  getMp3().end_track();
  execute_cycle();
  EXPECT_TRUE(SM_tonuino::is_in_state<Play>());
  EXPECT_TRUE(getMp3().is_playing_folder());
  EXPECT_EQ(getMp3().df_folder, 3);
  EXPECT_EQ(getMp3().df_folder_track, 4);
  card_out();

  card_in({ 0, pmode_t::kindergarden, 0, 0 });
  card_out();

  EXPECT_EQ(getModifier().getActive(), pmode_t::none);

  goto_idle();
  //EXPECT_TRUE(false) << "log: " << Print::get_output();
}

// =======================================================
// Test RepeatSingleModifier
// =======================================================

TEST_F(tonuino_test_fixture, RepeatSingleModifier) {

  goto_play({ 2, pmode_t::album, 0, 0 });
  card_out();
  Print::clear_output();

  card_in({ 0, pmode_t::repeat_single, 0, 0 });

  EXPECT_EQ(getModifier().getActive(), pmode_t::repeat_single);
  EXPECT_TRUE(getMp3().is_playing_adv());
  EXPECT_EQ(getMp3().df_adv_track, static_cast<uint16_t>(advertTracks::t_260_activate_mod_card));
  card_out();
  EXPECT_TRUE(SM_tonuino::is_in_state<Play>());
  EXPECT_TRUE(getMp3().is_playing_folder());
  EXPECT_EQ(getMp3().df_folder, 2);
  EXPECT_EQ(getMp3().df_folder_track, 1);

  button_for_command(command::next, state_for_command::play);
  EXPECT_TRUE(SM_tonuino::is_in_state<Play>());
  EXPECT_TRUE(getMp3().is_playing_folder());
  EXPECT_EQ(getMp3().df_folder, 2);
  EXPECT_EQ(getMp3().df_folder_track, 1);

  button_for_command(command::previous, state_for_command::play);
  EXPECT_TRUE(SM_tonuino::is_in_state<Play>());
  EXPECT_TRUE(getMp3().is_playing_folder());
  EXPECT_EQ(getMp3().df_folder, 2);
  EXPECT_EQ(getMp3().df_folder_track, 1);

#ifdef FIVEBUTTONS
  button_for_command(command::next10, state_for_command::play);
  EXPECT_TRUE(SM_tonuino::is_in_state<Play>());
  EXPECT_TRUE(getMp3().is_playing_folder());
  EXPECT_EQ(getMp3().df_folder, 2);
  EXPECT_EQ(getMp3().df_folder_track, 1);

  button_for_command(command::previous10, state_for_command::play);
  EXPECT_TRUE(SM_tonuino::is_in_state<Play>());
  EXPECT_TRUE(getMp3().is_playing_folder());
  EXPECT_EQ(getMp3().df_folder, 2);
  EXPECT_EQ(getMp3().df_folder_track, 1);
#endif

  getMp3().end_track();
  execute_cycle();
  execute_cycle();
  EXPECT_TRUE(SM_tonuino::is_in_state<Play>());
  EXPECT_TRUE(getMp3().is_playing_folder());
  EXPECT_EQ(getMp3().df_folder, 2);
  EXPECT_EQ(getMp3().df_folder_track, 1);
  card_out();

  card_in({ 0, pmode_t::repeat_single, 0, 0 });
  card_out();

  EXPECT_EQ(getModifier().getActive(), pmode_t::none);

  goto_idle();
  //EXPECT_TRUE(false) << "log: " << Print::get_output();
}


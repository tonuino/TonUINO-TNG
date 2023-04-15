
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

  goto_idle();
  //EXPECT_TRUE(false) << "log: " << Print::get_output();
}

// =======================================================
// Test FreezeDance
// =======================================================

constexpr uint8_t maxSecondsBetweenStops = 30;

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
  while (getMp3().is_playing_folder() && ++loop_count < maxSecondsBetweenStops * 1000 / cycleTime) {
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


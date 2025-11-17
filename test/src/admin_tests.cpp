
#include <gtest/gtest.h>

#include <tonuino.hpp>
#include <state_machine.hpp>
#include <chip_card.hpp>
#include <commands.hpp>

#include "tonuino_fixture.hpp"

class admin_test_fixture: public tonuino_fixture {
public:
  enum class Admin_Entry_menu_items: uint16_t {
    Admin_NewCard                  = 1,
    Admin_SimpleSetting_maxVolume  = 2,
    Admin_SimpleSetting_minVolume  = 3,
    Admin_SimpleSetting_initVolume = 4,
    Admin_SimpleSetting_eq         = 5,
    Admin_ModCard                  = 6,
    Admin_ShortCut                 = 7,
    Admin_StandbyTimer             = 8,
    Admin_CardsForFolder           = 9,
    Admin_InvButtons               = 10,
    Admin_ResetEeprom              = 11,
    Admin_LockAdmin                = 12,
    Admin_PauseIfCardRemoved       = 13,
  };

  mp3Tracks get_intro_track(Admin_Entry_menu_items menu_item) {
    switch (menu_item) {
    case Admin_Entry_menu_items::Admin_NewCard                 : return mp3Tracks::t_310_select_mode;
    case Admin_Entry_menu_items::Admin_SimpleSetting_maxVolume : return mp3Tracks::t_930_max_volume_intro;
    case Admin_Entry_menu_items::Admin_SimpleSetting_minVolume : return mp3Tracks::t_931_min_volume_into;
    case Admin_Entry_menu_items::Admin_SimpleSetting_initVolume: return mp3Tracks::t_932_init_volume_into;
    case Admin_Entry_menu_items::Admin_SimpleSetting_eq        : return mp3Tracks::t_920_eq_intro;
    case Admin_Entry_menu_items::Admin_ModCard                 : return mp3Tracks::t_970_modifier_Intro;
    case Admin_Entry_menu_items::Admin_ShortCut                : return mp3Tracks::t_940_shortcut_into;
    case Admin_Entry_menu_items::Admin_StandbyTimer            : return mp3Tracks::t_960_timer_intro;
    case Admin_Entry_menu_items::Admin_CardsForFolder          : return mp3Tracks::t_301_select_folder;
    case Admin_Entry_menu_items::Admin_InvButtons              : return mp3Tracks::t_933_switch_volume_intro;
    case Admin_Entry_menu_items::Admin_ResetEeprom             : return mp3Tracks::t_999_reset_ok;
    case Admin_Entry_menu_items::Admin_LockAdmin               : return mp3Tracks::t_985_admin_lock_intro;
    case Admin_Entry_menu_items::Admin_PauseIfCardRemoved      : return mp3Tracks::t_913_pause_on_card_removed;
    }
    return mp3Tracks::t_0;
  }

  void check_state(Admin_Entry_menu_items menu_item) {
    switch (menu_item) {
    case Admin_Entry_menu_items::Admin_NewCard                 : EXPECT_TRUE(SM_tonuino::is_in_state<Admin_NewCard           >()); break;
    case Admin_Entry_menu_items::Admin_SimpleSetting_maxVolume : EXPECT_TRUE(SM_tonuino::is_in_state<Admin_SimpleSetting     >()); break;
    case Admin_Entry_menu_items::Admin_SimpleSetting_minVolume : EXPECT_TRUE(SM_tonuino::is_in_state<Admin_SimpleSetting     >()); break;
    case Admin_Entry_menu_items::Admin_SimpleSetting_initVolume: EXPECT_TRUE(SM_tonuino::is_in_state<Admin_SimpleSetting     >()); break;
    case Admin_Entry_menu_items::Admin_SimpleSetting_eq        : EXPECT_TRUE(SM_tonuino::is_in_state<Admin_SimpleSetting     >()); break;
    case Admin_Entry_menu_items::Admin_ModCard                 : EXPECT_TRUE(SM_tonuino::is_in_state<Admin_ModCard           >()); break;
    case Admin_Entry_menu_items::Admin_ShortCut                : EXPECT_TRUE(SM_tonuino::is_in_state<Admin_ShortCut          >()); break;
    case Admin_Entry_menu_items::Admin_StandbyTimer            : EXPECT_TRUE(SM_tonuino::is_in_state<Admin_StandbyTimer      >()); break;
    case Admin_Entry_menu_items::Admin_CardsForFolder          : EXPECT_TRUE(SM_tonuino::is_in_state<Admin_CardsForFolder    >()); break;
    case Admin_Entry_menu_items::Admin_InvButtons              : EXPECT_TRUE(SM_tonuino::is_in_state<Admin_InvButtons        >()); break;
    case Admin_Entry_menu_items::Admin_ResetEeprom             : EXPECT_TRUE(SM_tonuino::is_in_state<Admin_ResetEeprom       >()); break;
    case Admin_Entry_menu_items::Admin_LockAdmin               : EXPECT_TRUE(SM_tonuino::is_in_state<Admin_LockAdmin         >()); break;
    case Admin_Entry_menu_items::Admin_PauseIfCardRemoved      : EXPECT_TRUE(SM_tonuino::is_in_state<Admin_PauseIfCardRemoved>()); break;
    }
  }
  void goto_Admin_Entry() {
    goto_idle();

    // button admin
    button_for_command(command::admin, state_for_command::idle_pause);
    EXPECT_TRUE(SM_tonuino::is_in_state<Admin_Allow>());
    execute_cycle(); // select_method
    execute_cycle(); // allow
    EXPECT_TRUE(SM_tonuino::is_in_state<Admin_Entry>());

    // play t_900_admin
    execute_cycle_for_ms(time_check_play);
    EXPECT_TRUE(SM_tonuino::is_in_state<Admin_Entry>());
    EXPECT_TRUE(getMp3().is_playing_mp3());
    EXPECT_EQ(getMp3().df_mp3_track, static_cast<uint16_t>(mp3Tracks::t_900_admin));

    // end t_900_admin
    getMp3().end_track();
    execute_cycle();
    EXPECT_TRUE(SM_tonuino::is_in_state<Admin_Entry>());
    EXPECT_TRUE(getMp3().is_stopped());
  }

  void goto_Admin_Entry_menu_item(Admin_Entry_menu_items menu_item) {
    goto_Admin_Entry();

    uint16_t start_track = static_cast<uint16_t>(mp3Tracks::t_900_admin)+1;
    uint16_t menu_item_track   = static_cast<uint16_t>(mp3Tracks::t_900_admin)+static_cast<uint16_t>(menu_item);
    for (uint16_t track = start_track; track <= menu_item_track; ++track) {
      // button up --> play t_900++
      button_for_command(command::next, state_for_command::admin);
      EXPECT_TRUE(SM_tonuino::is_in_state<Admin_Entry>());
      execute_cycle_for_ms(time_check_play);
      EXPECT_TRUE(getMp3().is_playing_mp3());
      EXPECT_EQ(getMp3().df_mp3_track, track);
    }

    // button select --> play intro
    button_for_command(command::select, state_for_command::admin);
    check_state(menu_item);
    execute_cycle_for_ms(time_check_play);
    EXPECT_TRUE(getMp3().is_playing_mp3());
    EXPECT_EQ(getMp3().df_mp3_track, static_cast<uint16_t>(get_intro_track(menu_item)));

    // end intro
    getMp3().end_track();
    execute_cycle();
    if (menu_item == Admin_Entry_menu_items::Admin_ResetEeprom) {
      EXPECT_TRUE(SM_tonuino::is_in_state<Admin_Entry>());
      // end t_919_continue_admin
      getMp3().end_track();
      execute_cycle();
    }
    else
      check_state(menu_item);
    EXPECT_TRUE(getMp3().is_stopped());
    EXPECT_EQ(getMp3().df_mp3_track, 0);
  }

  void admin_setup_card(folderSettings card) {

    ASSERT_TRUE(SM_setupCard::is_in_state<ChMode>());

    // ===== select mode
    uint16_t start_track = static_cast<uint16_t>(mp3Tracks::t_310_select_mode)+1;
    uint16_t mode_track  = static_cast<uint16_t>(mp3Tracks::t_310_select_mode)+static_cast<uint16_t>(card.mode);
    for (uint16_t track = start_track; track <= mode_track; ++track) {
      // button up --> play t_310++
      button_for_command(command::next, state_for_command::admin);
      EXPECT_TRUE(SM_setupCard::is_in_state<ChMode>());
      execute_cycle_for_ms(time_check_play);
      EXPECT_TRUE(getMp3().is_playing_mp3());
      EXPECT_EQ(getMp3().df_mp3_track, track);
    }

    // button select --> select mode
    button_for_command(command::select, state_for_command::admin);
    if (card.mode == pmode_t::admin || card.mode == pmode_t::repeat_last) {
      EXPECT_TRUE(SM_setupCard::is_in_state<finished_setupCard>());
      ASSERT_EQ(SM_setupCard::folder.mode  , (card.mode == pmode_t::repeat_last)? card.mode : pmode_t::admin_card);
      ASSERT_EQ(SM_setupCard::folder.folder, (card.mode == pmode_t::repeat_last)? 0xff : 0);
      return;
    }
    EXPECT_TRUE(SM_setupCard::is_in_state<ChFolder>());
    execute_cycle_for_ms(time_check_play);
    EXPECT_TRUE(getMp3().is_playing_mp3());
    EXPECT_EQ(getMp3().df_mp3_track, static_cast<uint16_t>(mp3Tracks::t_301_select_folder));

    // ===== select folder
    for (uint8_t folder = 1; folder <= card.folder; ++folder) {
      // button up --> play folder number and track
      button_for_command(command::next, state_for_command::admin);
      EXPECT_TRUE(SM_setupCard::is_in_state<ChFolder>());
      execute_cycle_for_ms(time_check_play);
      EXPECT_TRUE(getMp3().is_playing_mp3());
      EXPECT_EQ(getMp3().df_mp3_track, folder);
      getMp3().end_track();
      current_time += 1001;
      execute_cycle(); // end mp3 track
      execute_cycle(); // enqueue folder track
      execute_cycle_for_ms(time_check_play);
      EXPECT_TRUE(getMp3().is_playing_folder());
      EXPECT_EQ(getMp3().df_folder, folder);
      EXPECT_EQ(getMp3().df_folder_track, 1);
    }
    // button select --> select folder
    button_for_command(command::select, state_for_command::admin);
    if (card.mode == pmode_t::hoerspiel || card.mode == pmode_t::album    ||
        card.mode == pmode_t::party     || card.mode == pmode_t::hoerbuch   ) {
      EXPECT_TRUE(SM_setupCard::is_in_state<finished_setupCard>());
      ASSERT_EQ(SM_setupCard::folder, card);
      return;
    }
    if (card.mode == pmode_t::einzel)
      EXPECT_TRUE(SM_setupCard::is_in_state<ChTrack>());
    else if (card.mode == pmode_t::hoerbuch_1)
      EXPECT_TRUE(SM_setupCard::is_in_state<ChNumTracks>());
    else
      EXPECT_TRUE(SM_setupCard::is_in_state<ChFirstTrack>());
    execute_cycle_for_ms(time_check_play);
    EXPECT_TRUE(getMp3().is_playing_mp3());
    if (card.mode == pmode_t::einzel)
      EXPECT_EQ(getMp3().df_mp3_track, static_cast<uint16_t>(mp3Tracks::t_327_select_file));
    else if (card.mode == pmode_t::hoerbuch_1)
      EXPECT_EQ(getMp3().df_mp3_track, static_cast<uint16_t>(mp3Tracks::t_340_num_tracks));
    else
      EXPECT_EQ(getMp3().df_mp3_track, static_cast<uint16_t>(mp3Tracks::t_328_select_first_file));

    if (card.mode == pmode_t::hoerbuch_1) {
      // ===== select number of tracks
      for (uint8_t num = 1; num <= 5; ++num) {
        // button up --> play track number and track
        button_for_command(command::next, state_for_command::admin);
        EXPECT_TRUE(SM_setupCard::is_in_state<ChNumTracks>());
        execute_cycle_for_ms(time_check_play);
        EXPECT_TRUE(getMp3().is_playing_mp3());
        EXPECT_EQ(getMp3().df_mp3_track, num);
        getMp3().end_track();
      }
      // button select --> finish
      button_for_command(command::select, state_for_command::admin);
      EXPECT_TRUE(SM_setupCard::is_in_state<finished_setupCard>());
      EXPECT_EQ(SM_setupCard::folder.special, 5-1);
      return;
    }

    // ===== select track/first track
    for (uint8_t track = 1; track <= card.special; ++track) {
      // button up --> play track number and track
      button_for_command(command::next, state_for_command::admin);
      if (card.mode == pmode_t::einzel)
        EXPECT_TRUE(SM_setupCard::is_in_state<ChTrack>());
      else
        EXPECT_TRUE(SM_setupCard::is_in_state<ChFirstTrack>());
      execute_cycle_for_ms(time_check_play);
      EXPECT_TRUE(getMp3().is_playing_mp3());
      EXPECT_EQ(getMp3().df_mp3_track, track);
      getMp3().end_track();
      current_time += 1001;
      execute_cycle(); // end mp3 track
      execute_cycle(); // enqueue folder track
      execute_cycle_for_ms(time_check_play);
      EXPECT_TRUE(getMp3().is_playing_folder());
      EXPECT_EQ(getMp3().df_folder, card.folder);
      EXPECT_EQ(getMp3().df_folder_track, track);
    }
    // button select --> select last track
    button_for_command(command::select, state_for_command::admin);
    if (card.mode == pmode_t::einzel) {
      EXPECT_TRUE(SM_setupCard::is_in_state<finished_setupCard>());
      ASSERT_EQ(SM_setupCard::folder, card);
      return;
    }
    EXPECT_TRUE(SM_setupCard::is_in_state<ChLastTrack>());

    // ===== select last track
    for (uint8_t track = card.special+1; track <= card.special2; ++track) {
      // button up --> play track number and track
      button_for_command(command::next, state_for_command::admin);
      EXPECT_TRUE(SM_setupCard::is_in_state<ChLastTrack>());
      execute_cycle_for_ms(time_check_play);
      EXPECT_TRUE(getMp3().is_playing_mp3());
      EXPECT_EQ(getMp3().df_mp3_track, track);
      getMp3().end_track();
      current_time += 1001;
      execute_cycle(); // end mp3 track
      execute_cycle(); // enqueue folder track
      execute_cycle_for_ms(time_check_play);
      EXPECT_TRUE(getMp3().is_playing_folder());
      EXPECT_EQ(getMp3().df_folder, card.folder);
      EXPECT_EQ(getMp3().df_folder_track, track);
    }
    // button select --> select last track
    button_for_command(command::select, state_for_command::admin);
    EXPECT_TRUE(SM_setupCard::is_in_state<finished_setupCard>());

    ASSERT_EQ(SM_setupCard::folder, card);
  }

  void write_card(bool with_card_already_in = false) {

    ASSERT_TRUE(SM_writeCard::is_in_state<WriteCard>());

    execute_cycle(); // --> run_writeCard (in SM_tonuino)
    if (not with_card_already_in) {
      execute_cycle_for_ms(time_check_play);
      EXPECT_TRUE(getMp3().is_playing_mp3());
      EXPECT_EQ(getMp3().df_mp3_track, static_cast<uint16_t>(mp3Tracks::t_800_waiting_for_card));

      execute_cycle(); // --> run_writeCard (in SM_writeCard)

      card_in({ 1, pmode_t::album       , 0, 0 });
    }
    execute_cycle(); // --> end_writeCard
    execute_cycle_for_ms(time_check_play);
    EXPECT_TRUE(getMp3().is_playing_mp3());
    EXPECT_EQ(getMp3().df_mp3_track, static_cast<uint16_t>(mp3Tracks::t_400_ok));

    card_out();
    execute_cycle_for_ms(dfPlayer_timeUntilStarts); // --> run_waitCardRemoved
    getMp3().end_track();
    execute_cycle();
    execute_cycle();
    execute_cycle();
    execute_cycle();

    EXPECT_TRUE(SM_writeCard::is_in_state<finished_writeCard>());

  }
};

// =======================================================
// Test sunny day scenarios
// =======================================================

TEST_F(admin_test_fixture, sunny_day_adm) {

  EXPECT_TRUE(SM_tonuino::is_in_state<Idle>());
  Print::clear_output();
  goto_idle();

  // button admin
  button_for_command(command::admin, state_for_command::idle_pause);
  EXPECT_TRUE(SM_tonuino::is_in_state<Admin_Allow>());
  execute_cycle(); // select_method
  execute_cycle(); // allow
  EXPECT_TRUE(SM_tonuino::is_in_state<Admin_Entry>());

  // play t_900_admin
  execute_cycle();
  EXPECT_TRUE(SM_tonuino::is_in_state<Admin_Entry>());
  execute_cycle_for_ms(time_check_play);
  EXPECT_TRUE(getMp3().is_playing_mp3());
  EXPECT_EQ(getMp3().df_mp3_track, static_cast<uint16_t>(mp3Tracks::t_900_admin));

  // end t_900_admin
  getMp3().end_track();
  execute_cycle();
  EXPECT_TRUE(SM_tonuino::is_in_state<Admin_Entry>());
  EXPECT_TRUE(getMp3().is_stopped());

  uint16_t start_track = static_cast<uint16_t>(mp3Tracks::t_900_admin)+1;
  uint16_t end_track   = static_cast<uint16_t>(mp3Tracks::t_900_admin)+1+13;
  for (uint16_t track = start_track; track < end_track; ++track) {
    // button up --> play t_900++
    button_for_command(command::next, state_for_command::admin);
    EXPECT_TRUE(SM_tonuino::is_in_state<Admin_Entry>());
    execute_cycle_for_ms(time_check_play);
    EXPECT_TRUE(getMp3().is_playing_mp3());
    EXPECT_EQ(getMp3().df_mp3_track, track);

    // end t_900++
    getMp3().end_track();
    execute_cycle();
    EXPECT_TRUE(SM_tonuino::is_in_state<Admin_Entry>());
    EXPECT_TRUE(getMp3().is_stopped());
  }
  // button down --> play t_912_admin_lock
  button_for_command(command::previous, state_for_command::admin);
  EXPECT_TRUE(SM_tonuino::is_in_state<Admin_Entry>());
  execute_cycle_for_ms(time_check_play);
  EXPECT_TRUE(getMp3().is_playing_mp3());
  EXPECT_EQ(getMp3().df_mp3_track, static_cast<uint16_t>(mp3Tracks::t_912_admin_lock));

  // end t_912_admin_lock
  getMp3().end_track();
  execute_cycle();
  EXPECT_TRUE(SM_tonuino::is_in_state<Admin_Entry>());
  EXPECT_TRUE(getMp3().is_stopped());

  // button select --> play t_980_admin_lock_intro
  button_for_command(command::select, state_for_command::admin);
  EXPECT_TRUE(SM_tonuino::is_in_state<Admin_LockAdmin>());
  execute_cycle_for_ms(time_check_play);
  EXPECT_TRUE(getMp3().is_playing_mp3());
  EXPECT_EQ(getMp3().df_mp3_track, static_cast<uint16_t>(mp3Tracks::t_985_admin_lock_intro));

  // end t_980_admin_lock_intro
  getMp3().end_track();
  execute_cycle();
  EXPECT_TRUE(SM_tonuino::is_in_state<Admin_LockAdmin>());
  EXPECT_TRUE(getMp3().is_stopped());

  // button up --> play t_981_admin_lock_disabled
  button_for_command(command::next, state_for_command::admin);
  EXPECT_TRUE(SM_tonuino::is_in_state<Admin_LockAdmin>());
  execute_cycle_for_ms(time_check_play);
  EXPECT_TRUE(getMp3().is_playing_mp3());
  EXPECT_EQ(getMp3().df_mp3_track, static_cast<uint16_t>(mp3Tracks::t_986_admin_lock_disabled));

  // end t_981_admin_lock_disabled
  getMp3().end_track();
  execute_cycle();
  EXPECT_TRUE(SM_tonuino::is_in_state<Admin_LockAdmin>());
  EXPECT_TRUE(getMp3().is_stopped());

  // button select --> play t_402_ok_settings
  button_for_command(command::select, state_for_command::admin);
  EXPECT_TRUE(SM_tonuino::is_in_state<Admin_LockAdmin>());
  execute_cycle_for_ms(time_check_play);
  EXPECT_TRUE(getMp3().is_playing_mp3());
  EXPECT_EQ(getMp3().df_mp3_track, static_cast<uint16_t>(mp3Tracks::t_402_ok_settings));

  // end t_402_ok_settings
  getMp3().end_track();
  execute_cycle();
  execute_cycle();
  EXPECT_TRUE(SM_tonuino::is_in_state<Admin_Entry>());
  execute_cycle_for_ms(time_check_play);
  EXPECT_TRUE(getMp3().is_playing_mp3());
  EXPECT_EQ(getMp3().df_mp3_track, static_cast<uint16_t>(mp3Tracks::t_919_continue_admin));

  // end t_919_continue_admin
  getMp3().end_track();
  execute_cycle();
  EXPECT_TRUE(SM_tonuino::is_in_state<Admin_Entry>());
  EXPECT_TRUE(getMp3().is_stopped());

  // button up --> play t_913_pause_on_card_removed
  button_for_command(command::next, state_for_command::admin);
  EXPECT_TRUE(SM_tonuino::is_in_state<Admin_Entry>());
  execute_cycle_for_ms(time_check_play);
  EXPECT_TRUE(getMp3().is_playing_mp3());
  EXPECT_EQ(getMp3().df_mp3_track, static_cast<uint16_t>(mp3Tracks::t_913_pause_on_card_removed));

  // end t_913_pause_on_card_removed
  getMp3().end_track();
  execute_cycle();
  EXPECT_TRUE(SM_tonuino::is_in_state<Admin_Entry>());
  EXPECT_TRUE(getMp3().is_stopped());

  // button admin_end --> play t_802_reset_aborted
  button_for_command(command::adm_end, state_for_command::admin);
  execute_cycle();
  EXPECT_TRUE(SM_tonuino::is_in_state<Idle>());
  execute_cycle_for_ms(time_check_play);
  EXPECT_TRUE(getMp3().is_playing_mp3());
  EXPECT_EQ(getMp3().df_mp3_track, static_cast<uint16_t>(mp3Tracks::t_802_reset_aborted));

  // end t_802_reset_aborted
  getMp3().end_track();
  execute_cycle();
  EXPECT_TRUE(SM_tonuino::is_in_state<Idle>());
  EXPECT_TRUE(getMp3().is_stopped());
}

// =======================================================
// Test Admin_Entry
// =======================================================

TEST_F(admin_test_fixture, all_in_Admin_Entry) {

  Print::clear_output();

  goto_Admin_Entry_menu_item(Admin_Entry_menu_items::Admin_NewCard                 );
  goto_Admin_Entry_menu_item(Admin_Entry_menu_items::Admin_SimpleSetting_maxVolume );
  goto_Admin_Entry_menu_item(Admin_Entry_menu_items::Admin_SimpleSetting_minVolume );
  goto_Admin_Entry_menu_item(Admin_Entry_menu_items::Admin_SimpleSetting_initVolume);
  goto_Admin_Entry_menu_item(Admin_Entry_menu_items::Admin_SimpleSetting_eq        );
  goto_Admin_Entry_menu_item(Admin_Entry_menu_items::Admin_ModCard                 );
  goto_Admin_Entry_menu_item(Admin_Entry_menu_items::Admin_ShortCut                );
  goto_Admin_Entry_menu_item(Admin_Entry_menu_items::Admin_StandbyTimer            );
  goto_Admin_Entry_menu_item(Admin_Entry_menu_items::Admin_CardsForFolder          );
  goto_Admin_Entry_menu_item(Admin_Entry_menu_items::Admin_InvButtons              );
  goto_Admin_Entry_menu_item(Admin_Entry_menu_items::Admin_ResetEeprom             );
  goto_Admin_Entry_menu_item(Admin_Entry_menu_items::Admin_LockAdmin               );
  goto_Admin_Entry_menu_item(Admin_Entry_menu_items::Admin_PauseIfCardRemoved      );

//  EXPECT_TRUE(false) << "log: " << Print::get_output();

}

// =======================================================
// Test Admin_NewCard
// =======================================================

TEST_F(admin_test_fixture, Admin_NewCard) {

  folderSettings cards[] = {
      { 0, pmode_t::admin       , 0, 0 },
      { 0, pmode_t::repeat_last , 0, 0 },
      { 1, pmode_t::hoerspiel   , 0, 0 },
      { 2, pmode_t::album       , 0, 0 },
      { 3, pmode_t::party       , 0, 0 },
      { 4, pmode_t::einzel      , 1, 0 },
      { 5, pmode_t::hoerbuch    , 0, 0 },
      { 6, pmode_t::hoerspiel_vb, 1, 2 },
      { 7, pmode_t::album_vb    , 2, 3 },
      { 8, pmode_t::party_vb    , 4, 5 },
      { 9, pmode_t::hoerbuch_1  , 0, 0 }
  };

  Print::clear_output();

  for (const folderSettings& card: cards) {

    getMp3().set_folder_track_count(card.folder, 10);

    goto_Admin_Entry_menu_item(Admin_Entry_menu_items::Admin_NewCard);
    ASSERT_TRUE(SM_tonuino::is_in_state<Admin_NewCard>());

    admin_setup_card(card);
    execute_cycle(); // --> end_setupCard
    execute_cycle(); // --> start_writeCard
    execute_cycle(); // --> run_writeCard
    write_card();

    folderSettings card_expected = card;
    if (card.mode == pmode_t::admin)
      card_expected.mode = pmode_t::admin_card;
    if (card.mode == pmode_t::repeat_last)
      card_expected.folder = 0xff;

    EXPECT_EQ(card_expected, card_decode());

    goto_idle();

  }
//  EXPECT_TRUE(false) << "log: " << Print::get_output();

}

// =======================================================
// Test Admin_SimpleSetting_maxVolume
// =======================================================

TEST_F(admin_test_fixture, Admin_SimpleSetting_maxVolume) {

  Print::clear_output();

  getSettings().spkMaxVolume = 10;

  const uint8_t maxVolume = 21;

  goto_Admin_Entry_menu_item(Admin_Entry_menu_items::Admin_SimpleSetting_maxVolume);
  ASSERT_TRUE(SM_tonuino::is_in_state<Admin_SimpleSetting>());

  // ===== select volume
  for (uint8_t volume = getSettings().spkMaxVolume+1; volume <= maxVolume; ++volume) {
    // button up --> play number
    button_for_command(command::next, state_for_command::admin);
    execute_cycle_for_ms(time_check_play);
    EXPECT_TRUE(getMp3().is_playing_mp3());
    EXPECT_EQ(getMp3().df_mp3_track, volume);
  }
  // button select --> select volume
  button_for_command(command::select, state_for_command::admin);
  EXPECT_TRUE(SM_tonuino::is_in_state<Admin_Entry>());
  execute_cycle_for_ms(time_check_play);
  EXPECT_TRUE(getMp3().is_playing_mp3());
  EXPECT_EQ(getMp3().df_mp3_track, static_cast<uint16_t>(mp3Tracks::t_402_ok_settings));
  getMp3().end_track();
  execute_cycle();
  execute_cycle();

  execute_cycle_for_ms(time_check_play);
  EXPECT_TRUE(getMp3().is_playing_mp3());
  EXPECT_EQ(getMp3().df_mp3_track, static_cast<uint16_t>(mp3Tracks::t_919_continue_admin));

  EXPECT_EQ(getSettings().spkMaxVolume, maxVolume);
  getSettings().loadSettingsFromFlash();
  EXPECT_EQ(getSettings().spkMaxVolume, maxVolume);

  goto_idle();

//  EXPECT_TRUE(false) << "log: " << Print::get_output();

}

// =======================================================
// Test Admin_SimpleSetting_minVolume
// =======================================================

TEST_F(admin_test_fixture, Admin_SimpleSetting_minVolume) {

  Print::clear_output();

  getSettings().spkMinVolume = 10;

  const uint8_t minVolume = 16;

  goto_Admin_Entry_menu_item(Admin_Entry_menu_items::Admin_SimpleSetting_minVolume);
  ASSERT_TRUE(SM_tonuino::is_in_state<Admin_SimpleSetting>());

  // ===== select volume
  for (uint8_t volume = getSettings().spkMinVolume+1; volume <= minVolume; ++volume) {
    // button up --> play number
    button_for_command(command::next, state_for_command::admin);
    execute_cycle_for_ms(time_check_play);
    EXPECT_TRUE(getMp3().is_playing_mp3());
    EXPECT_EQ(getMp3().df_mp3_track, volume);
  }
  // button select --> select volume
  button_for_command(command::select, state_for_command::admin);
  EXPECT_TRUE(SM_tonuino::is_in_state<Admin_Entry>());
  execute_cycle_for_ms(time_check_play);
  EXPECT_TRUE(getMp3().is_playing_mp3());
  EXPECT_EQ(getMp3().df_mp3_track, static_cast<uint16_t>(mp3Tracks::t_402_ok_settings));
  getMp3().end_track();
  execute_cycle();
  execute_cycle();

  execute_cycle_for_ms(time_check_play);
  EXPECT_TRUE(getMp3().is_playing_mp3());
  EXPECT_EQ(getMp3().df_mp3_track, static_cast<uint16_t>(mp3Tracks::t_919_continue_admin));

  EXPECT_EQ(getSettings().spkMinVolume, minVolume);
  getSettings().loadSettingsFromFlash();
  EXPECT_EQ(getSettings().spkMinVolume, minVolume);

  goto_idle();

//  EXPECT_TRUE(false) << "log: " << Print::get_output();

}

// =======================================================
// Test Admin_SimpleSetting_initVolume
// =======================================================

TEST_F(admin_test_fixture, Admin_SimpleSetting_initVolume) {

  Print::clear_output();

  getSettings().spkInitVolume = 10;

  const uint8_t initVolume = 16;

  goto_Admin_Entry_menu_item(Admin_Entry_menu_items::Admin_SimpleSetting_initVolume);
  ASSERT_TRUE(SM_tonuino::is_in_state<Admin_SimpleSetting>());

  // ===== select volume
  for (uint8_t volume = getSettings().spkInitVolume+1; volume <= initVolume; ++volume) {
    // button up --> play number
    button_for_command(command::next, state_for_command::admin);
    execute_cycle_for_ms(time_check_play);
    EXPECT_TRUE(getMp3().is_playing_mp3());
    EXPECT_EQ(getMp3().df_mp3_track, volume);
  }
  // button select --> select volume
  button_for_command(command::select, state_for_command::admin);
  EXPECT_TRUE(SM_tonuino::is_in_state<Admin_Entry>());
  execute_cycle_for_ms(time_check_play);
  EXPECT_TRUE(getMp3().is_playing_mp3());
  EXPECT_EQ(getMp3().df_mp3_track, static_cast<uint16_t>(mp3Tracks::t_402_ok_settings));
  getMp3().end_track();
  execute_cycle();
  execute_cycle();

  execute_cycle_for_ms(time_check_play);
  EXPECT_TRUE(getMp3().is_playing_mp3());
  EXPECT_EQ(getMp3().df_mp3_track, static_cast<uint16_t>(mp3Tracks::t_919_continue_admin));

  EXPECT_EQ(getSettings().spkInitVolume, initVolume);
  getSettings().loadSettingsFromFlash();
  EXPECT_EQ(getSettings().spkInitVolume, initVolume);

  goto_idle();

//  EXPECT_TRUE(false) << "log: " << Print::get_output();

}

// =======================================================
// Test Admin_SimpleSetting_eq
// =======================================================

TEST_F(admin_test_fixture, Admin_SimpleSetting_eq) {

  Print::clear_output();

  getSettings().eq = 0;

  const uint8_t eq_set = 6; // bass

  goto_Admin_Entry_menu_item(Admin_Entry_menu_items::Admin_SimpleSetting_eq);
  ASSERT_TRUE(SM_tonuino::is_in_state<Admin_SimpleSetting>());

  // ===== select volume
  for (uint8_t eq = getSettings().eq+1; eq <= eq_set; ++eq) {
    // button up --> play number
    button_for_command(command::next, state_for_command::admin);
    execute_cycle_for_ms(time_check_play);
    EXPECT_TRUE(getMp3().is_playing_mp3());
    EXPECT_EQ(getMp3().df_mp3_track, static_cast<uint16_t>(mp3Tracks::t_920_eq_intro) + eq);
  }
  // button select --> select eq
  button_for_command(command::select, state_for_command::admin);
  EXPECT_TRUE(SM_tonuino::is_in_state<Admin_Entry>());
  execute_cycle_for_ms(time_check_play);
  EXPECT_TRUE(getMp3().is_playing_mp3());
  EXPECT_EQ(getMp3().df_mp3_track, static_cast<uint16_t>(mp3Tracks::t_402_ok_settings));
  getMp3().end_track();
  execute_cycle();
  execute_cycle();

  execute_cycle_for_ms(time_check_play);
  EXPECT_TRUE(getMp3().is_playing_mp3());
  EXPECT_EQ(getMp3().df_mp3_track, static_cast<uint16_t>(mp3Tracks::t_919_continue_admin));

  EXPECT_EQ(getSettings().eq, eq_set);
  getSettings().loadSettingsFromFlash();
  EXPECT_EQ(getSettings().eq, eq_set);
  EXPECT_EQ(getMp3().current_eq, eq_set - 1);

  goto_idle();

//  EXPECT_TRUE(false) << "log: " << Print::get_output();

}

// =======================================================
// Test Admin_ModCard
// =======================================================

TEST_F(admin_test_fixture, Admin_ModCard) {

  pmode_t modes[] = {
      pmode_t::sleep_timer  ,
      pmode_t::freeze_dance ,
      pmode_t::fi_wa_ai     ,
      pmode_t::toddler      ,
      pmode_t::kindergarden ,
      pmode_t::repeat_single,
  };
  uint8_t timer_set = 4;
  uint8_t mode_set = 2;
  uint8_t dance_time_set = 3;
  uint8_t special_expect = 60 + 0x80;

  Print::clear_output();

  for (pmode_t mode : modes) {
    goto_Admin_Entry_menu_item(Admin_Entry_menu_items::Admin_ModCard);
    ASSERT_TRUE(SM_tonuino::is_in_state<Admin_ModCard>());

    // ===== select mode
    for (uint8_t m = 1; m <= static_cast<uint8_t>(mode); ++m) {
      // button up --> play mode
      button_for_command(command::next, state_for_command::admin);
      execute_cycle_for_ms(time_check_play);
      EXPECT_TRUE(getMp3().is_playing_mp3());
      EXPECT_EQ(getMp3().df_mp3_track, static_cast<uint16_t>(mp3Tracks::t_970_modifier_Intro) + m);
    }
    // button select --> select mode
    button_for_command(command::select, state_for_command::admin);
    if (mode == pmode_t::sleep_timer) {
      // select time
      execute_cycle_for_ms(time_check_play);
      EXPECT_TRUE(getMp3().is_playing_mp3());
      EXPECT_EQ(getMp3().df_mp3_track, static_cast<uint16_t>(mp3Tracks::t_960_timer_intro));
      for (uint8_t t = 1; t <= timer_set; ++t) {
        // button up --> play mode
        button_for_command(command::next, state_for_command::admin);
        execute_cycle_for_ms(time_check_play);
        EXPECT_TRUE(getMp3().is_playing_mp3());
        EXPECT_EQ(getMp3().df_mp3_track, static_cast<uint16_t>(mp3Tracks::t_960_timer_intro) + t);
      }
      // button select --> select timer
      button_for_command(command::select, state_for_command::admin);

      // select mode
      execute_cycle_for_ms(time_check_play);
      EXPECT_TRUE(getMp3().is_playing_mp3());
      EXPECT_EQ(getMp3().df_mp3_track, static_cast<uint16_t>(mp3Tracks::t_938_modifier_sleep_mode));
      for (uint8_t t = 1; t <= mode_set; ++t) {
        // button up --> play mode
        button_for_command(command::next, state_for_command::admin);
        execute_cycle_for_ms(time_check_play);
        EXPECT_TRUE(getMp3().is_playing_mp3());
        EXPECT_EQ(getMp3().df_mp3_track, static_cast<uint16_t>(mp3Tracks::t_933_switch_volume_intro) + t);
      }
      // button select --> select timer
      button_for_command(command::select, state_for_command::admin);
    }
    if (mode == pmode_t::freeze_dance || mode == pmode_t::fi_wa_ai) {
      // select time
      execute_cycle_for_ms(time_check_play);
      EXPECT_TRUE(getMp3().is_playing_mp3());
      EXPECT_EQ(getMp3().df_mp3_track, static_cast<uint16_t>(mp3Tracks::t_966_dance_pause_intro));
      for (uint8_t t = 1; t <= dance_time_set; ++t) {
        // button up --> play mode
        button_for_command(command::next, state_for_command::admin);
        execute_cycle_for_ms(time_check_play);
        EXPECT_TRUE(getMp3().is_playing_mp3());
        EXPECT_EQ(getMp3().df_mp3_track, static_cast<uint16_t>(mp3Tracks::t_966_dance_pause_intro) + t);
      }
      // button select --> select timer
      button_for_command(command::select, state_for_command::admin);
    }

    execute_cycle(); // --> start_writeCard
    execute_cycle(); // --> run_writeCard
    write_card();

    folderSettings card_expected = { 0, mode , 0, 0 };
    if (mode == pmode_t::sleep_timer)
      card_expected.special = special_expect;
    if (mode == pmode_t::freeze_dance || mode == pmode_t::fi_wa_ai)
      card_expected.special = dance_time_set-1;

    EXPECT_EQ(card_expected, card_decode());

    goto_idle();

//    EXPECT_TRUE(false) << "log: " << Print::get_output();
  }
}

// =======================================================
// Test Admin_ShortCut
// =======================================================

TEST_F(admin_test_fixture, Admin_ShortCut) {

  folderSettings cards[] = {
      { 0, pmode_t::admin       , 0, 0 },
      { 0, pmode_t::repeat_last , 0, 0 },
      { 1, pmode_t::hoerspiel   , 0, 0 },
      { 2, pmode_t::album       , 0, 0 },
      { 3, pmode_t::party       , 0, 0 },
      { 4, pmode_t::einzel      , 1, 0 },
      { 5, pmode_t::hoerbuch    , 0, 0 },
      { 6, pmode_t::hoerspiel_vb, 1, 2 },
      { 7, pmode_t::album_vb    , 2, 3 },
      { 8, pmode_t::party_vb    , 4, 5 },
      { 9, pmode_t::hoerbuch_1  , 0, 0 }
  };

  Print::clear_output();

  for (const folderSettings& card: cards) {
    for (uint8_t shortcut = 1; shortcut <= 4; ++shortcut) {

      getMp3().set_folder_track_count(card.folder, 10);

      goto_Admin_Entry_menu_item(Admin_Entry_menu_items::Admin_ShortCut);
      ASSERT_TRUE(SM_tonuino::is_in_state<Admin_ShortCut>());

      for (uint8_t s = 1; s <= shortcut; ++s) {
        // button up --> select shortcut
        button_for_command(command::next, state_for_command::admin);
        execute_cycle_for_ms(time_check_play);
        EXPECT_TRUE(getMp3().is_playing_mp3());
        EXPECT_EQ(getMp3().df_mp3_track, static_cast<uint16_t>(mp3Tracks::t_940_shortcut_into) + s);
      }
      // button select --> select shortcut
      button_for_command(command::select, state_for_command::admin);
      execute_cycle(); // --> start_setupCard

      admin_setup_card(card);
      execute_cycle(); // --> end_setupCard

      folderSettings card_expected = card;
      if (card.mode == pmode_t::admin)
        card_expected.mode = pmode_t::admin_card;
      if (card.mode == pmode_t::repeat_last)
        card_expected.folder = 0xff;

      EXPECT_EQ(getSettings().shortCuts[shortcut-1], card_expected);
      getSettings().loadSettingsFromFlash();
      EXPECT_EQ(getSettings().shortCuts[shortcut-1], card_expected);

      goto_idle();

    }
  }
//  EXPECT_TRUE(false) << "log: " << Print::get_output();

}

#ifdef BUTTONS3X3
TEST_F(admin_test_fixture, Admin_ShortCut_extButtons) {

  folderSettings cards[] = {
      { 0, pmode_t::admin       , 0, 0 },
      { 0, pmode_t::repeat_last , 0, 0 },
      { 1, pmode_t::hoerspiel   , 0, 0 },
      { 2, pmode_t::album       , 0, 0 },
      { 3, pmode_t::party       , 0, 0 },
      { 4, pmode_t::einzel      , 1, 0 },
      { 5, pmode_t::hoerbuch    , 0, 0 },
      { 6, pmode_t::hoerspiel_vb, 1, 2 },
      { 7, pmode_t::album_vb    , 2, 3 },
      { 8, pmode_t::party_vb    , 4, 5 },
      { 9, pmode_t::hoerbuch_1  , 0, 0 }
  };

  Print::clear_output();

  // reset linearAnalogKeypad from longpress
  reset_value_for_3x3();
  execute_cycle();

  for (const folderSettings& card: cards) {
//    EXPECT_TRUE(false) << "card: " << static_cast<uint8_t>(card.mode);
    for (uint8_t index = 0; index < Buttons3x3::numLevels; ++index) {
//      EXPECT_TRUE(false) << "index: " << index;

      getMp3().set_folder_track_count(card.folder, 10);

      goto_Admin_Entry_menu_item(Admin_Entry_menu_items::Admin_ShortCut);
      ASSERT_TRUE(SM_tonuino::is_in_state<Admin_ShortCut>());

      // ext shortcut
      set_value_for_3x3(index);
      execute_cycle();
      reset_value_for_3x3();
      execute_cycle();

      execute_cycle(); // --> start_setupCard

      admin_setup_card(card);
      execute_cycle(); // --> end_setupCard

      folderSettings card_expected = card;
      if (card.mode == pmode_t::admin)
        card_expected.mode = pmode_t::admin_card;
      if (card.mode == pmode_t::repeat_last)
        card_expected.folder = 0xff;

      folderSettings fs;
      const int address = startAddressExtraShortcuts + index * sizeof(folderSettings);
      Settings::EEPROM_get(address, fs);

      EXPECT_EQ(fs, card_expected);

      goto_idle();

    }
  }
//  EXPECT_TRUE(false) << "log: " << Print::get_output();

}

TEST_F(admin_test_fixture, Admin_ShortCut_extButtons_longPress) {

  folderSettings cards[] = {
      { 0, pmode_t::admin       , 0, 0 },
      { 0, pmode_t::repeat_last , 0, 0 },
      { 1, pmode_t::hoerspiel   , 0, 0 },
      { 2, pmode_t::album       , 0, 0 },
      { 3, pmode_t::party       , 0, 0 },
      { 4, pmode_t::einzel      , 1, 0 },
      { 5, pmode_t::hoerbuch    , 0, 0 },
      { 6, pmode_t::hoerspiel_vb, 1, 2 },
      { 7, pmode_t::album_vb    , 2, 3 },
      { 8, pmode_t::party_vb    , 4, 5 },
      { 9, pmode_t::hoerbuch_1  , 0, 0 }
  };

  Print::clear_output();

  // reset linearAnalogKeypad from longpress
  reset_value_for_3x3();
  execute_cycle();

  for (const folderSettings& card: cards) {
    for (uint8_t index = 0; index < Buttons3x3::numLevels; ++index) {

      getMp3().set_folder_track_count(card.folder, 10);

      goto_Admin_Entry_menu_item(Admin_Entry_menu_items::Admin_ShortCut);
      ASSERT_TRUE(SM_tonuino::is_in_state<Admin_ShortCut>());

      // ext shortcut long press
      set_value_for_3x3(index);
      execute_cycle();
      current_time += buttonLongPress;
      execute_cycle();
      reset_value_for_3x3();
      execute_cycle();

      execute_cycle(); // --> start_setupCard

      admin_setup_card(card);
      execute_cycle(); // --> end_setupCard

      folderSettings card_expected = card;
      if (card.mode == pmode_t::admin)
        card_expected.mode = pmode_t::admin_card;
      if (card.mode == pmode_t::repeat_last)
        card_expected.folder = 0xff;

      folderSettings fs;
      const int address = startAddressExtraShortcuts + (Buttons3x3::numLevels+index) * sizeof(folderSettings);
      Settings::EEPROM_get(address, fs);

      EXPECT_EQ(fs, card_expected);

      goto_idle();

    }
  }
//  EXPECT_TRUE(false) << "log: " << Print::get_output();

}
#endif

// =======================================================
// Test New (empty) Card
// =======================================================

TEST_F(admin_test_fixture, New_Card) {

  folderSettings cards[] = {
      { 0, pmode_t::admin       , 0, 0 },
      { 0, pmode_t::repeat_last , 0, 0 },
      { 1, pmode_t::hoerspiel   , 0, 0 },
      { 2, pmode_t::album       , 0, 0 },
      { 3, pmode_t::party       , 0, 0 },
      { 4, pmode_t::einzel      , 1, 0 },
      { 5, pmode_t::hoerbuch    , 0, 0 },
      { 6, pmode_t::hoerspiel_vb, 1, 2 },
      { 7, pmode_t::album_vb    , 2, 3 },
      { 8, pmode_t::party_vb    , 4, 5 },
      { 9, pmode_t::hoerbuch_1  , 0, 0 }
  };

  Print::clear_output();

  goto_idle();

  for (const folderSettings& card: cards) {

    getMp3().set_folder_track_count(card.folder, 10);

    card_in(0, 0, 0, 0, 0, 0);

    ASSERT_TRUE(SM_tonuino::is_in_state<Admin_NewCard>());

    execute_cycle_for_ms(time_check_play);
    EXPECT_TRUE(getMp3().is_playing_mp3());
    EXPECT_EQ(getMp3().df_mp3_track, static_cast<uint16_t>(mp3Tracks::t_300_new_tag));

    // wait for end t_300_new_tag
    execute_cycle_for_ms(dfPlayer_timeUntilStarts);
    getMp3().end_track();
    execute_cycle();
    execute_cycle();
    execute_cycle();
    execute_cycle(); // --> start_setupCard

    admin_setup_card(card);
    execute_cycle(); // --> end_setupCard
    execute_cycle(); // --> start_writeCard
    execute_cycle(); // --> run_writeCard
    write_card(true /*with_cars_already_in*/);

    folderSettings card_expected = card;
    if (card.mode == pmode_t::admin)
      card_expected.mode = pmode_t::admin_card;
    if (card.mode == pmode_t::repeat_last)
      card_expected.folder = 0xff;

    EXPECT_EQ(card_expected, card_decode());

    EXPECT_TRUE(SM_tonuino::is_in_state<Idle>());
    ASSERT_TRUE(getMp3().is_stopped());
  }
//  EXPECT_TRUE(false) << "log: " << Print::get_output();

}

// =======================================================
// Test Admin_StandbyTimer
// =======================================================

TEST_F(admin_test_fixture, Admin_StandbyTimer) {

  Print::clear_output();

  getSettings().standbyTimer = 5;

  const uint8_t standbyTimer        = 3;
  const long    standbyTimer_expect = 30;

  goto_Admin_Entry_menu_item(Admin_Entry_menu_items::Admin_StandbyTimer);
  ASSERT_TRUE(SM_tonuino::is_in_state<Admin_StandbyTimer>());

  // ===== select standbyTimer
  for (uint8_t s = 1; s <= standbyTimer; ++s) {
    // button up --> play track for timer
    button_for_command(command::next, state_for_command::admin);
    execute_cycle_for_ms(time_check_play);
    EXPECT_TRUE(getMp3().is_playing_mp3());
    EXPECT_EQ(getMp3().df_mp3_track, static_cast<uint16_t>(mp3Tracks::t_960_timer_intro) + s);
  }
  // button select --> select standbyTimer
  button_for_command(command::select, state_for_command::admin);
  EXPECT_TRUE(SM_tonuino::is_in_state<Admin_Entry>());
  execute_cycle_for_ms(time_check_play);
  EXPECT_TRUE(getMp3().is_playing_mp3());
  EXPECT_EQ(getMp3().df_mp3_track, static_cast<uint16_t>(mp3Tracks::t_402_ok_settings));
  getMp3().end_track();
  execute_cycle();
  execute_cycle();

  execute_cycle_for_ms(time_check_play);
  EXPECT_TRUE(getMp3().is_playing_mp3());
  EXPECT_EQ(getMp3().df_mp3_track, static_cast<uint16_t>(mp3Tracks::t_919_continue_admin));

  EXPECT_EQ(getSettings().standbyTimer, standbyTimer_expect);
  getSettings().loadSettingsFromFlash();
  EXPECT_EQ(getSettings().standbyTimer, standbyTimer_expect);

  goto_idle();

//  EXPECT_TRUE(false) << "log: " << Print::get_output();

}

// =======================================================
// Test Admin_CardsForFolder
// =======================================================

TEST_F(admin_test_fixture, Admin_CardsForFolder) {

  const uint8_t folder = 4;
  const uint8_t first  = 3;
  const uint8_t last   = 6;

  Print::clear_output();

  getMp3().set_folder_track_count(folder, last+1);

  goto_Admin_Entry_menu_item(Admin_Entry_menu_items::Admin_CardsForFolder);
  ASSERT_TRUE(SM_tonuino::is_in_state<Admin_CardsForFolder>());

  // ===== select folder
  for (uint8_t f = 1; f <= folder; ++f) {
    // button up --> play folder number and track
    button_for_command(command::next, state_for_command::admin);
    execute_cycle_for_ms(time_check_play);
    EXPECT_TRUE(getMp3().is_playing_mp3());
    EXPECT_EQ(getMp3().df_mp3_track, f);
    getMp3().end_track();
    current_time += 1001;
    execute_cycle(); // end mp3 track
    execute_cycle(); // enqueue folder track
    execute_cycle_for_ms(time_check_play);
    EXPECT_TRUE(getMp3().is_playing_folder());
    EXPECT_EQ(getMp3().df_folder, f);
    EXPECT_EQ(getMp3().df_folder_track, 1);
  }
  // button select --> select folder
  button_for_command(command::select, state_for_command::admin);

  execute_cycle_for_ms(time_check_play);
  EXPECT_TRUE(getMp3().is_playing_mp3());
  EXPECT_EQ(getMp3().df_mp3_track, static_cast<uint16_t>(mp3Tracks::t_328_select_first_file));

  // ===== select first track
  for (uint8_t track = 1; track <= first; ++track) {
    // button up --> play track number and track
    button_for_command(command::next, state_for_command::admin);
    execute_cycle_for_ms(time_check_play);
    EXPECT_TRUE(getMp3().is_playing_mp3());
    EXPECT_EQ(getMp3().df_mp3_track, track);
    getMp3().end_track();
    current_time += 1001;
    execute_cycle(); // end mp3 track
    execute_cycle(); // enqueue folder track
    execute_cycle_for_ms(time_check_play);
    EXPECT_TRUE(getMp3().is_playing_folder());
    EXPECT_EQ(getMp3().df_folder, folder);
    EXPECT_EQ(getMp3().df_folder_track, track);
  }
  // button select --> select last track
  button_for_command(command::select, state_for_command::admin);

  execute_cycle_for_ms(time_check_play);
  EXPECT_TRUE(getMp3().is_playing_mp3());
  EXPECT_EQ(getMp3().df_mp3_track, static_cast<uint16_t>(mp3Tracks::t_329_select_last_file));

  // ===== select last track
  for (uint8_t track = first+1; track <= last; ++track) {
    // button up --> play track number and track
    button_for_command(command::next, state_for_command::admin);
    execute_cycle_for_ms(time_check_play);
    EXPECT_TRUE(getMp3().is_playing_mp3());
    EXPECT_EQ(getMp3().df_mp3_track, track);
    getMp3().end_track();
    current_time += 1001;
    execute_cycle(); // end mp3 track
    execute_cycle(); // enqueue folder track
    execute_cycle_for_ms(time_check_play);
    EXPECT_TRUE(getMp3().is_playing_folder());
    EXPECT_EQ(getMp3().df_folder, folder);
    EXPECT_EQ(getMp3().df_folder_track, track);
  }
  // button select --> select last track
  button_for_command(command::select, state_for_command::admin);

  execute_cycle_for_ms(time_check_play);  // --> prepare_writeCard
  EXPECT_TRUE(getMp3().is_playing_mp3());
  EXPECT_EQ(getMp3().df_mp3_track, static_cast<uint16_t>(mp3Tracks::t_936_batch_cards_intro));

  for (uint8_t track = first; track <= last; ++track) {

    getMp3().end_track();
    execute_cycle(); // end mp3 track
    execute_cycle_for_ms(time_check_play);
    EXPECT_TRUE(getMp3().is_playing_mp3());
    EXPECT_EQ(getMp3().df_mp3_track, track);

    getMp3().end_track();
    execute_cycle(); // end mp3 track
    execute_cycle_for_ms(dfPlayer_timeUntilStarts); // end timer

    execute_cycle(); // --> run_writeCard

    write_card();

    folderSettings card_expected = { folder, pmode_t::einzel , track, 0 };
    EXPECT_EQ(card_expected, card_decode());
  }

  goto_idle();

//  EXPECT_TRUE(false) << "log: " << Print::get_output();
}

// =======================================================
// Test Admin_InvButtons
// =======================================================

TEST_F(admin_test_fixture, Admin_InvButtons) {

  Print::clear_output();

  for (bool on : { false, true }) {

    goto_Admin_Entry_menu_item(Admin_Entry_menu_items::Admin_InvButtons);
    ASSERT_TRUE(SM_tonuino::is_in_state<Admin_InvButtons>());

    // ===== select off/on
    for (uint8_t s = 1; s <= (on?2:1); ++s) {
      // button up --> play off/on
      button_for_command(command::next, state_for_command::admin);
      execute_cycle_for_ms(time_check_play);
      EXPECT_TRUE(getMp3().is_playing_mp3());
      EXPECT_EQ(getMp3().df_mp3_track, static_cast<uint16_t>(mp3Tracks::t_933_switch_volume_intro) + s);
    }
    // button select --> select off/on
    button_for_command(command::select, state_for_command::admin);
    EXPECT_TRUE(SM_tonuino::is_in_state<Admin_Entry>());
    execute_cycle_for_ms(time_check_play);
    EXPECT_TRUE(getMp3().is_playing_mp3());
    EXPECT_EQ(getMp3().df_mp3_track, static_cast<uint16_t>(mp3Tracks::t_402_ok_settings));
    getMp3().end_track();
    execute_cycle();
    execute_cycle();

    execute_cycle_for_ms(time_check_play);
    EXPECT_TRUE(getMp3().is_playing_mp3());
    EXPECT_EQ(getMp3().df_mp3_track, static_cast<uint16_t>(mp3Tracks::t_919_continue_admin));

    EXPECT_EQ(getSettings().invertVolumeButtons, on);
    getSettings().loadSettingsFromFlash();
    EXPECT_EQ(getSettings().invertVolumeButtons, on);

    goto_idle();
  }

//  EXPECT_TRUE(false) << "log: " << Print::get_output();

}

// =======================================================
// Test Admin_ResetEeprom
// =======================================================

inline bool operator==(const Settings &lhs, const Settings &rhs) {
  return
  lhs.cookie               == rhs.cookie               &&
  lhs.version              == rhs.version              &&
  lhs.spkMaxVolume         == rhs.spkMaxVolume         &&
  lhs.spkMinVolume         == rhs.spkMinVolume         &&
  lhs.spkInitVolume        == rhs.spkInitVolume        &&
  lhs.eq                   == rhs.eq                   &&
  lhs.dummy                == rhs.dummy                &&
  lhs.standbyTimer         == rhs.standbyTimer         &&
  lhs.invertVolumeButtons  == rhs.invertVolumeButtons  &&
  lhs.shortCuts[0]         == rhs.shortCuts[0]         &&
  lhs.shortCuts[1]         == rhs.shortCuts[1]         &&
  lhs.shortCuts[2]         == rhs.shortCuts[2]         &&
  lhs.shortCuts[3]         == rhs.shortCuts[3]         &&
  lhs.adminMenuLocked      == rhs.adminMenuLocked      &&
  lhs.adminMenuPin[0]      == rhs.adminMenuPin[0]      &&
  lhs.adminMenuPin[1]      == rhs.adminMenuPin[1]      &&
  lhs.adminMenuPin[2]      == rhs.adminMenuPin[2]      &&
  lhs.adminMenuPin[3]      == rhs.adminMenuPin[3]      &&
  lhs.pauseWhenCardRemoved == rhs.pauseWhenCardRemoved &&
  lhs.hpMaxVolume          == rhs.hpMaxVolume          &&
  lhs.hpMinVolume          == rhs.hpMinVolume          &&
  lhs.hpInitVolume         == rhs.hpInitVolume         ;
}

TEST_F(admin_test_fixture, Admin_ResetEeprom) {

  const Settings default_settings = {
      cardCookie    ,//uint32_t    cookie;
      2             ,//byte        version;
      25            ,//uint8_t     spkMaxVolume;
      5             ,//uint8_t     spkMinVolume;
      15            ,//uint8_t     spkInitVolume;
      1             ,//uint8_t     eq;
      false         ,//bool        locked;
      0             ,//long        standbyTimer;
      true          ,//bool        invertVolumeButtons;
      {{
          { 0, pmode_t::none, 0, 0 },
          { 0, pmode_t::none, 0, 0 },
          { 0, pmode_t::none, 0, 0 },
          { 0, pmode_t::none, 0, 0 }
      }}            ,//shortCuts_t shortCuts;
      0             ,//uint8_t     adminMenuLocked;
      {{1,1,1,1}}   ,//pin_t       adminMenuPin;
      0             ,//uint8_t     pauseWhenCardRemoved;
      25            ,//uint8_t     hpMaxVolume;
      5             ,//uint8_t     hpMinVolume;
      15            ,//uint8_t     hpInitVolume;
  };
  const Settings other_settings = {
      cardCookie    ,//uint32_t    cookie;
      2             ,//byte        version;
      26            ,//uint8_t     spkMaxVolume;
      6             ,//uint8_t     spkMinVolume;
      16            ,//uint8_t     spkInitVolume;
      1             ,//uint8_t     eq;
      false         ,//bool        locked;
      1000          ,//long        standbyTimer;
      false         ,//bool        invertVolumeButtons;
      {{
          { 1, pmode_t::album   , 0, 0 },
          { 2, pmode_t::album_vb, 1, 2 },
          { 3, pmode_t::hoerbuch, 0, 0 },
          { 4, pmode_t::party   , 0, 0 }
      }}            ,//shortCuts_t shortCuts;
      0             ,//uint8_t     adminMenuLocked;
      {{1,2,3,4}}   ,//pin_t       adminMenuPin;
      1             ,//uint8_t     pauseWhenCardRemoved;
      24            ,//uint8_t     hpMaxVolume;
      4             ,//uint8_t     hpMinVolume;
      16            ,//uint8_t     hpInitVolume;
  };
  Print::clear_output();

  getSettings() = other_settings;
  getSettings().writeSettingsToFlash();
  ASSERT_EQ(getSettings(), other_settings);
  getSettings().loadSettingsFromFlash();
  ASSERT_EQ(getSettings(), other_settings);

  goto_Admin_Entry_menu_item(Admin_Entry_menu_items::Admin_ResetEeprom);

  EXPECT_TRUE(SM_tonuino::is_in_state<Admin_Entry>());

  EXPECT_EQ(getSettings(), default_settings);
  getSettings().loadSettingsFromFlash();
  EXPECT_EQ(getSettings(), default_settings);

  goto_idle();

//  EXPECT_TRUE(false) << "log: " << Print::get_output();

}

// =======================================================
// Test Admin_LockAdmin
// =======================================================

TEST_F(admin_test_fixture, Admin_LockAdmin) {

  const Settings::pin_t pin = {              2,                 1,                 1,                3 };
  const commandRaw cmd[]    = { commandRaw::up, commandRaw::pause, commandRaw::pause, commandRaw::down };

  Print::clear_output();

  for (uint8_t mode : { 1, 2, 3 }) {

    goto_Admin_Entry_menu_item(Admin_Entry_menu_items::Admin_LockAdmin);
    ASSERT_TRUE(SM_tonuino::is_in_state<Admin_LockAdmin>());

    // ===== select off/on
    for (uint8_t m = 1; m <= mode; ++m) {
      // button up --> play mode
      button_for_command(command::next, state_for_command::admin);
      execute_cycle_for_ms(time_check_play);
      EXPECT_TRUE(getMp3().is_playing_mp3());
      EXPECT_EQ(getMp3().df_mp3_track, static_cast<uint16_t>(mp3Tracks::t_985_admin_lock_intro) + m);
    }
    // button select --> select mode
    button_for_command(command::select, state_for_command::admin);

    if (mode == 3) {// pin
      execute_cycle_for_ms(time_check_play);
      EXPECT_TRUE(getMp3().is_playing_mp3());
      EXPECT_EQ(getMp3().df_mp3_track, static_cast<uint16_t>(mp3Tracks::t_991_admin_pin));
      for (uint8_t p = 0; p < 4; ++p) {
        button_for_raw_command(cmd[p]);
      }
    }
    execute_cycle();
    EXPECT_TRUE(SM_tonuino::is_in_state<Admin_Entry>());
    execute_cycle_for_ms(time_check_play);
    EXPECT_TRUE(getMp3().is_playing_mp3());
    EXPECT_EQ(getMp3().df_mp3_track, static_cast<uint16_t>(mp3Tracks::t_402_ok_settings));
    getMp3().end_track();
    execute_cycle();
    execute_cycle();

    execute_cycle_for_ms(time_check_play);
    EXPECT_TRUE(getMp3().is_playing_mp3());
    EXPECT_EQ(getMp3().df_mp3_track, static_cast<uint16_t>(mp3Tracks::t_919_continue_admin));

    EXPECT_EQ(getSettings().adminMenuLocked, mode-1);
    if (mode == 3)
      EXPECT_EQ(getSettings().adminMenuPin, pin);
    getSettings().loadSettingsFromFlash();
    EXPECT_EQ(getSettings().adminMenuLocked, mode-1);
    if (mode == 3)
      EXPECT_EQ(getSettings().adminMenuPin, pin);

    getSettings().resetSettings();
    goto_idle();
  }

//  EXPECT_TRUE(false) << "log: " << Print::get_output();

}

// =======================================================
// Test Admin_PauseIfCardRemoved
// =======================================================

TEST_F(admin_test_fixture, Admin_PauseIfCardRemoved) {

  Print::clear_output();

  for (bool on : { false, true }) {

    goto_Admin_Entry_menu_item(Admin_Entry_menu_items::Admin_PauseIfCardRemoved);
    ASSERT_TRUE(SM_tonuino::is_in_state<Admin_PauseIfCardRemoved>());

    // ===== select off/on
    for (uint8_t s = 1; s <= (on?2:1); ++s) {
      // button up --> play off/on
      button_for_command(command::next, state_for_command::admin);
      execute_cycle_for_ms(time_check_play);
      EXPECT_TRUE(getMp3().is_playing_mp3());
      EXPECT_EQ(getMp3().df_mp3_track, static_cast<uint16_t>(mp3Tracks::t_933_switch_volume_intro) + s);
    }
    // button select --> select off/on
    button_for_command(command::select, state_for_command::admin);
    EXPECT_TRUE(SM_tonuino::is_in_state<Admin_Entry>());
    execute_cycle_for_ms(time_check_play);
    EXPECT_TRUE(getMp3().is_playing_mp3());
    EXPECT_EQ(getMp3().df_mp3_track, static_cast<uint16_t>(mp3Tracks::t_402_ok_settings));
    getMp3().end_track();
    execute_cycle();
    execute_cycle();

    execute_cycle_for_ms(time_check_play);
    EXPECT_TRUE(getMp3().is_playing_mp3());
    EXPECT_EQ(getMp3().df_mp3_track, static_cast<uint16_t>(mp3Tracks::t_919_continue_admin));

    EXPECT_EQ(getSettings().pauseWhenCardRemoved, on);
    getSettings().loadSettingsFromFlash();
    EXPECT_EQ(getSettings().pauseWhenCardRemoved, on);

    goto_idle();
  }

//  EXPECT_TRUE(false) << "log: " << Print::get_output();

}



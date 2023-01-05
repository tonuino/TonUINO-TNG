
#include <gtest/gtest.h>

#include <tonuino.hpp>
#include <mp3.hpp>

class mp3_test_fixture: public ::testing::Test {
public:
  mp3_test_fixture()
  : tonuino(Tonuino::getTonuino())
  , mp3(tonuino.getMp3())
  {}

  void execute_cycle() {
    mp3.loop();
  }

  // because mp3::OnPlayFinished() uses Tonuino::nextTrack() we have to use mp3 from Tonuino
  Tonuino& tonuino;
  Mp3&     mp3;
};


TEST_F(mp3_test_fixture, initial_state) {
  execute_cycle();
  EXPECT_TRUE(mp3.is_stopped());
}

TEST_F(mp3_test_fixture, enqueue_mp3) {
  mp3.enqueueMp3FolderTrack(mp3Tracks::t_262_pling);
  execute_cycle();
  EXPECT_TRUE(mp3.is_playing_mp3());
  EXPECT_EQ(mp3.df_mp3_track, static_cast<uint16_t>(mp3Tracks::t_262_pling));

  mp3.enqueueMp3FolderTrack(mp3Tracks::t_300_new_tag);
  execute_cycle();
  EXPECT_TRUE(mp3.is_playing_mp3());
  EXPECT_EQ(mp3.df_mp3_track, static_cast<uint16_t>(mp3Tracks::t_300_new_tag));

  mp3.end_track();
  execute_cycle();
  EXPECT_TRUE(mp3.is_stopped());
}

TEST_F(mp3_test_fixture, enqueue_mp3_with_playafter) {
  mp3.enqueueMp3FolderTrack(mp3Tracks::t_262_pling);
  execute_cycle();
  EXPECT_TRUE(mp3.is_playing_mp3());
  EXPECT_EQ(mp3.df_mp3_track, static_cast<uint16_t>(mp3Tracks::t_262_pling));

  mp3.enqueueMp3FolderTrack(mp3Tracks::t_300_new_tag, true /*playAfter*/);
  execute_cycle();
  EXPECT_TRUE(mp3.is_playing_mp3());
  EXPECT_EQ(mp3.df_mp3_track, static_cast<uint16_t>(mp3Tracks::t_262_pling));

  mp3.end_track();
  execute_cycle();
  execute_cycle();
  EXPECT_TRUE(mp3.is_playing_mp3());
  EXPECT_EQ(mp3.df_mp3_track, static_cast<uint16_t>(mp3Tracks::t_300_new_tag));

  mp3.end_track();
  execute_cycle();
  EXPECT_TRUE(mp3.is_stopped());
}


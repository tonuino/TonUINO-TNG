
#include <gtest/gtest.h>

#include <tonuino.hpp>
#include <mp3.hpp>

class mp3_test_fixture: public ::testing::Test {
public:
  mp3_test_fixture()
  : tonuino(Tonuino::getTonuino())
  , mp3(tonuino.getMp3())
  {
    mp3.reset_to_initial_state();
  }

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

TEST_F(mp3_test_fixture, enqueue_folder_track) {
  mp3.enqueueTrack(1, 2);
  execute_cycle();
  EXPECT_TRUE(mp3.is_playing_folder());
  EXPECT_EQ(mp3.df_folder, 1);
  EXPECT_EQ(mp3.df_folder_track, 2);

  mp3.enqueueTrack(4, 5);
  execute_cycle();
  EXPECT_TRUE(mp3.is_playing_folder());
  EXPECT_EQ(mp3.df_folder, 4);
  EXPECT_EQ(mp3.df_folder_track, 5);

  mp3.end_track();
  execute_cycle();
  EXPECT_TRUE(mp3.is_stopped());
}

TEST_F(mp3_test_fixture, enqueue_folder_tracks) {
  mp3.enqueueTrack(1, 2, 12);
  execute_cycle();

  for (uint8_t t = 2; t <= 12; ++t) {
    EXPECT_TRUE(mp3.is_playing_folder());
    EXPECT_EQ(mp3.df_folder, 1);
    EXPECT_EQ(mp3.df_folder_track, t);
    mp3.end_track();
    execute_cycle();
    execute_cycle();
  }
  EXPECT_TRUE(mp3.is_stopped());
}

TEST_F(mp3_test_fixture, enqueue_folder_tracks_no_chrash) {
  mp3.enqueueTrack(1, 2, 255);
  execute_cycle();

  for (uint16_t t = 2; t <= 255; ++t) {
    EXPECT_TRUE(mp3.is_playing_folder());
    EXPECT_EQ(mp3.df_folder, 1);
    EXPECT_EQ(mp3.df_folder_track, t);
    mp3.end_track();
    execute_cycle();
    execute_cycle();
  }
  EXPECT_TRUE(mp3.is_stopped());
}

TEST_F(mp3_test_fixture, enqueue_folder_tracks_with_current) {
  mp3.enqueueTrack(1, 2, 12, 3);
  execute_cycle();

  for (uint8_t t = 2+3; t <= 12; ++t) {
    EXPECT_TRUE(mp3.is_playing_folder());
    EXPECT_EQ(mp3.df_folder, 1);
    EXPECT_EQ(mp3.df_folder_track, t);
    mp3.end_track();
    execute_cycle();
    execute_cycle();
  }
  EXPECT_TRUE(mp3.is_stopped());
}

TEST_F(mp3_test_fixture, enqueue_folder_tracks_and_jump) {
  mp3.enqueueTrack(1, 2, 12);
  execute_cycle();

  EXPECT_TRUE(mp3.is_playing_folder());
  EXPECT_EQ(mp3.df_folder, 1);
  EXPECT_EQ(mp3.df_folder_track, 2);

  mp3.playNext();
  execute_cycle();
  execute_cycle();

  EXPECT_TRUE(mp3.is_playing_folder());
  EXPECT_EQ(mp3.df_folder, 1);
  EXPECT_EQ(mp3.df_folder_track, 3);

  mp3.playNext(2);
  execute_cycle();
  execute_cycle();

  EXPECT_TRUE(mp3.is_playing_folder());
  EXPECT_EQ(mp3.df_folder, 1);
  EXPECT_EQ(mp3.df_folder_track, 5);

  mp3.playPrevious();
  execute_cycle();
  execute_cycle();

  EXPECT_TRUE(mp3.is_playing_folder());
  EXPECT_EQ(mp3.df_folder, 1);
  EXPECT_EQ(mp3.df_folder_track, 4);

  mp3.playPrevious(2);
  execute_cycle();
  execute_cycle();

  EXPECT_TRUE(mp3.is_playing_folder());
  EXPECT_EQ(mp3.df_folder, 1);
  EXPECT_EQ(mp3.df_folder_track, 2);

  mp3.clearFolderQueue();
  mp3.end_track();
  execute_cycle();

  EXPECT_TRUE(mp3.is_stopped());
}


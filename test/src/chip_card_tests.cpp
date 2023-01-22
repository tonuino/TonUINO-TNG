
#include <gtest/gtest.h>

#include <SPI.h>

#include <chip_card.hpp>
#include <mp3.hpp>

class chip_card_test_fixture: public ::testing::Test {
public:
  chip_card_test_fixture()
  : settings()
  , mp3(settings)
  , chip_card(mp3)
  {
    chip_card.initCard();
  }

  MFRC522& getMFRC522() { return chip_card.mfrc522; }

  Settings  settings;
  Mp3       mp3;
  Chip_card chip_card;
};

TEST_F(chip_card_test_fixture, initial_state) {
  EXPECT_TRUE(SPI.called_begin);
  EXPECT_TRUE(getMFRC522().called_Init);
}


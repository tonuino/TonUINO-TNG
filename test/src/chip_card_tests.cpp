
#include <gtest/gtest.h>

#include <SPI.h>

#include <chip_card.hpp>
#include <mp3.hpp>
#include <constants.hpp>

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

  cardEvent execute_cycle(unsigned long t = cycleTime) {
    current_time += t;
    return chip_card.getCardEvent();
  }

  void card_in(uint32_t cookie, uint8_t version, uint8_t folder, uint8_t mode, uint8_t special, uint8_t special2) {
    getMFRC522().card_in(cookie, version, folder, mode, special, special2);
  }

  void card_in(folderSettings card) {
    card_in(cardCookie, cardVersion, card.folder, static_cast<uint8_t>(card.mode), card.special, card.special2);
  }

  void card_out() {
    getMFRC522().card_out();
  }

  folderSettings card_decode() {
    folderSettings card;
    uint32_t cookie;
    uint8_t  version;
    uint8_t  mode;
    getMFRC522().card_decode(cookie, version, card.folder, mode, card.special, card.special2);
    card.mode = static_cast<pmode_t>(mode);
    if (cookie == cardCookie && version == cardVersion)
      return card;
    else
      return folderSettings{};
  }

  Settings  settings;
  Mp3       mp3;
  Chip_card chip_card;
};

TEST_F(chip_card_test_fixture, initial_state) {
  EXPECT_TRUE(SPI.called_begin);
  EXPECT_TRUE(getMFRC522().called_Init);
}

TEST_F(chip_card_test_fixture, read_card_works) {
  folderSettings test_data[] = {
      { 1, pmode_t::hoerspiel    , 0, 0 },
      { 1, pmode_t::album        , 0, 0 },
      { 1, pmode_t::party        , 0, 0 },
      { 1, pmode_t::einzel       , 1, 0 },
      { 1, pmode_t::hoerbuch     , 0, 0 },
      { 1, pmode_t::hoerspiel_vb , 1, 3 },
      { 1, pmode_t::album_vb     , 2, 4 },
      { 1, pmode_t::party_vb     , 5, 9 },
      { 1, pmode_t::hoerbuch_1   , 0, 0 },
  };

  for (folderSettings card: test_data) {
    card_in(card);
    cardEvent ce = execute_cycle();
    EXPECT_EQ(ce, cardEvent::inserted);

    folderSettings nfcTag;
    Chip_card::readCardEvent rce = chip_card.readCard(nfcTag);
    EXPECT_EQ(rce, Chip_card::readCardEvent::known);
    EXPECT_EQ(nfcTag, card);

    card_out();
    execute_cycle();
    execute_cycle();
    ce = execute_cycle();
    EXPECT_EQ(ce, cardEvent::removed);
  }
}

TEST_F(chip_card_test_fixture, read_card_with_bad_version) {
  // version 0
  card_in(cardCookie, 0, 1, static_cast<uint8_t>(pmode_t::album), 0, 0);
  cardEvent ce = execute_cycle();
  EXPECT_EQ(ce, cardEvent::inserted);

  folderSettings nfcTag;
  Chip_card::readCardEvent rce = chip_card.readCard(nfcTag);
  EXPECT_EQ(rce, Chip_card::readCardEvent::none);

  card_out();
  execute_cycle();
  execute_cycle();
  ce = execute_cycle();
  EXPECT_EQ(ce, cardEvent::removed);

  // version 3
  card_in(cardCookie, 3, 1, static_cast<uint8_t>(pmode_t::album), 0, 0);
  ce = execute_cycle();
  EXPECT_EQ(ce, cardEvent::inserted);

  rce = chip_card.readCard(nfcTag);
  EXPECT_EQ(rce, Chip_card::readCardEvent::none);

  card_out();
  execute_cycle();
  execute_cycle();
  ce = execute_cycle();
  EXPECT_EQ(ce, cardEvent::removed);

}

TEST_F(chip_card_test_fixture, read_card_with_zero_cookie) {
  card_in(0, 2, 1, static_cast<uint8_t>(pmode_t::album), 0, 0);
  cardEvent ce = execute_cycle();
  EXPECT_EQ(ce, cardEvent::inserted);

  folderSettings nfcTag;
  Chip_card::readCardEvent rce = chip_card.readCard(nfcTag);
  EXPECT_EQ(rce, Chip_card::readCardEvent::empty);

  card_out();
  execute_cycle();
  execute_cycle();
  ce = execute_cycle();
  EXPECT_EQ(ce, cardEvent::removed);
}

TEST_F(chip_card_test_fixture, read_card_with_bad_cookie) {
  card_in(cardCookie+1, 2, 1, static_cast<uint8_t>(pmode_t::album), 0, 0);
  cardEvent ce = execute_cycle();
  EXPECT_EQ(ce, cardEvent::inserted);

  folderSettings nfcTag;
  Chip_card::readCardEvent rce = chip_card.readCard(nfcTag);
  EXPECT_EQ(rce, Chip_card::readCardEvent::none);

  card_out();
  execute_cycle();
  execute_cycle();
  ce = execute_cycle();
  EXPECT_EQ(ce, cardEvent::removed);
}

TEST_F(chip_card_test_fixture, write_card_works) {
  folderSettings test_data[] = {
      { 1, pmode_t::hoerspiel    , 0, 0 },
      { 1, pmode_t::album        , 0, 0 },
      { 1, pmode_t::party        , 0, 0 },
      { 1, pmode_t::einzel       , 1, 0 },
      { 1, pmode_t::hoerbuch     , 0, 0 },
      { 1, pmode_t::hoerspiel_vb , 1, 3 },
      { 1, pmode_t::album_vb     , 2, 4 },
      { 1, pmode_t::party_vb     , 5, 9 },
      { 1, pmode_t::hoerbuch_1   , 0, 0 },
  };

  for (folderSettings card: test_data) {
    folderSettings nfcTag{card};
    card_in(0, 0, 0, 0, 0, 0);
    cardEvent ce = execute_cycle();
    EXPECT_EQ(ce, cardEvent::inserted);

    chip_card.writeCard(nfcTag);
    EXPECT_EQ(card, card_decode());
    card_out();
    execute_cycle();
    execute_cycle();
    ce = execute_cycle();
    EXPECT_EQ(ce, cardEvent::removed);
  }
}


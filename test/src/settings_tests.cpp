
#include <gtest/gtest.h>

#include <settings.hpp>
#include <chip_card.hpp>

class settings_test_fixture: public ::testing::Test {
public:
  settings_test_fixture()
  : settings()
  {}
  const Settings default_settings = {
      cardCookie    ,//uint32_t    cookie;
      2             ,//byte        version;
      25            ,//uint8_t     maxVolume;
      5             ,//uint8_t     minVolume;
      15            ,//uint8_t     initVolume;
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
  };
  const Settings other_settings = {
      cardCookie    ,//uint32_t    cookie;
      2             ,//byte        version;
      26            ,//uint8_t     maxVolume;
      6             ,//uint8_t     minVolume;
      16            ,//uint8_t     initVolume;
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
  };

  const int startAddressAdminSettings = sizeof(folderSettings::folder) * 100;

  void init_brand_new() {
    for (int i = 0; i < EEPROM.max_len; ++i)
      EEPROM.eeprom_mem[i] = 0xff;
  }
  void init_with_settings(const Settings& a_settings) {
    memcpy(&EEPROM.eeprom_mem[startAddressAdminSettings], &a_settings, sizeof(Settings));
  }

  Settings  settings;
};

bool operator==(const Settings &lhs, const Settings &rhs) {
  return
  lhs.cookie               == rhs.cookie               &&
  lhs.version              == rhs.version              &&
  lhs.maxVolume            == rhs.maxVolume            &&
  lhs.minVolume            == rhs.minVolume            &&
  lhs.initVolume           == rhs.initVolume           &&
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
  lhs.pauseWhenCardRemoved == rhs.pauseWhenCardRemoved;
}

TEST_F(settings_test_fixture, initial_state_on_brand_new_box) {
  init_brand_new();
  settings.loadSettingsFromFlash();
  EXPECT_EQ(settings, default_settings);
}

TEST_F(settings_test_fixture, initial_state_on_old_box) {
  init_with_settings(other_settings);
  settings.loadSettingsFromFlash();
  EXPECT_EQ(settings, other_settings);
}

TEST_F(settings_test_fixture, initial_state_on_old_box_from_v2) {
  Settings t_settings = default_settings;
  init_with_settings(t_settings);
  std::size_t addr_pauseWhenCardRemoved = offsetof(Settings, pauseWhenCardRemoved);
  EEPROM.eeprom_mem[startAddressAdminSettings+addr_pauseWhenCardRemoved] = 0xff;
  settings.loadSettingsFromFlash();
  EXPECT_EQ(settings.pauseWhenCardRemoved==1, default_settings.pauseWhenCardRemoved==1);

//  EXPECT_TRUE(false) << "log: " << Print::get_output();
}

TEST_F(settings_test_fixture, writeSettingsToFlash_works) {
  init_brand_new();
  Settings t_settings = other_settings;
  t_settings.writeSettingsToFlash();
  settings.loadSettingsFromFlash();
  EXPECT_EQ(settings, other_settings);
}

TEST_F(settings_test_fixture, reset_settings_works) {
  settings = other_settings;
  settings.resetSettings();
  EXPECT_EQ(settings, default_settings);
}

TEST_F(settings_test_fixture, read_write_folderSettings_works) {
  init_brand_new();

  for (uint16_t track:  { 10, 15,150,255 }) {
    for (uint8_t folder: { 0, 1, 5, 10, 99 }) {
      settings.writeFolderSettingToFlash(folder, track);
      uint8_t r_track = settings.readFolderSettingFromFlash(folder);
      EXPECT_EQ(r_track, track);
    }
  }
}

TEST_F(settings_test_fixture, write_folderSettings_does_nothing_if_folder_bigger_than_99) {
  const uint16_t track = 22;
  for (uint8_t folder:  { 100, 150, 255 }) {
    init_brand_new();
    settings.writeFolderSettingToFlash(folder, track);
    for (int i = 0; i < EEPROM.max_len; ++i)
      EXPECT_EQ(EEPROM.eeprom_mem[i], 0xff);
  }
}

TEST_F(settings_test_fixture, read_folderSettings_returns_0_if_folder_bigger_than_99) {
  for (uint8_t folder:  { 100, 150, 255 }) {
    init_brand_new();
    uint8_t r_track = settings.readFolderSettingFromFlash(folder);
    EXPECT_EQ(r_track, 0);
  }
}

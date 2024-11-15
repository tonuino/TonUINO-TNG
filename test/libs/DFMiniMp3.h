/*-------------------------------------------------------------------------
DFMiniMp3 library

Written by Michael C. Miller.

I invest time and resources providing this open source code,
please support me by dontating (see https://github.com/Makuna/DFMiniMp3)

-------------------------------------------------------------------------
This file is part of the Makuna/DFMiniMp3 library.

DFMiniMp3 is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as
published by the Free Software Foundation, either version 3 of
the License, or (at your option) any later version.

DFMiniMp3 is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with DFMiniMp3.  If not, see
<http://www.gnu.org/licenses/>.

Boerge1: adapted for mocking in unit tests
-------------------------------------------------------------------------*/
#pragma once

#include <Arduino.h>

#include <constants.hpp>

enum DfMp3_Error
{
    // from device
    DfMp3_Error_Busy = 1,
    DfMp3_Error_Sleeping,
    DfMp3_Error_SerialWrongStack,
    DfMp3_Error_CheckSumNotMatch,
    DfMp3_Error_FileIndexOut,
    DfMp3_Error_FileMismatch,
    DfMp3_Error_Advertise,
    // from library
    DfMp3_Error_RxTimeout = 0x81,
    DfMp3_Error_PacketSize,
    DfMp3_Error_PacketHeader,
    DfMp3_Error_PacketChecksum,
    DfMp3_Error_General = 0xff
};

enum DfMp3_PlaybackMode
{
    DfMp3_PlaybackMode_Repeat,
    DfMp3_PlaybackMode_FolderRepeat,
    DfMp3_PlaybackMode_SingleRepeat,
    DfMp3_PlaybackMode_Random
};

enum DfMp3_Eq
{
    DfMp3_Eq_Normal,
    DfMp3_Eq_Pop,
    DfMp3_Eq_Rock,
    DfMp3_Eq_Jazz,
    DfMp3_Eq_Classic,
    DfMp3_Eq_Bass
};

enum DfMp3_PlaySource // value - only one can be set
{
    DfMp3_PlaySource_Usb = 1,
    DfMp3_PlaySource_Sd,
    DfMp3_PlaySource_Aux,
    DfMp3_PlaySource_Sleep,
    DfMp3_PlaySource_Flash
};

enum DfMp3_PlaySources // bitfield - more than one can be set
{
    DfMp3_PlaySources_Usb = 0x01,
    DfMp3_PlaySources_Sd = 0x02,
    DfMp3_PlaySources_Pc = 0x04,
    DfMp3_PlaySources_Flash = 0x08,
};

class Mp3ChipMH2024K16SS
{
};

class Mp3ChipOriginal
{
};

class Mp3ChipIncongruousNoAck
{
};

template <class T_SERIAL_METHOD, class T_NOTIFICATION_METHOD, class T_CHIP_VARIANT = Mp3ChipOriginal, uint32_t C_ACK_TIMEOUT = 900>
class DFMiniMp3
{
public:
    explicit DFMiniMp3(T_SERIAL_METHOD& serial)
    {
    }

    void reset()
    {
    }

    bool called_begin = false;
    void begin(unsigned long baud = 9600)
    {
      called_begin = true;
    }

    bool df_playing = false;
    bool df_playing_adv = false;
    int df_playing_adv_counter = 0;
    bool df_stopped = true;
    uint16_t df_mp3_track = 0;
    uint8_t df_folder = 0;
    uint8_t df_folder_track = 0;
    uint16_t df_adv_track = 0;
    void loop()
    {
      if (called_stop) {
        called_stop = false;
        df_playing = false;
        df_playing_adv = false;
        df_stopped = true;
        df_folder = 0;
        df_folder_track = 0;
        df_mp3_track = 0;
        df_adv_track = 0;
      }
      if (called_start) {
        called_start = false;
        if (!df_stopped) { // id not stopped before, start first track
          df_stopped = false;
          if (!df_folder) df_folder = 1;
          if (!df_folder_track) df_folder_track = 1;
        }
        df_playing = true;
      }
      if (called_pause) {
        called_pause = false;
        df_playing = false;
      }

      if (df_playing_adv && --df_playing_adv_counter <= 0)
        end_adv();

      pin_value[dfPlayer_busyPin] = !(df_playing && !called_end_adv);
      called_end_adv = false;


      if (called_end_track) {
        called_end_track = false;
        uint16_t replyArg = df_folder*255+(df_folder_track+df_mp3_track);
        df_playing = false;
        df_stopped = true;
        df_folder = 0;
        df_folder_track = 0;
        df_mp3_track = 0;
        T_NOTIFICATION_METHOD::OnPlayFinished(*this, DfMp3_PlaySources_Sd, replyArg);
      }
      if (error_code) {
        uint16_t replyArg = error_code;
        error_code = 0;
        T_NOTIFICATION_METHOD::OnError(*this, replyArg);
      }
    }

    // sd:/mp3/####track name
    void playMp3FolderTrack(uint16_t track)
    {
      df_stopped = false;
      called_start = true;
      df_mp3_track = track;
      df_folder = 0;
      df_folder_track = 0;
    }

    // older devices: sd:/###/###track name
    // newer devices: sd:/##/###track name
    // folder and track numbers are zero padded
    void playFolderTrack(uint8_t folder, uint8_t track)
    {
      df_stopped = false;
      called_start = true;
      df_folder = folder;
      df_folder_track = track;
      df_mp3_track = 0;
    }

    // 0- 30
    uint8_t current_volume = 0;
    void setVolume(uint8_t volume)
    {
      current_volume = volume;
    }

    uint8_t getVolume()
    {
      return current_volume;
    }

    DfMp3_Eq current_eq = DfMp3_Eq_Normal;
    void setEq(DfMp3_Eq eq)
    {
      current_eq = eq;
    }

    bool called_sleep = false;
    void sleep()
    {
      called_sleep = true;
    }

    bool called_start = false;
    void start()
    {
      called_start = true;
    }

    bool called_pause = false;
    void pause()
    {
      called_pause = true;
    }

    bool called_stop = false;
    void stop()
    {
      called_stop = true;
    }

    uint16_t getTotalTrackCount()
    {
      return 1;
    }

    uint16_t getFolderTrackCount(uint16_t folder)
    {
        return df_folder_track_count[static_cast<uint8_t>(folder)];
    }

    // sd:/advert/####track name
    void playAdvertisement(uint16_t track)
    {
      if (df_playing) {
        df_playing_adv = true;
        df_adv_track = track;
        df_playing_adv_counter = 3;
      }
    }

    // test functions
    bool called_end_track = false;
    void end_track() {
      called_end_track = true;
    }
    bool called_end_adv = false;
    void end_adv() {
      if (df_playing_adv) {
        df_playing_adv = false;
        df_adv_track = 0;
        called_end_adv = true;
        df_playing_adv_counter = 0;
      }
    }
    uint16_t error_code = 0;
    void set_error(uint16_t code) {
      error_code = code;
    }
    uint16_t df_folder_track_count[0xff] = { 0 };
    void set_folder_track_count(uint8_t folder, uint16_t count) {
      df_folder_track_count[folder] = count;
    }
    bool is_playing_folder() {
      return df_playing && !df_playing_adv && df_folder_track > 0;
    }
    bool is_playing_mp3() {
      return df_playing && !df_playing_adv && df_mp3_track > 0;
    }
    bool is_playing_adv() {
      return df_playing && df_playing_adv && df_adv_track > 0;
    }
    bool is_pause() {
      return !df_playing && !df_stopped && df_folder_track > 0;
    }
    bool is_stopped() {
      return df_stopped;
    }

    void reset_to_initial_state() {
      df_playing = false;
      df_playing_adv = false;
      df_playing_adv_counter = 0;
      df_stopped = true;
      df_mp3_track = 0;
      df_folder = 0;
      df_folder_track = 0;
      df_adv_track = 0;
      called_sleep = false;
      called_start = false;
      called_pause = false;
      called_stop = false;
    }

};

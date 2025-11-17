#ifndef SRC_STATE_MACHINE_HPP_
#define SRC_STATE_MACHINE_HPP_

#include "tinyfsm.hpp"
#include "commands.hpp"
#include "chip_card.hpp"
#include "mp3.hpp"
#include "timer.hpp"

class Tonuino;

// ----------------------------------------------------------------------------
// Event Declarations
//
struct command_e: tinyfsm::Event {
  command_e(commandRaw cmd_raw):cmd_raw{cmd_raw} {}
  commandRaw cmd_raw;
};
struct card_e  : tinyfsm::Event {
  card_e(cardEvent card_ev): card_ev{card_ev} {}
  cardEvent card_ev;
};

// ----------------------------------------------------------------------------
// State Machine Base Class Declaration
//
class finished_setupCard      ;
class finished_abort_setupCard;
class finished_writeCard      ;
class finished_abort_writeCard;
class Idle                    ;

enum class SM_type: uint8_t {
  tonuino,
  setupCard,
  writeCard,
};
template<SM_type SMT>
class SM: public tinyfsm::Fsm<SM<SMT>>
{
public:
  typedef typename conditional<
             (SMT == SM_type::setupCard), finished_setupCard,
          typename conditional<
             (SMT == SM_type::writeCard), finished_writeCard,
          Idle>::type
  >::type finished;
  typedef typename conditional<
             (SMT == SM_type::setupCard), finished_abort_setupCard,
          typename conditional<
             (SMT == SM_type::writeCard), finished_abort_writeCard,
          Idle>::type
  >::type finished_abort;

  virtual void react(command_e const &) { };
  virtual void react(card_e    const &) { };

  virtual void entry(void) { };
  void         exit (void) { waitForPlayFinish = false; };

  bool isAbort(command cmd);

  static folderSettings folder;
  static const __FlashStringHelper* state_str;
protected:
  static Timer          timer;
  static bool           waitForPlayFinish; // with this it needs 66 Byte lesser program code ;-)
};

using SM_tonuino   = SM<SM_type::tonuino  >;
using SM_setupCard = SM<SM_type::setupCard>;
using SM_writeCard = SM<SM_type::writeCard>;

class Base: public SM_tonuino
{
protected:
  bool readCard();
  bool handleShortcut(uint8_t shortCut);
  void handleReadCard();
  bool checkForShortcutAndShutdown(command cmd);
#ifdef TonUINO_Esp32
  bool checkForWritingCard(command cmd, command_e const &cmd_e);
  bool writingCard{};
#endif
#ifdef NEO_RING
  void handleBrightness(command cmd);
#endif
  static folderSettings lastCardRead;
};

class Idle: public Base
{
public:
  void entry() override;
  void react(command_e const &) override;
  void react(card_e    const &) override;
};

template<class P> class StartPlay: public Base
{
public:
  void entry() override;
  void react(command_e const &) override;
};

class Play: public Base
{
public:
  void entry() override;
  void react(command_e const &) override;
  void react(card_e    const &) override;
};

class Pause: public Base
{
public:
  void entry() override;
  void react(command_e const &) override;
  void react(card_e    const &) override;
};

class Quiz: public Base
{
public:
  void entry() override;
  void react(command_e const &) override;
  void react(card_e    const &) override;
private:
  enum class QuizState: uint8_t {
    playQuestion,
    playAnswer,
    playSolution,
    playWeiter,
  };

  void finish();

  uint8_t   numAnswer    {};
  uint8_t   numSolution  {};
  QuizState quizState    {};
  uint8_t   question     {};
  uint8_t   trackQuestion{};
  uint8_t   numQuestion  {};
  uint8_t   actAnswer    {};
  queue<uint8_t, 4>     a{};
  bitfield<128>         r{}; // max in buzzer mode (num answer = 0, num solution = 1)
  uint8_t remainingQuestions{};
  static constexpr long timeout{ 5 * 60 * 1000l};
};

class Memory: public Base
{
public:
  void entry() override;
  void react(command_e const &) override;
  void react(card_e    const &) override;
private:

  void finish();

  uint8_t     first {};
  uint8_t     second{};
  static constexpr long timeout{ 5 * 60 * 1000l};
};

// ----------------------------------------------------------------------------
// State Machine end states
//
class finished_setupCard      : public SM_setupCard{};
class finished_abort_setupCard: public SM_setupCard{};
class finished_writeCard      : public SM_writeCard{};
class finished_abort_writeCard: public SM_writeCard{};

// ----------------------------------------------------------------------------
// State Declarations
//
template<SM_type SMT>
class VoiceMenu : public SM<SMT>
{
protected:
  void entry(bool entryPlayAfter = false);
  void react(command cmd);
  void playCurrentValue();

  static uint8_t   numberOfOptions  ;
  static mp3Tracks startMessage     ;
  static mp3Tracks messageOffset    ;
  static bool      preview          ;
  static uint8_t   previewFromFolder;
  static uint8_t   currentValue     ;

  static bool      previewStarted   ;
};

using VoiceMenu_tonuino   = VoiceMenu<SM_type::tonuino  >;
using VoiceMenu_setupCard = VoiceMenu<SM_type::setupCard>;

class ChMode : public VoiceMenu_setupCard
{
public:
  void entry() final;
  void react(command_e const &) final;
};

class ChFolder : public VoiceMenu_setupCard
{
public:
  void entry() final;
  void react(command_e const &) final;
};

class ChTrack : public VoiceMenu_setupCard
{
public:
  void entry() final;
  void react(command_e const &) final;
};

class ChFirstTrack : public VoiceMenu_setupCard
{
public:
  void entry() final;
  void react(command_e const &) final;
};

class ChLastTrack : public VoiceMenu_setupCard
{
public:
  void entry() final;
  void react(command_e const &) final;
};

class ChNumAnswer : public VoiceMenu_setupCard
{
public:
  void entry() final;
  void react(command_e const &) final;
};

class ChNumTracks : public VoiceMenu_setupCard
{
public:
  void entry() final;
  void react(command_e const &) final;
};

class WriteCard : public SM_writeCard
{
public:
  void entry() final;
  void react(command_e const &) final;
private:
  enum subState: uint8_t {
    start_waitCardInserted,
    run_writeCard,
    end_writeCard,
    run_waitCardRemoved,
  };
  subState current_subState{};
};

class Admin_BaseSetting: public VoiceMenu_tonuino
{
protected:
  void saveAndTransit();
};

class Amin_BaseWriteCard: public VoiceMenu_tonuino {
protected:
  bool handleWriteCard(command_e const &cmd_e, bool return_to_idle = false);
};

class Admin_Allow: public VoiceMenu_tonuino
{
public:
  void entry() final;
  void react(command_e const &) final;
private:
  enum subState: uint8_t {
    select_method,
    get_pin,
//    start_match,
//    play_match_intro,
//    play_match_a,
//    play_match_operation,
//    play_match_b,
//    get_match_c,
    allow,
    not_allow,
  };
  subState        current_subState{};
  Settings::pin_t pin             {};
  uint8_t         pin_number      {};
//  uint8_t         av{}, bv{}, cv{};
};

class Admin_Entry: public VoiceMenu_tonuino
{
public:
  void entry() final;
  void react(command_e const &) final;
  static uint8_t lastCurrentValue;
};

class Admin_NewCard: public Amin_BaseWriteCard
{
public:
  void entry() final;
  void react(command_e const &) final;
  static bool return_to_idle;
  static bool wait_track_finished;
private:
  enum subState: uint8_t {
    wait_track,
    start_setupCard,
    run_setupCard,
    end_setupCard,
    start_writeCard,
    run_writeCard,
  };
  subState current_subState{};
};

class Admin_SimpleSetting: public Admin_BaseSetting
{
public:
  void entry() final;
  void react(command_e const &) final;
  enum Type: uint8_t {
    maxVolume,
    minVolume,
    initVolume,
    eq,
  };
  static Type type;
};

class Admin_ModCard: public Amin_BaseWriteCard
{
public:
  void entry() final;
  void react(command_e const &) final;
private:
  enum subState: uint8_t {
    get_mode,
    get_sleeptime_timer,
    get_sleeptime_mode,
    get_play_time,
    start_writeCard,
    run_writeCard,
  };
  subState current_subState{};
};

class Admin_ShortCut: public Admin_BaseSetting
{
public:
  void entry() final;
  void react(command_e const &) final;
private:
  enum subState: uint8_t {
    start_setupCard,
    run_setupCard,
    end_setupCard,
  };
  subState current_subState{};
  uint8_t  shortcut        {};
};

class Admin_StandbyTimer: public Admin_BaseSetting
{
public:
  void entry() final;
  void react(command_e const &) final;
};

class Admin_CardsForFolder: public Amin_BaseWriteCard
{
public:
  void entry() final;
  void react(command_e const &) final;
private:
  enum subState: uint8_t {
    start_setupCard,
    run_setupCard,
    end_setupCard,
    prepare_writeCard,
    start_writeCard,
    run_writeCard,
  };
  subState current_subState{};
  uint8_t special          {};
  uint8_t special2         {};
};

class Admin_InvButtons: public Admin_BaseSetting
{
public:
  void entry() final;
  void react(command_e const &) final;
};

class Admin_ResetEeprom: public SM_tonuino
{
public:
  void entry() final;
  void react(command_e const &) final;
};

class Admin_LockAdmin: public Admin_BaseSetting
{
public:
  void entry() final;
  void react(command_e const &) final;
private:
  enum subState: uint8_t {
    get_mode,
    get_pin,
    finished,
  };
  subState        current_subState{};
  Settings::pin_t pin             {};
  uint8_t         pin_number      {};
};

class Admin_PauseIfCardRemoved: public Admin_BaseSetting
{
public:
  void entry() final;
  void react(command_e const &) final;
};

//#ifdef MEMORY_GAME
class Admin_MemoryGameCards: public Amin_BaseWriteCard
{
public:
  void entry() final;
  void react(command_e const &) final;
private:
  enum subState: uint8_t {
    prepare_writeCard,
    start_writeCard,
    run_writeCard,
  };
  subState current_subState{};
};
//#endif


#endif /* SRC_STATE_MACHINE_HPP_ */

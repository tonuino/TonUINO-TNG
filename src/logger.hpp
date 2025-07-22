#ifndef SRC_LOGGER_HPP_
#define SRC_LOGGER_HPP_

#include "log.hpp"

DEFINE_LOGGER(tonuino_log   , s_debug  , void);

#ifndef ALLinONE
DEFINE_LOGGER(init_log      , s_info   , tonuino_log);
DEFINE_LOGGER(card_log      , s_info   , tonuino_log);
DEFINE_LOGGER(play_log      , s_info   , tonuino_log);
DEFINE_LOGGER(standby_log   , s_info   , tonuino_log);
DEFINE_LOGGER(state_log     , s_info   , tonuino_log);
DEFINE_LOGGER(button_log    , s_info   , tonuino_log);
DEFINE_LOGGER(modifier_log  , s_info   , tonuino_log);
DEFINE_LOGGER(mp3_log       , s_info   , tonuino_log);
DEFINE_LOGGER(settings_log  , s_info   , tonuino_log);
DEFINE_LOGGER(batvol_log    , s_info   , tonuino_log);
DEFINE_LOGGER(webserv_log   , s_info   , tonuino_log);
DEFINE_LOGGER(ledManager_log, s_info   , tonuino_log);
#else
DEFINE_LOGGER(init_log      , s_info   , tonuino_log);
DEFINE_LOGGER(card_log      , s_warning, tonuino_log);
DEFINE_LOGGER(play_log      , s_info   , tonuino_log);
DEFINE_LOGGER(standby_log   , s_warning, tonuino_log);
DEFINE_LOGGER(state_log     , s_info   , tonuino_log);
DEFINE_LOGGER(button_log    , s_info   , tonuino_log);
DEFINE_LOGGER(modifier_log  , s_info   , tonuino_log);
DEFINE_LOGGER(mp3_log       , s_info   , tonuino_log);
DEFINE_LOGGER(settings_log  , s_info   , tonuino_log);
DEFINE_LOGGER(batvol_log    , s_none   , tonuino_log);
DEFINE_LOGGER(ledManager_log, s_info   , tonuino_log);
#endif

#endif /* SRC_LOGGER_HPP_ */

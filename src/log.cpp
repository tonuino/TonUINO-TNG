#include "log.hpp"

#ifdef TonUINO_Esp32
Webserial  webserial{};
bool logger::printTimestamp = true;
#endif

const __FlashStringHelper* getSeverityName(severity sev) {
  switch (sev) {
  case s_debug  : return F("D");
  case s_info   : return F("I");
  case s_warning: return F("W");
  case s_error  : return F("E");
  case s_none   : return F("?");
  }
  return F("unknown");
}
 

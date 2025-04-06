#ifndef SRC_LOG_HPP_
#define SRC_LOG_HPP_

#include <Arduino.h>

#include "constants.hpp"
#include "type_traits.hpp"

#ifdef TonUINO_Esp32
#include "webserial.hpp"
extern Webserial  webserial;
#endif

#define DEFINE_LOGGER(Logger_, MinSeverity_, Forwarder_)                         \
  struct Logger_ : public logger_base<Logger_, MinSeverity_, Forwarder_>         \
  { static __FlashStringHelper const* name() {return F(#Logger_);} }

#define LOG(Logger_, Severity_, Expression_...)                                  \
  if constexpr ( Logger_::will_log(Severity_) )                                  \
    Logger_::template log< Severity_ >(Logger_::name(), Expression_)

#define LOG_CODE(Logger_, Severity_, Statement)                                  \
  if constexpr ( Logger_::will_log(Severity_) )                                  \
    Statement

enum severity: uint8_t {
  s_debug  ,
  s_info   ,
  s_warning,
  s_error  ,
  s_none   ,
};
enum lineFeed: uint8_t {
  lf_yes,
  lf_no ,
};

extern const __FlashStringHelper* getSeverityName(severity sev);

class logger {
public:
  constexpr static bool will_log(severity) {
    return true;
  }

  static void log(lineFeed lf = lf_yes) {
    if (lf == lf_yes) {
      Serial.println();
#ifdef TonUINO_Esp32
      printTimestamp = true;
      webserial.println();
#endif
    }
  }
  template<typename T, typename ... Types>
  static void log(T t, Types ... types) {
    Serial.print(t);
#ifdef TonUINO_Esp32
    webserial.print(t);
#endif
    log(types...);
  }

  template<severity Severity, typename ... Types>
  static void log(const __FlashStringHelper* /*logname*/, Types ... types) {
//    Serial.print(millis());
//    Serial.print(F("-"));
//    Serial.print(getSeverityName(Severity));
//    Serial.print(F("-"));
//    Serial.print(logname);
//    Serial.print(F(": "));
#ifdef TonUINO_Esp32
    if (printTimestamp) {
      int64_t mseconds = esp_timer_get_time()  / 1000ULL;
      int64_t seconds = mseconds  / 1000ULL;
      String minsStr = "00" + String(seconds / 60 %100);
      minsStr = minsStr.substring(minsStr.length()-3);
      String secondsStr = "0" + String(seconds % 60);
      secondsStr = secondsStr.substring(secondsStr.length()-2);
      String msecondsStr = "00" + String(mseconds % 1000ULL);
      msecondsStr = msecondsStr.substring(msecondsStr.length()-3);

      webserial.print("[" + minsStr + " " + secondsStr + "." + msecondsStr + "] ");
      printTimestamp = false;
    }
#endif
    log(types...);
  }
private:
#ifdef TonUINO_Esp32
  static bool printTimestamp;
#endif

};

template<typename Derived, severity MinSeverity, class FwdLogger>
class logger_base {
public:
  typedef typename if_<is_same_type<FwdLogger, void>::value, logger, FwdLogger>::result_type forward_logger_type;

  static constexpr bool will_log(severity s) {
    return (s >= MinSeverity) && forward_logger_type::will_log(s);
  }

  template<severity Severity, typename ... Types>
  static void log(const __FlashStringHelper* logname, Types ... types) {
    forward_logger_type::template log<Severity>(logname, types...);
  }
};

#endif /* SRC_LOG_HPP_ */

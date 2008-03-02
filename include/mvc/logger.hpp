#ifndef __mvc_logger_hpp__
#define __mvc_logger_hpp__

#include <string>
#include <sstream>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <iostream>

namespace mvc
{
  class logger
  {
    public:
      enum log_level
      {
        debug = 0,
        info,
        notice,
        warning,
        error,
        critical,
        alert,
        emergency
      };

      void log(log_level ll, const std::string &module, const std::string &message)
      {
        std::stringstream ss;

        ss << "[" << boost::posix_time::microsec_clock::universal_time() << "] (" << to_string(ll) << ") <" << module << "> " << message << std::endl;

        std::cerr << ss.str();
      }

    private:
      std::string to_string(log_level ll)
      {
        switch (ll)
        {
          case debug: return "debug";
          case info: return "info";
          case notice: return "notice";
          case warning: return "warning";
          case error: return "error";
          case critical: return "critical";
          case alert: return "alert";
          case emergency: return "emergency";
          default: return "debug";
        }
      }

  };
}

#endif

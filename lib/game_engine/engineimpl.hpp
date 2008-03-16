#ifndef __game_engine_engineimpl_hpp__
#define __game_engine_engineimpl_hpp__

#include "game_personality/engine.hpp"
#include "mvc/logger.hpp"

namespace game_engine
{
  class engineimpl : public game_personality::engine
  {
    public: 
      engineimpl(const boost::function<void (mvc::logger::log_level, 
            const std::string &, const std::string &)> &t_logger, 
          game_personality::view &v, 
          game_personality::world &w);

    private:
      boost::function<void (mvc::logger::log_level, const std::string &)> m_logger;
  };
}

#endif

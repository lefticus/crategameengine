#include "engineimpl.hpp"

namespace game_engine
{
  engineimpl::engineimpl(
      const boost::function<void (mvc::logger::log_level, const std::string &, const std::string &)> 
      &t_logger, game_personality::personality::view &v, game_personality::personality::world &w)
    : game_personality::personality::engine(t_logger, v, w),
      m_logger(boost::bind(t_logger, _1, "engineimpl", _2))
  {
    m_logger(mvc::logger::debug, "constructor called");
  }
}

extern "C" game_personality::personality::engine* create(
    const boost::function<void (mvc::logger::log_level,
      const std::string&, const std::string &)> &t_logger,
    game_personality::personality::view &t_v,
    game_personality::personality::world &t_w)
{
  return new game_engine::engineimpl(t_logger, t_v, t_w);
}

extern "C" void destroy(game_personality::personality::engine *t_w)
{
  delete t_w;
}


#include "viewstub.hpp"

namespace game_viewers
{
  viewstub::viewstub(
      const boost::function<void (mvc::logger::log_level, const std::string &, const std::string &)> 
      &t_logger)
    : game_personality::view(t_logger),
      m_logger(boost::bind(t_logger, _1, "viewstub", _2))
  {
    m_logger(mvc::logger::debug, "constructor called");
  }

  void viewstub::before_world_changed(const game_personality::event_world_changed&)
  {
  }

  void viewstub::after_world_changed(const game_personality::event_world_changed&)
  {
  }

  void viewstub::character_response(const game_personality::event_character_response&)
  {
  }

  void viewstub::character_speak(const game_personality::event_character_speak&)
  {
  }

  void viewstub::announcement(const game_personality::event_announcement &e)
  {
    m_logger(mvc::logger::debug, "announcement event: " + e.messages[0]);
  }
}

extern "C" game_personality::view* create(
    const boost::function<void (mvc::logger::log_level,
      const std::string&, const std::string &)> &t_logger)
{
  return new game_viewers::viewstub(t_logger);
}

extern "C" void destroy(game_personality::view *t_w)
{
  delete t_w;
}


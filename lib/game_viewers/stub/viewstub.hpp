#ifndef __game_viewers_viewstub_hpp__
#define __game_viewers_viewstub_hpp__

#include "game_personality/personality.hpp"
#include "mvc/logger.hpp"

namespace game_viewers
{
  class viewstub : public game_personality::personality::view
  {
    public:
      viewstub(const boost::function<void (mvc::logger::log_level, const std::string &, const std::string &)> 
          &t_logger);

    private:
      boost::function<void (mvc::logger::log_level, const std::string &)> m_logger;

      void before_world_changed(const mvc::event_world_changed<game_personality::personality::change_set>&);

      void after_world_changed(const mvc::event_world_changed<game_personality::personality::change_set>&);

      void character_response(const game_personality::event_character_response&);

      void character_speak(const game_personality::event_character_speak&);

      void announcement(const game_personality::event_announcement &e);
  };
}
#endif

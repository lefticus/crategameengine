#ifndef __game_viewers_viewstub_hpp__
#define __game_viewers_viewstub_hpp__

#include "game_personality/view.hpp"
#include "mvc/logger.hpp"

namespace game_viewers
{
  class viewstub : public game_personality::view
  {
    public:
      viewstub(const boost::function<void (mvc::logger::log_level, 
            const std::string &, const std::string &)> 
          &t_logger);

    private:
      boost::function<void (mvc::logger::log_level, const std::string &)> m_logger;

      void before_world_changed(const game_personality::event_world_changed &);

      void after_world_changed(const game_personality::event_world_changed &);

      void character_response(const game_personality::event_character_response &);

      void character_speak(const game_personality::event_character_speak&);

      void announcement(const game_personality::event_announcement &e);
  };
}
#endif

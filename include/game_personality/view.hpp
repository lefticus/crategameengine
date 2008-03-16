#ifndef __game_personality_view_hpp_
#define __game_personality_view_hpp_

#include "mvc/event.hpp"
#include "events.hpp"
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <string>
#include "world_change_set.hpp"
#include "world_view_access.hpp"

namespace game_personality
{
  class view : public mvc::event_emitter<event_take_item>,
               public mvc::event_emitter<event_use_item>,
               public mvc::event_emitter<event_use_item_with>,
               public mvc::event_emitter<event_talk_to_character>,
               public mvc::event_emitter<event_ask_character>,
               public mvc::event_emitter<event_move_to>,
               public mvc::event_listener<event_character_response>,
               public mvc::event_listener<event_character_speak>,
               public mvc::event_listener<event_announcement>,
               public mvc::event_listener<event_world_changed>,
               public mvc::event_handler
    {
    public: 
      view(const boost::function<void (mvc::logger::log_level, const std::string &, const std::string &)> 
                 &t_logger)
        : mvc::event_listener<event_character_response>(
              boost::bind(&view::queue_event, this, _1), boost::bind(&view::character_response, this, _1)),
          mvc::event_listener<event_character_speak>(
              boost::bind(&view::queue_event, this, _1), boost::bind(&view::character_speak, this, _1)),
          mvc::event_listener<event_announcement>(
              boost::bind(&view::queue_event, this, _1), boost::bind(&view::announcement, this, _1)), 
          mvc::event_listener<event_world_changed>(
              boost::bind(&view::queue_event, this, _1), boost::bind(&view::handle_world_changed, this, _1)),
          mvc::event_handler(boost::bind(t_logger, _1, "mvc::view::event_handler", _2)),
          m_logger(boost::bind(t_logger, _1, "game_personality::view", _2))
      {
      }

    private:
      boost::function<void (mvc::logger::log_level, const std::string &)> m_logger;

      world_view_access m_wvi;

      virtual void character_response(const event_character_response &e) = 0;
      virtual void character_speak(const event_character_speak &e) = 0;
      virtual void announcement(const event_announcement &e) = 0;
      virtual void before_world_changed(const event_world_changed &e) = 0;
      virtual void after_world_changed(const event_world_changed &e) = 0;

      void handle_world_changed(const event_world_changed &e)
      {
        before_world_changed(e);
        m_wvi.apply_change_set(e.change_set);
        after_world_changed(e);
      }
  };
}

#endif

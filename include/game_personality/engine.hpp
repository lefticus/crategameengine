#ifndef __mvc_engine__
#define __mvc_engine__

#include "mvc/logger.hpp"
#include "script.hpp"
#include "compiled_script.hpp"
#include "mvc/event.hpp"
#include "view.hpp"
#include "world.hpp"

namespace game_personality
{

  struct engine : public mvc::event_listener<event_take_item>,
                  public mvc::event_listener<event_use_item>,
                  public mvc::event_listener<event_use_item_with>,
                  public mvc::event_listener<event_talk_to_character>,
                  public mvc::event_listener<event_ask_character>,
                  public mvc::event_listener<event_move_to>,
                  public mvc::event_emitter<event_run_script>,
                  public mvc::event_emitter<event_run_named_script>,
                  public mvc::event_handler
  {
    engine(const boost::function<void (mvc::logger::log_level, const std::string &, const std::string &)> 
                 &t_logger, view &v, world &w)
      : mvc::event_listener<event_take_item>(
          boost::bind(&engine::queue_event, this, _1), boost::bind(&engine::take_item, this, _1)),
        mvc::event_listener<event_use_item>(
          boost::bind(&engine::queue_event, this, _1), boost::bind(&engine::use_item, this, _1)),
        mvc::event_listener<event_use_item_with>(
          boost::bind(&engine::queue_event, this, _1), boost::bind(&engine::use_item_with, this, _1)),
        mvc::event_listener<event_talk_to_character>(
          boost::bind(&engine::queue_event, this, _1), boost::bind(&engine::talk_to_character, this, _1)),
        mvc::event_listener<event_ask_character>(
          boost::bind(&engine::queue_event, this, _1), boost::bind(&engine::ask_character, this, _1)),
        mvc::event_listener<event_move_to>(
          boost::bind(&engine::queue_event, this, _1), boost::bind(&engine::move_to, this, _1)),
        mvc::event_handler(boost::bind(t_logger, _1, "mvc::view::event_handler", _2)),
        m_logger(boost::bind(t_logger, _1, "game_personality::engine", _2))
    {
      m_logger(mvc::logger::debug, "constructor called");

      w.register_script_handler(compiled_script_handler(t_logger));

      w.register_script(compiled_script("use_item",
            boost::bind(&engine::use_item_impl, this, _1, _2, _3)));
      w.register_script(compiled_script("use_item_with",
            boost::bind(&engine::use_item_with_impl, this, _1, _2, _3)));
      w.register_script(compiled_script("take_item",
            boost::bind(&engine::take_item_impl, this, _1, _2, _3)));
      w.register_script(compiled_script("talk_to_character",
            boost::bind(&engine::talk_to_character_impl, this, _1, _2, _3)));
      w.register_script(compiled_script("ask_character",
            boost::bind(&engine::ask_character_impl, this, _1, _2, _3)));
      w.register_script(compiled_script("move_to",
            boost::bind(&engine::move_to_impl, this, _1, _2, _3)));


      mvc::attach<event_run_script>(*this, w);
      mvc::attach<event_run_named_script>(*this, w);
      mvc::attach<event_world_changed>(w, v);
      mvc::attach<event_take_item>(v, *this);
      mvc::attach<event_use_item>(v, *this);
      mvc::attach<event_use_item_with>(v, *this);
      mvc::attach<event_talk_to_character>(v, *this);
      mvc::attach<event_ask_character>(v, *this);
      mvc::attach<event_move_to>(v, *this);
      mvc::attach<event_character_response>(w, v);
      mvc::attach<event_character_speak>(w, v);
      mvc::attach<event_announcement>(w, v);
    }  

     void ask_character_impl(world_script_access &w,
        const std::vector<mvc::object_id_base> &objects, 
        const std::vector<std::string> &strings)
    {
      m_logger(mvc::logger::debug, "ask_character_impl called");
      std::vector<std::string> messages;
      messages.push_back("blah");
      w.emit(event_announcement(messages));
    }

    void talk_to_character_impl(world_script_access &,
        const std::vector<mvc::object_id_base> &objects, 
        const std::vector<std::string> &strings)
    {
      m_logger(mvc::logger::debug, "talk_to_character_impl called");
    }

    void move_to_impl(world_script_access &,
        const std::vector<mvc::object_id_base> &objects, 
        const std::vector<std::string> &strings)
    {
      m_logger(mvc::logger::debug, "move_to_impl called");
    }

    void take_item_impl(world_script_access &,
        const std::vector<mvc::object_id_base> &objects, 
        const std::vector<std::string> &strings)
    {
      m_logger(mvc::logger::debug, "take_item_impl called");
    }

    void use_item_with_impl(world_script_access &,
        const std::vector<mvc::object_id_base> &objects, 
        const std::vector<std::string> &strings)
    {
      m_logger(mvc::logger::debug, "use_item_with_impl called");
    }


    void use_item_impl(world_script_access &,
        const std::vector<mvc::object_id_base> &objects, 
        const std::vector<std::string> &strings)
    {
      m_logger(mvc::logger::debug, "use_item_impl called");
    }

    void take_item(const event_take_item &e)
    {
      m_logger(mvc::logger::info, "take_item event called");
      std::vector<mvc::object_id_base> objects;     
      objects.push_back(e.m_player);
      objects.push_back(e.m_item);
      mvc::emit<event_run_named_script>(*this, 
          event_run_named_script("take_item", objects, std::vector<std::string>()));
    }

    void use_item(const event_use_item &e)
    {
      m_logger(mvc::logger::info, "use_item event called");
      std::vector<mvc::object_id_base> objects;
      objects.push_back(e.m_player);
      objects.push_back(e.m_item);
      mvc::emit<event_run_named_script>(*this, 
          event_run_named_script("use_item", objects, std::vector<std::string>()));
    }

    void use_item_with(const event_use_item_with &e)
    {
      m_logger(mvc::logger::info, "use_item_with event called");
      std::vector<mvc::object_id_base> objects;
      objects.push_back(e.m_player);
      objects.push_back(e.m_item);
      objects.push_back(e.m_item_with);
      mvc::emit<event_run_named_script>(*this, 
          event_run_named_script("use_item_with", objects, std::vector<std::string>()));
    }


    void talk_to_character(const event_talk_to_character &e)
    {
      m_logger(mvc::logger::info, "talk_to_character event called");
      std::vector<mvc::object_id_base> objects;
      objects.push_back(e.m_player);
      objects.push_back(e.m_character);
      mvc::emit<event_run_named_script>(*this, 
          event_run_named_script("talk_to_character", objects, std::vector<std::string>()));
    }

    void ask_character(const event_ask_character &e)
    {
      m_logger(mvc::logger::info, "ask_character event called");
      std::vector<mvc::object_id_base> objects;
      objects.push_back(e.m_player);
      objects.push_back(e.m_character);
      std::vector<std::string> strings;
      strings.push_back(e.m_question);
      mvc::emit<event_run_named_script>(*this, 
          event_run_named_script("ask_character", objects, strings));
    }

    void move_to(const event_move_to &e)
    {
      m_logger(mvc::logger::info, "move_to event called");
      std::vector<mvc::object_id_base> objects;
      objects.push_back(e.m_player);
      objects.push_back(e.m_place);
      mvc::emit<event_run_named_script>(*this, 
          event_run_named_script("move_to", objects, std::vector<std::string>()));
    }

    boost::function<void (mvc::logger::log_level, const std::string &)> m_logger;
  };
    
    
    
      
      
      
}

#endif

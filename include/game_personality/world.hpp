#ifndef __mvc_world__
#define __mvc_world__

#include "mvc/logger.hpp"
#include "script.hpp"
#include "mvc/event.hpp"
#include "mvc/object_id.hpp"
#include <boost/shared_ptr.hpp>
#include "world_exceptions.hpp"
#include "world_reader.hpp"
#include "world_creator.hpp"
#include "world_script_access.hpp"
#include "world_change_set.hpp"

namespace game_personality
{
  class world : public world_reader,
                public world_creator,
                public mvc::event_emitter<event_character_response>,
                public mvc::event_emitter<event_character_speak>,
                public mvc::event_emitter<event_announcement>,
                public mvc::event_emitter<event_world_changed>,
                public mvc::event_listener<event_run_script>,
                public mvc::event_listener<event_run_named_script>,
                public mvc::event_handler
  {
    public: 

      friend class world_script_access;

      world(const boost::function<void (mvc::logger::log_level, const std::string &, const std::string &)> 
                 &t_logger)
        : mvc::event_listener<event_run_script>(
              boost::bind(&world::queue_event, this, _1), boost::bind(&world::execute_script, this, _1)),
          mvc::event_listener<event_run_named_script>(
              boost::bind(&world::queue_event, this, _1), boost::bind(&world::execute_named_script, this, _1)),
          mvc::event_handler(boost::bind(t_logger, _1, "mvc::world::event_handler", _2)),
          m_logger(boost::bind(t_logger, _1, "game_personality::world", _2))
      {
      }

      void register_script_handler(const script_handler &t_sh)
      {
        m_script_handlers[t_sh.name] = boost::shared_ptr<script_handler>(t_sh.clone());
      }

      void register_script(const script &t_s)
      {
        m_script_handlers[t_s.type]->register_named_script(t_s);
        m_named_scripts[t_s.name] = t_s.type;
      }

      virtual void apply_change_set(const world_change_set &cs) = 0;

    protected:
      void execute_named_script(const event_run_named_script &e)
      {
        m_logger(mvc::logger::info, "execute named script event: " + e.name);
        execute_named_script(e.name, e.objects, e.strings);
      }

      void execute_script(const event_run_script &e)
      {
        execute_script(e.script_to_run, e.objects, e.strings);
      }

      void execute_named_script(const std::string &name, 
          const std::vector<mvc::object_id_base> &objects,
          const std::vector<std::string> &strings)
      {
        std::map<std::string, std::string>::const_iterator itr = 
          m_named_scripts.find(name);

        if (itr == m_named_scripts.end())
        {
          throw unknown_named_script(name);
        }

        world_script_access wsi(*this);
        m_script_handlers[itr->second]->execute_named_script(name, wsi, objects, strings);
        world_change_set cs = wsi.get_change_set();
        apply_change_set(cs);
        mvc::emit<event_world_changed>(*this, 
          event_world_changed(cs));
      }

      void execute_script(const script &script_to_run,
          const std::vector<mvc::object_id_base> &objects,
          const std::vector<std::string> &strings)
      {
        world_script_access wsi(*this);
        m_script_handlers[script_to_run.type]->execute(script_to_run, wsi, objects, strings);
        world_change_set cs = wsi.get_change_set();
        apply_change_set(cs);
        mvc::emit<event_world_changed>(*this, 
          event_world_changed(cs));
      }

      std::map<std::string, boost::shared_ptr<script_handler> > m_script_handlers;
      std::map<std::string, std::string> m_named_scripts;

    private:
      boost::function<void (mvc::logger::log_level, const std::string &)> m_logger;
  };


}

#endif

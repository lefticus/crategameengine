#ifndef __mvc_world__
#define __mvc_world__

#include "logger.hpp"
#include <script.hpp>
#include <event.hpp>
#include <mvc_events.hpp>
#include <boost/shared_ptr.hpp>

namespace mvc
{
  struct object_not_found : std::runtime_error
  {
    object_not_found(const object_id_base &t_oid)
      : std::runtime_error("object not found"),
        oid(t_oid)
    {
    }

    virtual ~object_not_found() throw()
    {
    }

    object_id_base oid;
  };

  template<typename T>
  class world : 
      public event_listener<event_run_script>,
      public event_listener<event_run_named_script>,
      public event_emitter<event_world_changed<typename T::change_set> >,
      public event_handler
  {  
    public:
      typedef T world_personality;
      friend class world_personality::world_script_access;

      world(const boost::function<void (logger::log_level, const std::string &, const std::string &)> 
                   &t_logger)
        : event_listener<event_run_script>(
            boost::bind(&world::queue_event, this, _1), boost::bind(&world::execute_script, this, _1)),
          event_listener<event_run_named_script>(
            boost::bind(&world::queue_event, this, _1), boost::bind(&world::execute_named_script, this, _1)),
          m_logger(boost::bind(t_logger, _1, "mvc::world", _2))
      {
      }

      virtual ~world()
      {
      }

      void register_script_handler(const script_handler<T> &t_sh)
      {
        m_script_handlers[t_sh.name] = boost::shared_ptr<script_handler<T> >(t_sh.clone());
      }

      void register_script(const script &t_s)
      {
        m_script_handlers[t_s.type].register_named_script(t_s);
        m_named_scripts[t_s.name] = t_s.type;
      }

      virtual void apply_change_set(const typename world_personality::change_set &cs) = 0;

    protected:

      void execute_named_script(const event_run_named_script &e)
      {
        execute_named_script(e.name, e.objects, e.strings);
      }

      void execute_script(const event_run_script &e)
      {
        execute_script(e.script_to_run, e.objects, e.strings);
      }

      void execute_named_script(const std::string &name, 
                                const std::vector<object_id_base> &objects,
                                const std::vector<std::string> &strings)
      {
        typename world_personality::world_script_access wsi(*this);
        m_script_handlers[m_named_scripts[name] ]->execute_named_script(name, wsi, objects, strings);
        typename world_personality::change_set cs = wsi.get_change_set();
        apply_change_set(cs);
        emit(event_world_changed<typename world_personality::change_set>(cs));
      }

      void execute_script(const script &script_to_run,
                          const std::vector<object_id_base> &objects,
                          const std::vector<std::string> &strings)
      {
        typename world_personality::world_script_access wsi(*this);
        m_script_handlers[script_to_run.type]->execute(script_to_run, wsi, objects, strings);
        typename world_personality::change_set cs = wsi.get_change_set();
        apply_change_set(cs);
        emit(event_world_changed<typename world_personality::change_set>(cs));
      }

      std::map<std::string, boost::shared_ptr<script_handler<T> > > m_script_handlers;
      std::map<std::string, std::string> m_named_scripts;

    private:
      boost::function<void (logger::log_level, const std::string &)> m_logger;
  };
}

#endif

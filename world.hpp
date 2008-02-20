#ifndef __mvc_world__
#define __mvc_world__

#include <script.hpp>
#include <event.hpp>

namespace mvc
{
  template<typename T>
  class world : 
      public event_listener<event_run_script>,
      public event_listener<event_run_named_script>,
      public event_emitter<event_world_changed<typename T::change_set> >,
      public event_handler
  {  
    public:
      typedef T world_personality;

      world()
        : event_listener<event_run_named_script>(
            boost::bind(&world::queue_event, this), boost::bind(&world::execute_named_script, this, _1)),
          event_listener<event_run_script>(
            boost::bind(&world::queue_event, this), boost::bind(&world::execute_script, this, _1))
      {
      }

      void register_script_handler(const script_handler &t_sh)
      {
        m_script_handlers[sh.name] = boost::shared_ptr<script_handler>(t_sh.clone());
      }

      void register_script(const script &t_s)
      {
        m_script_handlers[s.type].register_named_script(t_s);
        m_named_scripts[s.name] = t_s.type;
      }

    protected:

      void execute_named_script(const event_run_named_script &e)
      {
        typename world_personality::world_script_interface wsi(*this);
        m_script_handlers[m_named_scripts[name] ].execute_named_script(e.name, wsi, e.objects);
        typename world_personality::change_set cs = wsi.get_change_set();
        apply_changeset(cs);
        emit(event_world_changed<typename world_personality::change_set>(cs));
      }

      void execute_script(const event_run_script &e)
      {
        typename world_personality::world_script_interface wsi(*this);
        m_script_handlers[s.type].execute(e.script, wsi, e.objects);
        typename world_personality::change_set cs = wsi.get_change_set();
        apply_changeset(cs);
        emit(event_world_changed<typename world_personality::change_set>(cs));

      }

  };
}

#endif

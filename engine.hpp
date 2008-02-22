#ifndef __mvc_engine__
#define __mvc_engine__

#include <script.hpp>
#include <event.hpp>
#include <mvc_events.hpp>
#include <view.hpp>
#include <world.hpp>

namespace mvc
{
  template<typename T>
  class engine : public event_emitter<event_run_script>,
                 public event_emitter<event_run_named_script>
  {
    public:
      typedef T world_personality;

      engine(view<T> &v, world<T> &w)
      {
        w.register_script_handler(compiled_script_handler<T>());
        attach<event_run_script>(*this, w);
        attach<event_run_named_script>(*this, w);
        attach<event_world_changed<typename T::change_set> >(w, v);
      }

    private:

    protected:
      typename world_personality::world_view_access m_wvi;
  };
}

#endif

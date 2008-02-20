#ifndef __mvc_engine__
#define __mvc_engine__

#include <script.hpp>
#include <event.hpp>

namespace mvc
{
  template<typename T>
  class engine : public event_emitter<run_script_event>,
                 public event_emitter<run_named_script_event>
  {
    public:
      typedef T world_personality;

      engine(view *v, world *w)
      {
        attach<run_script_event>(this, w);
        attach<run_named_script_event>(this, w);
        attach<world_changed_event<typename T::change_set> >(w, v);
      }

    private:

    protected:
      typename world_personality::world_view_interface m_wvi;
  };
}

#endif

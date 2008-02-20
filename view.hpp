#ifndef __mvc_view__
#define __mvc_view__

#include <script.hpp>
#include <event.hpp>

namespace mvc
{
  template<typename T>
    class view : public event_listener<event_world_changed<typename T::change_set> >,
                 public event_handler
  {
    public:
      typedef T world_personality;

      view()
        : event_listener<event_world_changed<typename T::change_set> >(
            boost::bind(&view::queue_event, this), boost::bind(&view::handle_world_changed, this, _1))
      {
      }

    private:
      void before_world_changed(const event_world_changed<typename T:change_set &e) = 0;
      void after_world_changed(const event_world_changed<typename T:change_set &e) = 0;

      handle_world_changed(const event_world_changed<typename T::change_set> &e)
      {
        before_world_changed(e);
        m_wvi.apply_change_set(e.change_set);
        after_world_changed(e);
      }

    protected:
      typename world_personality::world_view_interface m_wvi;
  };
}

#endif

#ifndef __mvc_engine__
#define __mvc_engine__

#include "logger.hpp"
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

      engine(const boost::function<void (logger::log_level, const std::string &, const std::string &)> 
                   &t_logger,
          view<T> &v, world<T> &w)
        : m_logger(boost::bind(t_logger, _1, "mvc::engine", _2))
      {
        w.register_script_handler(compiled_script_handler<T>());
        attach<event_run_script>(*this, w);
        attach<event_run_named_script>(*this, w);
        attach<event_world_changed<typename T::change_set> >(w, v);
      }

    private:
      boost::function<void (logger::log_level, const std::string &)> m_logger;

    protected:
      typename world_personality::world_view_access m_wvi;
  };
}

#endif

#ifndef __mvc_view__
#define __mvc_view__

#include "script.hpp"
#include "event.hpp"
#include "logger.hpp"

namespace mvc
{
  template<typename T>
  class view : public event_listener<
                        event_world_changed<typename T::change_set> >,
               public event_handler
  {
    public:
      typedef T world_personality;

      view(const boost::function<void (logger::log_level, const std::string &, const std::string &)> 
                   &t_logger)
        : event_listener<event_world_changed<typename T::change_set> >(
            boost::bind(&view::queue_event, this, _1), boost::bind(&view::handle_world_changed, this, _1)),
          event_handler(boost::bind(t_logger, _1, "mvc::view::event_handler", _2)),
          m_logger(boost::bind(t_logger, _1, "mvc::view", _2))
      {
      }

      virtual ~view()
      {
      }

    private:
      boost::function<void (logger::log_level, const std::string &)> m_logger;

      virtual void before_world_changed(const event_world_changed<typename T::change_set> &e) = 0;
      virtual void after_world_changed(const event_world_changed<typename T::change_set> &e) = 0;

      void handle_world_changed(const event_world_changed<typename T::change_set> &e)
      {
        before_world_changed(e);
        m_wvi.apply_change_set(e.change_set);
        after_world_changed(e);
      }

    protected:
      typename world_personality::world_view_access m_wvi;
  };
}

#endif

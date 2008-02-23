#ifndef __event__
#define __event__

#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <vector>
#include "queue.hpp"

namespace mvc
{
  class event_handler
  {
    public:
      ~event_handler()
      {
        m_queue.cancel_get();
        if (m_thread)
        {
          m_thread->join();
        }
      }

      void queue_event(const boost::function<void ()> &e)
      {
        m_queue.add(e);
      }

      void run()
      {
        while (true)
        {
          m_queue.get_next()();
        }
      }

      void start()
      {
        m_thread = boost::shared_ptr<boost::thread>(
            new boost::thread(boost::bind(&event_handler::run, this)));
      }

    private:
      queue<boost::function<void ()> > m_queue;
      boost::shared_ptr<boost::thread> m_thread;
  };

  template<typename event_type>
  class event_listener
  {
    public:
      typedef event_type type;

      event_listener(const boost::function<void (boost::function0<void>) > &queue, 
                     const boost::function<void (const type &)> &handler)
        : m_queue(queue), m_handler(handler)
      {
      }

      void handle_event(const type &t)
      {
        m_queue(boost::bind(m_handler, t));
      }

    private:
      boost::function<void (boost::function0<void>) > m_queue;
      boost::function<void (const type &)> m_handler;
  };

  template <typename event_type>
  class event_emitter
  {
    public:
      typedef event_type type;
      typedef std::vector<boost::function<void (const type &)> > listenerstype;

      void attach_listener(event_listener<type> &listener)
      {
        m_listeners.push_back(
            boost::bind(&event_listener<type>::handle_event, &listener, _1));
      }

      void emit(const type &event)
      {
        for(typename listenerstype::iterator itr = m_listeners.begin();
            itr != m_listeners.end();
            ++itr)
        {
          (*itr)(event);
        }
      }

    private:
      listenerstype m_listeners;
  };

  template<typename event_type>
  void attach(event_emitter<event_type> &emitter, 
              event_listener<event_type> &listener)
  {
    emitter.attach_listener(listener);
  }
}
#endif

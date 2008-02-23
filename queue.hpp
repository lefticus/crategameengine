#ifndef __mvc_queue__
#define __mvc_queue__

#include <boost/thread/condition.hpp>
#include <boost/thread/mutex.hpp>
#include <stdexcept>
#include <deque>

namespace mvc
{
  struct queue_read_canceled : public std::runtime_error
  {
    queue_read_canceled()
      : std::runtime_error("Queue read cancled during read operation")
    {
    }
  };

  //! Thread safe queue for message processing
  //! implemented entirely in this header file
  //! for the sake of template portability
  template <typename T>
    class queue 
    {
      //  typedef T Type;
      public:

        queue() 
          : m_canceled(false)
        {
        }

        ~queue()
        {
          cancel_get();
        }

        //! get the next available item in the message queue
        //! \param[out] item object to populate with next item
        //! \return true if item is populated false if not,
        //!         if the read was canceled before an item was
        //!         made available.
        T get_next() 
        {
          boost::mutex::scoped_lock l(m_mutex);

          if (m_canceled) {
            throw queue_read_canceled();
          }

          if (m_deque.empty()) {
            m_condition.wait(l);
          }

          if (m_deque.empty() || m_canceled) {
            throw queue_read_canceled();
          } else {
            T retitem = m_deque.front();
            m_deque.pop_front();
            return retitem;
          }
        }

        //! Cancel the current read operation 
        void cancel_get()
        {
          boost::mutex::scoped_lock l(m_mutex);

          m_canceled = true;
          m_condition.notify_one();
        }

        //! Reset canceled state to false
        void reset()
        {
          m_canceled = false;
        }

        //! Returns if in canceled state
        //! \return true if queue had been canceled and not reset
        bool canceled()
        {
          return m_canceled;
        }

        //! Add an item to the queue for processing
        //! \param[in] item item to add
        void add(const T &item)
        {
          boost::mutex::scoped_lock l(m_mutex);

          if (!m_canceled)
          {
            m_deque.push_back(item);
            m_condition.notify_one();
          }
        }

      private:
        //! Variable to wait on when waiting for more items
        boost::condition m_condition; 

        //! Protection mutex for local data
        boost::mutex m_mutex;

        //! stl deque of incomming items.
        std::deque<T> m_deque;

        //! Copy constructor, private and explicitly not implemented
        queue(const queue<T> &);

        //! Assignment operator, private and explicitly not implemented
        queue<T> &operator=(const queue<T> &);

        //! Set to true if a read operation was canceled
        volatile bool m_canceled;
    };
}

#endif


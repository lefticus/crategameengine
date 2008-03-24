#ifndef __mvc_script__
#define __mvc_script__

#include <string>
#include "mvc/object_id.hpp"
#include "mvc/logger.hpp"
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>

namespace game_personality
{
  class world;
  class world_script_access;

  struct script
  {
    script(const std::string &t_type, 
           const std::string &t_name, 
           const std::string &t_text)
      : type(t_type), name(t_name), text(t_text)
    {
    }

    script()
    {
    }

    virtual ~script()
    {
    }

    std::string type;
    std::string name;
    std::string text;
  };

  class script_handler
  {
    public:
      script_handler(const std::string &t_name, 
          const boost::function<void (mvc::logger::log_level, 
           const std::string &, const std::string &)> &t_logger)
        : name(t_name), m_logger(boost::bind(t_logger, _1, "script_handler:" + t_name, _2))
      {
      }

      virtual ~script_handler()
      {
      }

      struct invalid_script : std::runtime_error
      {
        invalid_script(const std::string &reason)
          : std::runtime_error("Script could not be used: " + reason)
        {
        }
      };

      struct unknown_named_script : std::runtime_error
      {
        unknown_named_script(const std::string &scriptname)
          : std::runtime_error("Named script does not exist: " + scriptname)
        {
        }
      };

      virtual void register_named_script(const script &s) = 0;
      virtual void execute_named_script(const std::string &, 
          world_script_access &world,
          const std::vector<mvc::object_id_base> &objects, 
          const std::vector<std::string> &strings) = 0;

      virtual void execute(const script &, 
          world_script_access &world,
          const std::vector<mvc::object_id_base> &objects,
          const std::vector<std::string> &strings) = 0;

      virtual boost::shared_ptr<script_handler> clone() const = 0;

      const std::string name;

      boost::function<void (mvc::logger::log_level, const std::string &)> m_logger;
  };
}

#endif

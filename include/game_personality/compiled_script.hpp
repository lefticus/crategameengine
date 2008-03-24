#ifndef __mvc_compiled_script__
#define __mvc_compiled_script__

#include <string>
#include <boost/function.hpp>
#include "mvc/object_id.hpp"
#include "world_script_access.hpp"
#include <vector>
#include <boost/shared_ptr.hpp>
#include <map>

namespace game_personality
{
  class compiled_script : public script
  {
    public:
      compiled_script(const std::string &name, 
          const boost::function<void (world_script_access &, 
            const std::vector<mvc::object_id_base> &,
            const std::vector<std::string> &)> &action)
        : script("compiled", name, ""), m_action(action)
      {
      }

      virtual ~compiled_script()
      {
      }

      void execute(world_script_access &world, 
          const std::vector<mvc::object_id_base> &objects,
          const std::vector<std::string> &strings) const
      {
        m_action(world, objects, strings);
      }

    private:
      boost::function<void (world_script_access &, 
          const std::vector<mvc::object_id_base> &,
          const std::vector<std::string> &)> m_action;
  };

  class compiled_script_handler : public script_handler
  {
    public:
      typedef std::map<std::string, compiled_script> script_map;

      compiled_script_handler(const boost::function<void (mvc::logger::log_level, 
          const std::string &, const std::string &)> &t_logger)
        : script_handler("compiled", t_logger)
      {
      }

      virtual ~compiled_script_handler()
      {
      }

      struct not_a_compiled_script : script_handler::invalid_script
      {
        not_a_compiled_script()
          : script_handler::invalid_script("Not a compiled script")
        {
        }
      };

      virtual void register_named_script(const script &s)
      {
        try { 
          script_map::iterator itr = m_named_scripts.find(s.name);
          if (itr != m_named_scripts.end())
          {
            itr->second = dynamic_cast<const compiled_script &>(s);
          } else {
            m_named_scripts.insert(make_pair(s.name, dynamic_cast<const compiled_script &>(s)));
          }
        } catch (const std::bad_cast &) {
          throw not_a_compiled_script();
        }
      }

      virtual void execute_named_script(
          const std::string &name, 
          world_script_access &world, 
          const std::vector<mvc::object_id_base> &objects,
          const std::vector<std::string> &strings)
      {
        script_map::const_iterator itr =
          m_named_scripts.find(name);

        if (itr != m_named_scripts.end())
        {
          itr->second.execute(world, objects, strings);
        } else {
          throw script_handler::unknown_named_script(name);
        }
      }

      virtual void execute(const script &script, 
          world_script_access &world, 
          const std::vector<mvc::object_id_base> &objects,
          const std::vector<std::string> &strings) 
      {
        try {   
          dynamic_cast<const compiled_script&>(script).execute(world, objects, strings);
        } catch (const std::bad_cast &) {
          throw not_a_compiled_script();
        }
      }

      virtual boost::shared_ptr<script_handler> clone() const
      {
        return boost::shared_ptr<script_handler>(new compiled_script_handler(*this));
      }

    private:
       script_map m_named_scripts;
  };
}

#endif

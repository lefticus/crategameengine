#ifndef __mvc_script__
#define __mvc_script__

#include <string>
#include <boost/function.hpp>
#include "object_id.hpp"
#include <vector>
#include <boost/shared_ptr.hpp>
#include <map>

namespace mvc
{
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

  template<typename T>
  class world_script_access
  {
    public: 
      typedef T world_personality;

      virtual ~world_script_access()
      {
      }

      virtual void execute_script(const script &s, 
          const std::vector<object_id_base> &objects,
          const std::vector<std::string> &strings) = 0;

      virtual void execute_named_script(const std::string &name, 
          const std::vector<object_id_base> &objects,
          const std::vector<std::string> &strings) = 0;

      virtual typename world_personality::change_set get_change_set() = 0;

  };

  template<typename T>
  class script_handler
  {
    public:
      script_handler(const std::string &t_name)
        : name(t_name)
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

      typedef T world_personality;

      virtual void register_named_script(const script &s) = 0;
      virtual void execute_named_script(const std::string &, 
          typename world_personality::world_script_access &world,
          const std::vector<object_id_base> &objects, 
          const std::vector<std::string> &strings) = 0;

      virtual void execute(const script &, 
          typename world_personality::world_script_access &world,
          const std::vector<object_id_base> &objects,
          const std::vector<std::string> &strings) = 0;

      virtual boost::shared_ptr<script_handler> clone() const = 0;

      std::string name;
  };


  template<typename T>
  class compiled_script : public script
  {
    public:
      typedef T world_personality;

      compiled_script(const std::string &name, 
          const boost::function<void (typename world_personality::world_script_access &, 
            const std::vector<object_id_base> &,
            const std::vector<std::string> &)> &action)
        : script("compiled", name, ""), m_action(action)
      {
      }

      virtual ~compiled_script()
      {
      }

      void execute(typename world_personality::world_script_access &world, 
          const std::vector<object_id_base> &objects,
          const std::vector<std::string> &strings) const
      {
        m_action(world, objects, strings);
      }

    private:
      boost::function<void (typename world_personality::world_script_access &, 
          const std::vector<object_id_base> &,
          const std::vector<std::string> &)> m_action;
  };

  template<typename T>
  class compiled_script_handler : public script_handler<T>
  {
    public:
      typedef T world_personality;
      typedef std::map<std::string, compiled_script<T> > script_map;

      compiled_script_handler()
        : script_handler<T>("compiled")
      {
      }

      virtual ~compiled_script_handler()
      {
      }


      struct not_a_compiled_script : script_handler<T>::invalid_script
      {
        not_a_compiled_script()
          : script_handler<T>::invalid_script("Not a compiled script")
        {
        }
      };

      virtual void register_named_script(const script &s)
      {
        try { 
          typename script_map::iterator itr = m_named_scripts.find(s.name);
          if (itr != m_named_scripts.end())
          {
            itr->second = dynamic_cast<const compiled_script<T> &>(s);
          } else {
            m_named_scripts.insert(make_pair(s.name, dynamic_cast<const compiled_script<T> &>(s)));
          }
        } catch (const std::bad_cast &) {
          throw not_a_compiled_script();
        }
      }

      virtual void execute_named_script(
          const std::string &name, 
          typename world_personality::world_script_access &world, 
          const std::vector<object_id_base> &objects,
          const std::vector<std::string> &strings)
      {
        typename script_map::const_iterator itr =
          m_named_scripts.find(name);

        if (itr != m_named_scripts.end())
        {
          itr->second.execute(world, objects, strings);
        } else {
          throw typename script_handler<T>::unknown_named_script(name);
        }
      }

      virtual void execute(const script &script, 
          typename world_personality::world_script_access &world, 
          const std::vector<object_id_base> &objects,
          const std::vector<std::string> &strings) 
      {
        try {   
          dynamic_cast<const compiled_script<T> &>(script).execute(world, objects, strings);
        } catch (const std::bad_cast &) {
          throw not_a_compiled_script();
        }
      }

      virtual boost::shared_ptr<script_handler<world_personality> > clone() const
      {
        return boost::shared_ptr<script_handler<world_personality> >(new compiled_script_handler(*this));
      }

    private:
       script_map m_named_scripts;
  };
}

#endif

#ifndef __mvc_events__
#define __mvc_events__

#include "script.hpp"
#include "object_id.hpp"
#include <string>
#include <vector>

namespace mvc
{
  template<typename T>
  struct event_world_changed
  {
    event_world_changed(const T &cs)
      : change_set(cs)
    {
    }

    T change_set;
  };

  struct event_run_script
  {
    event_run_script(const script &t_s,
                     const std::vector<object_id_base> &t_objects,
                     const std::vector<std::string> &t_strings)
      : script_to_run(t_s), objects(t_objects), strings(t_strings)
    {
    }

    script script_to_run;
    std::vector<object_id_base> objects;
    std::vector<std::string> strings;
  };

  struct event_run_named_script
  {
    event_run_named_script(const std::string &t_name,
                     const std::vector<object_id_base> &t_objects,
                     const std::vector<std::string> &t_strings)
      : name(t_name), objects(t_objects), strings(t_strings)
    {
    }

    std::string name;
    std::vector<object_id_base> objects;
    std::vector<std::string> strings;
  };
}


#endif

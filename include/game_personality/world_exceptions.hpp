#ifndef __mvc_world_exceptions__
#define __mvc_world_exceptions__

#include <stdexcept>
#include "mvc/object_id.hpp"

namespace game_personality
{
  struct object_not_found : std::runtime_error
  {
    object_not_found(const mvc::object_id_base &t_oid)
      : std::runtime_error("object not found"),
        oid(t_oid)
    {
    }

    virtual ~object_not_found() throw()
    {
    }

    mvc::object_id_base oid;
  };

  struct unknown_named_script : std::runtime_error
  {
    unknown_named_script(const std::string &t_name)
      : std::runtime_error("uknown named script: " + t_name)
    {
    }
  };


}

#endif

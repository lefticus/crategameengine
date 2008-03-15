#ifndef __game_personality_world_reader_hpp__
#define __game_personality_world_reader_hpp__

#include "mvc/object_id.hpp"
#include "objects.hpp"

namespace game_personality
{
  struct world_reader
  {
    virtual ~world_reader()
    {
    }

    virtual game_object getobject(const mvc::object_id<game_object> &oid) const = 0;
    virtual environmental_object getobject(const mvc::object_id<environmental_object> &oid) const = 0;
    virtual item getobject(const mvc::object_id<item> &oid) const = 0;
    virtual player getobject(const mvc::object_id<player> &oid) const = 0;
    virtual location getobject(const mvc::object_id<location> &oid) const = 0;

    game_object get_game_object(const mvc::object_id_base &oid) const
    {
      return getobject(mvc::object_id<game_object>(oid));
    }

    environmental_object get_environmental_object(const mvc::object_id_base &oid) const
    {
      return getobject(mvc::object_id<environmental_object>(oid));
    }

    item get_item(const mvc::object_id_base &oid) const
    {
      return getobject(mvc::object_id<item>(oid));
    }

    player get_player(const mvc::object_id_base &oid) const
    {
      return getobject(mvc::object_id<player>(oid));
    }

    location get_location(const mvc::object_id_base &oid) const
    {
      return getobject(mvc::object_id<location>(oid));
    }
  };    

}

#endif

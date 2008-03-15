#ifndef __game_personality_world_creator_hpp__
#define __game_personality_world_creator_hpp__

#include "objects.hpp"

namespace game_personality
{
  struct world_creator
  {
    virtual ~world_creator()
    {
    }

    virtual game_object create(const mvc::object_id<game_object> &oid) = 0;
    virtual environmental_object create(const mvc::object_id<environmental_object> &oid) = 0;
    virtual item create(const mvc::object_id<item> &oid) = 0;
    virtual player create(const mvc::object_id<player> &oid) = 0;
    virtual location create(const mvc::object_id<location> &oid) = 0;
  };
}

#endif

#ifndef __game_personality_world_writer_hpp__
#define __game_personality_world_writer_hpp__

#include "objects.hpp"

namespace game_personality
{
  struct world_writer
  {
    virtual ~world_writer()
    {
    }

    virtual void update(const game_object &) = 0;
    virtual void update(const environmental_object &) = 0;
    virtual void update(const item &) = 0;
    virtual void update(const player &) = 0;
    virtual void update(const location &) = 0;
  };


}

#endif

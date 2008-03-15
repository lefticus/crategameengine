#ifndef __game_personality_world_script_access_hpp__
#define __game_personality_world_script_access_hpp__

#include "world_reader.hpp"
#include "world_writer.hpp"
#include "mvc/script.hpp"


namespace game_personality
{
  class world_script_access : public world_reader, public world_writer, 
                              public mvc::world_script_access<world_change_set>
  {
  };

}

#endif

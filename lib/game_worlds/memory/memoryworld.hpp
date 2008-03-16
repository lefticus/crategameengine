#ifndef __game_worlds_memoryworld_hpp__
#define __game_worlds_memoryworld_hpp__

#include "game_personality/world.hpp"
#include "mvc/logger.hpp"

namespace game_worlds
{
  class memoryworld : public game_personality::world
  {
    public:
      memoryworld(const boost::function<void (mvc::logger::log_level, 
            const std::string &, const std::string &)> &t_logger);

      void apply_change_set(const game_personality::world_change_set &cs);

      game_personality::game_object 
        create(const mvc::object_id<game_personality::game_object>& oid);

      game_personality::environmental_object 
        create( const mvc::object_id<game_personality::environmental_object>& oid);
      game_personality::item create(
          const mvc::object_id<game_personality::item>& oid);
      game_personality::player create(
          const mvc::object_id<game_personality::player>& oid);
      game_personality::location create(
          const mvc::object_id<game_personality::location>& oid);
      game_personality::game_object getobject(
          const mvc::object_id<game_personality::game_object>& oid) const;
      game_personality::environmental_object getobject(
          const mvc::object_id<game_personality::environmental_object>& oid) const;
      game_personality::item getobject(
          const mvc::object_id<game_personality::item>& oid) const;
      game_personality::player getobject(
          const mvc::object_id<game_personality::player>& oid) const;
      game_personality::location getobject(
          const mvc::object_id<game_personality::location>& oid) const;

    private:
      boost::function<void (mvc::logger::log_level, const std::string &)> m_logger;
      game_personality::world_change_set m_change_set;
  };
}

#endif

#include "game_personality/personality.hpp"

class worldimpl : public game_personality::world
{
  public:
    void apply_change_set(const game_personality::change_set &cs)
    {
    }

    game_object create(const mvc::object_id<game_object>& oid)
    {
      return game_object(oid);
    }

    environmental_object create(const mvc::object_id<environmental_object>& oid)
    {
      return environmental_object(oid);
    }

    item create(const mvc::object_id<item>& oid)
    {
      return item(oid);
    }

    player create(const mvc::object_id<player>& oid)
    {
      return player(oid);
    }

    location create(const mvc::object_id<location>& oid)
    {
      return location(oid);
    }

    game_object getobject(const mvc::object_id<game_object>& oid) const
    {
      return game_object(oid);
    }

    environmental_object getobject(const mvc::object_id<environmental_object>& oid) const
    {
      return environmental_object(oid);
    }

    item getobject(const mvc::object_id<item>& oid) const
    {
      return item(oid);
    }

    player getobject(const mvc::object_id<player>& oid) const
    {
      return player(oid);
    }

    location getobject(const mvc::object_id<location>& oid) const
    {
      return location(oid);
    }
};

class viewimpl : public game_personality::view
{
  void before_world_changed(const mvc::event_world_changed<game_personality::change_set>&)
  {
  }

  void after_world_changed(const mvc::event_world_changed<game_personality::change_set>&)
  {
  }

  void character_response(const event_character_response&)
  {
  }

  void character_speak(const event_character_speak&)
  {
  }

  void announcement(const event_announcement&)
  {
  }
};

class engineimpl : public game_personality::engine
{
  public: 
    engineimpl(viewimpl &v, worldimpl &w)
      : game_personality::engine(v, w)
    {
    }
};

int main(int, char **)
{
  worldimpl w;
  viewimpl v;
  engineimpl e(v,w);
}

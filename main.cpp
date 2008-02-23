#include "game_personality/personality.hpp"

class worldimpl : public game_personality::personality::world
{
  public:
    void apply_change_set(const game_personality::personality::change_set &cs)
    {
    }

    game_personality::game_object create(const mvc::object_id<game_personality::game_object>& oid)
    {
      return game_personality::game_object(oid);
    }

    game_personality::environmental_object create(
        const mvc::object_id<game_personality::environmental_object>& oid)
    {
      return game_personality::environmental_object(oid);
    }

    game_personality::item create(
        const mvc::object_id<game_personality::item>& oid)
    {
      return game_personality::item(oid);
    }

    game_personality::player create(
        const mvc::object_id<game_personality::player>& oid)
    {
      return game_personality::player(oid);
    }

    game_personality::location create(
        const mvc::object_id<game_personality::location>& oid)
    {
      return game_personality::location(oid);
    }

    game_personality::game_object getobject(
        const mvc::object_id<game_personality::game_object>& oid) const
    {
      return game_personality::game_object(oid);
    }

    game_personality::environmental_object getobject(
        const mvc::object_id<game_personality::environmental_object>& oid) const
    {
      return game_personality::environmental_object(oid);
    }

    game_personality::item getobject(
        const mvc::object_id<game_personality::item>& oid) const
    {
      return game_personality::item(oid);
    }

    game_personality::player getobject(
        const mvc::object_id<game_personality::player>& oid) const
    {
      return game_personality::player(oid);
    }

    game_personality::location getobject(
        const mvc::object_id<game_personality::location>& oid) const
    {
      return game_personality::location(oid);
    }
};

class viewimpl : public game_personality::personality::view
{
  void before_world_changed(const mvc::event_world_changed<game_personality::personality::change_set>&)
  {
  }

  void after_world_changed(const mvc::event_world_changed<game_personality::personality::change_set>&)
  {
  }

  void character_response(const game_personality::event_character_response&)
  {
  }

  void character_speak(const game_personality::event_character_speak&)
  {
  }

  void announcement(const game_personality::event_announcement&)
  {
  }
};

class engineimpl : public game_personality::personality::engine
{
  public: 
    engineimpl(viewimpl &v, worldimpl &w)
      : game_personality::personality::engine(v, w)
    {
    }
};

int main(int, char **)
{
  worldimpl w;
  viewimpl v;
  engineimpl e(v,w);
}

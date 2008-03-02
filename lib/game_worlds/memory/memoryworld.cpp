#include "memoryworld.hpp"

namespace game_worlds
{
  memoryworld::memoryworld(
      const boost::function<void (mvc::logger::log_level, const std::string &, const std::string &)> 
              &t_logger)
    : game_personality::personality::world(t_logger),
      m_logger(boost::bind(t_logger, _1, "memoryworld", _2))
  {
    m_logger(mvc::logger::debug, "constructor called");
  }


  void memoryworld::apply_change_set(const game_personality::personality::change_set &cs)
  {
    m_change_set.merge(cs);
  }

  game_personality::game_object memoryworld::create(const mvc::object_id<game_personality::game_object>& oid)
  {
    game_personality::game_object o(oid);
    m_change_set.update(o);
    return o;
  }

  game_personality::environmental_object memoryworld::create(
      const mvc::object_id<game_personality::environmental_object>& oid)
  {
    game_personality::environmental_object o(oid);
    m_change_set.update(o);
    return o;
  }

  game_personality::item memoryworld::create(
      const mvc::object_id<game_personality::item>& oid)
  {
    game_personality::item o(oid);
    m_change_set.update(o);
    return o;
  }

  game_personality::player memoryworld::create(
      const mvc::object_id<game_personality::player>& oid)
  {
    game_personality::player o(oid);
    m_change_set.update(o);
    return o;
  }

  game_personality::location memoryworld::create(
      const mvc::object_id<game_personality::location>& oid)
  {
    game_personality::location o(oid);
    m_change_set.update(o);
    return o;
  }

  game_personality::game_object memoryworld::getobject(
      const mvc::object_id<game_personality::game_object>& oid) const
  {
    return m_change_set.getobject(oid);
  }

  game_personality::environmental_object memoryworld::getobject(
      const mvc::object_id<game_personality::environmental_object>& oid) const
  {
    return m_change_set.getobject(oid);
  }

  game_personality::item memoryworld::getobject(
      const mvc::object_id<game_personality::item>& oid) const
  {
    return m_change_set.getobject(oid);
  }

  game_personality::player memoryworld::getobject(
      const mvc::object_id<game_personality::player>& oid) const
  {
    return m_change_set.getobject(oid);
  }

  game_personality::location memoryworld::getobject(
      const mvc::object_id<game_personality::location>& oid) const
  {
    return m_change_set.getobject(oid);
  }

}


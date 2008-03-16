#include "game_personality/world_script_access.hpp"
#include "game_personality/world.hpp"


namespace game_personality
{
  
  world_script_access::world_script_access(world &w)
    : m_world(w)
  {
  }

  void world_script_access::emit(const event_character_response &e)
  {
    mvc::emit<event_character_response>(m_world, e);
  }

  void world_script_access::emit(const event_character_speak &e)
  {
    mvc::emit<event_character_speak>(m_world, e);
  }

  void world_script_access::emit(const event_announcement &e)
  {
    mvc::emit<event_announcement>(m_world, e);
  }

  world_change_set world_script_access::get_change_set()
  {
    return m_changeset;
  }

  void world_script_access::execute_script(const script &s,
      const std::vector<mvc::object_id_base> &objects,
      const std::vector<std::string> &strings)
  {
    m_world.execute_script(s, objects, strings);
  }

  void world_script_access::execute_named_script(const std::string &name,
      const std::vector<mvc::object_id_base> &objects,
      const std::vector<std::string> &strings)
  {
    m_world.execute_named_script(name, objects, strings);
  }

  game_object world_script_access::getobject(const mvc::object_id<game_object> &oid) const
  {
    return get_object_impl(oid, m_changeset, m_world); 
  }

  environmental_object world_script_access::getobject(const mvc::object_id<environmental_object> &oid) const
  {
    return get_object_impl(oid, m_changeset, m_world); 
  }

  item world_script_access::getobject(const mvc::object_id<item> &oid) const
  {
    return get_object_impl(oid, m_changeset, m_world); 
  }

  player world_script_access::getobject(const mvc::object_id<player> &oid) const
  {
    return get_object_impl(oid, m_changeset, m_world); 
  }

  location world_script_access::getobject(const mvc::object_id<location> &oid) const
  {
    return get_object_impl(oid, m_changeset, m_world); 
  }

  void world_script_access::update(const game_object &o)
  {
    m_changeset.update(o);
  }

  void world_script_access::update(const environmental_object &o)
  {
    m_changeset.update(o);
  }

  void world_script_access::update(const item &o)
  {
    m_changeset.update(o);
  }

  void world_script_access::update(const player &o)
  {
    m_changeset.update(o);
  }

  void world_script_access::update(const location &o)
  {
    m_changeset.update(o);
  }  



}


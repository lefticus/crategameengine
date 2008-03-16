#ifndef __game_personality_world_view_access__
#define __game_personality_world_view_access__

#include "world_reader.hpp"
#include "world_change_set.hpp"

namespace game_personality
{
  class world_view_access : public world_reader
  {
    private:
      world_change_set m_changeset;

    public:
      void apply_change_set(const world_change_set &cs)
      {
        m_changeset.merge(cs);
      }


      virtual game_object getobject(const mvc::object_id<game_object> &oid) const
      {
        return m_changeset.getobject(oid); 
      }

      virtual environmental_object getobject(const mvc::object_id<environmental_object> &oid) const
      {
        return m_changeset.getobject(oid); 
      }

      virtual item getobject(const mvc::object_id<item> &oid) const
      {
        return m_changeset.getobject(oid); 
      }

      virtual player getobject(const mvc::object_id<player> &oid) const
      {
        return m_changeset.getobject(oid); 
      }

      virtual location getobject(const mvc::object_id<location> &oid) const
      {
        return m_changeset.getobject(oid); 
      }
  };

}

#endif

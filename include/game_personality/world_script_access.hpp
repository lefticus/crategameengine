#ifndef __game_personality_world_script_access_hpp__
#define __game_personality_world_script_access_hpp__

#include "world_reader.hpp"
#include "world_writer.hpp"
#include "world_change_set.hpp"
#include "world_exceptions.hpp"
#include "script.hpp"

#include "events.hpp"


namespace game_personality
{

  class world;

  class world_script_access : public world_reader, public world_writer
  {
    private:
      world &m_world;
      world_change_set m_changeset;

      template<typename T, typename Primary, typename Secondary>
        typename T::object_type get_object_impl(const T &oid,
            const Primary &p, const Secondary &s) const
        {
          try {
            return p.getobject(oid);
          } catch (const object_not_found &) {
            return s.getobject(oid);
          }
        }

    public:
      world_script_access(world &w);

      void emit(const game_personality::event_character_response &e);
      void emit(const game_personality::event_character_speak &e);
      void emit(const game_personality::event_announcement &e);

      world_change_set get_change_set();

      virtual void execute_script(const script &s,
          const std::vector<mvc::object_id_base> &objects,
          const std::vector<std::string> &strings);
      virtual void execute_named_script(const std::string &name,
          const std::vector<mvc::object_id_base> &objects,
          const std::vector<std::string> &strings);

      virtual game_object getobject(const mvc::object_id<game_object> &oid) const;
      virtual environmental_object getobject(const mvc::object_id<environmental_object> &oid) const;
      virtual item getobject(const mvc::object_id<item> &oid) const;
      virtual player getobject(const mvc::object_id<player> &oid) const;
      virtual location getobject(const mvc::object_id<location> &oid) const;

      void update(const game_object &o);
      void update(const environmental_object &o);
      void update(const item &o);
      void update(const player &o);
      void update(const location &o);
  };


}

#endif

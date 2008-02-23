#ifndef __game_personality_personality_hpp__
#define __game_personality_personality_hpp__

#include "events.hpp"
#include "objects.hpp"

#include "object_id.hpp"

#include <string>
#include <boost/lexical_cast.hpp>
#include <set>
#include <map>
#include "world.hpp"
#include "view.hpp"
#include "engine.hpp"


namespace game_personality
{

  template<typename T, typename T2, typename Iterator>
  void merge(std::map<T, T2> &dest, Iterator begin, Iterator end)
  {
    for (typename std::map<T, T2>::const_iterator itr = begin;
        itr != end;
        ++itr)
    {
      dest.erase(itr->first);
    }

    dest.insert(begin, end);
  }

  struct personality
  {
    struct world_reader
    {
      virtual ~world_reader()
      {
      }

      virtual game_object getobject(const mvc::object_id<game_object> &oid) const = 0;
      virtual environmental_object getobject(const mvc::object_id<environmental_object> &oid) const = 0;
      virtual item getobject(const mvc::object_id<item> &oid) const = 0;
      virtual player getobject(const mvc::object_id<player> &oid) const = 0;
      virtual location getobject(const mvc::object_id<location> &oid) const = 0;

      game_object get_game_object(const mvc::object_id_base &oid) const
      {
        return getobject(mvc::object_id<game_object>(oid));
      }

      environmental_object get_environmental_object(const mvc::object_id_base &oid) const
      {
        return getobject(mvc::object_id<environmental_object>(oid));
      }

      item get_item(const mvc::object_id_base &oid) const
      {
        return getobject(mvc::object_id<item>(oid));
      }

      player get_player(const mvc::object_id_base &oid) const
      {
        return getobject(mvc::object_id<player>(oid));
      }

      location get_location(const mvc::object_id_base &oid) const
      {
        return getobject(mvc::object_id<location>(oid));
      }
    };    

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

    struct change_set : world_reader, world_writer
    {
      private:
        std::map<mvc::object_id<game_object>, game_object> gameobjects;
        std::map<mvc::object_id<environmental_object>, environmental_object> environmentalobjects;
        std::map<mvc::object_id<item>, item> items;
        std::map<mvc::object_id<player>, player> players;
        std::map<mvc::object_id<location>, location> locations;

        template<typename T>
          T getobject_impl(const mvc::object_id<T> &oid, 
                           const std::map<typename mvc::object_id<T>, T> &m) const
          {
            typename std::map<typename mvc::object_id<T>, T>::const_iterator itr = m.find(oid);
            if (itr != m.end())
            {
              return itr->second;
            } else {
              throw mvc::object_not_found(oid);
            }
          }

        template<typename T>
          void updateobject_impl(const T &object, 
                           std::map<typename mvc::object_id<T>, T> &m) 
          {
            typename std::map<typename mvc::object_id<T>, T>::iterator itr = m.find(object.oid);

            if (itr == m.end())
            {
              m.insert(std::make_pair(object.oid, object));
            } else {
              itr->second = object;
            }

          }


        mvc::object_id<player> currentplayer;

      public:
        virtual ~change_set()
        {
        }

        virtual game_object getobject(const mvc::object_id<game_object> &oid) const
        {
          return getobject_impl(oid, gameobjects); 
        }

        virtual environmental_object getobject(const mvc::object_id<environmental_object> &oid) const
        {
          return getobject_impl(oid, environmentalobjects); 
        }

        virtual item getobject(const mvc::object_id<item> &oid) const
        {
          return getobject_impl(oid, items); 
        }

        virtual player getobject(const mvc::object_id<player> &oid) const
        {
          return getobject_impl(oid, players); 
        }

        virtual location getobject(const mvc::object_id<location> &oid) const
        {
          return getobject_impl(oid, locations); 
        }

        virtual void merge(const change_set &cs)
        {
          game_personality::merge(gameobjects, cs.gameobjects.begin(), cs.gameobjects.end());
          game_personality::merge(environmentalobjects, 
              cs.environmentalobjects.begin(), cs.environmentalobjects.end());
          game_personality::merge(items, cs.items.begin(), cs.items.end());
          game_personality::merge(players, cs.players.begin(), cs.players.end());
          game_personality::merge(locations, cs.locations.begin(), cs.locations.end());
        }

        void reset()
        {
          gameobjects.clear();
          environmentalobjects.clear();
          items.clear();
          players.clear();
          locations.clear();
        }

        void update(const game_object &o)
        {
          updateobject_impl(o, gameobjects);
        }

        void update(const environmental_object &o)
        {
          updateobject_impl(o, environmentalobjects);
        }

        void update(const item &o)
        {
          updateobject_impl(o, items);
        }

        void update(const player &o)
        {
          updateobject_impl(o, players);
        }

        void update(const location &o)
        {
          updateobject_impl(o, locations);
        }
    };


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



    class world_view_access : public world_reader
    {
      private:
        change_set m_changeset;

      public:
        void apply_change_set(const change_set &cs)
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


    class world;

    class world_script_access : public world_reader, public world_writer, 
                                public mvc::world_script_access<personality>
    {
      private:
        world &m_world;
        change_set m_changeset;

        template<typename T>
          typename T::object_type get_object_impl(const T &oid) const
          {
            try {
              return m_changeset.getobject(oid);
            } catch (const mvc::object_not_found &) {
              return m_world.getobject(oid);
            }
          }

      public:
        change_set get_change_set()
        {
          return m_changeset;
        }

        world_script_access(mvc::world<personality> &w)
          : m_world(dynamic_cast<world&>(w))
        {
        }

        virtual void execute_script(const mvc::script &s,
            const std::vector<mvc::object_id_base> &objects,
            const std::vector<std::string> &strings)
        {
          m_world.execute_script(s, objects, strings);
        }

        virtual void execute_named_script(const std::string &name,
            const std::vector<mvc::object_id_base> &objects,
            const std::vector<std::string> &strings)
        {
          m_world.execute_named_script(name, objects, strings);
        }

        virtual game_object getobject(const mvc::object_id<game_object> &oid) const
        {
          return get_object_impl(oid); 
        }

        virtual environmental_object getobject(const mvc::object_id<environmental_object> &oid) const
        {
          return get_object_impl(oid); 
        }

        virtual item getobject(const mvc::object_id<item> &oid) const
        {
          return get_object_impl(oid); 
        }

        virtual player getobject(const mvc::object_id<player> &oid) const
        {
          return get_object_impl(oid); 
        }

        virtual location getobject(const mvc::object_id<location> &oid) const
        {
          return get_object_impl(oid); 
        }

        void update(const game_object &o)
        {
          m_changeset.update(o);
        }

        void update(const environmental_object &o)
        {
          m_changeset.update(o);
        }

        void update(const item &o)
        {
          m_changeset.update(o);
        }

        void update(const player &o)
        {
          m_changeset.update(o);
        }

        void update(const location &o)
        {
          m_changeset.update(o);
        }  

    };

    class view : public mvc::view< personality >,
                 public mvc::event_emitter<event_take_item>,
                 public mvc::event_emitter<event_use_item>,
                 public mvc::event_emitter<event_use_item_with>,
                 public mvc::event_emitter<event_talk_to_character>,
                 public mvc::event_emitter<event_ask_character>,
                 public mvc::event_emitter<event_move_to>,
                 public mvc::event_listener<event_character_response>,
                 public mvc::event_listener<event_character_speak>,
                 public mvc::event_listener<event_announcement>
      {
      public: 
        view()
          : mvc::event_listener<event_character_response>(
              boost::bind(&view::queue_event, this, _1), boost::bind(&view::character_response, this, _1)),
            mvc::event_listener<event_character_speak>(
              boost::bind(&view::queue_event, this, _1), boost::bind(&view::character_speak, this, _1)),
            mvc::event_listener<event_announcement>(
              boost::bind(&view::queue_event, this, _1), boost::bind(&view::announcement, this, _1)) 
        {
        }

      private:
        virtual void character_response(const event_character_response &e) = 0;
        virtual void character_speak(const event_character_speak &e) = 0;
        virtual void announcement(const event_announcement &e) = 0;
    };

    class world : public world_reader,
                  public world_creator,
                  public mvc::event_emitter<event_character_response>,
                  public mvc::event_emitter<event_character_speak>,
                  public mvc::event_emitter<event_announcement>,
                  public mvc::world<personality>
    {
    };

    struct engine : public mvc::event_listener<event_take_item>,
                    public mvc::event_listener<event_use_item>,
                    public mvc::event_listener<event_use_item_with>,
                    public mvc::event_listener<event_talk_to_character>,
                    public mvc::event_listener<event_ask_character>,
                    public mvc::event_listener<event_move_to>,
                    public mvc::event_handler,
                    public mvc::engine<personality>
    {
      engine(view &v, world &w)
        : mvc::event_listener<event_take_item>(
            boost::bind(&engine::queue_event, this, _1), boost::bind(&engine::take_item, this, _1)),
          mvc::event_listener<event_use_item>(
            boost::bind(&engine::queue_event, this, _1), boost::bind(&engine::use_item, this, _1)),
          mvc::event_listener<event_use_item_with>(
            boost::bind(&engine::queue_event, this, _1), boost::bind(&engine::use_item_with, this, _1)),
          mvc::event_listener<event_talk_to_character>(
            boost::bind(&engine::queue_event, this, _1), boost::bind(&engine::talk_to_character, this, _1)),
          mvc::event_listener<event_ask_character>(
            boost::bind(&engine::queue_event, this, _1), boost::bind(&engine::ask_character, this, _1)),
          mvc::event_listener<event_move_to>(
            boost::bind(&engine::queue_event, this, _1), boost::bind(&engine::move_to, this, _1)),
          mvc::engine< personality >(v, w)
      {
        mvc::attach<event_take_item>(v, *this);
        mvc::attach<event_use_item>(v, *this);
        mvc::attach<event_use_item_with>(v, *this);
        mvc::attach<event_talk_to_character>(v, *this);
        mvc::attach<event_ask_character>(v, *this);
        mvc::attach<event_move_to>(v, *this);
        mvc::attach<event_character_response>(w, v);
        mvc::attach<event_character_speak>(w, v);
        mvc::attach<event_announcement>(w, v);
      }

      void take_item(const event_take_item &e)
      {
  //      std::vector<mvc::object_id_base> objects;
  //      objects.push_back(e.player.mvc::object_id);
  //      objects.push_back(e.item.mvc::object_id);
  //      emit(event_run_named_script("takeitem", objects, vector<string>()));
      }

      void use_item(const event_use_item &e)
      {
  //      std::vector<mvc::object_id_base> objects;
  //      objects.push_back(e.item.mvc::object_id);
  //      emit(event_run_named_script("useitem", objects, vector<string>()));
      }

      void use_item_with(const event_use_item_with &e)
      {
  //      std::vector<mvc::object_id_base> objects;
  //      objects.push_back(e.item.mvc::object_id);
  //      emit(event_run_named_script("useitem", objects, vector<string>()));
      }


      void talk_to_character(const event_talk_to_character &e)
      {
      }

      void ask_character(const event_ask_character &e)
      {
      }

      void move_to(const event_move_to &e)
      {
      }
    };
  };
}

#endif

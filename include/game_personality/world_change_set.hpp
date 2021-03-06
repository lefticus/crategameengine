#ifndef __game_personality_world_change_set_hpp__
#define __game_personality_world_change_set_hpp__

#include "objects.hpp"
#include "world_reader.hpp"
#include "world_writer.hpp"
#include "mvc/logger.hpp"
#include "mvc/object_id.hpp"
#include "world_exceptions.hpp"

#include <map>


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

  class world_change_set : public world_reader, public world_writer
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
            throw object_not_found(oid);
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
      virtual ~world_change_set()
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

      virtual void merge(const world_change_set &cs)
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

}

#endif

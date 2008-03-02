#ifndef __game_personality_objects_hpp__
#define __game_personality_objects_hpp__

#include "mvc/object_id.hpp"
#include "mvc/script.hpp"

#include <string>
#include <boost/lexical_cast.hpp>
#include <set>
#include <map>

namespace game_personality
{
  class properties : private std::map<std::string, std::string>
  {
    public:
      template<typename wrappedtype, typename interfacetype>
        class lexical_cast_wrapper
        {
          friend class properties;

          public:
            lexical_cast_wrapper(wrappedtype &v)
              : m_value(v)
            {
            }

            const interfacetype operator=(const interfacetype& value)
            {
              m_value = boost::lexical_cast<wrappedtype>(value);
            }

            operator const interfacetype() const
            {
              return boost::lexical_cast<interfacetype>(m_value);
            }

          private:
            wrappedtype &m_value;
        };

      using std::map<std::string, std::string>::operator[];

      template<typename T>
        lexical_cast_wrapper<std::string, T> get(const std::string &key)
        {
          return lexical_cast_wrapper<std::string, T>(operator[](key));
        }
  };

  class item
  {
    public:
      static const char class_name[];

      item(const mvc::object_id<item> &id)
        : oid(id)
      {
      }

      mvc::object_id<item> oid;
      std::string name;
      std::string description;
      properties viewproperties;
      properties scriptproperties;

      mvc::script usescript;
  };

  struct position
  {
    int x;
    int y;
  };

  class conversation_tree
  {
  };

  class location;

  class game_object
  {
    public:
      static const char class_name[];
      game_object(const mvc::object_id<game_object> &id)
        : oid(id)
      {
      }

      mvc::object_id<game_object> oid;
      std::string name;
      std::string description;
      properties viewproperties;
      properties scriptproperties;

      bool walkable;

      position location_position;
      conversation_tree object_conversation_tree;
      std::set<mvc::object_id<item> > items;

      mvc::object_id<location> linkedlocation;
      position linkedpositionatlocation;

      mvc::script enterscript;
      mvc::script leavescript;
  };

  class environmental_object
  {
    public:
      static const char class_name[];
      environmental_object(const mvc::object_id<environmental_object> &id)
        : oid(id)
      {
      }

      mvc::object_id<environmental_object> oid;
      std::string name;
      std::string description;
      properties viewproperties;
      properties scriptproperties;

      position location_position;
  };


  class location
  {
    public:
      static const char class_name[];
      location(const mvc::object_id<location> &id)
        : oid(id)
      {
      }

      mvc::object_id<location> oid;
      std::string name;
      std::string description;
      properties viewproperties;
      properties scriptproperties;

      std::set<mvc::object_id<game_object> > gameobjects;
      std::set<mvc::object_id<environmental_object> > environmentalobjects;
  };

  class player
  {
    public:
      static const char class_name[];
      player(const mvc::object_id<player> &id)
        : oid(id)
      {
      }

      mvc::object_id<player> oid;
      std::string name;
      std::string description;
      properties viewproperties;
      properties scriptproperties;

      std::set<mvc::object_id<item> > items;

      mvc::object_id<location> currentlocation;
      position currentposition;
  };
}

#endif

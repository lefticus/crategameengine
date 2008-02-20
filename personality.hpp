
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
      lexical_cast_wrapper<std::string, T> get<T>(const std::string &key)
      {
        return lexical_cast_wrapper(operator[](key));
      }
};

class player
{
  private:
    player(const object_id<player> &id)
      : oid(id)
    {
    }

  public:
    const object_id<player> oid;
    std::string name;
    std::string description;
    properties viewproperties;
    properties scriptproperties;

    std::set<object_id<item> > items;

    object_id<location> currentlocation;
    position currentposition;
};

class game_object
{
  private:
    game_object(const object_id<game_object> &id)
      : oid(id)
    {
    }

  public:
    const object_id<game_object> oid;
    std::string name;
    std::string description;
    properties viewproperties;
    properties scriptproperties;

    bool walkable;

    Position position;
    conversation_tree conversationtree;
    std::set<object_id<item> > items;

    object_id<location> linkedlocation;
    position linkedpositionatlocation;

    action enteraction;
    action leaveaction;
};

class environmental_object
{
  private:
    environmental_object(const object_id<environmental_object> &id)
      : oid(id)
    {
    }

  public:
    const object_id<environmental_object> oid;
    std::string name;
    std::string description;
    properties viewproperties;
    properties scriptproperties;

    position position;
};

class item
{
  private:
    item(const object_id<item> &id)
      : oid(id)
    {
    }

  public:
    const object_id<item> oid;
    std::string name;
    std::string description;
    properties viewproperties;
    properties scriptproperties;

    action useaction;
};


class location
{
  private:
    item(const object_id<location> &id)
      : oid(id)
    {
    }

  public:
    const object_id<location> oid;
    std::string name;
    std::string description;
    properties viewproperties;
    properties scriptproperties;

    std::set<object_id<game_object> > gameobjects;
    std::set<object_id<environmental_object> > environmentalobjects;
};

template<typename T, typename Iterator>
void merge(std::map<T> &dest, Iterator begin, Iterator end)
{
  for (typename std::map<T>::const_iterator itr = begin;
      itr != end;
      ++itr)
  {
    dest.erase(itr->first);
  }

  dest.insert(begin, end);
}


struct game_personality
{
  struct world_reader
  {
    game_object getobject(const object_id<game_object> &oid) const = 0;
    environmental_object getobject(const object_id<environmental_object> &oid) const = 0;
    item getobject(const object_id<item> &oid) const = 0;
    player getobject(const object_id<player> &oid) const = 0;
    location getobject(const object_id<location> &oid) const = 0;

    game_object get_game_object(const object_id_base &oid) const
    {
      return getobject(object_id<game_object>(oid));
    }

    environmental_object get_environmental_object(const object_id_base &oid) const
    {
      return getobject(object_id<environmental_object>(oid));
    }

    item get_item(const object_id_base &oid) const
    {
      return getobject(object_id<item>(oid));
    }

    player get_player(const object_id_base &oid) const
    {
      return getobject(object_id<player>(oid));
    }

    location get_location(const object_id_base &oid) const
    {
      return getobject(object_id<location>(oid));
    }
  };    

  struct world_writer
  {
    virtual void update(const game_object &) = 0;
    virtual void update(const environmental_object &) = 0;
    virtual void update(const item &) = 0;
    virtual void update(const player &) = 0;
    virtual void update(const location &) = 0;
  };

  struct change_set : world_reader, world_writer
  {
    private:
      std::map<object_id<game_object>, game_object> gameobjects;
      std::map<object_id<environmental_object>, environmental_object> environmentalobjects;
      std::map<object_id<item>, item> items;
      std::map<object_id<player>, player> players;
      std::map<object_id<location>, location> locations;

      template<typename T>
        typename T::object_type getobject_impl(const object_id<T> &oid, const std::map<object_id<T>, typename T::object_type> &m) const
        {
          std::map<object_id<T>, typename T::object_type>::const_iterator itr = m.find(oid);
          if (itr != m.end())
          {
            return *itr;
          } else {
            throw object_not_found(oid);
          }
        }

      object_id<player> currentplayer;

    public:
      virtual game_object getobject(const object_id<game_object> &oid)
      {
        return getobject_impl(oid, gameobjects); 
      }

      virtual environmental_object getobject(const object_id<environmental_object> &oid)
      {
        return getobject_impl(oid, environmentalobjects); 
      }

      virtual item getobject(const object_id<item> &oid)
      {
        return getobject_impl(oid, items); 
      }

      virtual player getobject(const object_id<player> &oid)
      {
        return getobject_impl(oid, players); 
      }

      virtual location getobject(const object_id<location> &oid)
      {
        return getobject_impl(oid, locations); 
      }

      virtual void merge(const change_set &cs)
      {
        merge(gameobjects, cs.gameobjects.begin(), cs.gameobjects.end());
        merge(environmentalobjects, cs.environmentalobjects.begin(), cs.environmentalobjects.end());
        merge(items, cs.items.begin(), cs.items.end());
        merge(players, cs.players.begin(), cs.players.end());
        merge(locations, cs.locations.begin(), cs.locations.end());
      }

      reset()
      {
        gameobjects.clear();
        environmentalobjects.clear();
        items.clear();
        players.clear();
        locations.clear();
      }

      void update(const game_object &o)
      {
        gameobjects[o.oid] = o;
      }

      void update(const environmental_object &o)
      {
        environmentalobjects[o.oid] = o;
      }

      void update(const item &o)
      {
        items[o.oid] = o;
      }

      void update(const player &o)
      {
        players[o.oid] = o;
      }

      void update(const location &o)
      {
        locations[o.oid] = o;
      }
  };


  struct world_creator
  {
    game_object create(const object_id<game_object> &oid) = 0;
    environmental_object create(const object_id<environmental_object> &oid) = 0;
    item create(const object_id<item> &oid) = 0;
    player create(const object_id<player> &oid) = 0;
    location create(const object_id<location> &oid) = 0;
  };

  class view : public event_emitter<event_take_item>,
               public event_emitter<event_use_item>,
               public event_emitter<event_talk_to_character>,
               public event_emitter<event_ask_character>,
               public event_emitter<event_move_to>,
               public event_listener<event_character_response>,
               public event_listener<event_character_speak>,
               public event_listener<event_announcement>,
               public mvc::view< view >
  {
    public: 
    view()
      : event_listener<event_character_response>(
          boost::bind(&view::queue_event, this), boost::bind(&view::character_response, this, _1)),
        event_listener<event_character_speak>(
          boost::bind(&view::queue_event, this), boost::bind(&view::character_speak, this, _1)),
        event_listener<event_announcement>(
          boost::bind(&view::queue_event, this), boost::bind(&view::announcement, this, _1))
    {
    }

    private:
      void character_response(const event_character_response &e) = 0;
      void character_speak(const event_character_speak &e) = 0;
      void announcement(const event_announcement &e) = 0;
  };

  struct engine : public event_listener<event_take_item>,
                  public event_listener<event_use_item>,
                  public event_listener<event_talk_to_character>,
                  public event_listener<event_ask_character>,
                  public event_listener<event_move_to>,
                  public mvc::engine< engine >
  {
    engine(view *v, world *w)
      : event_listener<event_take_item>(
          boost::bind(&view::queue_event, this), boost::bind(&view::take_item, this, _1)),
        event_listener<event_use_item>(
          boost::bind(&view::queue_event, this), boost::bind(&view::use_item, this, _1)),
        event_listener<event_take_to_character>(
          boost::bind(&view::queue_event, this), boost::bind(&view::take_to_character, this, _1)),
        event_listener<event_ask_character>(
          boost::bind(&view::queue_event, this), boost::bind(&view::ask_character, this, _1)),
        event_listener<event_move_to>(
          boost::bind(&view::queue_event, this), boost::bind(&view::move_to, this, _1)),
        mvc::engine< engine >(v, w)
    {
      attach<event_take_item>(v, this);
      attach<event_use_item>(v, this);
      attach<event_talk_to_character>(v, this);
      attach<event_ask_character>(v, this);
      attach<event_move_to>(v, this);
      attach<event_character_response>(w, v);
      attach<event_character_speak>(w, v);
      attach<event_announcement>(w, v);
    }

    void take_item(const event_take_item &e)
    {
      vector<object_id> objects;
      objects.push_back(e.player.object_id);
      objects.push_back(e.item.object_id);
      emit(event_run_named_script("takeitem", objects, vector<string>()));
    }

    void use_item(const event_use_item &e)
    {
      vector<object_id> objects;
      objects.push_back(e.item.object_id);
      emit(event_run_named_script("useitem", objects, vector<string>()));
    }
  };


  class world : public world_reader,
                public world_creator,
                public mvc::world< world >

  {
    void apply_change_set(const change_set &cs) = 0;
  }

  class world_view_access : public world_reader
  {
    private:
      change_set m_changeset;

    public:
      void apply_change_set(const change_set &cs)
      {
        m_changeset.merge(cs);
      }

      virtual game_object getobject(const object_id<game_object> &oid)
      {
        return m_changeset.getobject(oid); 
      }

      virtual environmental_object getobject(const object_id<environmental_object> &oid)
      {
        return m_changeset.getobject(oid); 
      }

      virtual item getobject(const object_id<item> &oid)
      {
        return m_changeset.getobject(oid); 
      }

      virtual player getobject(const object_id<player> &oid)
      {
        return m_changeset.getobject(oid); 
      }

      virtual location getobject(const object_id<location> &oid)
      {
        return m_changeset.getobject(oid); 
      }
  }

  class world_script_access : public world_reader, public world_writer,
  {
    private:
      world &m_world;
      changeset m_changeset;

      template<typename T>
        typename T::object_type get_object_impl(const T &oid)
        {
          try {
            return m_changeset.getobject(T);
          } catch (const object_not_found &) {
            return m_word.getobject(T);
          }
        }

    protected:
      change_set get_change_set()
      {
        change_set cs(m_changeset);
        m_changeset.reset();
        return cs;
      }

    public:
      world_script_access(world &w)
        : m_world(w)
      {
      }

      virtual game_object getobject(const object_id<game_object> &oid)
      {
        return getobject_impl(oid); 
      }

      virtual environmental_object getobject(const object_id<environmental_object> &oid)
      {
        return getobject_impl(oid); 
      }

      virtual item getobject(const object_id<item> &oid)
      {
        return getobject_impl(oid); 
      }

      virtual player getobject(const object_id<player> &oid)
      {
        return getobject_impl(oid); 
      }

      virtual location getobject(const object_id<location> &oid)
      {
        return getobject_impl(oid); 
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

      void update(const player &)
      {
        m_changeset.update(o);
      }

      void update(const location &)
      {
        m_changeset.update(o);
      }  

  };
};


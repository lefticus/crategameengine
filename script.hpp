struct script
{
  script(const std::string &t_type, 
         const std::string &t_name, 
         const std::string &t_text)
    : type(t_type), name(t_name), text(t_text)
  {
  }

  std::string type;
  std::string name;
  std::string text;
};

template<typename T>
class world_script_interface : public typename T::world_script_access
{
  public: 
    typedef T world_personality;

    virtual void execute_script(const script &s, 
        const std::vector<object_id_base> &objects,
        const std::vector<std::string> &strings) = 0;

    virtual void execute_named_script(const std::string &name, 
        const std::vector<object_id_base> &objects,
        const std::vector<std::string> &strings) = 0;
};

template<typename T>
class script_handler
{
  public:
    struct invalid_script : std::runtime_error
    {
      invalid_script(const std::string &reason)
        : std::runtime_error("Script could not be used: " + reason)
      {
      }
    };

      struct unknown_named_script : std::runtime_error
    {
      uknown_named_script(const std::string &scriptname)
        : std::runtime_error("Named script does not exist: " + scriptname)
      {
      }
    };

    typedef T world_personality;

    virtual void register_named_script(const script &s) = 0;
    virtual void execute_named_script(const std::string &, 
        world_script_interface<T> &world, 
        const std::vector<object_id_base> &objects, 
        const std::vector<std::string> &strings) = 0;
    virtual void execute(const script &, 
        world_script_interface<T> &world,
        const std::vector<object_id_base> &objects,
        const std::vector<std::string> &strings) = 0;
};


template<typename T>
class compiled_script : public script
{
  public:
    typedef T world_personality;

    compiled_script(const std::string &name, 
        const boost::function<void (world_script_interface<T> &, 
          const std::vector<object_id_base> &,
          const std::vector<std::string> &)> &action)
      : script("compiled", name, ""), m_action(action)
    {
    }

    void execute(world_script_interface<T> &world, 
                 const std::vector<object_id_base> &objects,
                 const std::vector<std::string> &strings) const
    {
      m_action(world, items, strings);
    }

  private:
    boost::function<void (world_script_interface<T> &, 
                          const std::vector<object_id_base> &,
                          const std::vector<std::string> &)> m_action
};

template<typename T>
class compiled_script_handler : public script_handler<T>
{
  public:
    typedef T world_personality;

    struct not_a_compiled_script : invalid_script
    {
      not_a_compiled_script()
        : invalid_script("Not a compiled script")
      {
      }
    };

    void register_named_script(const script &s)
    {
      try {
        m_named_scripts[s.name] = dynamic_cast<const compiled_script<T> &>(s);
      } catch (const std::bad_cast &) {
        throw not_a_compiled_script();
      }
    }

    void execute_named_script(const std::string &name, 
                          world_script_interface<T> &world, 
                          const std::vector<object_id_base> &objects,
                          const std::vector<std::string> &strings)
    {
      std::map<std::string, compiled_script>::iterator itr = m_namedscripts.find(name);

      if (itr != m_named_scripts.end())
      {
        itr->execute(world, objects, strings);
      } else {
        throw unknown_named_script(name);
      }
    }

    void execute(const script &script, 
                 world_script_interface<T> &world, 
                 const std::vector<object_id_base> &objects,
                 const std::vector<std::string> &strings) const
    {
      try {   
        dynamic_cast<const compiled_script<T> &>(script).execute(world, objects, strings);
      } catch (const std::bad_cast &) {
        throw not_a_compiled_script();
      }
    }

  private:
    std::map<std::string, compiled_script<T> > m_named_scripts;
}



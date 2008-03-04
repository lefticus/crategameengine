#include "mvc/logger.hpp"
#include <dlfcn.h>

#include "game_personality/personality.hpp"


class module_holder
{
  public:
    module_holder(const std::string &filename)
    {
      m_mod = dlopen(filename.c_str(), RTLD_LAZY);
      if (!m_mod)
      {
        throw std::runtime_error(std::string("Error loading module: ") + dlerror());
      }
    }

    ~module_holder()
    {
      dlclose(m_mod);
    }

    template<typename T>
    T get_function(const std::string &name)
    {
      T func = (T)dlsym(m_mod, name.c_str());  
      const char* dlsym_error = dlerror();
      if (dlsym_error) 
      {
        throw std::runtime_error("Error loading function: " + name + ": " 
            + dlsym_error);
      }

      return func;
    }
  private:
    void *m_mod;
};

template<typename T>
class game_module
{
  public:
    typedef T* (*create_type)(
        const boost::function<void (mvc::logger::log_level,
        const std::string&, const std::string &)> &);

    typedef void (*destroy_type)(T *);

    game_module(
        const std::string &modname,
        const boost::function<void (mvc::logger::log_level,
          const std::string&, const std::string &)> &logfunc)
      : m_game_mod(modname),
        m_create(m_game_mod.get_function<create_type>("create")),
        m_destroy(m_game_mod.get_function<destroy_type>("destroy")),
        m_mod(m_create(logfunc))
    {
    }

    ~game_module()
    {
      m_destroy(m_mod);
    }

    T *operator->() const 
    {
      return m_mod;
    }

    T &operator*() const
    {
      return *m_mod;
    }

  private:
    module_holder m_game_mod;

    create_type m_create;
    destroy_type m_destroy;

    T *m_mod;
};


class game_engine_module
{
  public:
    typedef game_personality::personality::engine* (*create_type)(
        const boost::function<void (mvc::logger::log_level,
        const std::string&, const std::string &)> &,
        game_personality::personality::view &,
        game_personality::personality::world &);

    typedef void (*destroy_type)(game_personality::personality::engine *);

    game_engine_module(
        const std::string &modname,
        const boost::function<void (mvc::logger::log_level,
          const std::string&, const std::string &)> &logfunc,
        game_personality::personality::view &v,
        game_personality::personality::world &w)
      : m_game_engine_mod(modname),
        m_create(m_game_engine_mod.get_function<create_type>("create")),
        m_destroy(m_game_engine_mod.get_function<destroy_type>("destroy")),
        m_engine(m_create(logfunc, v, w))
    {

    }

    ~game_engine_module()
    {
      m_destroy(m_engine);
    }


    game_personality::personality::engine *operator->() const 
    {
      return m_engine;
    }

  private:
    module_holder m_game_engine_mod;

    create_type m_create;
    destroy_type m_destroy;

    game_personality::personality::engine *m_engine;

};

int main(int, char **)
{
  mvc::logger l;

  boost::function<void (mvc::logger::log_level, const std::string &, const std::string &)>
    logger_func(boost::bind(&mvc::logger::log, &l, _1, _2, _3));

  game_module<game_personality::personality::world> w("./libmemory_world.so", logger_func);
  game_module<game_personality::personality::view> v("./libviewerstub.so", logger_func);

  game_engine_module e("./libgame_engine.so", logger_func, *v, *w);

  w->create(mvc::object_id<game_personality::player>("1"));
  w->create(mvc::object_id<game_personality::item>("1"));
  w->create(mvc::object_id<game_personality::item>("2"));
  w->create(mvc::object_id<game_personality::game_object>("1"));
  w->create(mvc::object_id<game_personality::game_object>("2"));


  w->start();
  v->start();

  mvc::emit<game_personality::event_take_item>(*v, game_personality::event_take_item(
        w->getobject(mvc::object_id<game_personality::player>("1")),
        w->getobject(mvc::object_id<game_personality::item>("1"))));

  mvc::emit<game_personality::event_use_item>(*v, game_personality::event_use_item(
        w->getobject(mvc::object_id<game_personality::player>("1")),
        w->getobject(mvc::object_id<game_personality::item>("1"))));

  mvc::emit<game_personality::event_use_item_with>(*v, game_personality::event_use_item_with(
        w->getobject(mvc::object_id<game_personality::player>("1")),
        w->getobject(mvc::object_id<game_personality::item>("1")),
        w->getobject(mvc::object_id<game_personality::item>("2"))));

  mvc::emit<game_personality::event_talk_to_character>(*v, game_personality::event_talk_to_character(
        w->getobject(mvc::object_id<game_personality::player>("1")),
        w->getobject(mvc::object_id<game_personality::game_object>("1"))));

  mvc::emit<game_personality::event_ask_character>(*v, game_personality::event_ask_character(
        w->getobject(mvc::object_id<game_personality::player>("1")),
        w->getobject(mvc::object_id<game_personality::game_object>("1")),
        "what's up?"));

  mvc::emit<game_personality::event_move_to>(*v, game_personality::event_move_to(
        w->getobject(mvc::object_id<game_personality::player>("1")),
        w->getobject(mvc::object_id<game_personality::game_object>("2"))));


  e->run();

  
}

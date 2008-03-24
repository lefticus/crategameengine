#ifndef __game_personality_lua_handler__
#define __game_personality_lua_handler__

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}



namespace game_personality
{
#include "world_script_access_wrap.hpp"

  class lua_handler : public script_handler
  {
    public:
    struct not_a_lua_script : script_handler::invalid_script
    {
      not_a_lua_script()
        : script_handler::invalid_script("Not a lua script")
      {
      }
    };


    lua_handler(const boost::function<void (mvc::logger::log_level, 
          const std::string &, const std::string &)> &t_logger)
      : script_handler("lua", t_logger)
    {

    }

    virtual void register_named_script(const script &s)
    {
      if (s.type != name) throw not_a_lua_script();
      m_registered_scripts[s.name] = s.text;
    }

    virtual void execute_named_script(const std::string &name, 
        world_script_access &world,
        const std::vector<mvc::object_id_base> &objects, 
        const std::vector<std::string> &strings)
    {
      std::map<std::string, std::string>::const_iterator itr = m_registered_scripts.find(name);

      if (itr == m_registered_scripts.end())
      {
        throw unknown_named_script(name);
      } else {
        run_script(itr->second, world, objects, strings);
      }
    }
    
    virtual void execute(const script &s, 
        world_script_access &world,
        const std::vector<mvc::object_id_base> &objects,
        const std::vector<std::string> &strings) 
    {
      if (s.type != name) throw not_a_lua_script();
      run_script(s.text, world, objects, strings);
    }

    private:
    void run_script(
        const std::string &script,
        world_script_access &world,
        const std::vector<mvc::object_id_base> &objects, 
        const std::vector<std::string> &strings)
    {

      m_logger(mvc::logger::debug, "Running script: " + script);

      m_logger(mvc::logger::debug, "Opening Lua");
      lua_State *l = lua_open();

      luaL_reg lualibs[] = {
        {"base", luaopen_base},
        {"table", luaopen_table},
        /* {"io", luaopen_io}, */
        {"string", luaopen_string},
        {"math", luaopen_math},
        {"debug", luaopen_debug},
        /*{"loadlib", luaopen_loadlib}, */
        /* add your libraries here */
        {SWIG_name, SWIG_init},
        {NULL, NULL}
      };


      m_logger(mvc::logger::debug, "Loading Lua Libs");

      for(int i=0; lualibs[i].func != 0 ; i++) {
        m_logger(mvc::logger::debug, std::string("Lua Lib: ") + lualibs[i].name + " Loaded");
        lualibs[i].func(l);  /* open library */
        lua_settop(l, 0);  /* discard any results */
      }

      m_logger(mvc::logger::debug, "Setting up Lua Variables");
      SWIG_NewPointerObj(l, &world, SWIGTYPE_p_game_personality__world_script_access, false);
      lua_setglobal(l, "m_world");
      SWIG_NewPointerObj(l, &objects, SWIGTYPE_p_std__vectorTmvc__object_id_base_t, false);
      lua_setglobal(l, "m_objects");
      SWIG_NewPointerObj(l, &strings, SWIGTYPE_p_std__vectorTstd__string_t, false);
      lua_setglobal(l, "m_strings");

      m_logger(mvc::logger::debug, "Loading Lua command buffer");
      int result = luaL_loadbuffer(l, script.c_str(), script.length(), "script");

      if (result == 0) {
        m_logger(mvc::logger::debug, "No errors loading Lua Script");
        m_logger(mvc::logger::debug, "Calling lua script");

        if (lua_pcall(l, 0, 0, 0) != 0) {
            m_logger(mvc::logger::error, std::string("Error executing Lua Script: ") 
                + lua_tostring(l, lua_gettop(l)));
        }

      } else if (result == LUA_ERRSYNTAX) {
        m_logger(mvc::logger::error, 
          std::string("Syntax error loading Lua Script: ") + lua_tostring(l, lua_gettop(l)));
      } else if (result == LUA_ERRMEM) {
        m_logger(mvc::logger::error, 
          std::string("Memory error loading Lua Script: ") + lua_tostring(l, lua_gettop(l)));
      }

      m_logger(mvc::logger::debug, "Closing Lua Engine");
      lua_close(l);
    }

    std::map<std::string, std::string> m_registered_scripts;
  };
}

#endif

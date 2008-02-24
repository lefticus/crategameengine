#include "game_personality/personality.hpp"
#include "logger.hpp"

class worldimpl : public game_personality::personality::world
{
  public:
    worldimpl(const boost::function<void (mvc::logger::log_level, const std::string &, const std::string &)> 
        &t_logger)
      : game_personality::personality::world(t_logger),
        m_logger(boost::bind(t_logger, _1, "worldimpl", _2))
    {
      m_logger(mvc::logger::debug, "constructor called");
    }


    void apply_change_set(const game_personality::personality::change_set &cs)
    {
    }

    game_personality::game_object create(const mvc::object_id<game_personality::game_object>& oid)
    {
      return game_personality::game_object(oid);
    }

    game_personality::environmental_object create(
        const mvc::object_id<game_personality::environmental_object>& oid)
    {
      return game_personality::environmental_object(oid);
    }

    game_personality::item create(
        const mvc::object_id<game_personality::item>& oid)
    {
      return game_personality::item(oid);
    }

    game_personality::player create(
        const mvc::object_id<game_personality::player>& oid)
    {
      return game_personality::player(oid);
    }

    game_personality::location create(
        const mvc::object_id<game_personality::location>& oid)
    {
      return game_personality::location(oid);
    }

    game_personality::game_object getobject(
        const mvc::object_id<game_personality::game_object>& oid) const
    {
      return game_personality::game_object(oid);
    }

    game_personality::environmental_object getobject(
        const mvc::object_id<game_personality::environmental_object>& oid) const
    {
      return game_personality::environmental_object(oid);
    }

    game_personality::item getobject(
        const mvc::object_id<game_personality::item>& oid) const
    {
      return game_personality::item(oid);
    }

    game_personality::player getobject(
        const mvc::object_id<game_personality::player>& oid) const
    {
      return game_personality::player(oid);
    }

    game_personality::location getobject(
        const mvc::object_id<game_personality::location>& oid) const
    {
      return game_personality::location(oid);
    }

  private:
    boost::function<void (mvc::logger::log_level, const std::string &)> m_logger;
};

class viewimpl : public game_personality::personality::view
{
  public:
    viewimpl(const boost::function<void (mvc::logger::log_level, const std::string &, const std::string &)> 
        &t_logger)
      : game_personality::personality::view(t_logger),
        m_logger(boost::bind(t_logger, _1, "viewimpl", _2))
    {
      m_logger(mvc::logger::debug, "constructor called");
    }

  private:
    boost::function<void (mvc::logger::log_level, const std::string &)> m_logger;

    void before_world_changed(const mvc::event_world_changed<game_personality::personality::change_set>&)
    {
    }

    void after_world_changed(const mvc::event_world_changed<game_personality::personality::change_set>&)
    {
    }

    void character_response(const game_personality::event_character_response&)
    {
    }

    void character_speak(const game_personality::event_character_speak&)
    {
    }

    void announcement(const game_personality::event_announcement&)
    {
    }
};

class engineimpl : public game_personality::personality::engine
{
  public: 
    engineimpl(const boost::function<void (mvc::logger::log_level, const std::string &, const std::string &)> 
                   &t_logger, viewimpl &v, worldimpl &w)
      : game_personality::personality::engine(t_logger, v, w),
        m_logger(boost::bind(t_logger, _1, "engineimpl", _2))
    {
      m_logger(mvc::logger::debug, "constructor called");
    }

  private:
    boost::function<void (mvc::logger::log_level, const std::string &)> m_logger;
};

int main(int, char **)
{
  mvc::logger l;

  worldimpl w(boost::bind(&mvc::logger::log, &l, _1, _2, _3));
  viewimpl v(boost::bind(&mvc::logger::log, &l, _1, _2, _3));
  engineimpl e(boost::bind(&mvc::logger::log, &l, _1, _2, _3), v, w);

  w.start();
  v.start();

  mvc::emit<game_personality::event_take_item>(v, game_personality::event_take_item(
        w.create(mvc::object_id<game_personality::player>()),
        w.create(mvc::object_id<game_personality::item>())));

  e.run();
}

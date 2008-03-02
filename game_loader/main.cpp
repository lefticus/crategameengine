#include "game_viewers/stub/viewstub.hpp"
#include "game_engine/engineimpl.hpp"
#include "game_worlds/memory/memoryworld.hpp"

int main(int, char **)
{
  mvc::logger l;

  boost::shared_ptr<game_personality::personality::world> w 
    (new game_worlds::memoryworld(boost::bind(&mvc::logger::log, &l, _1, _2, _3)));
  boost::shared_ptr<game_personality::personality::view> v
    (new game_viewers::viewstub(boost::bind(&mvc::logger::log, &l, _1, _2, _3)));
  boost::shared_ptr<game_personality::personality::engine> e
    (new game_engine::engineimpl(boost::bind(&mvc::logger::log, &l, _1, _2, _3), *v, *w));

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

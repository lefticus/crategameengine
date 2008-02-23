#ifndef __game_personality_events_h__
#define __game_personality_events_h__

#include "objects.hpp"

namespace game_personality
{
  struct event_take_item
  {
    event_take_item(const mvc::object_id<player> &t_player,
                    const mvc::object_id<item> &t_item)
      : taking_player(t_player),
        item_to_take(t_item)
    {
    }

    mvc::object_id<player> taking_player;
    mvc::object_id<item> item_to_take;
  };

  struct event_use_item
  {
    event_use_item(const mvc::object_id<player> &t_player,
                   const mvc::object_id<item> &t_item)
      : using_player(t_player),
        item_to_use(t_item)
    {
    }

    mvc::object_id<player> using_player;
    mvc::object_id<item> item_to_use;
  };

  struct event_use_item_with
  {
    event_use_item_with(const mvc::object_id<player> &t_player,
                        const mvc::object_id<item> &t_item,
                        const mvc::object_id<item> &t_item_with)
      : using_player(t_player),
        item_to_use(t_item),
        item_to_use_with(t_item_with)
    {
    }

    mvc::object_id<player> using_player;
    mvc::object_id<item> item_to_use;
    mvc::object_id<item> item_to_use_with;
  };

  struct event_talk_to_character
  {
    event_talk_to_character(const mvc::object_id<game_object> &t_object)
      : character_to_talk_to(t_object)
    {
    }

    mvc::object_id<game_object> character_to_talk_to;
  };

  struct event_ask_character
  {
    event_ask_character(const mvc::object_id<game_object> &t_object,
                        const std::string &t_question)
      : character_to_ask(t_object),
        question(t_question)
    {
    }

    mvc::object_id<game_object> character_to_ask;
    std::string question;
  };

  struct event_character_response
  {
    event_character_response(const mvc::object_id<game_object> &t_character,
                             const std::vector<std::string> &t_responses)
      : character_responding(t_character),
        responses(t_responses)
    {
    }

    mvc::object_id<game_object> character_responding;
    std::vector<std::string> responses;
  };

  struct event_character_speak
  {
    event_character_speak(const mvc::object_id<game_object> &t_character,
                             const std::vector<std::string> &t_messages)
      : speaking_character(t_character),
        messages(t_messages)
    {
    }

    mvc::object_id<game_object> speaking_character;
    std::vector<std::string> messages;
  };

  struct event_move_to
  {
    event_move_to(const mvc::object_id<player> &t_player,
                  const mvc::object_id<game_object> &t_move_to)
      : moving_player(t_player),
        object_to_move_to(t_move_to)
    {
    }

    mvc::object_id<player> moving_player;
    mvc::object_id<game_object> object_to_move_to;
  };

  struct event_announcement
  {
    event_announcement(const std::vector<std::string> &t_messages)
      : messages(t_messages)
    {
    }

    std::vector<std::string> messages;
  };
}


#endif

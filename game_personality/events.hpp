#ifndef __game_personality_events_h__
#define __game_personality_events_h__

#include "objects.hpp"

namespace game_personality
{
  struct event_take_item
  {
    event_take_item(const mvc::object_id<player> &t_player,
                    const mvc::object_id<item> &t_item)
      : m_player(t_player),
        m_item(t_item)
    {
    }

    mvc::object_id<player> m_player;
    mvc::object_id<item> m_item;
  };

  struct event_use_item
  {
    event_use_item(const mvc::object_id<player> &t_player,
                   const mvc::object_id<item> &t_item)
      : m_player(t_player),
        m_item(t_item)
    {
    }

    mvc::object_id<player> m_player;
    mvc::object_id<item> m_item;
  };

  struct event_use_item_with
  {
    event_use_item_with(const mvc::object_id<player> &t_player,
                        const mvc::object_id<item> &t_item,
                        const mvc::object_id<item> &t_item_with)
      : m_player(t_player),
        m_item(t_item),
        m_item_with(t_item_with)
    {
    }

    mvc::object_id<player> m_player;
    mvc::object_id<item> m_item;
    mvc::object_id<item> m_item_with;
  };

  struct event_talk_to_character
  {
    event_talk_to_character(const mvc::object_id<player> &t_player,
                            const mvc::object_id<game_object> &t_character)
      : m_player(t_player),
        m_character(t_character)
    {
    }

    mvc::object_id<game_object> m_player;
    mvc::object_id<game_object> m_character;
  };

  struct event_ask_character
  {
    event_ask_character(const mvc::object_id<player> &t_player,
                        const mvc::object_id<game_object> &t_character,
                        const std::string &t_question)
      : m_player(t_player),
        m_character(t_character),
        m_question(t_question)
    {
    }

    mvc::object_id<game_object> m_player;
    mvc::object_id<game_object> m_character;
    std::string m_question;
  };

  struct event_character_response
  {
    event_character_response(const mvc::object_id<player> &t_player,
                             const mvc::object_id<game_object> &t_character,
                             const std::vector<std::string> &t_responses)
      : m_player(t_player),
        m_character(t_character),
        m_responses(t_responses)
    {
    }

    mvc::object_id<player> m_player;
    mvc::object_id<game_object> m_character;
    std::vector<std::string> m_responses;
  };

  struct event_character_speak
  {
    event_character_speak(const mvc::object_id<player> &t_player,
                          const mvc::object_id<game_object> &t_character,
                          const std::vector<std::string> &t_messages)
      : m_player(t_player),
        m_character(t_character),
        m_messages(t_messages)
    {
    }

    mvc::object_id<player> m_player;
    mvc::object_id<game_object> m_character;
    std::vector<std::string> m_messages;
  };

  struct event_move_to
  {
    event_move_to(const mvc::object_id<player> &t_player,
                  const mvc::object_id<game_object> &t_place)
      : m_player(t_player),
        m_place(t_place)
    {
    }

    mvc::object_id<player> m_player;
    mvc::object_id<game_object> m_place;
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

#ifndef PLAYER_ACTION_HPP
#define PLAYER_ACTION_HPP

#include "common/alias.hpp"

enum class PlayerAction : byte
{
  NONE = 0,         //!< @todo
  ROLL,             //!< @todo
  HOLD,             //!< @todo
  QUIT,             //!< @todo
  CONFIRM_QUIT_NO,  //!< @todo
  CONFIRM_QUIT_YES, //!< @todo
  UNKNOWN,          //!< @todo
  INVALID           //!< @todo
};

using action_t = PlayerAction;
using action_map_t = std::unordered_map<char, action_t>;

#endif //!< PLAYER_ACTION_HPP
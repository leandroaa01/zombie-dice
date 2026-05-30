/**
 * @file player.hpp
 * @author José Carlos da Paz Silva (carlos.paz.707@ufrn.edu.br)
 * @author Leandro Andrade (leandro.andrade.401@ufrn.edu.br)
 * @brief Player class definition
 * @version 0.1
 * @date 2025-06-24
 *
 * @copyright Copyright (c) 2025
 *
 */
#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "common/alias.hpp"

struct Player
{
  size_type m_id{}; //<! Player ID
  str m_name{};     //<! Player name
  flag m_is_bot{false};

  /**
   * @brief Construct a new Player object
   * this constructor initializes the player with an ID and a name.
   * @param id  represents the player's ID, which is a unique identifier for each player.
   * @param name  represents the player's name, which is a string that identifies the player.
   */
  Player(size_type id, str& name, flag is_bot = false) : m_id(id), m_name(name), m_is_bot(is_bot) { }
  /**
   * @brief Get the id player object
   *
   * @return size_type  returns the player's ID, which is a unique identifier for each player.
   */
  size_type get_id() const { return m_id; }

  /**
   * @brief Get the name player object
   *
   * @return str  returns the player's name, which is a string that identifies the player.
   */
  str get_name() const { return m_name; }
  /**
   * @brief Check if the player is a bot.
   *
   * @return flag  returns true if the player is a bot, false otherwise.
   */
  flag is_bot() const { return m_is_bot; }
};

using players_t = vec<Player>;
using player_ptr = Player*;

#endif //<! PLAYER_HPP
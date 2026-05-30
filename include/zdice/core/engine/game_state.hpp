/**
 * @file game_state.hpp
 * @author José Carlos da Paz Silva (carlos.paz.707@ufrn.edu.br)
 * @author Leandro Andrade (leandro.andrade.401@ufrn.edu.br) 
 * @brief  This file defines the GameState enum class, which represents the various states of the game.
 * @version 0.1
 * @date 2025-06-24
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP

#include "common/alias.hpp"

/// @brief Enumeration representing the different states of the game.
enum class GameState : byte
{
  UNDEFINED = 0,       //!< @todo
  WELCOME,             //!< @todo
  LOBBY,               //!< @todo
  ERROR_IN_LOBBY,      //!< @todo
  EXIT_PROGRAM,        //!< @todo
  START_GAME,          //!< @todo
  SELECTING_PLAYER,    //!< @todo
  PLAYING,             //!< @todo
  ERROR_IN_PLAYING,    //!< @todo
  HOLDING,             //!< @todo
  ROLLING,             //!< @todo
  RECYLING_DICES,      //!< @todo
  DIED,                //!< @todo
  ALIVE,               //!< @todo
  QUITTING,            //!< @todo
  ERROR_IN_QUITTING,   //!< @todo
  PLAYER_LEFT_GAME,    //!< @todo
  ENDING_TURN,         //!< @todo
  TIE_BREAKER_SETUP,   //! preparação do desempate
  TIE_BREAKER_PLAYING, //! turno de desempate
  TIE_BREAKER_END,     //! final do desempate
  GAME_END             //!< @todo
};

/// @brief Alias for the GameState enum class.
using state = GameState;

#endif //!< GAME_STATE_HPP
/**
 * @file dice.hpp class for dice representation
 * @author José Carlos da Paz Silva (carlos.paz.707@ufrn.edu.br)
 * @author Leandro Andrade (leandro.andrade.401@ufrn.edu.br)
 * @version 0.1
 * @date 2025-06-24
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef DICE_HPP
#define DICE_HPP

#include "color.hpp"
#include "common/alias.hpp"
#include "face.hpp"
#include "zdice/config/zdice_file.hpp"

/**
 * @brief  Class representing a dice in the ZDice game.
 * 
 */
struct Dice
{
  Color m_color{ Color::NONE };  //!< Color of the dice.
  size_type m_brains_count{ 0 }; //!< Number of "brain" faces on the dice.
  size_type m_shots_count{ 0 };  //!< Number of "shots" faces on the dice.
  size_type m_foots_count{ 0 };  //!< Number of "footprint" faces on the dice.
  /**
   * @brief  Default constructor for Dice.
   * 
   */
  Dice() = default;
  /**
   * @brief  Constructor for Dice with specified parameters.
   * 
   * @param c Color of the dice.
   * @param b Number of "brain" faces.
   * @param s Number of "shot" faces.
   * @param f Number of "footprint" faces.
   */
  explicit
  Dice(Color c, size_type b, size_type s, size_type f)
  : m_color(c), m_brains_count(b), m_shots_count(s), m_foots_count(f)
  {
  }
  
  /**
   * @brief  total_faces 
   * 
   * @return size_type  returns the total number of faces on the dice.
   */
  size_type total_faces() const { return m_brains_count + m_shots_count + m_foots_count; }
  
  /**
   * @brief roll dice 
   *  this function simulates rolling the dice and returns a random face based on the distribution of faces.
   * @return Face  returns a random face from the dice.
   */
  Face roll() const;
};

using die_area_t = std::vector<Dice>;

#endif //!< DICE_HPP
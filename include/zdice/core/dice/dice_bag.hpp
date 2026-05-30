/**
 * @file dice_bag.hpp
 * @author José Carlos da Paz Silva (carlos.paz.707@ufrn.edu.br)
 * @author Leandro Andrade (leandro.andrade.401@ufrn.edu.br) 
 * @version 0.1
 * @date 2025-06-24
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef DICE_BAG_HPP
#define DICE_BAG_HPP

#include "color.hpp"
#include "common/alias.hpp"
#include "common/utils.hpp"
#include "dice.hpp"

//alias for a vector of Dice objects
using vec_dice = vec<Dice>;
/**
 * @brief  DiceBag class represents a collection of Dice objects, allowing operations such as adding, picking, and placing dice.
 * 
 */
class DiceBag
{
private:
  vec_dice m_dices;

public:
  /**
   * @brief  Default constructor for DiceBag.
   * 
   */
  DiceBag() = default;

  /**
   * @brief  this method checks that the dice bag has enough to roll the 3 dice
   * 
   * @param n  the number of dices to check
   * @return true  se the dice bag has enough dices to roll
   * @return false  if the dice bag does not have enough dices to roll
   */
  bool has_sufficient(size_type n) const { return m_dices.size() >= n; }
 
  /**
   * @brief  Returns the number of dices in the bag.
   * @return size_type  returns the number of dices in the bag.
   */
  size_type size() const { return m_dices.size(); }

  /**
   * @brief putting the dices in the bag
   * 
   */
  void push(Dice);

  /**
   * @brief Picks a specified number of dices from the bag and places them in the provided vector.
   * 
   * @param n The number of dices to pick.
   * @param destination The vector where the picked dices will be placed.
   */
  void pick(size_type, vec_dice&);
   
  /**
   * @brief  Places the dices from the provided vector back into the bag.
   * 
   */
  void place(vec_dice&);

  /**
   * @brief  Returns a string representation of the dices in the bag.
   * 
   * @return str  returns a string representation of the dices in the bag.
   */
  [[nodiscard]]
  str to_dices_color() const
  {
    std::ostringstream msg;
    msg << " [";
    for (const auto& dice : m_dices)
    {
      msg << color_symbol(dice.m_color) << " ";
    }
    msg << "]\n";
    return msg.str();
  }
  /**
   * @brief  Returns a vector of dices in the bag.
   * 
   * @return vec_dice  returns a vector of dices in the bag.
   */
  [[nodiscard]]
  vec_dice get_dices() const
  {
    return m_dices;
  }
};

#endif //<! DICE_BAG_HPP
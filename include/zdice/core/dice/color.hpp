/**
 * @file color.hpp
 * @author José Carlos da Paz Silva (carlos.paz.707@ufrn.edu.br)
 * @author Leandro Andrade (leandro.andrade.401@ufrn.edu.br) 
 * @version 0.1
 * @date 2025-06-24
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef COLOR_HPP
#define COLOR_HPP

#include "common/alias.hpp"

/**
 * @brief  Enum representing colors used in the dice game.
 * 
 */
enum class Color : byte
{
  NONE = 0, //!< No color.
  GREEN,    //!< Color green.
  YELLOW,   //!< Color yellow.
  RED,      //!< Color red.
};

/**
 * @brief  Returns the string representation of the color symbol.
 * 
 * @param  color The color to get the symbol for.
 * @return str The string representation of the color symbol.
 */
[[nodiscard]]
inline str color_symbol(Color color)
{
  switch (color)
  {
    case Color::GREEN:
      return "🟩";
    case Color::YELLOW:
      return "🟨";
    case Color::RED:
      return "🟥";
    default:
      return "  ";
  }
}

#endif //<! COLOR_HPP
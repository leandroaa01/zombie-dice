/**
 * @file face.hpp
 * @author José Carlos da Paz Silva (carlos.paz.707@ufrn.edu.br)
 * @author Leandro Andrade (leandro.andrade.401@ufrn.edu.br) 
 * @date 2025-06-24
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef FACE_HPP
#define FACE_HPP

#include "common/alias.hpp"

/**
 * @brief  Enumeration representing different types of faces on dice.
 * 
 */
enum class Face : byte
{
  NONE = 0,  //!< No face.
  BRAIN,     //!< Face with a brain.
  SHOTGUN,   //!< Face with a shotgun.
  FOOTPRINT, //!< Face with a footprint.
};

/**
 * @brief  Returns a string representation of the face symbol.
 * 
 * @param face The face to get the symbol for.
 * @return A string representing the face symbol.
 */
[[nodiscard]]
inline str face_symbol(Face face)
{
  switch (face)
  {
    case Face::BRAIN:
      return "🧠";
    case Face::SHOTGUN:
      return "💥";
    case Face::FOOTPRINT:
      return "👣";
    default:
      return "  ";
  }
}

#endif //<! face_hpp
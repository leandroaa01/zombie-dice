#include "zdice/core/dice/dice.hpp"
#include "common/alias.hpp"
#include "zdice/core/dice/face.hpp"

#include <random>

Face Dice::roll() const
{
  //!< Randomly selected face.
  std::random_device rd;
  std::mt19937 g(rd());
  std::uniform_int_distribution<> distrib(1, total_faces());

  size_type random_face{ static_cast<size_type>(distrib(g)) };

  /*
   * The following checks take the intervals into account:
   *
   * - [1; n_brain_faces]
   * - [n_brain_faces + 1; n_shot_faces] and
   * - [n_shot_faces + 1; total_faces],
   *
   * respectively.
   */
  // if (m_brains_count != 0 and random_face <= m_brains_count) return Face::BRAIN;
  // if (m_foots_count != 0 and random_face <= (m_brains_count + m_shots_count)) return Face::FOOTPRINT;
  // return (m_shots_count != 0) ? Face::SHOTGUN : Face::NONE;
  if (random_face <= m_brains_count) return Face::BRAIN;
  if (random_face <= (m_brains_count + m_shots_count)) return Face::SHOTGUN;
  return Face::FOOTPRINT;
}

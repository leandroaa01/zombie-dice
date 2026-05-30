/**
 * @file main.cpp
 *
 * @description
 * This program implements an interactive Zombie Dice Game.
 *
 * ===========================================================================
 * @license
 *
 * This file is part of ZOMBIE DICE GAME project.
 *
 * ZOMBIE DICE GAME is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ZOMBIE DICE GAME is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ZOMBIE DICE GAME.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright (C) 2024-2025 Selan R. dos Santos.
 * ===========================================================================
 *
 * @author	Selan R dos Santos, <selan.santos@ufrn.br>
 * @date	2024
 */

#include <cstdlib>
#include <iostream>

#include "common/alias.hpp"
#include "zdice/config/zdice_file.hpp"
#include "zdice/core/engine/game_controller.hpp"

raw_str help = R"(  Welcome to zdice cpp, version 1.0, (c) Dimap/UFRN.

  NAME
    zdice - zombie dice.
  
  SYPOPSIS
    zdice [-h | --help ] <file.ini>
  
  EXAMPLES
    zdice 
      In this mode, you use the game's default settings

    zdice zd.ini
      In this mode, a configuration file that the user creates 'zd.ini' is used.

  DESCRIPTION
    In Zombie Dice you and the other players are zombies (🧟) trying to collect brains
    and avoid the very mean humans who are trying to blow you away with their shotguns.
    The player who has the most brains when the game ends will win.

  OPTIONS FOR .INI:
    [Game]
    WEAK_DICE         Quantity of weak dices in the bag.
    STRONG_DICE       Quantity of strong dices in the bag.
    TOUGH_DICE        Quantity of tough dices in the bag.
    MAX_PLAYERS       Max quantity of players.
    BRAINS_TO_WIN     Minimal number of brains need to win.
    MAX_TURNS         Max number of turns in a round.
    ISBOT             If true, the game will play with bots.
    N_BOTS            Number of bots to be used in the game.

    [Dice]
    WEAK_DIE_FACES    Number of faces and the configuration for the weak dice.
    STRONG_DIE_FACES  Number of faces and the configuration for the strong dice.
    TOUGH_DIE_FACES   Number of faces and the configuration for the tough dice.
  
  EXAMPLES
    These are the default settings:
    [Game]
    weak_dice = 6
    strong_dice = 4
    tough_dice = 3
    max_players = 6
    brains_to_win = 13
    max_turns = 0 # Value 0 means the game plays without limit of turns.
    isbot = false
    n_bots = 0

    [Dice]
    weak_die_faces   = "bbbffs"
    strong_die_faces = "bbffss"
    tough_die_faces  = "bffsss"

)";

void parse_arguments(int argc, char* argv[], ZDiceFile& zdf)
{
  if (argc <= 1) return;

  str arg{ argv[1] };

  if (arg == "-h" or arg == "--help")
  {
    /// @todo Print help message and exit.
    std::cout << help << "\n";
    exit(EXIT_SUCCESS);
  }
  else
  {
    const str& path{ arg };
    zdf.load(path);
  }
}

int main(int argc, char* argv[])
{
  // [!] 1. Create the ZDiceFile object with default values already defined.
  ZDiceFile zdice_file{};

  // [!] 2. Handle the possibles CLI arguments.
  parse_arguments(argc, argv, zdice_file);

  // [!] 3. Create the game controller.
  GameController gc{ zdice_file };

  // [!] 4. Start the game.
  gc.run();

  return EXIT_SUCCESS;
}

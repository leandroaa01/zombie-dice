/**
 * @file zdice_interface.hpp
* @author José Carlos da Paz Silva (carlos.paz.707@ufrn.edu.br)
 * @author Leandro Andrade (leandro.andrade.401@ufrn.edu.br) 
 * @brief  This file defines the ZDiceInterface class, which provides methods to
 *  interact with the game interface.
 * @version 0.1
 * @date 2025-06-24
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef ZDICE_INTERFACE_HPP
#define ZDICE_INTERFACE_HPP

//== ZDice includes ==//
#include "common/alias.hpp"
#include "zdice/core/dice/color.hpp"
#include "zdice/core/dice/dice_bag.hpp"
#include "zdice/core/dice/face.hpp"
#include "zdice/core/player/player.hpp"

//== STL includes ==//
#include <algorithm>
#include <array>
#include <cstddef>
#include <iomanip>
#include <ios>
#include <iostream>
#include <sstream>
#include <strstream>
#include <sys/types.h>


class ZDiceInterface
{
private:
  const ZDiceFile& m_config;

public:
  ZDiceInterface(const ZDiceFile& file) : m_config(file) { }

private:
  //<! logo of game
  const raw_str logo = R"(
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ ⠀⠀⣀⣠⣤⡶⢶⣲⢲⠦⣄⡄⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ ⠀⠀⢀⣤⣶⢿⣻⣯⢷⣟⣯⢷⣏⡞⣶⠉⣼⡑⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ ⠀⣠⣾⣿⣳⣯⣟⡷⣯⢿⣞⣯⢿⡞⣽⢪⠱⢲⡹⣱⠆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ ⢠⣼⣿⢯⣷⣟⡷⣯⣻⢽⣻⣾⣯⢿⡽⡇⡾⣅⢶⢱⠏⣯⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⣭⣽⣾⡿⣿⣾⣿⣽⡿⣎⢿⣷⣹⢿⣻⠶⣉⣬⢞⡭⣛⠀⣧⣀⠾⢩⠇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ ⣻⣿⣽⡷⣿⣟⣷⠛⣾⣣⠻⢰⣍⡾⢯⣣⢞⣧⢩⢦⡹⢞⡀⢳⣜⡃⡼⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀ ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢨⣿⣯⣿⣝⢿⣻⡳⢮⣱⢻⡜⣾⠹⣧⢛⡾⣜⠋⠀⠠⠆⣤⢏⡄⠧⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ ⣿⣿⡾⣽⡷⡕⣥⢳⣡⡹⣝⡞⣸⠧⣏⠶⠃⡀⠀⠀⠀⠈⢶⢢⠝⡶⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ ⠀ ⣝⣧⣿⢯⡹⣍⠶⣌⠃⡧⢞⠀⣽⣛⢾⠋⡀⢀⣴⣶⠆⠀⣸⠭⢦⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ ⠀⠀⣼⣬⣿⣛⣉⣚⣨⠜⡋⠇⠸⠀⠷⡏⠏⠀⢠⣿⣿⠟⢀⡰⣧⣛⠾⣼⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢰⣿⣿⣽⡿⣿⣿⣿⣿⠌⠂⠄⡤⡤⡔⡀⣀⠀⠁⠀⣠⢼⣛⡦⣯⣳⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ ⠀⠀⢸⣿⣿⠛⠉⠉⠉⠈⠁⣸⡏⡟⠁⠱⠭⣳⢭⡻⢽⣻⠽⠚⠱⣻⠆⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ ⠀⠈⠿⠁⠸⡀⠀⠀⠀⢠⣏⡏⡅⠀⣦⢄⡹⣎⠳⡦⢬⠡⠀⠀⠈⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀  ⠀⣿⣔⢬⡐⣼⢎⢇⣳⡨⣇⢫⠘⢵⠂⡙⣂⢻⣦⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣼⣿⣿⣯⡾⢇⡎⡏⢼⡅⣌⣰⣼⣧⡃⣷⣩⣷⣫⢃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ ⠀⠀⠀⠈⠻⠋⠁⠀⠈⡴⣤⣷⡊⣿⢾⣧⣿⣿⡽⣧⠛⡩⡏⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ ⠀⠀⠀⠀⠀⠀⠀⢸⠿⢛⣿⡇⣿⢼⣿⠞⠏⠐⣫⡜⣷⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⠈⢬⡂⠁⣌⠠⣤⣼⢰⣧⡿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ ⠀⠀⠀⠀⠀ ⠀⠀⠀⠀⠀⠀⠈⢯⣿⡽⣇⢿⣿⢸⡇⢧⡱⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡈⢷⠹⡜⣿⢌⠛⡰⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀ ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ ⠀⠀⠀⠀⠀⠀⠀⠀⠙⢦⡈⡄⡝⣸⣸⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⣿⢓⣩⣬⡻⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ ⠀⠀⠀  ⠀⠀⠀⠛⠋⠛⠁⠀⠀)";

  const raw_str global_score_label = R"(
         🧟☣️ 🧟 Zombie Dice Delux, v 0.1  🧟☣️ 🧟
 ┌────────────────────────────────────────────────────┐
 │                   GLOBAL SCORE                     │
 └────────────────────────────────────────────────────┘)";

public:
  /**
   * @brief Displays a welcome message for the Zombi Dice game.
   */
  void show_welcome() const
  {
    std::ostringstream msg;

    msg << "\033[32m";
    msg << "🧟        ---> Welcome to the Zombi Dice game (v 0.1) <---        🧟 \n";
    msg << "               -- copyright DIMAp/UFRN 2024-2025 --\n ";
    msg << "\033[0m\n";

    msg << "\033[32m" << logo << "\033[0m"
        << "\n";

    msg << "The object of the jeopardy dice game Zombie Dice is to be the\n "
        << "first to eat " << m_config.n_brains_to_win << " or more human brains in a turn.\n";
    msg << " Each player's turn consists of repeatedly rolling 3 dice.\n";
    msg << " A die may produce 3 outcomes: you ate a brain, the human escaped,\n"
        << " or you were shot!\n";
    msg << " If you get shot 3 or more times you die and loose all brains you\n"
        << " ate in that turn; then the turn goes to the next player.\n";
    msg << " However, if you decide to stop before get shot 3 times, you keep\n"
        << " the brains you ate.\n";

    if (m_config.n_max_turns > 0)
    {
      msg << " You are then in fixed turn mode, where the winner is the\n";
      msg << " one who gets the most brains in " << m_config.n_max_turns << " turns.\n\n";
    }
    if( m_config.isbot == "true")
    {
      msg << " Bots are active and will be added to the game\n in the following quantities: "
          << m_config.n_bots << "\n\n";
    }
   
    std::cout << msg.str();
  }
  /**
   * @brief Displays a message indicating that the game is over.
   */
  void show_lobby_message()
  {
    std::ostringstream msg;
    msg << " Before we begin, please enter the names of the players\n"
        << " in a single line, separated by comma,no repeat . For example:\n"
        << " >>> Jack , Menace, Ultraz, Boris\n\n";
    if(m_config.isbot == "true")
    {
      msg << " Minimum of 1 player, maximum of " << m_config.n_max_players << " names.\n"
          << " In addition, the bots are active\n"
          << " and will be added to the game in the following quantities: " << m_config.n_bots << "\n"
          << " with name bot-<id>.\n"
         << " Providing an empty line will end the game.\n";
    }
    else
    {
      msg << " Minimum of 2 player, maximum of "<< m_config.n_max_players<<" names.\n"
          << " Providing an empty line will end the game.\n";
    }
    std::cout << msg.str();
  }

  /**
   * @brief  Displays a list of players in the game.
   * 
   * @param m_players A vector of players in the game. 
   */
  void show_list_players(const players_t& m_players)
  {
    std::ostringstream oss; // <<< REMOVA o ``

    oss << "\n >>> The players in the game IN ORDER OF PLAY are:\n";
    for (const auto& player : m_players)
    {
      oss << "    > 🧟 " << player.m_name << '\n';
    }
    oss << '\n';

    std::cout << oss.str();
  }

  using scoreboard_t = omap<size_type, std::pair<size_type, size_type>>;
  /**
   * @brief Generates a string representation of the game score.
   * 
   * @param players A vector of players in the game.
   * @param scores A map containing the scores of each player.
   * @param current_player An optional pointer to the current player.
   * @return A string representation of the game score.
   */
  str game_score(const vec<Player>& players, const scoreboard_t& scores, Player* current_player = nullptr) const
  {
    auto max_turns{ m_config.n_max_turns };
    std::ostringstream msg;
    auto max_bra{ m_config.n_brains_to_win };
    msg << global_score_label << "\n";

    msg << u8" ┌────────────────────────────────────────────────────┐\n";
    msg << u8" │   Name" << std::right << std::setw(26) << "Brains-Captured" << std::setw(14) << "Turns  " << std::setw(10)
        << "│ \n";

    for (const auto& player : players)
    {
      str name = player.m_name;
      size_type id = player.m_id;
      size_type score = scores.at(id).first; // Obtém o score do jogador pelo ID
      size_type turn = scores.at(id).second; // Obtém o score do jogador pelo ID
      int n{ 0 };

      msg << u8" │" << ((current_player and current_player->m_id == id) ? "🔹 " : "   ");
      msg << name << std::right << std::setw(22 - name.size());
      msg << score << "/" << max_bra << "🧠" << std::setw(14);
      n = (max_bra < 10) ? 2 : 0;
      auto a{ n };
      n = (max_bra > 100) ? -2 : a;
      msg << turn << (max_turns > 0 ? "/" + std::to_string(max_turns) : "  ")
          << std::right << std::setw(((max_turns >= 10)?9:10) - n) << "│\n";

    }
    msg << u8" └────────────────────────────────────────────────────┘\n";

    return msg.str();
  }

  void show_winner(const str& name, const size_type& score) const
  {
    std::ostringstream oss;
    oss << " 🏆 Congratulations, " << name << "! You won the game! 🏆\n";
    oss << "    You captured " << score << " brains in total.\n";
    oss << "    Thank you for playing Zombi Dice Delux!\n";

    std::cout << oss.str();
  }

  void print_die_area(const die_area_t& die_area, const str& symbol, const size_type& extra_size = 0) const
  {
    std::ostringstream oss{};

    oss << ' ' << symbol << ": ";
    for (const auto& dice : die_area)
    {
      oss << color_symbol(dice.m_color) << ' ';
    }
    oss << '(' << die_area.size() << ')';

    if (extra_size != 0)
    {
      oss << "+(" << extra_size << " )";
    }

    oss << '\n';

    std::cout << oss.str();
  }
  /**
   * @brief Prints the scroll tray with the current state of the dice.
   * 
   * @param m_scroll_tray An array of pairs representing the dice in the scroll tray.
   */
  void print_scroll_tray(const std::array<std::pair<Face, Dice>, 3>& m_scroll_tray) const
  {
    std::ostringstream oss;
    //<! Extrai as cores e faces dos dados do tuple
    std::pair<Face, Dice> bx = m_scroll_tray[0];
    std::pair<Face, Dice> fy = m_scroll_tray[1];
    std::pair<Face, Dice> sz = m_scroll_tray[2];

    //<! Converte as cores e faces para strings
    // Verifica se os dados são vazios e ajusta a saída
    auto xf = (bx.first == Face::NONE) ? "  " : face_symbol(bx.first);
    auto yf = (fy.first == Face::NONE) ? "  " : face_symbol(fy.first);
    auto zf = (sz.first == Face::NONE) ? "  " : face_symbol(sz.first);

    auto xc = (bx.second.m_color == Color::NONE) ? "  " : "(" + color_symbol(bx.second.m_color) + ")";
    auto yc = (fy.second.m_color == Color::NONE) ? "  " : "(" + color_symbol(fy.second.m_color) + ")";
    auto zc = (sz.second.m_color == Color::NONE) ? "  " : "(" + color_symbol(sz.second.m_color) + ")";

    //<! Formata a mensagem com os dados
    oss << " ┌────────────────────────────────────────────────────┐\n";
    oss << " │                      SCROLL TRAY                   │\n";
    oss << " ├───────────────┬─────────────────────┬──────────────┤\n";
    oss << " │" << std::setw(8) << xf << xc << std::setw(8) << "│" << std::setw(10) << yf << yc << std::setw(12) << "│"
        << std::setw(8) << zf << zc << std::setw(8) << "│\n";
    oss << " └───────────────┴─────────────────────┴──────────────┘\n";
    std::cout << oss.str();
  }

  /**
   * @brief Displays the rolled status of the dice in the scroll tray.
   * 
   * @param m_scroll_tray An array of pairs representing the dice in the scroll tray.
   */
  void show_rolled_status(const std::array<std::pair<Face, Dice>, 3>& m_scroll_tray)
  {
    std::ostringstream msg;
    size_type cb{ 0 };
    size_type cf{ 0 };
    size_type cs{ 0 };

    for (const auto& tray_slot : m_scroll_tray)
    {
      if (tray_slot.first == Face::BRAIN)
      {
        cb++;
      }
      else if (tray_slot.first == Face::SHOTGUN)
      {
        cs++;
      }
      else
      {
        cf++;
      }
    }
    msg << u8" ┌──[ 📣 STATUS AREA   ]───────────────────────────────────────────┐\n";
    msg << u8" │  After taking the data out of the bag, he got:                  │\n";
    if (cb >= 0 or cf >= 0 or cs >= 0)
    {
      if (cb == 3)
      {
        msg << u8" │   The night was excellent, he managed to capture 3 humans (🧠), │\n";
        msg << u8" │   without any 0 escapes(👣) or 0 combat (💥).                   │\n";
      }
      if (cf == 3)
      {
        msg << u8" │   The night was bad, he managed to escape 3 humans (👣),        │\n";
        msg << u8" │   without any 0 captures(🧠) or 0 combat(💥).                   │\n";
      }
      if (cs == 3)
      {
        msg << u8" │   On  3 (💥) humans hit you, so you died that turn.             │\n";
      }
      if (cb == 2 and cf == 1)
      {
        msg << u8" │   The night was good, he managed to capture 2 humans (🧠)       │\n";
        msg << u8" │   and 1 human escaped(👣).                                      │\n";
      }
      if (cb == 1 and cf == 2)
      {
        msg << u8" │   The night was bad, he managed to capture 1 human (🧠)         │\n";
        msg << u8" │   and 2 humans escaped (👣).                                    │\n";
      }
      if (cb == 2 and cs == 1)
      {
        msg << u8" │   The night was good, he managed to capture 2 humans (🧠)       │\n";
        msg << u8" │   and 1 combat(💥).                                             │\n";
      }
      if (cb == 1 and cs == 2)
      {
        msg << u8" │   The night was bad, he managed to capture 1 human (🧠)         │\n";
        msg << u8" │   and 2 combats (💥).                                           │\n";
      }
      if (cf == 2 and cs == 1)
      {
        msg << u8" │   The night was horrible, he managed to escape 2 humans (👣)    │\n";
        msg << u8" │   and 1 combat(💥).                                             │\n";
      }
      if (cf == 1 and cs == 2)
      {
        msg << u8" │   The night was terrible, he managed to escape 1 human (👣)     │\n";
        msg << u8" │   and 2 combats (💥).                                           │\n";
      }
      if (cb == 1 and cf == 1 and cs == 1)
      {
        msg << u8" │   The night was long, he managed to capture 1 human (🧠),       │\n";
        msg << u8" │   escape 1 human(👣) and 1 combat(💥).                          │\n";
      }
    }
    msg << u8" └─────────────────────────────────────────────────────────────────┘\n";

    std::cout << msg.str();
  }
  
  /**
   * @brief Displays a message in the message area.
   * 
   * @param msg The message to display. If empty, a default message is shown.
   */
  void show_message(const str& msg = "")
  {
    std::ostringstream oss{};
    vec<str> lines;
    str line;
    size_type max_line_length{ 0 };
    std::istringstream iss;

    if (msg.empty())
    {
      iss.str(
        "Ready to play?\nPress R + <enter> - roll dices\nPress H + <enter> - hold turn\nPress Q + <enter> - quit game");
    }
    else
    {
      iss.str(msg);
    }

    while (getline(iss, line))
    {
      lines.push_back(line);
      max_line_length = std::max(max_line_length, line.size());
    }

    size_type content_width{ std::max(max_line_length, size_type(49)) };

    oss << " ┌─[ 🚨 MESSAGE AREA ]";
    for (size_type i{ 18 }; i < content_width; ++i)
    {
      oss << "─";
    }
    oss << "┐\n";

    // Conteúdo
    for (const auto& l : lines)
    {
      oss << " │ " << std::left << std::setw(content_width) << std::setfill(' ') << l << " │\n";
    }

    oss << " └";
    for (size_type i{ 0 }; i < content_width + 2; ++i) oss << "─";
    oss << "┘\n";

    std::cout << oss.str();
  }
};
#endif // ZDICE_INTERFACE_HPP

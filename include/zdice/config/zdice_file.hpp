#ifndef ZDICE_FILE_HPP
#define ZDICE_FILE_HPP

#include "dotini/dotini.hpp"

#include <exception>
#include <iostream>
#include <string>

class ZDiceFile
{
public:
  std::size_t n_weak_dice{ 6 };
  std::size_t n_strong_dice{ 4 };
  std::size_t n_tough_dice{ 3 };
  std::size_t n_brains_to_win{ 13 };
  std::size_t n_max_players{ 6 };
  std::size_t n_max_turns{ 0 };
  std::string weak_die_faces{ "bbbffs" };
  std::string strong_die_faces{ "bbffss" };
  std::string tough_die_faces{ "bffsss" };
  std::string isbot{ "true" };
  std::size_t n_bots{ 2 };

  ZDiceFile() = default;

  ZDiceFile(const std::string& filename) { load(filename); }

  void load(const std::string& filename)
  {
    try
    {
      Dotini source(filename);

      n_weak_dice = source.get<std::size_t>("Game", "weak_dice", n_weak_dice);
      n_strong_dice = source.get<std::size_t>("Game", "strong_dice", n_strong_dice);
      n_tough_dice = source.get<std::size_t>("Game", "tough_dice", n_tough_dice);
      n_max_players = source.get<std::size_t>("Game", "max_players", n_max_players);
      n_brains_to_win = source.get<std::size_t>("Game", "brains_to_win", n_brains_to_win);
      n_max_turns = source.get<std::size_t>("Game", "max_turns", n_max_turns);
      n_bots = source.get<std::size_t>("Game", "n_bots", n_bots);
      isbot = source.get<std::string>("Game", "isbot", isbot);

      weak_die_faces = source.get<std::string>("Dice", "weak_die_faces", weak_die_faces);
      strong_die_faces = source.get<std::string>("Dice", "strong_die_faces", strong_die_faces);
      tough_die_faces = source.get<std::string>("Dice", "tough_die_faces", tough_die_faces);
    }
    catch (const std::exception&)
    {
      std::cerr << "[!] File '" << filename << "' not found or invalid.\n";
      std::cout << "[!] Using default values.\n";
    }
  }

  void print() const
  {
    std::cout << "=== Config ===\n";
    std::cout << "Weak dice:   " << n_weak_dice << " faces: " << weak_die_faces << '\n';
    std::cout << "Strong dice: " << n_strong_dice << " faces: " << strong_die_faces << '\n';
    std::cout << "Tough dice:  " << n_tough_dice << " faces: " << tough_die_faces << '\n';
    std::cout << "Brains to win: " << n_brains_to_win << '\n';
    std::cout << "Max players:   " << n_max_players << '\n';
    std::cout << "Max turns:     " << n_max_turns << " (0 = unlimited)\n";
    std::cout << "Bot game: " << isbot << ", max bots: " << n_bots;
  }
};

#endif //!< ZDICE_FILE_HPP
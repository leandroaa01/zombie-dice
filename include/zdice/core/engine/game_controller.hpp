/**
 * @file game_controller.hpp
 * @author José Carlos da Paz Silva (carlos.paz.707@ufrn.edu.br)
 * @author Leandro Andrade (leandro.andrade.401@ufrn.edu.br)
 * @brief  class responsible for controlling the game flow.
 * @version 0.1
 * @date 2025-06-24
 *
 * @copyright Copyright (c) 2025
 *
 */
#ifndef GAME_CONTROLLER_HPP
#define GAME_CONTROLLER_HPP

#include "common/alias.hpp"
#include "zdice/core/dice/dice.hpp"
#include "zdice/core/dice/face.hpp"
#include "zdice/core/engine/game_state.hpp"
#include "zdice/core/engine/zdice_interface.hpp"
#include "zdice/core/player/player.hpp"
#include "zdice/core/player/player_action.hpp"
#include <cstdio>
#include <cstdlib>
#include <set>

/// @brief enum class for the game flags.
enum class Flag : byte
{
  NONE,
  PLAYER_NAMES_EMPTY,
  PLAYER_NAMES_OUT_0F_BOUNDS,
  PLAYER_NAMES_DUPLICATES,
  PLAYER_NAMES_OK,
  A_PLAYER_LEFT_THE_GAME,
  PLAYER_ROLLED_THE_DICE
};

using Tuple = tuple<u_int, u_int, u_int>;

/**
 * @brief count the number of faces in a string.
 *
 * @param faces  String containing the faces to be counted.
 * @return Tuple  A tuple containing the counts of 'b', 's', and 'f' faces.
 */
Tuple face_count(const str& faces)
{
  u_int b{ 0 };
  u_int s{ 0 };
  u_int f{ 0 };
  for (const char& value : faces)
  {
    if (value == 'b')
    {
      b++;
    }
    else if (value == 's')
    {
      s++;
    }
    else if (value == 'f')
    {
      f++;
    }
  }
  return { b, s, f };
}

flag has_duplicates(const vec<str>& names)
{

  std::set<str> unique_names; // Conjunto para armazenar nomes únicos

  for (const auto& name : names)
  {

    // Tenta inserir o nome no conjunto

    auto result = unique_names.insert(name);

    if (!result.second)
    {

      // Se a inserção falhar, significa que o nome já existe

      return true; // Duplicado encontrado
    }
  }

  return false; // Nenhum duplicado encontrado
}

/**
 * @brief Divide uma string usando múltiplos delimitadores.
 * @param text String de entrada
 * @param delimiters Conjunto de delimitadores
 * @return vec<str> Vetor com os pedaços da string
 */
vec<str> split(const str& text, const str& delimiters)
{
  vec<str> result;             // Vetor de resultados
  size_t start{ 0 }, end{ 0 }; // Índices para marcação do início e fim dos pedaços

  // Percorre a string até o final
  while (end < text.size())
  {
    // Verifica se o caractere atual é um delimitador
    if (delimiters.find(text[end]) != str::npos)
    {
      if (start != end) result.push_back(text.substr(start, end - start)); // Adiciona pedaço
      start = end + 1;                                                     // Atualiza o início
    }
    end++; // Avança para o próximo caractere
  }

  // Adiciona o último pedaço (se houver)
  if (start < end) result.push_back(text.substr(start, end - start));

  return result;
}



class GameController
{
public:
  using scoreboard_t = omap<size_type, std::pair<size_type, size_type>>;
  using tray_slot_t = std::pair<Face, Dice>;
  using scroll_tray_t = std::array<tray_slot_t, 3>;

private:
  const ZDiceFile& m_config;  //!< Configuration file for the game.
  ZDiceInterface m_interface; //!< Interface for displaying messages.
  DiceBag m_bag;              //!< The dice bag used in the game.

  players_t m_players{};       //!< Vector of players in the game.
  scoreboard_t m_scoreboard{}; //!< Scoreboard of the game.

  bool is_bots{ (m_config.isbot == "true") };
  std::size_t max_bots{ m_config.n_bots };

  GameState m_state_game{ GameState::UNDEFINED }; //!< Current game state.
  Flag m_current_flag{ Flag::NONE };
  PlayerAction m_current_action{ PlayerAction::NONE }; //!< Current player action.

  player_ptr m_player_of_turn{};      //!< Player of the turn.
  size_type m_player_of_turn_index{}; //!< Index of current player.
  size_type m_turns_played{ 0 };      //!< Turns counter.

  str m_error_msg{};   //!< Error message.
  str m_regular_msg{}; //!< Regular message.
  str m_alert_msg{};   //!< Alert message.

  die_area_t bsa{};                        //!< Brains storage area.
  die_area_t ssa{};                        //!< Shotguns storage area.
  die_area_t dra{};                        //!< Die rolling area.
  size_type m_accumulated_brains{ 0 };     //!< Number of accumulated brains at turn.
  size_type m_accumulated_shots{ 0 };      //!< Number of accumulated shots at turn.
  size_type m_accumulated_footprints{ 0 }; //!< Number of accumulated footprints at turn.
  scroll_tray_t m_scroll_tray{};           //!< Rolling table.
  action_map_t action_map{ { 'r', action_t::ROLL },
                           { 'h', action_t::HOLD },
                           { 'q', action_t::QUIT },
                           { 'n', action_t::CONFIRM_QUIT_NO },
                           { 'y', action_t::CONFIRM_QUIT_YES } };

  std::vector<size_type> m_tie_players;
  std::map<size_type, size_type> m_tie_scores;
  size_type m_tie_current_player_index;
  size_type m_winner_id{};
  flag in_tie = false;

  std::size_t max_brains{ m_config.n_brains_to_win }; //!< Maximum number of brains to win.


  char bot_zombie(int id)
{
  // [!] Verificar se alguém já ganhou
  flag ja_tem_vencedor{ false };
  auto valor_do_possivel_vencedor{ 0 };
  for (const auto& [id, possivel_ganhador] : m_scoreboard)
  {
    if (possivel_ganhador.first >= max_brains)
    {
      ja_tem_vencedor = true;
      valor_do_possivel_vencedor = possivel_ganhador.first;
      break;
    }
  }


  auto current_score{ m_scoreboard[id].first };

  if(ja_tem_vencedor and (current_score + bsa.size() + m_accumulated_brains) <= valor_do_possivel_vencedor)
  {
    return 'r'; // Se o bot não pode vencer, ele rola os dados
  }

  // [!] Verificar se o bot já ganhou
  if ((current_score + bsa.size() + m_accumulated_brains ) >= max_brains and (not ja_tem_vencedor)) return 'h';

  // [!] Se a área de rolagem estiver vazia, o bot deve rolar os dados

  if (not ssa.empty())
  {
    if ((current_score + bsa.size()+ m_accumulated_brains ) >=  max_brains) return 'h';
    // [!] Se a área de tiro tiver de 1 a 2 dados, o bot deve rolar
    if (ssa.size() <= 2 and ((current_score + bsa.size() + m_accumulated_brains) < max_brains)){
      //[!] se  a gente 50% ou mais do total de cerebros, a gente segura os dados
    if (ja_tem_vencedor)
    {
      if ((current_score + bsa.size() +  m_accumulated_brains ) >= valor_do_possivel_vencedor)
      {
        return 'h'; // Se o bot pode vencer, ele segura os dados
      }else{
        return 'r'; // Se o bot não pode vencer, ele rola os dados
      }
    }else if ((current_score + bsa.size()  + m_accumulated_brains ) >= (max_brains / 2)){
        return 'h'; // Se o bot tem 50% ou mais dos cérebros necessários, ele segura os dados
      }
  }}else{
     if((bsa.size() +  m_accumulated_brains) > max_brains and (not ja_tem_vencedor)) return 'h'; // Se o bot já tem muitos cérebros, ele segura os dados
  }

  // [!] Verificar se o bot já acumulou cérebros suficientes para vencer
  if ((current_score + bsa.size()) +  m_accumulated_brains  >= max_brains and (not ja_tem_vencedor)) return 'h';


  return 'r'; // Se não tiver dados de tiro, rola os dados
}

public:
  /// @brief Constructor for the GameController class.
  GameController(const ZDiceFile& file)
  : m_config(file), m_interface(file), m_player_of_turn(nullptr), m_player_of_turn_index(-1)
  {
    init_dice_bag();
  }
  /**
   * @brief  Runs the game loop.
   *
   */
  void run()
  {
    while (not game_over())
    {
      process();
      update();
      render();
    }
  }

private:
  //==== GameStates manipulation.
  /// @brief Manages player inputs.
  void process()
  {
    switch (m_state_game)
    {
      case GameState::UNDEFINED:
        // std::cout << "[UNDEFINED]\n";
        // Do nothing...
        break;

      case GameState::WELCOME:
        // std::cout << "[WELCOME]\n";
        process_welcome();
        break;

      case GameState::LOBBY:
        // std::cout << "[LOBBY]\n";
        process_lobby();
        break;

      case GameState::ERROR_IN_LOBBY:
        // std::cout << "[ERROR_IN_LOBBY]\n";
        // Do nothing...
        break;

      case GameState::EXIT_PROGRAM:
        // std::cout << "[EXIT_PROGRAM]\n";
        // Do nothing...
        break;

      case GameState::START_GAME:
        // std::cout << "[START_GAME]\n";
        process_start_game();
        break;

      case GameState::SELECTING_PLAYER:
        // std::cout << "[SELECTING_PLAYER]\n";
        // Do nothing...
        break;

      case GameState::PLAYING:
        // std::cout << "[PLAYING]\n";
        process_playing();
        break;

      case GameState::ERROR_IN_PLAYING:
        // std::cout << "[PLAYING]\n";
        // Do nothing...
        break;

      case GameState::HOLDING:
        // std::cout << "[HOLDING]\n";
        process_holding();
        break;

      case GameState::ROLLING:
        // std::cout << "[ROLLING]\n";
        // Do nothing...
        break;

      case GameState::RECYLING_DICES:
        // std::cout << "[RECYLING_DICES]\n";
        process_recycling_dice();
        break;

      case GameState::DIED:
        // std::cout << "[DIED]\n";
        process_died();
        break;

      case GameState::ALIVE:
        // std::cout << "[ALIVE]\n";
        process_alive();
        break;

      case GameState::QUITTING:
        // std::cout << "[QUITTING]\n";
        process_quitting();
        break;

      case GameState::ERROR_IN_QUITTING:
        // std::cout << "[ERROR_IN_QUITTING]\n";
        // Do nothing...
        break;

      case GameState::PLAYER_LEFT_GAME:
        // std::cout << "[PLAYER_LEFT_GAME]\n";
        // Do nothing...
        break;

      case GameState::ENDING_TURN:
        // std::cout << "[ENDING_TURN]\n";
        // Do nothing...
        break;

      case GameState::TIE_BREAKER_SETUP:
        // std::cout << "[TIE_BREAKER_SETUP]\n";
        process_tie_breaker_setup();
        break;

      case GameState::TIE_BREAKER_PLAYING:
        // std::cout << "[TIE_BREAKER_PLAYING]\n";
        process_playing();
        break;

      case GameState::TIE_BREAKER_END:
        // std::cout << "[TIE_BREAKER_END]\n";
        process_tie_breaker_setup();
        break;

      case GameState::GAME_END:
        // std::cout << "[GAME_END]\n";
        process_game_end();
        break;
    }
  }

  /// @brief Manages state updates.
  void update()
  {
    switch (m_state_game)
    {
      case GameState::UNDEFINED:
        // std::cout << "[UNDEFINED]\n";
        transition_to(GameState::WELCOME);
        break;

      case GameState::WELCOME:
        // std::cout << "[WELCOME]\n";
        transition_to(GameState::LOBBY);
        break;

      case GameState::LOBBY:
        // std::cout << "[LOBBY]\n";
        update_lobby();
        break;

      case GameState::ERROR_IN_LOBBY:
        // std::cout << "[ERROR_IN_LOBBY]\n";
        transition_to(GameState::LOBBY);
        break;

      case GameState::EXIT_PROGRAM:
        // std::cout << "[EXIT_PROGRAM]\n";
        exit(EXIT_SUCCESS);
        break;

      case GameState::START_GAME:
        // std::cout << "[START_GAME]\n";
        transition_to(GameState::SELECTING_PLAYER);
        break;

      case GameState::SELECTING_PLAYER:
        // std::cout << "[SELECTING_PLAYER]\n";
        update_selecting_player();
        break;

      case GameState::PLAYING:
        // std::cout << "[PLAYING]\n";
        update_playing();
        break;

      case GameState::ERROR_IN_PLAYING:
        // std::cout << "[ERROR_IN_PLAYING]\n";
        transition_to(GameState::PLAYING);
        break;

      case GameState::HOLDING:
        // std::cout << "[HOLDING]\n";
        update_holding();
        break;

      case GameState::ROLLING:
        // std::cout << "[ROLLING]\n";
        update_rolling();
        break;

      case GameState::RECYLING_DICES:
        // std::cout << "[RECYCLING DICES]\n";
        recycling_dices();
        break;

      case GameState::DIED:
        // std::cout << "[DEAD]\n";
        update_dead();
        break;

      case GameState::ALIVE:
        // std::cout << "[ALIVE]\n";
        update_alive();
        break;

      case GameState::QUITTING:
        // std::cout << "[QUITTING]\n";
        update_quitting();
        break;

      case GameState::ERROR_IN_QUITTING:
        // std::cout << "[ERROR_IN_QUITTING]\n";
        transition_to(GameState::QUITTING);
        break;

      case GameState::PLAYER_LEFT_GAME:
        // std::cout << "[PLAYER_LEFT_GAME]\n";
        update_player_left_game();
        break;

      case GameState::ENDING_TURN:
        // std::cout << "[ENDING_TURN]\n";
        update_ending_turn();
        break;

      case GameState::TIE_BREAKER_SETUP:
        // std::cout << "[TIE_BREAKER_SETUP]\n";
        update_tie_breaker_setup();
        break;

      case GameState::TIE_BREAKER_PLAYING:
        // std::cout << "[TIE_BREAKER_PLAYING]\n";
        update_playing();
        break;

      case GameState::TIE_BREAKER_END:
        // std::cout << "[TIE_BREAKER_END]\n";
        update_tie_breaker_end();
        break;

      case GameState::GAME_END:
        // std::cout << "[GAME_END]\n";
        exit(EXIT_SUCCESS);
        break;
    }
  }

  /// @brief Manages interface updates.
  void render()
  {
    switch (m_state_game)
    {
      case GameState::UNDEFINED:
        // std::cout << "[UNDEFINED]\n";
        // Do nothing...
        break;

      case GameState::WELCOME:
        // std::cout << "[WELCOME]\n";
        m_interface.show_welcome();
        break;

      case GameState::LOBBY:
        // std::cout << "[LOBBY]\n";
        m_interface.show_lobby_message();
        break;

      case GameState::ERROR_IN_LOBBY:
        // std::cout << "[ERROR_IN_LOBBY]\n";
        render_error_in_lobby();
        break;

      case GameState::EXIT_PROGRAM:
        // std::cout << "[EXIT_PROGRAM]\n";
        render_exit_program();
        break;

      case GameState::START_GAME:
        // std::cout << "[START_GAME]\n";
        m_interface.show_list_players(m_players);
        break;

      case GameState::SELECTING_PLAYER:
        // std::cout << "[SELECTING_PLAYER]\n";
        // Do nothing...
        break;

      case GameState::PLAYING:
        // std::cout << "[PLAYING]\n";
        render_playing();
        break;

      case GameState::ERROR_IN_PLAYING:
        // std::cout << "[ERROR_IN_PLAYING]\n";
        render_error_in_playing();
        break;

      case GameState::HOLDING:
        // std::cout << "[HOLDING]\n";
        render_holding();
        break;

      case GameState::ROLLING:
        // std::cout << "[ROLLING]\n";
        render_rolling();
        break;

      case GameState::RECYLING_DICES:
        // std::cout << "[RECYCLING DICES]\n";
        render_recycling_dices();
        break;

      case GameState::DIED:
        // std::cout << "[DIED]\n";
        render_died();
        break;

      case GameState::ALIVE:
        // std::cout << "[ALIVE]\n";
        render_alive();
        break;

      case GameState::QUITTING:
        // std::cout << "[QUITTING]\n";
        render_quitting();
        break;

      case GameState::ERROR_IN_QUITTING:
        // std::cout << "[ERROR_IN_QUITTING]\n";
        render_error_in_quitting();
        break;

      case GameState::PLAYER_LEFT_GAME:
        // std::cout << "[PLAYER_LEFT_GAME]\n";
        render_player_left_game();
        break;

      case GameState::ENDING_TURN:
        // std::cout << "[ENDING TURN]\n";
        // Do nothing...
        break;

      case GameState::TIE_BREAKER_SETUP:
        render_tie_breaker_setup();
        break;

      case GameState::TIE_BREAKER_PLAYING:
        render_playing();
        break;

      case GameState::TIE_BREAKER_END:
        render_tie_breaker_end();
        break;

      case GameState::GAME_END:
        // std::cout << "[GAME END]\n";
        render_game_end();
        break;
    }
  }
  //====

  //==== Raw GameStates manipulation.
  //==== Processors...
  void process_welcome()
  {
    // [!] Wainting for <enter>.
    std::cout << " Press <enter> to continue. ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }

  void process_lobby()
  {
    std::cout << " >>> ";
    str input{};
    std::getline(std::cin, input);

    vec<str> names{ split(input, ",") };
    for (auto& name : names) name = trim(name);
    if (has_duplicates(names))
    {
      m_error_msg = "Player names must be unique. Please, use different names.\n";
      transition_to(state::ERROR_IN_LOBBY);
      return;
    }
    if (not names.empty()) initialize_players(names);
  }

  void process_start_game()
  {
    // [!] Wainting for <enter>.
    std::cout << " Press <enter> to start the match. ";
    if (m_player_of_turn && not m_player_of_turn->is_bot())
    {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  }

  void process_playing()
  {
    char input{};
    if (m_player_of_turn->is_bot())
    {
       
        auto id{ m_player_of_turn->m_id };
        input = bot_zombie(id);

        // Processa automaticamente a ação do bot
        auto it{ action_map.find(input) };
        if (it != action_map.end())
        {
            m_current_action = it->second;
        }
        else
        {
            m_current_action = PlayerAction::UNKNOWN;
        }
    }
    else
    {
        // Jogador humano: aguarda entrada do usuário
        std::cin >> input;
        char ch = std::tolower(input);
        std::cin.ignore();

        auto it{ action_map.find(ch) };
        if (it != action_map.end())
        {
            m_current_action = it->second;
        }
        else
        {
            m_current_action = PlayerAction::UNKNOWN;
        }
    }
  }

  void process_holding()
  {
    // [!] Wainting for <enter>.
    if(not m_player_of_turn->is_bot())
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }

  void process_recycling_dice()
  {
    // [!] Wainting for <enter>.
   if(not m_player_of_turn->is_bot())
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }

  void process_died()
  {
    // [!] Wainting for <enter>.
    if(not m_player_of_turn->is_bot())
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }

  void process_alive()
  {
    // [!] Wainting for <enter>.
    if(not m_player_of_turn->is_bot())
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }

  void process_quitting()
  {
    char input = std::cin.get();

    if (input == '\n')
    {
      input = 'n';
    }
    else
    {
      input = std::tolower(input);
      std::cin.ignore();
    }

    auto it{ action_map.find(input) };
    if (it != action_map.end())
    {
      m_current_action = it->second;
    }
    else
    {
      m_current_action = PlayerAction::UNKNOWN;
    }
  }

  void process_tie_breaker_setup()
  {
    // [!] Wainting for <enter>.
   // if(not m_player_of_turn->is_bot())
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }

  void process_tie_breaker_end()
  {
    // [!] Wainting for <enter>.
   // if(not m_player_of_turn->is_bot())
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }

  void process_game_end()
  {
    // [!] Wainting for <enter>.
   // if(not m_player_of_turn->is_bot())
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }
  //====

  //==== Updaters
  void update_lobby()
  {
    if (m_players.empty())
    {
      transition_to(GameState::EXIT_PROGRAM);
      return;
    }

    if (is_bots and m_players.empty())
    {
      m_error_msg = "At least one human player is required to start the game.";
      transition_to(GameState::ERROR_IN_LOBBY);
      return;
    }
    if ((m_players.size() < 2 or m_players.size() > 6) and not is_bots)
    {
      m_error_msg = "At least two players a maximum of six are required to start the game.";
      transition_to(GameState::ERROR_IN_LOBBY);
      return;
    }

    transition_to(GameState::START_GAME);
  }

  void update_selecting_player()
  {
    // [!] Checks if the current player was the last to play in the turn.
    if (m_player_of_turn_index == m_players.size() - 1)
    {
      m_player_of_turn_index = -1;
      m_player_of_turn = nullptr;

      transition_to(GameState::ENDING_TURN);

      return;
    }

    m_player_of_turn_index = (m_player_of_turn_index + 1) % m_players.size();
    m_player_of_turn = &m_players.at(m_player_of_turn_index);

    transition_to(GameState::PLAYING);
  }

  void update_playing()
  {
    switch (m_current_action)
    {
      case PlayerAction::HOLD:
        transition_to(state::HOLDING);
        break;
      case PlayerAction::ROLL:
        transition_to(state::ROLLING);
        break;
      case PlayerAction::QUIT:
        transition_to(state::QUITTING);
        break;
      case PlayerAction::UNKNOWN:
        m_error_msg = "Action unknown. Please, try again.";
        transition_to(state::ERROR_IN_PLAYING);
        break;
      default:
        m_error_msg = "Action invalid in this context. Please, try again.";
        m_current_action = PlayerAction::INVALID;
        transition_to(GameState::ERROR_IN_PLAYING);
    }
  }

  void update_holding()
  {
    m_scoreboard[m_player_of_turn->m_id].first += bsa.size() + m_accumulated_brains;
    m_scoreboard[m_player_of_turn->m_id].second++;

    // [!] Reset all statistics.
    m_current_action = PlayerAction::NONE;
    m_accumulated_brains = 0;
    m_accumulated_shots = 0;
    m_accumulated_footprints = 0;

    // [!] Reset dice areas.
    reset_scroll_tray();
    reset_dice_bag();

    if (in_tie)
    {
      // Avançar para próximo jogador do desempate
      m_tie_current_player_index = (m_tie_current_player_index + 1) % m_tie_players.size();

      if (m_tie_current_player_index == 0)
      {
        // Todos jogaram uma rodada
        transition_to(GameState::TIE_BREAKER_END);
      }
      else
      {
        // Próximo jogador
        size_type next_id = m_tie_players[m_tie_current_player_index];
        m_player_of_turn_index = find_player_index(next_id);
        m_player_of_turn = &m_players[m_player_of_turn_index];
        transition_to(GameState::TIE_BREAKER_PLAYING);
      }
    }
    else
    {
      transition_to(GameState::SELECTING_PLAYER);
    }
  }

  void update_rolling()
  {
    // [!] Pega novos dados na sacola somente se não houver dados suficientes na mão.
    if (dra.size() != 3)
    {
      // [!] Checa se a sacola tem dados suficientes.
      if (not m_bag.has_sufficient(3 - dra.size()))
      {
        transition_to(GameState::RECYLING_DICES);
        return;
      }
      m_bag.pick(3 - dra.size(), dra);
    }

    size_type slot_idx{ 0 };
    size_type count_shots{ 0 };
    for (auto it{ dra.begin() }; it != dra.end();)
    {
      Face rolled_face{ it->roll() };

      if (rolled_face == Face::BRAIN)
      {
        m_scroll_tray[slot_idx++] = (std::make_pair(Face::BRAIN, *it));
        it = dra.erase(it);
      }
      else if (rolled_face == Face::SHOTGUN)
      {
        m_scroll_tray[slot_idx++] = (std::make_pair(Face::SHOTGUN, *it));
        it = dra.erase(it);
        ++count_shots;
      }
      else
      {
        m_scroll_tray[slot_idx++] = (std::make_pair(Face::FOOTPRINT, *it));
        ++it;
      }
    }

    if (ssa.size() + count_shots >= 3)
    {
      transition_to(GameState::DIED);
      return;
    }
    transition_to(GameState::ALIVE);
  }

  void recycling_dices()
  {
    m_accumulated_brains += bsa.size();
    m_bag.place(bsa);
    transition_to(GameState::ROLLING);
  }

  void update_alive()
  {
    for (size_type i{ 0 }; i < m_scroll_tray.size(); ++i)
    {
      if (m_scroll_tray[i].first == Face::BRAIN)
      {
        bsa.push_back(m_scroll_tray[i].second);
        m_scroll_tray[i] = { Face(), Dice() };
      }
      else if (m_scroll_tray[i].first == Face::SHOTGUN)
      {
        ssa.push_back(m_scroll_tray[i].second);
        m_scroll_tray[i] = { Face(), Dice() };
      }
      else
      {
        // dra.push_back(m_scroll_tray[i].second);
        //  m_scroll_tray[i] = { Face(), Dice() };
      }
    }
    transition_to(GameState::PLAYING);
  }

  void update_dead()
  {
    m_scoreboard[m_player_of_turn->m_id].second += 1;
    m_accumulated_brains = 0;
    m_accumulated_footprints = 0;
    m_accumulated_shots = 0;

    reset_scroll_tray();
    reset_dice_bag();

    if (in_tie)
    {
      m_tie_current_player_index = (m_tie_current_player_index + 1) % m_tie_players.size();

      if (m_tie_current_player_index == 0)
      {
        transition_to(GameState::TIE_BREAKER_END);
      }
      else
      {
        size_type next_id = m_tie_players[m_tie_current_player_index];
        m_player_of_turn_index = find_player_index(next_id);
        m_player_of_turn = &m_players[m_player_of_turn_index];
        transition_to(GameState::TIE_BREAKER_PLAYING);
      }
    }
    else
    {
      transition_to(GameState::SELECTING_PLAYER);
    }
  }
  /// @brief Update the state of quitting the game.
  void update_quitting()
  {
    switch (m_current_action)
    {
      case PlayerAction::CONFIRM_QUIT_NO:
        transition_to(GameState::PLAYING);
        break;
      case PlayerAction::CONFIRM_QUIT_YES:
        m_alert_msg = "(!) \"" + m_player_of_turn->m_name + "\" left the game.";
        transition_to(GameState::PLAYER_LEFT_GAME);
        break;
      case PlayerAction::UNKNOWN:
        m_error_msg = "Action unknown. Please, try again.";
        transition_to(GameState::ERROR_IN_QUITTING);
        break;
      default:
        m_error_msg = "Action invalid in this context. Please, try again.";
        m_current_action = PlayerAction::INVALID;
        transition_to(GameState::ERROR_IN_QUITTING);
    }
  }

  void update_player_left_game()
  {
    m_scoreboard.erase(m_player_of_turn->m_id);
    m_players.erase(m_players.begin() + m_player_of_turn_index);

    reset_scroll_tray();
    reset_dice_bag();

    if (m_players.size() == 1)
    {
      m_winner_id = m_players.at(0).m_id;
      transition_to(GameState::GAME_END);
      return;
    }

    if (m_player_of_turn_index >= m_players.size()) m_player_of_turn_index = 0;
    m_player_of_turn = &m_players.at(m_player_of_turn_index);

    transition_to(state::PLAYING);
  }

  void update_ending_turn()
  {
    auto [top_players, highest_score] = get_potential_winners();

    // [!] Fluxo de game regular.
    if (m_config.n_max_turns == 0)
    {
      if (highest_score >= m_config.n_brains_to_win)
      {
        if (top_players.size() > 1)
        {

          in_tie = true;
          transition_to(GameState::TIE_BREAKER_SETUP);
        }
        else
        {
          m_winner_id = top_players.at(0);
          transition_to(GameState::GAME_END);
        }
      }
      else
      {
        transition_to(GameState::SELECTING_PLAYER);
      }
    }

    // [!] Fluxo de game por turnos.
    if (m_config.n_max_turns > 0)
    {
      ++m_turns_played;

      if (m_turns_played >= m_config.n_max_turns)
      {
        if (top_players.size() > 1)
        {
          in_tie = true;
          transition_to(GameState::TIE_BREAKER_SETUP);
        }
        else
        {
          m_winner_id = top_players.at(0);
          transition_to(GameState::GAME_END);
        }
      }
      else
      {
        transition_to(GameState::SELECTING_PLAYER);
      }
    }
  }

  void update_tie_breaker_setup()
  {
    auto [top_players, highest_score] = get_potential_winners();
    m_tie_players = top_players;

    m_tie_current_player_index = 0;
    m_player_of_turn_index = find_player_index(m_tie_players[0]);
    m_player_of_turn = &m_players[m_player_of_turn_index];

    m_accumulated_brains = 0;
    m_accumulated_shots = 0;
    m_accumulated_footprints = 0;
    reset_scroll_tray();
    reset_dice_bag();

    transition_to(GameState::TIE_BREAKER_PLAYING);
  }

  void update_tie_breaker_end()
  {
    size_type winner_id = 0;
    size_type max_score = 0;

    for (const auto& [id, info] : m_scoreboard)
    {
      if (info.first > max_score)
      {
        max_score = info.first;
        winner_id = id;
      }
    }

    size_type count_winners = 0;
    for (const auto& [id, info] : m_scoreboard)
    {
      if (info.first == max_score)
      {
        count_winners++;
      }
    }

    if (count_winners == 1)
    {
      m_winner_id = winner_id;
      transition_to(GameState::GAME_END);
    }
    else
    {
      std::vector<size_type> new_tie_players;
      for (const auto& [id, info] : m_scoreboard)
      {
        if (info.first == max_score)
        {
          new_tie_players.push_back(id);
        }
      }

      m_tie_players = new_tie_players;
      for (auto& id : m_tie_players)
      {
        m_tie_scores[id] = 0;
      }

      transition_to(GameState::TIE_BREAKER_SETUP);
    }
  }
  //====

  //==== Renderers...
  void render_error_in_lobby()
  {
    std::cout << '\n';
    m_interface.show_message(m_error_msg);
    m_error_msg.clear();
  }

  void render_exit_program()
  {
    std::cout << '\n';
    m_interface.show_message("You left before the game started... What did I do wrong? :(");
  }

  void render_playing()
  {
    //== Render global score.
    std::cout << m_interface.game_score(m_players, m_scoreboard, m_player_of_turn) << '\n';

    //== Render player info.
    std::cout << " Player: \"" << m_player_of_turn->m_name << "\" 🧟";
    std::cout << " │ Turn #: " << m_scoreboard.at(m_player_of_turn->m_id).second + 1 << " 🔄";
    std::cout << " │ Bag Has: " << /* m_dice_bag.size() */ m_bag.size() << " 🎲\n\n";

    //== Render dice bag.
    // print(" Bag = [" << m_msgs.print_dice_bag(m_bag.get_dices()) << "]\n");

    //== Render Scroll Tray.
    m_interface.print_scroll_tray(m_scroll_tray);

    std::cout << '\n';

    //== Render Brains Storage Area and Shotguns Storage Area.
    m_interface.print_die_area(bsa, "🧠", m_accumulated_brains);
    m_interface.print_die_area(ssa, "💥");

    std::cout << '\n';

    //== Render rolled status area.
    if (m_current_flag == Flag::PLAYER_ROLLED_THE_DICE)
    {
        m_interface.show_rolled_status(m_scroll_tray);
        m_current_flag = Flag::NONE;
    }

    //== Render message area.
    if (not m_regular_msg.empty())
    {
        m_interface.show_message(m_regular_msg);
        m_regular_msg.clear();
    }
    else if (!m_player_of_turn->is_bot())
    {
        m_interface.show_message();
    }

    //== Render prompt area.
    if (!m_player_of_turn->is_bot())
    {
        std::cout << " <🧟> ";
    }
  }

  void render_error_in_playing()
  {
    std::cout << '\n';
    m_interface.show_message(m_error_msg);
    m_error_msg.clear();
  }

  void render_holding()
  {
    m_regular_msg = "Holding turn...\n";
    m_regular_msg += "  Adding brains to your score.\n\n";
    m_regular_msg += "Press <enter> to continue.\n";
    render_playing();
  }

  void render_rolling()
  {
    m_regular_msg = "Rolling dices...";
    render_playing();
  }

  void render_recycling_dices()
  {
    m_regular_msg = "Oops, bag without dices.\nSaving your brains and\n  moving the dices to the bag...\nPress "
                    "<enter> to continue.\n";
    std::cout << '\n';
    m_interface.show_message(m_regular_msg);
    m_regular_msg.clear();
  }

  void render_alive()
  {
    m_regular_msg = "You're still \"alive\"...\nPress <enter> to continue.";
    m_current_flag = Flag::PLAYER_ROLLED_THE_DICE;
    render_playing();
  }

  void render_died()
  {
    m_regular_msg = "Oh no, now you are \"100% DIED\"!!\nPress <enter> to continue.";
    m_current_flag = Flag::PLAYER_ROLLED_THE_DICE;
    render_playing();
  }

  void render_quitting()
  {
    m_regular_msg = "Are you sure you want to leave the game? :(\n\n(y/N)";
    render_playing();
  }

  void render_error_in_quitting()
  {
    std::cout << '\n';
    m_interface.show_message(m_error_msg);
    m_error_msg.clear();
  }

  void render_player_left_game()
  {
    std::cout << '\n';
    m_interface.show_message(m_alert_msg);
    m_alert_msg.clear();
  }

  void render_tie_breaker_setup()
  {
    auto [top_players, _] = get_potential_winners();

    std::ostringstream oss;
    oss << "Oops, we have a tie, sudden death mode activated...\n";
    oss << "Sudden death for the players:\n";
    for (const auto& id : top_players) oss << "\"" << m_players[id - 1].m_name << "\" ";
    oss << "\n\nPress <enter> to start the tie-breaker round...";
    m_regular_msg = oss.str();

    std::cout << m_interface.game_score(m_players, m_scoreboard, m_player_of_turn);
    m_interface.show_message(m_regular_msg);
    std::cout << "\n <🧟> ";
    m_regular_msg.clear();
  }

  void render_tie_breaker_end()
  {
    m_regular_msg = "Tie breaker end.\nPress <enter> to continue.";

    std::cout << m_interface.game_score(m_players, m_scoreboard, m_player_of_turn);
    m_interface.show_message(m_regular_msg);
    std::cout << "\n <🧟> ";
    m_regular_msg.clear();
  }

  void render_game_end()
  {
    if (m_winner_id != 0)
    {
      std::cout << " \n    🏆 I have a champion...\n";
      auto idx = find_player_index(m_winner_id);
      //mostra o placa final de dar os parabéns ao vencedor
      std::cout << m_interface.game_score(m_players, m_scoreboard, m_player_of_turn) << "\n";
      m_interface.show_winner(m_players[idx].m_name, m_scoreboard[m_winner_id].first);
    }
    else
    {
      // Fallback - não deve acontecer
      std::cout << "GAME ENDED WITHOUT A CLEAR WINNER\n";
    }
  }
  //====

  //==== Help methods
  /// @brief Check if game has ended.
  flag game_over() const { return m_state_game == state::GAME_END; }

  /// @brief Changes the current state of the game.
  void transition_to(GameState new_state) { m_state_game = new_state; }

  void create_bots(size_type n_bots)
  {
    for (size_type i{ 0 }; i < n_bots; ++i)
    {
      auto n{ "Bot-" + std::to_string(i + 1) };
      Player p(m_players.size() + 1, n, true);
      m_players.emplace_back(std::move(p));
    }
  }

  /// @brief Initialize players of the game.
  void initialize_players(vec<str>& names)
  {
    m_players.clear();
    m_players.reserve(names.size());

    size_type ids{ 1 };
    for (auto& name : names)
    {
      // [!] Garante que não vai ter espaço em branco extra no nome dos jogadores.
      name = trim(name);

      // [!] Constrói um jogo jogador.
      Player p(ids++, name);

      // [!] Adiciona o jogador à lista de jogadores.
      m_players.push_back(std::move(p));
    }
    // [!] Cria os bots se necessário.
    if (is_bots)
    {
      create_bots(max_bots);
    }

    for (const auto& player : m_players) m_scoreboard[player.m_id] = std::make_pair(0, 0);

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::shuffle(m_players.begin(), m_players.end(), gen);
  }

  /// @brief Instancies a dice bag with dices.
  void init_dice_bag()
  {
    /**
     *  cria o saco de dados com os dados fracos, fortes e resistentes.
     * 1 - Cria os dados fracos:
     * 2 - Cria os dados resistentes:
     * 3 - Cria os dados fortes:
     *
     */
    auto insert_dices = [this](Color color, size_type number, const auto& faces)
    {
      size_type x{ std::get<0>(faces) };
      size_type y{ std::get<1>(faces) };
      size_type z{ std::get<2>(faces) };

      for (size_type i{ 0 }; i < number; ++i) m_bag.push(Dice(color, x, y, z));
    };

    insert_dices(Color::GREEN, m_config.n_weak_dice, face_count(m_config.weak_die_faces));
    insert_dices(Color::YELLOW, m_config.n_strong_dice, face_count(m_config.strong_die_faces));
    insert_dices(Color::RED, m_config.n_tough_dice, face_count(m_config.tough_die_faces));
  }

  /// @brief Return all the dices to the bag.
  void reset_dice_bag()
  {
    m_bag.place(bsa);
    m_bag.place(ssa);
    m_bag.place(dra);
  }

  /// @brief Resets the scroll tray.
  /// @details This method clears the scroll tray and places the dice back into their respective areas.
  void reset_scroll_tray()
  {
    for (auto& slot : m_scroll_tray)
    {
      if (slot.first == Face::BRAIN)
      {
        bsa.push_back(slot.second);
      }
      else if (slot.first == Face::SHOTGUN)
      {
        ssa.push_back(slot.second);
      }
      else if (slot.first == Face::FOOTPRINT)
      {
        // dra.push_back(slot.second);
      }
      slot = { Face(), Dice() };
    }
  }

  /// @brief Gets the potential winners of the game.
  std::pair<std::vector<size_type>, size_type> get_potential_winners() const
  {
    std::vector<size_type> winners;
    size_type highest_score = 0;

    for (const auto& [player_id, score_info] : m_scoreboard)
    {
      if (score_info.first > highest_score)
      {
        highest_score = score_info.first;
        winners.clear();
        winners.push_back(player_id);
      }
      else if (score_info.first == highest_score)
      {
        winners.push_back(player_id);
      }
    }

    return { winners, highest_score };
  }

  /// @brief Find a player index in `m_players` given an id.
  size_type find_player_index(size_type player_id)
  {
    for (size_type i = 0; i < m_players.size(); ++i)
    {
      if (m_players[i].m_id == player_id)
      {
        return i;
      }
    }
    return -1;
  }
  //====
};

#endif //<! GAME_CONTROLLER_HPP
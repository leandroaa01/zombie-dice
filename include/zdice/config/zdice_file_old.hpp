// #ifndef ZDICE_FILE_HPP
// #define ZDICE_FILE_HPP

// //========== ZDice includes ==========//
// #include "common/alias.hpp"
// #include "option.hpp"

// //========== STL includes ==========//
// #include <filesystem>
// #include <string_view>

// namespace fs = std::filesystem;

// //========== ZDiceFile definition ==========//
// class ZDiceFile
// {
// public:
//   //!< Alias for an ordered map that stores configuration options and their
//   //!< string values.
//   using opt_map_t = omap<Option, str>;
//   //!< Alias for an unordered map that associates option names as strings to
//   //!< their corresponding Option enum keys.
//   using key_map_t = umap<str, Option>;
//   //!< Alias for an unordered map that associates option options values to their
//   //!< corresponding string name.
//   using name_map_t = umap<Option, str>;

//   /// @brief Default constructor that loads the config from the default path.
//   ZDiceFile();

//   /**
//    * @brief Loads an INI file from the given path and updates configuration
//    * data.
//    * @param path The path to the INI file to load.
//    */
//   void load(const fs::path& path);

//   /**
//    * @brief Retrieves an integer configuration value for the specified option.
//    *
//    * @param op The option to retrieve (must be an integer-type option).
//    * @return int The parsed integer value.
//    * @throws std::runtime_error If:
//    *         - The option doesn't exist in the configuration.
//    *         - The value cannot be converted to an integer.
//    *         - The option is not of integer type.
//    */
//   int get_int(const Option op) const;

//   /**
//    * @brief Retrieves a string configuration value for the specified option.
//    *
//    * @param op The option to retrieve (must be a string-type option).
//    * @return str_v The string value.
//    * @throws std::runtime_error If the option doesn't exist in the
//    * configuration.
//    */
//   str_v get_string(const Option op) const;

//   /// @brief Returns a string representation of the configuration.
//   [[nodiscard]] str to_string() const;

// private:
//   //!< Path of the ZDice configuration file.
//   fs::path m_zdice_file_path{};
//   //!< Map with configuration data.
//   opt_map_t m_data{ { Option::WEAK_DICE, "6" },
//                     { Option::STRONG_DICE, "4" },
//                     { Option::TOUGH_DICE, "3" },
//                     { Option::MAX_PLAYERS, "6" },
//                     { Option::BRAINS_TO_WIN, "13" },
//                     { Option::MAX_TURNS, "0" },
//                     { Option::WEAK_DIE_FACES, "bbbffs" },
//                     { Option::STRONG_DIE_FACES, "bbffss" },
//                     { Option::TOUGH_DIE_FACES, "bffsss" } };
//   //!< Maps string keys to Option enum values.
//   static const inline key_map_t m_options{
//     { "weak_dice", Option::WEAK_DICE },
//     { "strong_dice", Option::STRONG_DICE },
//     { "tough_dice", Option::TOUGH_DICE },
//     { "max_players", Option::MAX_PLAYERS },
//     { "brains_to_win", Option::BRAINS_TO_WIN },
//     { "max_turns", Option::MAX_TURNS },
//     { "weak_die_faces", Option::WEAK_DIE_FACES },
//     { "strong_die_faces", Option::STRONG_DIE_FACES },
//     { "tough_die_faces", Option::TOUGH_DIE_FACES }
//   };
//   //!< Maps Option to its respective string.
//   static const inline name_map_t m_names{
//     { Option::WEAK_DICE, "weak_dice" },
//     { Option::STRONG_DICE, "strong_dice" },
//     { Option::TOUGH_DICE, "tough_dice" },
//     { Option::MAX_PLAYERS, "max_players" },
//     { Option::BRAINS_TO_WIN, "brains_to_win" },
//     { Option::MAX_TURNS, "max_turns" },
//     { Option::WEAK_DIE_FACES, "weak_die_faces" },
//     { Option::STRONG_DIE_FACES, "strong_die_faces" },
//     { Option::TOUGH_DIE_FACES, "tough_die_faces" }
//   };

//   /// @brief Defines the default path of the ZDice configuration file.
//   static fs::path default_path();

//   /// @brief Ensures that the default configuration file always exists in the
//   /// default path defined.
//   void ensure_default_ini_exists();

//   bool is_valid_file(const fs::path& path) const;

//   void process_line(str& line, flag& in_section_valid);

//   /**
//    * @brief Validates a section header line in the INI file.
//    * @param line The line to validate.
//    */
//   bool process_section_line(const str& line);

//   /**
//    * @brief Parses a line with key=value format and updates `m_data`.
//    * @param line The line to parse.
//    */
//   void process_option_line(const str& line);
// };

// #endif
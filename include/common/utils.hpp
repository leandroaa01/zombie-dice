#ifndef UTILS_HPP
#define UTILS_HPP

#include "alias.hpp"
#include "constants.hpp"
#include <algorithm>
#include <charconv>
#include <random>
#include <sstream>

/**
 * @brief Removes whitespace to the left of the given string.
 * @param input The string to be processed.
 * @return A new string without whitespaces on the left.
 */
inline str ltrim(const str& input)
{
  // [!] Finds the index of the first character other than whitespace.
  const auto start{ input.find_first_not_of(WHITESPACE) };

  /* [!]
   * If it doesn't find any non-space characters (the whole string is space), it
   * returns an empty string. Otherwise, returns the substring starting from the
   * first non-space character.
   */
  return (start == str::npos) ? "" : input.substr(start);
}

/**
 * @brief Removes whitespace to the right of the given string.
 * @param input The string to be processed.
 * @return A new string without whitespaces on the right
 */
inline str rtrim(const str& input)
{
  // [!] Finds the index of the last character other than whitespace.
  const auto end{ input.find_last_not_of(WHITESPACE) };

  /* [!]
   * If it doesn't find any non-space characters (the whole string is space), it
   * returns an empty string. Otherwise, returns the substring starting from the
   * first non-space character.
   */
  return (end == str::npos) ? "" : input.substr(0, end + 1);
}

/**
 * @brief Removes whitespace to the left and right of the given string.
 * @param input The string to be processed.
 * @return A new string without whitespaces on the left and right.
 * @note Apply `ltrim`, after `rtrim` in `input`.
 */
inline str trim(const str& input) { return rtrim(ltrim(input)); }

/**
 * @brief Checks if the given string represents a valid number of type T.
 * @tparam T The numeric type to validate against (e.g., int, long, float).
 * @param input The string to validate.
 * @return true if the string is a valid representation of a number of type T;
 * false otherwise.
 *
 * This function uses `std::from_chars` to efficiently check if the entire
 * string can be converted to a number of type T without errors. It requires T
 * to be an arithmetic type.
 *
 * Example usage:
 * @code
 * is_number<int>("123");        // true
 * is_number<float>("12.34");    // true
 * is_number<int>("12.34");      // false
 * is_number<int>("abc");        // false
 * @endcode
 *
 * @note The function does not handle locale-dependent formats (e.g., commas as
 * decimal separators).
 * @note Leading or trailing whitespace must be trimmed before calling this
 * function.
 */
template<typename T>
inline bool is_number(const str& input)
{
  static_assert(std::is_arithmetic_v<T>, "T it must be a numeric type, like int, long, float...");
  if (input.empty()) return false;
  T value{};
  auto [ptr, ec]{ std::from_chars(input.data(), input.data() + input.size(), value) };
  return ec == std::errc{} and ptr == input.data() + input.size();
}

template<typename T>
void shuffle(vec<T>& v)
{
  //<! Para fabricar um generador de números aleatórios
  std::random_device rd;
  //<! Para  generar números aleatorios
  std::mt19937 g(rd());
  //<! faz um embaralhamento aleatório dos dados
  std::shuffle(v.begin(), v.end(), g);
}

#endif //!< UTILS_HPP
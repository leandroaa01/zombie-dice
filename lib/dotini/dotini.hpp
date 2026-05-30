#ifndef DOTINI_HPP
#define DOTINI_HPP

#include "common/alias.hpp"
#include "common/utils.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_map>

enum class dotcode
{
  NONE = 0,
  GOOD,
  BAD
};

class Dotini
{
public:
  using key_t = std::string;
  using value_t = std::string;
  using section_name_t = std::string;
  using section_t = std::unordered_map<key_t, value_t>;
  using ini_map_t = std::unordered_map<section_name_t, section_t>;

  Dotini(const std::string& filename) { parse(filename); }

  template<typename T>
  T get(const std::string& section, const key_t& key, const T& def = T())
  {
    auto section_itr{ m_data.find(section) };
    if (section_itr != m_data.end())
    {
      auto key_itr{ section_itr->second.find(key) };
      if (key_itr != section_itr->second.end())
      {
        const auto& value{ key_itr->second };
        m_code = dotcode::GOOD;

        if constexpr (std::is_same_v<T, std::string>)
        {
          return value;
        }
        else if constexpr (std::is_same_v<T, std::size_t>)
        {
          try
          {
            return std::stoull(value);
          }
          catch (...)
          {
            m_code = dotcode::BAD;
            return def;
          }
        }
        else
        {
          l(std::is_same_v<T, std::string> or std::is_same_v<T, std::size_t>,
            "Unsupported type. Only std::string and std::size_t are supported.");
        }
      }
    }

    m_code = dotcode::BAD;
    return def;
  }

  bool has_section(const std::string& section) const { return m_data.find(section) != m_data.end(); }

  bool has_key(const std::string& section, const key_t& key) const
  {
    auto section_itr{ m_data.find(section) };
    if (section_itr != m_data.end())
    {
      return section_itr->second.find(key) != section_itr->second.end();
    }
    return false;
  }

  [[nodiscard]] std::string to_string() const
  {
    std::ostringstream oss{};
    for (const auto& [section, keys] : m_data)
    {
      oss << "[" << section << "]\n";
      for (const auto& [key, value] : keys)
      {
        oss << key << " = " << value << '\n';
      }
      oss << '\n';
    }
    return oss.str();
  }

  const ini_map_t& data() const { return m_data; }

  bool good() const { return m_code == dotcode::GOOD; }

  bool bad() const { return m_code == dotcode::BAD; }

  void flush() { m_code = dotcode::NONE; }

private:
  ini_map_t m_data;
  dotcode m_code;

  void parse(const std::string& filename)
  {
    std::ifstream file(filename);
    if (not file.is_open())
    {
      throw std::runtime_error("Não foi possível abrir o arquivo: " + filename);
    }

    std::string line{};
    std::string current_section{ "root" };

    while (std::getline(file, line))
    {
      process_line(line, current_section);
    }

    file.close();
  }

  void process_line(std::string& line, std::string& current_section)
  {
    handle_comments(line);

    line = trim(line);
    if (line.empty())
    {
      return;
    }

    if (line.front() == '[' and line.back() == ']')
    {
      current_section = trim(line.substr(1, line.size() - 2));
      return;
    }

    handle_keys_and_values(line, current_section);
  }

  void handle_comments(std::string& line)
  {
    auto marker{ line.find_first_of(";#") };
    if (marker != std::string::npos)
    {
      line.erase(marker);
    }
  }

  void handle_keys_and_values(std::string& line, std::string& current_section)
  {
    auto delimeter{ line.find('=') };
    if (delimeter != std::string::npos)
    {
      std::string key{ trim(line.substr(0, delimeter)) };
      std::string value{ trim(line.substr(delimeter + 1)) };
      m_data[current_section][key] = value;
    }
  }
};

#endif //!< DOTINI_HPP
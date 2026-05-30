#ifndef ALIAS_HPP
#define ALIAS_HPP

#include <cstddef>       // std::size_t
#include <cstdint>       // std::uint8_t
#include <map>           // std::map
#include <string>        // std::string
#include <string_view>   // std::string_view
#include <tuple>         // std::tuple
#include <unordered_map> // std::unordered_map
#include <vector>        // std::vector

using byte = std::uint8_t;

using str = std::string;

using str_v = std::string_view;

using size_type = std::size_t;

using raw_str = const char*;

using flag = bool;

template<typename X, typename Y, typename Z>
using tuple = std::tuple<X, Y, Z>;

template<typename X, typename Y>
using par = std::pair<X, Y>;

template<typename T>
using vec = std::vector<T>;

template<typename K, typename T>
using umap = std::unordered_map<K, T>;

template<typename K, typename T>
using omap = std::map<K, T>;

#endif //!< ALIAS_HPP
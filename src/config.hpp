#pragma once
#include <type_traits>

namespace rog::config
{

template <typename T>
    requires std::is_arithmetic_v<T>
static const T tile_size = static_cast<T>(16);

template <typename T>
    requires std::is_arithmetic_v<T>
static const T map_size = static_cast<T>(30);

template <typename T>
    requires std::is_arithmetic_v<T>
static const T map_size_pixels = map_size<T>*tile_size<T>;

} // namespace rog::config

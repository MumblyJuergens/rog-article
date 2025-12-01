#pragma once
#include <type_traits>

namespace rog::config
{

template <typename T>
    requires std::is_arithmetic_v<T>
static const T tile_size = static_cast<T>(16);

} // namespace rog::config

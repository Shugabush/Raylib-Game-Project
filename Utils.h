#pragma once

#include "raylib.h"

#include <type_traits>

template<typename Enum>
Enum operator |(Enum lhs, Enum rhs)
{
    static_assert(std::is_enum<Enum>::value,
        "template parameter is not an enum type");

    using underlying = typename std::underlying_type<Enum>::type;

    return static_cast<Enum> (
        static_cast<underlying>(lhs) |
        static_cast<underlying>(rhs)
        );
}

// Operators that take the vector2 as the right hand side
#pragma region Vector2Operators
static Vector2 operator*(const float& a, const Vector2& b)
{
    return { a * b.x, a * b.y };
}

#pragma endregion
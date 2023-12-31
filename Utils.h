#pragma once

#include "raylib.h"

#include <type_traits>
#include <unordered_map>
#include <vector>

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

template<typename _Ty>
void RemoveFromCollection(std::vector<_Ty>& Vector, const _Ty& Element)
{
    Vector.erase(std::remove(Vector.begin(), Vector.end(), Element), Vector.end());
}

template<typename _Kty, typename _Ty>
bool MapContains(const std::unordered_map<_Kty, _Ty> Map, const _Kty& Key)
{
    return Map.find(Key) != Map.end();
}

// Operators that take vector2 as the right hand side
#pragma region Vector2Operators
static Vector2 operator*(const float& a, const Vector2& b)
{
    return { a * b.x, a * b.y };
}

#pragma endregion
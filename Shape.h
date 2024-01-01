#pragma once

#include "raylib.h"
#include "raymath.h"

#include <cstdint>
struct Circle
{
    float Radius;
};

struct AABB
{
    Vector2 HalfExtents;

    Vector2 GetMin(const Vector2& center) const
    {
        return Vector2Subtract(center, HalfExtents);
    }

    Vector2 GetMax(const Vector2& center) const
    {
        return Vector2Add(center, HalfExtents);
    }
};

enum class ShapeType : uint8_t
{
    NONE = 0,
    CIRCLE = 1 << 0,
    AABB = 1 << 1
};

struct Shape
{
    // an enum identifying
    ShapeType Type;

    // add new types of shapes to this anonymous union
    union
    {
        Circle CircleData;
        AABB AABBData;
    };

    void Draw(class GameObject* Owner);
};

bool CheckCircleCircle(Vector2 posA, Circle circleA, Vector2 posB, Circle circleB);
bool CheckCircleCircle(const Vector2& posA, const Shape& shapeA, const Vector2& posB, const Shape& shapeB);

bool CheckCircleAABB(Vector2 posA, Circle circleA, Vector2 posB, AABB aABB);
bool CheckCircleAABB(const Vector2& posA, const Shape& shapeA, const Vector2& posB, const Shape& shapeB);

bool CheckAABBAABB(Vector2 posA, AABB aABB1, Vector2 posB, AABB aABB2);
bool CheckAABBAABB(const Vector2& posA, const Shape& shapeA, const Vector2& posB, const Shape& shapeB);

Vector2 DepenetrateCircleCircle(const Vector2& posA, const Circle& circleA, const Vector2& posB, const Circle& circleB, float& pen);
Vector2 DepenetrateCircleCircle(const Vector2& posA, const Shape& shapeA, const Vector2& posB, const Shape& shapeB, float& pen);

Vector2 DepenetrateCircleAABB(const Vector2& posA, const Circle& circleA, const Vector2& posB, const AABB& aABB, float& pen);
Vector2 DepenetrateCircleAABB(const Vector2& posA, const Shape& shapeA, const Vector2& posB, const Shape& shapeB, float& pen);

Vector2 DepenetrateAABBAABB(const Vector2& posA, const AABB& aABB1, const Vector2& posB, const AABB& aABB2, float& pen);
Vector2 DepenetrateAABBAABB(const Vector2& posA, const Shape& shapeA, const Vector2& posB, const Shape& shapeB, float& pen);
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

struct OBB : public AABB
{
    float Rotation;
};

enum class ShapeType : uint8_t
{
    NONE = 0,
    CIRCLE = 1 << 0,
    AABB = 1 << 1,
    OBB = 1 << 2,
};

struct Collider
{
    bool IsTrigger;

    // an enum identifying
    ShapeType Type;

    // add new types of shapes to this anonymous union
    union
    {
        Circle CircleData;
        AABB AABBData;
        OBB OBBData;
    };

    void Draw(class GameObject* Owner);
};

bool CheckCircleCircle(Vector2 posA, Circle circleA, Vector2 posB, Circle circleB);
bool CheckCircleCircle(const Vector2& posA, const Collider& shapeA, const Vector2& posB, const Collider& shapeB);

bool CheckCircleAABB(Vector2 posA, Circle circleA, Vector2 posB, AABB aABB);
bool CheckCircleAABB(const Vector2& posA, const Collider& shapeA, const Vector2& posB, const Collider& shapeB);

bool CheckCircleOBB(Vector2 posA, Circle circleA, Vector2 posB, OBB oBB);
bool CheckCircleOBB(const Vector2& posA, const Collider& shapeA, const Vector2& posB, const Collider& shapeB);

bool CheckAABBAABB(Vector2 posA, AABB aABB1, Vector2 posB, AABB aABB2);
bool CheckAABBAABB(const Vector2& posA, const Collider& shapeA, const Vector2& posB, const Collider& shapeB);

bool CheckAABBOBB(Vector2 posA, AABB aABB, Vector2 posB, OBB oBB);
bool CheckAABBOBB(const Vector2& posA, const Collider& shapeA, const Vector2& posB, const Collider& shapeB);

bool CheckOBBOBB(Vector2 posA, OBB oBB1, Vector2 posB, OBB oBB2);
bool CheckOBBOBB(const Vector2& posA, const Collider& shapeA, const Vector2& posB, const Collider& shapeB);

Vector2 DepenetrateCircleCircle(const Vector2& posA, const Circle& circleA, const Vector2& posB, const Circle& circleB, float& pen);
Vector2 DepenetrateCircleCircle(const Vector2& posA, const Collider& shapeA, const Vector2& posB, const Collider& shapeB, float& pen);

Vector2 DepenetrateCircleAABB(const Vector2& posA, const Circle& circleA, const Vector2& posB, const AABB& aABB, float& pen);
Vector2 DepenetrateCircleAABB(const Vector2& posA, const Collider& shapeA, const Vector2& posB, const Collider& shapeB, float& pen);

Vector2 DepenetrateAABBAABB(const Vector2& posA, const AABB& aABB1, const Vector2& posB, const AABB& aABB2, float& pen);
Vector2 DepenetrateAABBAABB(const Vector2& posA, const Collider& shapeA, const Vector2& posB, const Collider& shapeB, float& pen);
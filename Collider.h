#pragma once

#include "raylib.h"
#include "raymath.h"

#include <cstdint>
struct Circle
{
    float Radius;
};

struct Box
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
    Box = 1 << 1
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
        Box BoxData;
    };

    void Draw(class GameObject* Owner);
};

bool CheckCircleCircle(Vector2 posA, Circle circleA, Vector2 posB, Circle circleB);
bool CheckCircleCircle(const Vector2& posA, const Collider& shapeA, const Vector2& posB, const Collider& shapeB);

bool CheckCircleBox(Vector2 posA, Circle circleA, Vector2 posB, Box aABB);
bool CheckCircleBox(const Vector2& posA, const Collider& shapeA, const Vector2& posB, const Collider& shapeB);

bool CheckBoxBox(Vector2 posA, Box aABB1, Vector2 posB, Box aABB2);
bool CheckBoxBox(const Vector2& posA, const Collider& shapeA, const Vector2& posB, const Collider& shapeB);

Vector2 DepenetrateCircleCircle(const Vector2& posA, const Circle& circleA, const Vector2& posB, const Circle& circleB, float& pen);
Vector2 DepenetrateCircleCircle(const Vector2& posA, const Collider& shapeA, const Vector2& posB, const Collider& shapeB, float& pen);

Vector2 DepenetrateCircleBox(const Vector2& posA, const Circle& circleA, const Vector2& posB, const Box& aABB, float& pen);
Vector2 DepenetrateCircleBox(const Vector2& posA, const Collider& shapeA, const Vector2& posB, const Collider& shapeB, float& pen);

Vector2 DepenetrateBoxBox(const Vector2& posA, const Box& aABB1, const Vector2& posB, const Box& aABB2, float& pen);
Vector2 DepenetrateBoxBox(const Vector2& posA, const Collider& shapeA, const Vector2& posB, const Collider& shapeB, float& pen);
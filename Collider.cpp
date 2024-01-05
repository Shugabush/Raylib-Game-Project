#include "Collider.h"
#include "GameObject.h"
#include "Utils.h"

#include <cmath>
#include <iostream>

void Collider::Draw(GameObject* Owner)
{
    switch (Type)
    {
    case ShapeType::AABB:
        DrawRectangleLines(Owner->Position.x, Owner->Position.y, AABBData.HalfExtents.x * 2, AABBData.HalfExtents.y * 2, Owner->Rotation, Owner->CollisionColor);
        break;
    case ShapeType::CIRCLE:
        DrawCircleLines((Owner->Position.x), (Owner->Position.y), CircleData.Radius, Owner->CollisionColor);
        break;
    }
}

bool CheckCircleCircle(Vector2 posA, Circle circleA, Vector2 posB, Circle circleB)
{
    float radii = circleA.Radius + circleB.Radius;

    float dst2 = Vector2Distance(posA, posB);

    return dst2 < (radii * radii);
}
bool CheckCircleCircle(const Vector2& posA, const Collider& shapeA, const Vector2& posB, const Collider& shapeB)
{
    return CheckCircleCircle(posA, shapeA.CircleData, posB, shapeB.CircleData);
}

bool CheckCircleAABB(Vector2 posA, Circle circleA, Vector2 posB, AABB aABB)
{
    // clamp the center of the circle to the bounds of the AABB
    float clampedX = Clamp(posA.x, aABB.GetMin(posB).x, aABB.GetMax(posB).x);
    float clampedY = Clamp(posA.y, aABB.GetMin(posB).y, aABB.GetMax(posB).y);

    float dstToClamped2 = Vector2Distance(posA, { clampedX, clampedY });

    return dstToClamped2 < (circleA.Radius * circleA.Radius);
}
bool CheckCircleAABB(const Vector2& posA, const Collider& shapeA, const Vector2& posB, const Collider& shapeB)
{
    return CheckCircleAABB(posA, shapeA.CircleData, posB, shapeB.AABBData);
}

bool CheckCircleOBB(Vector2 posA, Circle circleA, Vector2 posB, OBB oBB)
{
    std::cout << "circle-obb check" << std::endl;
    return false;
}

bool CheckCircleOBB(const Vector2& posA, const Collider& shapeA, const Vector2& posB, const Collider& shapeB)
{
    return CheckCircleOBB(posA, shapeA.CircleData, posB, shapeB.OBBData);
}

bool CheckAABBAABB(Vector2 posA, AABB aABB1, Vector2 posB, AABB aABB2)
{
    bool xOverlaps = false;
    bool yOverlaps = false;

    // X overlap checks
    if (posA.x < posB.x && posA.x + aABB1.HalfExtents.x > posB.x - aABB2.HalfExtents.x)
    {
        xOverlaps = true;
    }
    else if (posA.x > posB.x && posA.x - aABB1.HalfExtents.x < posB.x + aABB2.HalfExtents.x)
    {
        xOverlaps = true;
    }
    else if (posA.x == posB.x)
    {
        xOverlaps = true;
    }

    // Y overlap checks
    if (posA.y < posB.y && posA.y + aABB1.HalfExtents.y > posB.y - aABB2.HalfExtents.y)
    {
        yOverlaps = true;
    }
    else if (posA.y > posB.y && posA.y - aABB1.HalfExtents.y < posB.y + aABB2.HalfExtents.y)
    {
        yOverlaps = true;
    }
    else if (posA.y == posB.y)
    {
        yOverlaps = true;
    }

    return xOverlaps && yOverlaps;
}
bool CheckAABBAABB(const Vector2& posA, const Collider& shapeA, const Vector2& posB, const Collider& shapeB)
{
    return CheckAABBAABB(posA, shapeA.AABBData, posB, shapeB.AABBData);
}

bool CheckAABBOBB(Vector2 posA, AABB aABB, Vector2 posB, OBB oBB)
{
    std::cout << "aabb-obb check" << std::endl;
    return false;
}

bool CheckAABBOBB(const Vector2& posA, const Collider& shapeA, const Vector2& posB, const Collider& shapeB)
{
    return CheckAABBOBB(posA, shapeA.AABBData, posB, shapeB.OBBData);
}

bool CheckOBBOBB(Vector2 posA, OBB oBB1, Vector2 posB, OBB oBB2)
{
    return false;
}

bool CheckOBBOBB(const Vector2& posA, const Collider& shapeA, const Vector2& posB, const Collider& shapeB)
{
    return CheckOBBOBB(posA, shapeA.OBBData, posB, shapeB.OBBData);
}

Vector2 DepenetrateCircleCircle(const Vector2& posA, const Circle& circleA, const Vector2& posB, const Circle& circleB, float& pen)
{
    // grab the distance
    float dst = Vector2Distance(posB, posA);

    // sum of the radii
    float radii = circleA.Radius + circleB.Radius;

    // calculate penetration
    pen = radii - dst;

    // return the collision normal
    return Vector2Normalize(Vector2Subtract(posB, posA));
}

Vector2 DepenetrateCircleCircle(const Vector2& posA, const Collider& shapeA, const Vector2& posB, const Collider& shapeB, float& pen)
{
    return DepenetrateCircleCircle(posA, shapeA.CircleData, posB, shapeB.CircleData, pen);
}

Vector2 DepenetrateCircleAABB(const Vector2& posA, const Circle& circleA, const Vector2& posB, const AABB& aABB, float& pen)
{
    // cache min and max
    Vector2 min = aABB.GetMin(posB);
    Vector2 max = aABB.GetMax(posB);

    // find the nearest point within the AABB bounds
    Vector2 nearestPoint = Vector2Clamp(posA, min, max);

    // offset from the circle to the nearest point
    Vector2 offset = Vector2Subtract(nearestPoint, posA);

    // is the circle's center within the AABB?
    if (posA.x >= min.x && posA.x <= max.x &&
        posA.y >= min.y && posA.y <= max.y)
    {
        Vector2 rawOffset = Vector2Subtract(posB, posA);
        Vector2 absOffset = { abs(rawOffset.x), abs(rawOffset.y) };

        Vector2 overlap = Vector2Subtract(aABB.HalfExtents, absOffset);

        Vector2 normal = {};
        if (overlap.x < overlap.y)
        {
            pen = overlap.x;
            normal.x = copysignf(1, rawOffset.x);
        }
        else
        {
            pen = overlap.y;
            normal.y = copysignf(1, rawOffset.y);
        }
        return normal;
    }
    else
    {
        // generate the pen and return the normal
        pen = circleA.Radius - Vector2Length(offset);
        return Vector2Normalize(offset);
    }
}

Vector2 DepenetrateCircleAABB(const Vector2& posA, const Collider& shapeA, const Vector2& posB, const Collider& shapeB, float& pen)
{
    return DepenetrateCircleAABB(posA, shapeA.CircleData, posB, shapeB.AABBData, pen);
}

Vector2 DepenetrateAABBAABB(const Vector2& posA, const AABB& aABB1, const Vector2& posB, const AABB& aABB2, float& pen)
{
    Vector2 rawOffset = Vector2Subtract(posB, posA);
    Vector2 offset = { abs(rawOffset.x), abs(rawOffset.y) };

    // calculate the x-pen
    float xPen = aABB1.HalfExtents.x + aABB2.HalfExtents.x - offset.x;
    // calculate the y-pen
    float yPen = aABB1.HalfExtents.y + aABB2.HalfExtents.y - offset.y;

    Vector2 normal = {};

    // pick the one that is smaller
    if (xPen < yPen)
    {
        pen = xPen;
        normal.x = copysignf(1.0f, rawOffset.x);
    }
    else
    {
        pen = yPen;
        normal.y = copysignf(1.0f, rawOffset.y);
    }

    return normal;
}

Vector2 DepenetrateAABBAABB(const Vector2& posA, const Collider& shapeA, const Vector2& posB, const Collider& shapeB, float& pen)
{
    return DepenetrateAABBAABB(posA, shapeA.AABBData, posB, shapeB.AABBData, pen);
}
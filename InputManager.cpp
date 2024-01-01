#include "InputManager.h"
#include "raymath.h"

Vector2 InputManager::GetMovement()
{
    Vector2 Input = Vector2();

    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
    {
        Input.x--;
    }
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
    {
        Input.x++;
    }

    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W))
    {
        Input.y--;
    }
    if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S))
    {
        Input.y++;
    }

    return Input;
}

Vector2 InputManager::GetNormalizedMovement()
{
    return Vector2Normalize(GetMovement());
}

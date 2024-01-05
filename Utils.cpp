#include "Utils.h"

void DrawRectangleLines(int posX, int posY, int width, int height, float rotation, Color color)
{
    Vector2 recSize = { width, height };

    Vector2 pos = { posX, posY };

    Vector2 bottomRight = pos + Vector2Rotate(recSize, rotation);
    Vector2 bottomLeft = pos + Vector2Rotate({ -recSize.x, recSize.y }, rotation);
    Vector2 topRight = pos + Vector2Rotate({ recSize.x, -recSize.y }, rotation);
    Vector2 topLeft = pos + Vector2Rotate(-recSize, rotation);

    DrawLine(topLeft.x, topLeft.y, topRight.x, topRight.y, color);
    DrawLine(topRight.x, topRight.y, bottomRight.x, bottomRight.y, color);
    DrawLine(bottomRight.x, bottomRight.y, bottomLeft.x, bottomLeft.y, color);
    DrawLine(bottomLeft.x, bottomLeft.y, topLeft.x, topLeft.y, color);
}

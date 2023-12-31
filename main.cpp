#include "BaseScene.h"
#include "TextureLibrary.h"

#include <raylib-cpp.hpp>

int main() {
    
    // Initialization
    int screenWidth = 800;
    int screenHeight = 450;

    BaseScene* Game = new BaseScene();

    Game->Init();

    InitWindow(screenWidth, screenHeight, "Raylib Game");

    GameObject* SpawnedObject = Game->SpawnObject<GameObject>({ 250, 250 }, 0);
    SpawnedObject->Texture = GetTexture("Textures/CharacterPack/PNG/Skin/Tint1/tint1_head.png");


    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        // TODO: Update your variables here
        Game->Update(GetFrameTime());

        // Draw
        BeginDrawing();

        ClearBackground(RAYWHITE);

        Game->Draw();

        EndDrawing();
    }

    // Cleanup
    delete Game;

    return 0;
}
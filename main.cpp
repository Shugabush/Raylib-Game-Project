#include "BaseScene.h"
#include "Player.h"
#include "TextureLibrary.h"

#include <raylib-cpp.hpp>

int main() {
    
    // Initialization
    int screenWidth = 800;
    int screenHeight = 450;

    BaseScene* Game = new BaseScene();

    Game->Init();

    InitWindow(screenWidth, screenHeight, "Raylib Game");

    Player* SpawnedPlayer = Game->SpawnObject<Player>({ 250, 250 }, 0);
    SpawnedPlayer->Texture = GetTexture("Textures/CharacterPack/PNG/Skin/Tint1/tint1_head.png");

    GameObject* SpawnedObject = Game->SpawnObject<GameObject>({ 100, 100 }, 0);

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        // TODO: Update your variables here
        Game->Update(GetFrameTime());

        Game->FixedUpdate();

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
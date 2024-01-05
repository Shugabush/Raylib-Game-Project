#include "BaseScene.h"
#include "Player.h"
#include "TextureLibrary.h"

int main() {
    
    // Initialization
    int screenWidth = 800;
    int screenHeight = 450;

    BaseScene* Game = new BaseScene();

    Game->Init();

    InitWindow(screenWidth, screenHeight, "Raylib Game");

    Player* SpawnedPlayer = Game->SpawnObject<Player>({ 250, 250 }, 0);
    SpawnedPlayer->Texture = GetTexture("Textures/CharacterPack/PNG/Skin/Tint1/tint1_head.png");
    SpawnedPlayer->Name = "Player";

    GameObject* SpawnedObject = Game->SpawnObject<GameObject>({ 100, 100 }, 0);
    SpawnedObject->Col.IsTrigger = true;

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        // TODO: Update your variables here
        SpawnedObject->Rotation += GetFrameTime();

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
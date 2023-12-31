#include "BaseGame.h"

#include <algorithm>

#include "raylib.h"
#include "Utils.h"

BaseGame::BaseGame() { }

BaseGame::~BaseGame()
{
	for (auto it : GameObjects)
	{
		delete it;
	}
	GameObjects.clear();
}

void BaseGame::Init()
{
	collisionCheckers[ShapeType::CIRCLE | ShapeType::CIRCLE] = CheckCircleCircle;
	collisionCheckers[ShapeType::CIRCLE | ShapeType::AABB] = CheckCircleAABB;
	collisionCheckers[ShapeType::AABB | ShapeType::AABB] = CheckAABBAABB;

	collisionDepenetrators[ShapeType::CIRCLE | ShapeType::CIRCLE] = DepenetrateCircleCircle;
	collisionDepenetrators[ShapeType::CIRCLE | ShapeType::AABB] = DepenetrateCircleAABB;
	collisionDepenetrators[ShapeType::AABB | ShapeType::AABB] = DepenetrateAABBAABB;

	int screenWidth = 800;
	int screenHeight = 450;

	InitWindow(screenWidth, screenHeight, "raylib example");

	SetTargetFPS(60);

	OnInit();
}

void BaseGame::Tick()
{
	AccumulatedFixedTime += GetFrameTime();

	OnTick();
}

void BaseGame::TickFixed()
{
	AccumulatedFixedTime -= TargetFixedStep;

	for (auto& i : GameObjects)
	{
		i->TickPhys(TargetFixedStep);
		for (auto& j : GameObjects)
		{
			if (&i == &j) { continue; }

			auto left = i;
			auto right = j;

			if (i->Collider.Type > j->Collider.Type)
			{
				left = j;
				right = i;
			}

			ShapeType pairType = left->Collider.Type | right->Collider.Type;

			// Check every object against every other object
			bool isColliding = collisionCheckers[pairType](left->Position, left->Collider, right->Position, right->Collider);

			if (isColliding)
			{
				float pen = 0;
				Vector2 normal = collisionDepenetrators[pairType](left->Position, left->Collider, right->Position, right->Collider, pen);

				ResolvePhysObjects(*left, *right, 1, normal, pen);
			}
		}
	}
}

void BaseGame::Draw() const
{
	BeginDrawing();

	ClearBackground(RAYWHITE);

	for (auto obj : GameObjects)
	{
		obj->Draw();
	}

	OnDraw();

	EndDrawing();
}

void BaseGame::Exit()
{
	CloseWindow();
}

bool BaseGame::ShouldClose() const
{
	return WindowShouldClose();
}

bool BaseGame::ShouldTickFixed() const
{
	return AccumulatedFixedTime >= TargetFixedStep;
}

void BaseGame::Destroy(GameObject* obj)
{
	GameObjects.erase(std::remove(GameObjects.begin(), GameObjects.end(), obj), GameObjects.end());
}
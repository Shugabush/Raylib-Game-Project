#include "BaseScene.h"

#include <algorithm>

#include "raylib.h"
#include "Utils.h"

BaseScene::BaseScene() { }

BaseScene::~BaseScene()
{
	for (auto obj : GameObjects)
	{
		delete obj;
	}
	GameObjects.clear();
}

void BaseScene::Init()
{
	collisionCheckers[ShapeType::CIRCLE | ShapeType::CIRCLE] = CheckCircleCircle;
	collisionCheckers[ShapeType::CIRCLE | ShapeType::AABB] = CheckCircleAABB;
	collisionCheckers[ShapeType::AABB | ShapeType::AABB] = CheckAABBAABB;

	collisionDepenetrators[ShapeType::CIRCLE | ShapeType::CIRCLE] = DepenetrateCircleCircle;
	collisionDepenetrators[ShapeType::CIRCLE | ShapeType::AABB] = DepenetrateCircleAABB;
	collisionDepenetrators[ShapeType::AABB | ShapeType::AABB] = DepenetrateAABBAABB;

	SetTargetFPS(60);

	for (auto& i : GameObjects)
	{
		i->EarlyStart();
	}

	for (auto& i : GameObjects)
	{
		i->Start();
	}

	for (auto& i : GameObjects)
	{
		i->LateStart();
	}

	OnInit();
}

void BaseScene::Update(float deltaTime)
{
	for (auto& i : ObjectsToAdd)
	{
		GameObjects.push_back(i);
	}

	ObjectsToAdd.clear();
	
	for (auto& i : ObjectsToDestroy)
	{
		RemoveFromCollection(GameObjects, i);
		delete i;
	}

	ObjectsToDestroy.clear();

	AccumulatedFixedTime += deltaTime;

	OnTick(deltaTime);

	for (auto& i : GameObjects)
	{
		i->EarlyUpdate(deltaTime);
	}

	for (auto& i : GameObjects)
	{
		i->Update(deltaTime);
	}

	for (auto& i : GameObjects)
	{
		i->LateUpdate(deltaTime);
	}
}

void BaseScene::FixedUpdate()
{
	AccumulatedFixedTime -= TargetFixedStep;

	for (auto& i : GameObjects)
	{
		i->FixedUpdate(TargetFixedStep);
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

void BaseScene::Draw() const
{
	for (auto i : GameObjects)
	{
		i->EarlyDraw();
	}

	for (auto i : GameObjects)
	{
		i->Draw();
	}

	for (auto i : GameObjects)
	{
		i->LateDraw();
	}

	OnDraw();
}

void BaseScene::Exit()
{
	CloseWindow();
}

bool BaseScene::ShouldClose() const
{
	return WindowShouldClose();
}

bool BaseScene::ShouldTickFixed() const
{
	return AccumulatedFixedTime >= TargetFixedStep;
}

void BaseScene::Destroy(GameObject* obj)
{
	GameObjects.erase(std::remove(GameObjects.begin(), GameObjects.end(), obj), GameObjects.end());
}
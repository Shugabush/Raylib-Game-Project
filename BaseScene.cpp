#include "BaseScene.h"

#include "Utils.h"
#include "EnumUtils.h"

#include <raylib.h>
#include <algorithm>

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
	collisionCheckers[ShapeType::CIRCLE | ShapeType::OBB] = CheckCircleOBB;
	collisionCheckers[ShapeType::AABB | ShapeType::AABB] = CheckAABBAABB;
	collisionCheckers[ShapeType::AABB | ShapeType::OBB] = CheckAABBOBB;
	collisionCheckers[ShapeType::OBB | ShapeType::OBB] = CheckOBBOBB;

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
		i->PrimaryStart();
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
		i->PrimaryUpdate(deltaTime);
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

	for (int i = 0; i < GameObjects.size(); i++)
	{
		GameObject* obj1 = GameObjects[i];
		obj1->FixedUpdate(TargetFixedStep);
		for (int j = i + 1; j < GameObjects.size(); j++)
		{
			GameObject* obj2 = GameObjects[j];
			if (&obj1 == &obj2) { continue; }

			auto left = obj1;
			auto right = obj2;

			if (obj1->Col.Type > obj2->Col.Type)
			{
				left = obj2;
				right = obj1;
			}

			ShapeType pairType = left->Col.Type | right->Col.Type;

			// Check every object against every other object
			bool isColliding = collisionCheckers[pairType](left->Position, left->Col, right->Position, right->Col);

			if (isColliding)
			{
				float pen = 0;
				Vector2 normal = collisionDepenetrators[pairType](left->Position, left->Col, right->Position, right->Col, pen);

				if (!objectsColliding[{left, right}])
				{
					left->ComputeCollisionEnter(*right);
					right->ComputeCollisionEnter(*left);
					objectsColliding[{left, right}] = true;
				}

				left->ComputeCollisionStay(*right);
				right->ComputeCollisionStay(*left);

				if (!left->Col.IsTrigger && !right->Col.IsTrigger)
				{
					ResolvePhysObjects(*left, *right, 1, normal, pen);
				}
			}
			else
			{
				if (objectsColliding[{left, right}])
				{
					left->ComputeCollisionExit(*right);
					right->ComputeCollisionExit(*left);
					objectsColliding[{left, right}] = false;
				}
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
		i->PrimaryDraw();
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
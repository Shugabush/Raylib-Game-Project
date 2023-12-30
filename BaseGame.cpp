#include "BaseGame.h"

BaseGame::~BaseGame()
{
	for (auto obj : GameObjects)
	{
		delete obj;
	}
}

void BaseGame::Start()
{
	for (auto obj : GameObjects)
	{
		obj->EarlyStart();
	}

	for (auto obj : GameObjects)
	{
		obj->Start();
	}

	for (auto obj : GameObjects)
	{
		obj->LateStart();
	}
}

void BaseGame::Update(float DeltaTime)
{
	for (auto obj : GameObjects)
	{
		obj->EarlyUpdate(DeltaTime);
	}

	for (auto obj : GameObjects)
	{
		obj->Update(DeltaTime);
	}

	for (auto obj : GameObjects)
	{
		obj->LateUpdate(DeltaTime);
	}
}

void BaseGame::Draw()
{
	for (auto obj : GameObjects)
	{
		obj->EarlyDraw();
	}

	for (auto obj : GameObjects)
	{
		obj->Draw();
	}

	for (auto obj : GameObjects)
	{
		obj->LateDraw();
	}
}

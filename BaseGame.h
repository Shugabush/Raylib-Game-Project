#pragma once
#include <vector>

#include "GameObject.h"

class BaseGame
{
	std::vector<GameObject*> GameObjects;
	std::vector<GameObject*> ObjectsToAdd;
	std::vector<GameObject*> ObjectsToDestroy;

public:
	BaseGame() = default;
	~BaseGame();

	void Start();
	void Update(float DeltaTime);
	void Draw();

	template<typename T>
	T* SpawnObject(Vector2 Position = { 0, 0 }, float Rotation = 0);
};


template<typename T>
T* BaseGame::SpawnObject(Vector2 Position, float Rotation)
{
	static_assert(std::is_base_of<GameObject, T>::value, "T must inherit from GameObject!");

	GameObject* Object = new GameObject();
	Object->Position = Position;
	Object->Rotation = Rotation;

	ObjectsToAdd.push_back(Object);

	return static_cast<T*>(Object);

}
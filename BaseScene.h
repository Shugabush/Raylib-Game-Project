#pragma once

#include "GameObject.h"
#include "Shape.h"

#include <vector>
#include <unordered_map>

// the function signature for any collision detection test
using CollisionFunc = bool(*)(const Vector2&, const Shape&, const Vector2&, const Shape&);
using DepenetrationFunc = Vector2(*)(const Vector2&, const Shape&, const Vector2&, const Shape&, float&);

// a map that take a collision pair and returns the correct function to call
using CollisionMap = std::unordered_map<ShapeType, CollisionFunc>;
using DepenetrationMap = std::unordered_map<ShapeType, DepenetrationFunc>;

class BaseScene
{
protected:
	// Time since the last fixed update tick
	float AccumulatedFixedTime = 0;

	// Internal lifecycle events
	virtual void OnInit() { }

	virtual void OnTick(float deltaTime) { }

	virtual void OnDraw() const { }

	virtual void OnExit() { }

	std::vector<GameObject*> GameObjects;

	std::vector<GameObject*> ObjectsToAdd;
	std::vector<GameObject*> ObjectsToDestroy;

	CollisionMap collisionCheckers;
	DepenetrationMap collisionDepenetrators;

public:
	// defaults to 30fps
	float TargetFixedStep = 1.0f / 30.0f;

	Color BackgroundColor = RAYWHITE;

	// Default Constructor - set up good defaults
	BaseScene();

	~BaseScene();

	// Initialize the game
	void Init();

	// Poll for input and game logic
	void Update(float deltaTime);

	// Update any fixed update objects
	void FixedUpdate();

	// Drawing objects to the screen
	void Draw() const;

	// Terminate - clean-up resources
	void Exit();

	// Returns true if we need to exit
	bool ShouldClose() const;

	// Returns true if we are due for a fixed update
	bool ShouldTickFixed() const;

	template<typename T>
	T* SpawnObject(Vector2 Position, float Rotation);

	void Destroy(GameObject* obj);
};

template<typename T>
T* BaseScene::SpawnObject(Vector2 Position, float Rotation)
{
	static_assert(std::is_base_of<GameObject, T>::value, "T must inherit from GameObject!");

	GameObject* Object = new GameObject();
	Object->Position = Position;
	Object->Rotation = Rotation;

	ObjectsToAdd.push_back(Object);

	return static_cast<T*>(Object);

}
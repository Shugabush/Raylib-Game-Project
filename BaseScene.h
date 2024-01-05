#pragma once

#include "GameObject.h"
#include "PairHash.h"

#include <vector>
#include <unordered_map>

// the function signature for any collision detection test
using CollisionFunc = bool(*)(const Vector2&, const Collider&, const Vector2&, const Collider&);
using DepenetrationFunc = Vector2(*)(const Vector2&, const Collider&, const Vector2&, const Collider&, float&);

// maps that take a collision pair and returns the correct function to call
using CollisionFuncMap = std::unordered_map<ShapeType, CollisionFunc>;
using DepenetrationFuncMap = std::unordered_map<ShapeType, DepenetrationFunc>;

// a map that takes a game object pair and returns a bool
using CollisionMap = std::unordered_map<std::pair<GameObject*, GameObject*>, bool, PairHash>;

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

	CollisionFuncMap collisionCheckers;
	DepenetrationFuncMap collisionDepenetrators;

	CollisionMap objectsColliding;

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

	T* Object = new T();
	Object->Position = Position;
	Object->Rotation = Rotation;

	ObjectsToAdd.push_back(Object);

	return static_cast<T*>(Object);

}
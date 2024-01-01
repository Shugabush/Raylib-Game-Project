#include "GameObject.h"
#include "raylib.h"
#include "Utils.h"
#include <cassert>
#include <iostream>

GameObject::GameObject()
{
	Col = Collider();
	Col.Type = ShapeType::AABB;
	Col.AABBData.HalfExtents = { 25, 25 };
}

void GameObject::EarlyStart()
{

}

void GameObject::Start()
{

}

void GameObject::LateStart()
{

}

void GameObject::EarlyUpdate(float deltaTime)
{

}

void GameObject::Update(float deltaTime)
{

}

void GameObject::LateUpdate(float deltaTime)
{

}

void GameObject::EarlyDraw()
{

}

void GameObject::Draw()
{
	Vector2 TextureSize = { (float)Texture.width, (float)Texture.height };

	TextureSize = Vector2Normalize(TextureSize);

	TextureSize *= 250;

	DrawTexturePro(Texture,
		{ 0, 0, (float)Texture.width, (float)Texture.height },
		{ Position.x, Position.y, TextureSize.x, TextureSize.y },
		TextureSize / 2, Rotation, Tint);
}

void GameObject::LateDraw()
{
	if (DrawForwardDirection)
	{
		DrawLine((int)Position.x, (int)Position.y, (int)(Position.x), (int)(Position.y + 50), ForwardDirectionColor);
	}
	if (DrawCollision)
	{
		Col.Draw(this);
	}
}

void GameObject::FixedUpdate(float fixedDelta)
{
	if (UseGravity)
	{
		AddAccel({ 0, 9 });
	}
	Velocity += Forces * fixedDelta;

	// zero out forces
	Forces = {};

	// integrate linear drag
	Velocity *= 1.0f - fixedDelta * Drag;

	// integrate velocity into position
	Position += Velocity * fixedDelta;

	Position = Position + Velocity;
}

void GameObject::ComputeCollisionEnter(GameObject& OtherObject)
{
	if (OtherObject.Col.IsTrigger)
	{
		OnTriggerEnter(OtherObject);
	}
	else
	{
		OnCollisionEnter(OtherObject);
	}
}

void GameObject::ComputeCollisionStay(GameObject& OtherObject)
{
	if (OtherObject.Col.IsTrigger)
	{
		OnTriggerStay(OtherObject);
	}
	else
	{
		OnCollisionStay(OtherObject);
	}
}

void GameObject::ComputeCollisionExit(GameObject& OtherObject)
{
	if (OtherObject.Col.IsTrigger)
	{
		OnTriggerExit(OtherObject);
	}
	else
	{
		OnCollisionExit(OtherObject);
	}
}

void GameObject::AddAccel(Vector2 accel)
{
	Forces += accel;
}

void GameObject::AddVelocity(Vector2 velocity)
{
	Velocity += velocity;
}

void GameObject::AddForce(Vector2 force)
{
	Forces += force / Mass;
}

void GameObject::AddImpulse(Vector2 impulse)
{
	Velocity += impulse / Mass;
}

Vector2 GameObject::GetMomentum() const
{
	return Velocity * Mass;
}

void GameObject::SetParent(GameObject* NewParent)
{
	if (Parent != nullptr)
	{
		RemoveFromCollection<GameObject*>(Parent->Children, this);
	}
	Parent = NewParent;
	if (Parent != nullptr)
	{
		Parent->Children.push_back(this);
	}
}

void GameObject::OnCollisionEnter(GameObject& OtherObject)
{

}

void GameObject::OnCollisionStay(GameObject& OtherObject)
{

}

void GameObject::OnCollisionExit(GameObject& OtherObject)
{

}

void GameObject::OnTriggerEnter(GameObject& OtherObject)
{

}

void GameObject::OnTriggerStay(GameObject& OtherObject)
{

}

void GameObject::OnTriggerExit(GameObject& OtherObject)
{

}

void ResolveGameObjects(GameObject& left, GameObject& right, float elasticity, Vector2 normal, float pen)
{
	assert(fabs(Vector2Length(normal) - 1) < 0.001f && "normal needs to be unit length!!");
	assert(pen > 0.0f && "pen depth must be greater than zero!");

	// calculate impulse magnitude
	float impulseMag = ResolveCollision(left.Position, left.Velocity, left.Mass,
		right.Position, right.Velocity, right.Mass,
		elasticity, normal);

	// apply impulses to both objects and separate the objects
	Vector2 mtv = normal * pen;

	left.Position -= mtv;
	right.Position += mtv;

	Vector2 impulse = normal * impulseMag;
	left.AddImpulse(-impulse);
	right.AddImpulse(impulse);
}

void ResolvePhysObjects(GameObject& left, GameObject& right, float elasticity, Vector2 normal, float pen)
{
	assert(fabs(Vector2Length(normal) - 1) < 0.001f && "normal needs to be unit length!!");
	assert(pen > 0.0f && "pen depth must be greater than zero!");

	// calculate impulse magnitude
	float impulseMag = ResolveCollision(left.Position, left.Velocity, left.Mass,
		right.Position, right.Velocity, right.Mass,
		elasticity, normal);

	// apply impulses to both objects and separate the objects
	Vector2 mtv = normal * pen;

	left.Position -= mtv;
	right.Position += mtv;

	Vector2 impulse = normal * impulseMag;
	left.AddImpulse(-impulse);
	right.AddImpulse(impulse);
}

float ResolveCollision(const Vector2& posA, const Vector2& velA, float massA, const Vector2& posB, const Vector2& velB, float massB, float elasticity, const Vector2& normal)
{
	// calculate the relative velocity
	Vector2 relVel = velB - velA;

	// calculate the impulse magnitude
	// - 1st - determine how much the relative velocity aligns with the collision normal
	// - 2nd - account for the mass of each object (and how much it aligns with the collision normal)
	float impulseMag = Vector2DotProduct(-(1 + elasticity) * relVel, normal) /
		Vector2DotProduct(normal, normal * (1.0f / massA + 1.0f / massB));

	// return the impulse magnitude
	return impulseMag;
}
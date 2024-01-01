#pragma once

#include "Shape.h"

struct Collider : public Shape
{
	bool IsTrigger = false;
};
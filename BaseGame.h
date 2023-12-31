#pragma once
#include "BaseScene.h"

class BaseGame
{
	std::vector<BaseScene*> AllScenes;
	BaseScene* CurrentScene;
};
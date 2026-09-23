#pragma once
#include "Enemy.h"
class Asteroid : public Enemy
{
public:
	Asteroid(float x, float y, Game* game);
	void draw() override;
	void update() override;
};
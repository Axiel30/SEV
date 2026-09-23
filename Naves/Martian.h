#pragma once
#include "Enemy.h"
class Martian : public Enemy
{
public:
	void draw() override; // Va a sobrescribir
	void update() override;
	Martian(float x, float y, Game* game);
};


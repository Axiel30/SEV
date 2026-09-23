#pragma once
#include "Actor.h"
#include "Animation.h"
class Enemy : public Actor
{
public:
	Enemy(float x, float y, int width, int height, Game* game, string skin);
	virtual void draw() override = 0; // Va a sobrescribir
	virtual void update() = 0;
	Animation* aMoving;
	Animation* animation; // Referencia a la animación mostrada
};

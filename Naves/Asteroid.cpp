#include "Asteroid.h"
Asteroid::Asteroid(float x, float y, Game* game)
	: Enemy(x, y, 50, 51, game, "res/asteroide.png") {
	vx = 2;
}

void Asteroid::update() {
	vx = -2;
	x = x + vx;
}

void Asteroid::draw() {
	Actor::draw();
}
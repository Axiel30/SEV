#include "Bomb.h"

Bomb::Bomb(float x, float y, Game* game)
	: Actor("res/bomba.png", x, y, 64, 64, game) {
	vx = -7;
}

void Bomb::update() {
	x = x + vx;
}
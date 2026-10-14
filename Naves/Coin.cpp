#include "Coin.h"

Coin::Coin(float x, float y, Game* game)
	: Actor("res/moneda.png", x, y, 40, 40, game) {
	vx = 5;
}

void Coin::update() {
	vx = -5;
	x = x + vx;
}
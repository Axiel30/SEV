#include "Martian.h"

Martian::Martian(float x, float y, Game* game) 
	: Enemy(x, y, 36, 40, game, "res/enemigo.png") {
	vx = 1;
	aMoving = new Animation("res/enemigo_movimiento.png", width, height,
		108, 40, 6, 3, game);
	animation = aMoving;
}

void Martian::update() {
	// Actualizar la animación
	animation->update();

	vx = -1;
	x = x + vx;
}

void Martian::draw() {
	animation->draw(x, y);
}
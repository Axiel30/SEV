#include "GameLayer.h"

GameLayer::GameLayer(Game* game)
	: Layer(game) {
	//llama al constructor del padre : Layer(renderer)
	init();
}

void GameLayer::init() {
	audioBackground = Audio::createAudio("res/musica_ambiente.mp3", true);
	audioBackground->play();

	points = 0;
	textPoints = new Text("hola", WIDTH * 0.92, HEIGHT * 0.04, game);
	textPoints->content = to_string(points);

	player = new Player(50, 50, game);
	background = new Background("res/fondo.png", WIDTH * 0.5, HEIGHT * 0.5, 1, game);
	backgroundPoints = new Actor("res/icono_puntos.png",
		WIDTH * 0.85, HEIGHT * 0.05, 24, 24, game);

	// Inicializar los 3 corazones ---
	// Posicionados en la esquina superior izquierda
	for (int i = 0; i < 3; i++) {
		hearts[i] = new Actor("res/corazon.png", 30 + (i * 50), 30, 44, 36, game);
	}

	projectiles.clear(); // Vaciar por si reiniciamos el juego

	coins.clear(); // Vaciar monedas por si reiniciamos el juego
	newCoinTime = 180; // Número N de iteraciones para generar una moneda

	coins.clear(); // Vaciar monedas por si reiniciamos el juego
	newCoinTime = 180;

	bombs.clear(); // Vaciar bombas por si reiniciamos el juego
	newBombTime = 250; // Número N de iteraciones para generar una bomba

	enemies.clear(); // Vaciar por si reiniciamos el juego
	enemies.push_back(new Martian(300, 50, game));
	enemies.push_back(new Martian(300, 200, game));
	enemies.push_back(new Asteroid(300, 100, game));
}

void GameLayer::processControls() {
	// obtener controles
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		keysToControls(event);
	}
	//procesar controles
	// Disparar
	if (controlShoot) {
		Projectile* newProjectile = player->shoot();
		if (newProjectile != NULL) {
			projectiles.push_back(newProjectile);
		}

	}

	// Eje X
	if (controlMoveX > 0) {
		player->moveX(1);
	}
	else if (controlMoveX < 0) {
		player->moveX(-1);
	}
	else {
		player->moveX(0);
	}

	// Eje Y
	if (controlMoveY > 0) {
		player->moveY(1);
	}
	else if (controlMoveY < 0) {
		player->moveY(-1);
	}
	else {
		player->moveY(0);
	}

}

void GameLayer::keysToControls(SDL_Event event) {
	if (event.type == SDL_QUIT) {
		game->loopActive = false;
	}

	if (event.type == SDL_KEYDOWN) {
		int code = event.key.keysym.sym;
		// Pulsada
		switch (code) {
		case SDLK_ESCAPE:
			game->loopActive = false;
			break;
		case SDLK_1:
			game->scale();
			break;
		case SDLK_d: // derecha
			controlMoveX = 1;
			break;
		case SDLK_a: // izquierda
			controlMoveX = -1;
			break;
		case SDLK_w: // arriba
			controlMoveY = -1;
			break;
		case SDLK_s: // abajo
			controlMoveY = 1;
			break;
		case SDLK_SPACE: // dispara
			controlShoot = true;
			break;
		}

	}
	if (event.type == SDL_KEYUP) {
		int code = event.key.keysym.sym;
		// Levantada
		switch (code) {
		case SDLK_d: // derecha
			if (controlMoveX == 1) {
				controlMoveX = 0;
			}
			break;
		case SDLK_a: // izquierda
			if (controlMoveX == -1) {
				controlMoveX = 0;
			}
			break;
		case SDLK_w: // arriba
			if (controlMoveY == -1) {
				controlMoveY = 0;
			}
			break;
		case SDLK_s: // abajo
			if (controlMoveY == 1) {
				controlMoveY = 0;
			}
			break;
		case SDLK_SPACE: // dispara
			controlShoot = false;
			break;
		}

	}
}

void GameLayer::update() {
	background->update();
	// Generar enemigos
	newEnemyTime--;
	if (newEnemyTime <= 0) {
		int rX = (rand() % (600 - 500)) + 1 + 500;
		int rY = (rand() % (300 - 60)) + 1 + 60;
		enemies.push_back(new Martian(rX, rY, game));
		rX = (rand() % (600 - 500)) + 1 + 500;
		rY = (rand() % (300 - 60)) + 1 + 60;
		enemies.push_back(new Asteroid(rX, rY, game));
		newEnemyTime = 110;
	}

	// Generar moneda cada N iteraciones del juego (ej. N = 150)
	newCoinTime--;
	if (newCoinTime <= 0) {
		int rX = (rand() % (600 - 500)) + 1 + 500;
		int rY = (rand() % (300 - 60)) + 1 + 60;
		coins.push_back(new Coin(rX, rY, game));
		newCoinTime = 150;
	}

	// Generar bomba cada N iteraciones (ej. N = 250)
	newBombTime--;
	if (newBombTime <= 0) {
		int rX = (rand() % (600 - 500)) + 1 + 500;
		int rY = (rand() % (300 - 60)) + 1 + 60;
		bombs.push_back(new Bomb(rX, rY, game));
		newBombTime = 250;
	}

	player->update();
	for (auto const& enemy : enemies) {
		enemy->update();
	}
	for (auto const& coin : coins) {
		coin->update();
	}
	for (auto const& bomb : bombs) {
		bomb->update();
	}

	// Colisiones: Player - Coin (Suma 5 puntos)
	list<Coin*> deleteCoins;
	for (auto const& coin : coins) {
		if (player->isOverlap(coin)) {
			points += 5;
			textPoints->content = to_string(points);

			bool cInList = std::find(deleteCoins.begin(),
				deleteCoins.end(),
				coin) != deleteCoins.end();
			if (!cInList) {
				deleteCoins.push_back(coin);
			}
		}
	}

	for (auto const& delCoin : deleteCoins) {
		coins.remove(delCoin);
		delete delCoin;
	}
	deleteCoins.clear();
	
	// Colisiones
	for (auto const& enemy : enemies) {
		if (player->isOverlap(enemy)) {
			enemies.remove(enemy);
			player->takeDamage();
			if (!player->isAlive()) {
				init();
				return;
			}
			break;
		}
	}

	// Colisiones , Enemy - Projectile
	list<Enemy*> deleteEnemies;
	list<Projectile*> deleteProjectiles;

	// Colisiones: Player - Bomb (Elimina todos los enemigos en pantalla y suma sus puntos)
	list<Bomb*> deleteBombs;
	for (auto const& bomb : bombs) {
		if (player->isOverlap(bomb)) {
			bool bInList = std::find(deleteBombs.begin(),
				deleteBombs.end(),
				bomb) != deleteBombs.end();
			if (!bInList) {
				deleteBombs.push_back(bomb);
			}

			// Recorremos los enemigos para eliminar los que estén en pantalla
			for (auto const& enemy : enemies) {
				if (enemy->isInRender()) {
					bool eInList = std::find(deleteEnemies.begin(),
						deleteEnemies.end(),
						enemy) != deleteEnemies.end();
					if (!eInList) {
						deleteEnemies.push_back(enemy);
						points++;
					}
				}
			}
			textPoints->content = to_string(points);
		}
	}
	for (auto const& delBomb : deleteBombs) {
		bombs.remove(delBomb);
		delete delBomb;
	}
	deleteBombs.clear();

	for (auto const& projectile : projectiles) {
		if (projectile->isInRender() == false) {
			bool pInList = std::find(deleteProjectiles.begin(),
				deleteProjectiles.end(),
				projectile) != deleteProjectiles.end();
			if (!pInList) {
				deleteProjectiles.push_back(projectile);
			}
		}
	}

	for (auto const& enemy : enemies) {
		for (auto const& projectile : projectiles) {
			if (enemy->isOverlap(projectile)) {
				bool pInList = std::find(deleteProjectiles.begin(),
					deleteProjectiles.end(),
					projectile) != deleteProjectiles.end();

				if (!pInList) {
					deleteProjectiles.push_back(projectile);
				}

				bool eInList = std::find(deleteEnemies.begin(),
					deleteEnemies.end(),
					enemy) != deleteEnemies.end();

				if (!eInList) {
					deleteEnemies.push_back(enemy);
				}
				points++;
				textPoints->content = to_string(points);
			}
		}
	}

	for (auto const& delEnemy : deleteEnemies) {
		enemies.remove(delEnemy);
	}
	deleteEnemies.clear();

	for (auto const& delProjectile : deleteProjectiles) {
		projectiles.remove(delProjectile);
		delete delProjectile;
	}
	deleteProjectiles.clear();


	for (auto const& projectile : projectiles) {
		projectile->update();
	}
	
	cout << "update GameLayer" << endl;
}

void GameLayer::draw() {
	background->draw();
	for (auto const& projectile : projectiles) {
		projectile->draw();
	}
	player->draw();

	for (auto const& enemy : enemies) {
		enemy->draw();
	}
	for (auto const& coin : coins) {
		coin->draw();
	}
	for (auto const& bomb : bombs) {
		bomb->draw();
	}

	textPoints->draw();
	backgroundPoints->draw();

	for (int i = 0; i < player->lives; i++) {
		hearts[i]->draw();
	}

	SDL_RenderPresent(game->renderer); // Renderiza
}

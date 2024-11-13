#include "Scene1.h"
#include <VMath.h>
#include "Projectile.h"
#include "Enemy.h"
#include "EnemySpawner.h"

// See notes about this constructor in Scene1.h.
Scene1::Scene1(SDL_Window* sdlWindow_, GameManager* game_){
	window = sdlWindow_;
    game = game_;
	renderer = SDL_GetRenderer(window);
	xAxis = 25.0f;
	yAxis = 15.0f;
	enemySpawner = new EnemySpawner(game); // Initialize the spawner
}

Scene1::~Scene1(){
}

bool Scene1::OnCreate() {
	int w, h;
	SDL_GetWindowSize(window,&w,&h);

	Matrix4 ndc = MMath::viewportNDC(w, h);
	Matrix4 ortho = MMath::orthographic(0.0f, xAxis, 0.0f, yAxis, 0.0f, 1.0f);
	projectionMatrix = ndc * ortho;

	/// Turn on the SDL imaging subsystem
	IMG_Init(IMG_INIT_PNG);

	player = game->getPlayer();

	// Set player image to spaceship
	/*SDL_Surface* Playerimage;
	SDL_Texture* Playertexture;
	Playerimage = IMG_Load("Spaceship.png");
	Playertexture = SDL_CreateTextureFromSurface(renderer, Playerimage);
	game->getPlayer()->setImage(Playerimage);
	game->getPlayer()->setTexture(Playertexture);*/

	
	//dont know how to get screen h and w
	player->setPos(Vec3(25 / 2, 15 / 2, 0));
	
	

	enemySpawner->SpawnEnemy(Vec3(-1.0f, 7.0f, 0.0f));
	enemySpawner->SpawnEnemy(Vec3(7.0f, -1.0f, 0.0f));
	enemySpawner->SpawnEnemy(Vec3(10.0f, 16.0f, 0.0f));
	enemySpawner->SetProjectiles(&game->getShots());

	if (game == nullptr) {
		std::cerr << "Game Manager is not initialized!" << std::endl;
		return false;
	}

	if (!game->getPlayer()->OnCreate()) {
		return false;
	}
	

	return true;
}

void Scene1::OnDestroy() {

	if (game != nullptr) {
		player->OnDestroy(); // Call the player's OnDestroy method to free player resources
		delete player;
	}

	for (Projectile* projectile : shotProjectiles) {
		if (projectile != nullptr) {
			projectile->OnDestroy();
			delete projectile;
		}
	}
	shotProjectiles.clear(); // Clear the vector to avoid dangling pointers


	if (enemySpawner != nullptr) {
		enemySpawner->ClearEnemies();
		delete enemySpawner;
	}

	// Clean up SDL image subsystem if you are done using it
	IMG_Quit();

}

void Scene1::Update(const float deltaTime) {

	// Update player
	player->Update(deltaTime);

	//update shot
	/*if (game->getShots()->getActive()) {
		game->getShots()->Update(deltaTime);
	}*/
	//updtaes each projectile 
	auto& shots = game->getShots();
	for (auto it = shots.begin(); it != shots.end();) {
		Projectile* projectile = *it;
		
		projectile->Update(deltaTime);
		if (projectile->getActive() == false) {
			projectile->OnDestroy();
			delete projectile;
			it = shots.erase(it);
		}
		else {
			++it;
		}
	}

	
	enemySpawner->UpdateEnemies(deltaTime);
	
}

void Scene1::Render() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);

	// render the player
	game->RenderPlayer(0.10f);

	// Render each projectile
	auto& shotRender = game->getShots();
	for (Projectile* projectile : shotRender) {
		if (projectile->getActive()) {
			game->RenderShots(0.05f); // Render with desired scale
		}
	}

	enemySpawner->RenderEnemies(0.1f);

	SDL_RenderPresent(renderer);
	
}

void Scene1::HandleEvents(const SDL_Event& event)
{
	// send events to player as needed
	player->HandleEvents(event);
	
}

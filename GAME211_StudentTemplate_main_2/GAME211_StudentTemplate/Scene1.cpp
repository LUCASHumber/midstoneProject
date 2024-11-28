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
	player->setPos(Vec3(xAxis / 2, yAxis / 2, 0));

	SDL_Surface* background;
	background = IMG_Load("Space.png");
	backgroundTexture = SDL_CreateTextureFromSurface(game->getRenderer(), background);

	//Music Code
	//se.initMixer();
	////sound = se.loadSound("fly.wav");
	//song = se.loadMusic("Wii.mp3");
	//se.playMusic(song);
	
	auto& shots = game->getShots();
	shots.clear();
	
	enemySpawner->SetProjectiles(&shots);
	enemySpawner->SpawnEnemy(enemySpawner->GetRandomSpawnPosition());
	
	if (game == nullptr) {
		std::cerr << "Game Manager is not initialized!" << std::endl;
		return false;
	}

	if (!player->OnCreate()) {
		return false;
	}
	

	return true;
}

void Scene1::OnDestroy() {

	if (game != nullptr) {
		player->OnDestroy(); // Call the player's OnDestroy method to free player resources
		delete player;
		player = nullptr;
	}


	auto& shots = game->getShots();
	for (Projectile* projectile : shots) {

		if (projectile != nullptr) {
			projectile->OnDestroy();
			delete projectile;
		}
	}
	shots.clear(); // Ensure all projectiles are removed from the vector
	
	
	if (enemySpawner != nullptr) {
		enemySpawner->ClearEnemies();
		delete enemySpawner;
		enemySpawner = nullptr;
	}

	if (backgroundTexture != nullptr) {
		SDL_DestroyTexture(backgroundTexture); // Destroy background texture
	}


	// Clean up SDL image subsystem if you are done using it
	IMG_Quit();
	game->OnDestroy();
}

void Scene1::Update(const float deltaTime) {

	// Update player
	player->Update(deltaTime);

	 //Update each projectile
	auto& shots = game->getShots();
	for (auto it = shots.begin(); it != shots.end();)
	{
		Projectile* projectile = *it;
		projectile->Update(deltaTime);
		if (!projectile->getActive())
		{
			delete projectile;
			it = shots.erase(it);
		}
		else
		{
			++it;
		}
	}
	
	enemySpawner->UpdateEnemies(deltaTime);
	
}

void Scene1::Render() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);

	int    w, h;
	SDL_Rect square;
	SDL_QueryTexture(backgroundTexture, NULL, NULL, &w, &h);

	square.x = 0;
	square.y = 0;
	square.w = static_cast<int>(w);
	square.h = static_cast<int>(h);
	SDL_RenderCopyEx(renderer, backgroundTexture, nullptr, &square, 0.0, nullptr, SDL_FLIP_NONE);

	// render the player
	game->RenderPlayer(0.10f);

	game->RenderShots(0.05f); // Render projectiles

	enemySpawner->RenderEnemies(0.1f);

	SDL_RenderPresent(renderer);
	
}

void Scene1::HandleEvents(const SDL_Event& event)
{
	// send events to player as needed
	player->HandleEvents(event);
	
}

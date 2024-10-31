#include "Scene1.h"
#include <VMath.h>

// See notes about this constructor in Scene1.h.
Scene1::Scene1(SDL_Window* sdlWindow_, GameManager* game_){
	window = sdlWindow_;
    game = game_;
	renderer = SDL_GetRenderer(window);
	xAxis = 25.0f;
	yAxis = 15.0f;
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

	player = new PlayerBody();
	// Set player image to spaceship
	/*SDL_Surface* Playerimage;
	SDL_Texture* Playertexture;
	Playerimage = IMG_Load("Spaceship.png");
	Playertexture = SDL_CreateTextureFromSurface(renderer, Playerimage);
	game->getPlayer()->setImage(Playerimage);
	game->getPlayer()->setTexture(Playertexture);*/

	
	//dont know how to get screen h and w
	game->getPlayer()->setPos(Vec3(25/2,15/2,0));

	

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

	projectiles = new Projectile();

	if (projectiles != nullptr) {
		projectiles->OnDestroy();  // Free the resources when the scene is destroyed
		delete projectiles;
		
	}

	if (player != nullptr) {
		player->OnDestroy(); // Call the player's OnDestroy method to free player resources
		delete player;
	}

	// Clean up SDL image subsystem if you are done using it
	IMG_Quit();

}

void Scene1::Update(const float deltaTime) {

	// Update player
	game->getPlayer()->Update(deltaTime);

	//update shot
	/*if (game->getShots()->getActive()) {
		game->getShots()->Update(deltaTime);
	}*/
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
	
}

void Scene1::Render() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);

	// render the player
	game->RenderPlayer(0.10f);

	// Render each projectile
	auto& shots = game->getShots();
	for (Projectile* projectile : shots) {
		projectile->Render(0.05f); // Adjust scale as needed
	}

	SDL_RenderPresent(renderer);
	
}

void Scene1::HandleEvents(const SDL_Event& event)
{
	// send events to player as needed
	game->getPlayer()->HandleEvents(event);
	
}

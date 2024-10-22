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

	// Set player image to spaceship
	SDL_Surface* Playerimage;
	SDL_Texture* Playertexture;
	Playerimage = IMG_Load("Spaceship.png");
	Playertexture = SDL_CreateTextureFromSurface(renderer, Playerimage);
	game->getPlayer()->setImage(Playerimage);
	game->getPlayer()->setTexture(Playertexture);

	
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

	Projectile* projectiles = new Projectile();
	if (projectiles != nullptr) {
		projectiles->OnDestroy();  // Free the resources when the scene is destroyed
		delete projectiles;
		
	}

	if (game->getPlayer() != nullptr) {
		game->getPlayer()->OnDestroy(); // Call the player's OnDestroy method to free player resources
	}

	// Clean up SDL image subsystem if you are done using it
	IMG_Quit();

}

void Scene1::Update(const float deltaTime) {

	// Update player
	game->getPlayer()->Update(deltaTime);

	//update shot
	if (game->getShots()->getActive()) {
		game->getShots()->Update(deltaTime);
	}

	
}

void Scene1::Render() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);

	// render the player
	game->RenderPlayer(0.10f);

	if (game->getShots()->getActive()) {
		game->RenderShot(0.05f);  // Adjust scale if needed for projectile image
	}

	SDL_RenderPresent(renderer);
	
}

void Scene1::HandleEvents(const SDL_Event& event)
{
	// send events to player as needed
	game->getPlayer()->HandleEvents(event);
	
}

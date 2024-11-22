#include "StartScreen.h"
#include <VMath.h>

// See notes about this constructor in Scene1.h.
StartScreen::StartScreen(SDL_Window* sdlWindow_, GameManager* game_) {
	window = sdlWindow_;
	game = game_;
	renderer = SDL_GetRenderer(window);
	xAxis = 25.0f;
	yAxis = 15.0f;
	start = nullptr;

}

StartScreen::~StartScreen() {
}

bool StartScreen::OnCreate() {
	int w, h;
	SDL_GetWindowSize(window, &w, &h);

	Matrix4 ndc = MMath::viewportNDC(w, h);
	Matrix4 ortho = MMath::orthographic(0.0f, xAxis, 0.0f, yAxis, 0.0f, 1.0f);
	projectionMatrix = ndc * ortho;

	/// Turn on the SDL imaging subsystem
	IMG_Init(IMG_INIT_PNG);

	// set the background
	SDL_Surface* background;
	SDL_Texture* backgroundTexture;
	// i am pretty sure this is not the best way of doing it 
	background = IMG_Load("Space.png");
	backgroundTexture = SDL_CreateTextureFromSurface(renderer, background);
	SDL_FreeSurface(background);

	//SDL_RenderCopy(screenRenderer, backgroundTexture, nullptr, nullptr);

	// Set player image to spaceship

	SDL_Surface* image;
	SDL_Texture* texture;

	image = IMG_Load("Spaceship.png");
	texture = SDL_CreateTextureFromSurface(renderer, image);
	game->getPlayer()->setImage(image);
	game->getPlayer()->setTexture(texture);

	start = new button(game->getRenderer(), "pacman.png", Vec3(5,5,5));//inset file name of start buttion
	start->OnCreate();


	//dont know how to get screen h and w
	game->getPlayer()->setPos(Vec3(25 / 2, 15 / 2, 0));



	if (!game->getPlayer()->OnCreate()) {
		return false;
	}


	return true;
}

void StartScreen::OnDestroy() {}

void StartScreen::Update(const float deltaTime) {

	// Update player
	game->getPlayer()->Update(deltaTime);
}

void StartScreen::Render() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);

	// render the player
	game->RenderPlayer(0.10f);
	start->render(game);

	SDL_RenderPresent(renderer);

}

void StartScreen::HandleEvents(const SDL_Event& event)
{
	// send events to player as needed
	game->getPlayer()->HandleEvents(event);
	start->HandleEvents(event,game);

}

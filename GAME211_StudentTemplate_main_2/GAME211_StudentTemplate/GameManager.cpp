#include "GameManager.h"
#include "Scene1.h"
#include "StartScreen.h"
#include "Projectile.h"
#include "Enemy.h"
#include "EnemySpawner.h"

GameManager::GameManager() {
	windowPtr = nullptr;
	timer = nullptr;
	isRunning = true;
	currentScene = nullptr;
    player = nullptr;
    enemySpawner = nullptr;
}

bool GameManager::OnCreate() {
    // My display is 1920 x 1080 but the following seems to work best to fill the screen.
    //const int SCREEN_WIDTH = 1540;
    //const int SCREEN_HEIGHT = 860;

    // Use 1000x600 for less than full screen
    const int SCREEN_WIDTH = 1000;
    const int SCREEN_HEIGHT = 600;

    windowPtr = new Window(SCREEN_WIDTH, SCREEN_HEIGHT);
	if (windowPtr == nullptr) {
		OnDestroy();
		return false;
	}
	if (windowPtr->OnCreate() == false) {
		OnDestroy();
		return false;
	}

	timer = new Timer();
	if (timer == nullptr) {
		OnDestroy();
		return false;
	}

    // select scene for specific assignment

    //currentScene = new StartScreen(windowPtr->GetSDL_Window(), this);
    currentScene = new Scene1(windowPtr->GetSDL_Window(), this);
    
    // create player
    float mass = 1.0f;
    float radius = 0.5f;
    float orientation = 0.0f;
    float rotation = 0.0f;
    float angular = 0.0f;
    Vec3 position(0.5f * currentScene->getxAxis(), 0.5f * currentScene->getyAxis(), 0.0f);
    Vec3 velocity(0.0f, 0.0f, 0.0f);
    Vec3 acceleration(0.0f, 0.0f, 0.0f);

    player = new PlayerBody
    (
        position,
        velocity,
        acceleration,
        mass,
        radius,
        orientation,
        rotation,
        angular,
        this
    );
    
    Projectile* multilpleShots = new Projectile
    (
        position,
        velocity,
        acceleration,
        mass,
        radius,
        orientation,
        rotation,
        angular,
        this
    );
    shots.push_back(multilpleShots);

    enemySpawner = new EnemySpawner(this);

    if ( player->OnCreate() == false ) {
        OnDestroy();
        return false;
    }

    for (auto* shot : shots) {
        if (!shot->OnCreate()) {
            OnDestroy();
            return false;
        }
    }


    // need to create Player before validating scene
    if (!ValidateCurrentScene()) {
        OnDestroy();
        return false;
    }

    
           
	return true;
}


/// Here's the whole game loop
void GameManager::Run() {
    
	timer->Start();
    
	while (isRunning) {
        
        handleEvents();
		timer->UpdateFrameTicks();
        currentScene->Update(timer->GetDeltaTime());
        enemySpawner->UpdateEnemies(timer->GetDeltaTime());
		currentScene->Render();

       

		/// Keep the event loop running at a proper rate
		SDL_Delay(timer->GetSleepTime(60)); ///60 frames per sec
	}
}

void GameManager::handleEvents() 
{
    SDL_Event event;

    // Let's add mouse movement and position
    // https://wiki.libsdl.org/SDL_GetMouseState

    SDL_PumpEvents();  // make sure we have the latest mouse state.

    //https://www.youtube.com/watch?v=SYrRMr4BaD4&list=PLM7LHX-clszBIGsrh7_3B2Pi74AhMpKhj&index=3

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            isRunning = false;
        }
        else if (event.type == SDL_KEYDOWN)
        {
            switch (event.key.keysym.scancode)
            {
            case SDL_SCANCODE_ESCAPE:
                isRunning = false;
                break;
            case SDL_SCANCODE_Q:
                isRunning = false;
                break;
            case SDL_SCANCODE_DELETE:
                isRunning = false;
                break;
            case SDL_SCANCODE_1:
                LoadScene(1);
                break;
            case SDL_SCANCODE_2:
                LoadScene(2);
                break;
            default:
                break;
            }
        }
        currentScene->HandleEvents(event);
    }
}

GameManager::~GameManager() {
    OnDestroy();
}

void GameManager::OnDestroy(){
	if (windowPtr) delete windowPtr;
	if (timer) delete timer;
	if (currentScene) delete currentScene;
    if (player) delete player;
    for (auto* shot : shots) {
        if (shot) delete shot;
    }
    if (enemySpawner) delete enemySpawner;
    CleanupProjectiles();
}

// This might be unfamiliar
float GameManager::getSceneHeight() { return currentScene->getyAxis(); }

// This might be unfamiliar
float GameManager::getSceneWidth() { return currentScene->getxAxis(); }

// This might be unfamiliar
Matrix4 GameManager::getProjectionMatrix()
{ return currentScene->getProjectionMatrix(); }

// This might be unfamiliar
SDL_Renderer* GameManager::getRenderer()
{
    // [TODO] might be missing some SDL error checking
    SDL_Window* window = currentScene->getWindow();
    SDL_Renderer* renderer = SDL_GetRenderer(window);
    return renderer;
}

void GameManager::ChangeScene(int scene)
{
    LoadScene(scene);
}

// This might be unfamiliar
void GameManager::RenderPlayer(float scale)
{
    player->Render(scale);
}

void GameManager::RenderShots(float scale)
{
    
    for (auto* shot : shots) {
        if (shot->getActive()) {
            shot->Render(scale);  // Render each active projectile
        }
    }
}

void GameManager::RenderEnemies(float scale)
{
    if (enemySpawner) {
        enemySpawner->RenderEnemies(scale);
    }
}

void GameManager::CleanupProjectiles()
{
    for (Projectile* projectile : shots) {
        if (projectile) {
            projectile->OnDestroy();
            delete projectile;
        }
    }
    shots.clear();

}



void GameManager::LoadScene( int i )
{
    // cleanup of current scene before loading another one
    currentScene->OnDestroy();
    delete currentScene;

    switch ( i )
    {
        case 1:
            currentScene = new Scene1( windowPtr->GetSDL_Window(), this);
            break;
        case 2:
            currentScene = new StartScreen(windowPtr->GetSDL_Window(), this);
            break;
        default:
            currentScene = new Scene1( windowPtr->GetSDL_Window(), this );
            break;
    }

    // using ValidateCurrentScene() to safely run OnCreate
    if (!ValidateCurrentScene())
    {
        isRunning = false;
    }
}

bool GameManager::ValidateCurrentScene()
{
    if (currentScene == nullptr) {
        return false;
    }
    if (currentScene->OnCreate() == false) {
        return false;
    }
    return true;
}

#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H
#include <SDL.h>
#include <iostream>
#include <vector>
#include "Window.h"
#include "Timer.h"
#include "Scene.h"
#include "PlayerBody.h"



class Projectile;
class Enemy;
class enemySpawner;

using namespace std;


class GameManager {
private:
	/// These are called "forward declarations" The idea is that a pointer is 
	/// really just an unsigned int, so to create a pointer to it you really only
	/// need to tell the compiler that there is a class called Window and I need
	/// a pointer to it, thus the "class Window*" means trust me, there is a class 
	/// called Window, I just need a pointer to it.

	/// If that was too much for your brain, just #include "Window.h" and declare
	/// Window *ptr and don't use the word "class"  This is a second semester C++
	/// topic anyway
	class Window *windowPtr;
	class Timer *timer;
	bool isRunning;
	class Scene *currentScene;


	// This might be unfamiliar
    class PlayerBody *player;
	vector<Projectile*> shots;
	class EnemySpawner* enemySpawner;

	void handleEvents();
	void LoadScene(int i);
	bool ValidateCurrentScene();


public:
	GameManager();
	~GameManager();
	bool OnCreate();
	void OnDestroy();


	// These might be unfamiliar
	float getSceneHeight();
	float getSceneWidth();
	Matrix4 getProjectionMatrix();
    PlayerBody* getPlayer(){ return player; }
	vector<Projectile*>& getShots() { return shots; }
	void RenderPlayer(float scale = 1.0f);
	void RenderShots(float scale = 1.0f);
	void RenderEnemies(float scale = 1.0f);
	void CleanupProjectiles();
	void RestartGame();
	SDL_Renderer* getRenderer();
	void ChangeScene(int scene);

	void Run();
    
};
#endif



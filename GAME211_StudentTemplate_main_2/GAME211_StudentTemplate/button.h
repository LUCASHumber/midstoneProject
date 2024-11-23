#pragma once
#include <SDL.h>
#include <string>
#include "Vector.h"
#include "GameManager.h"


using namespace MATH;
class button 
{
private:
	SDL_Texture* texture;
	MATH::Vec3 pos;
	SDL_Renderer* ourRender;
	std::string filePath;
	float scale = 1.3;
	SDL_Point mPosition;

public:
	button(SDL_Renderer* _ourRender, std::string _filepath, Vec3 _pos);
	~button();
	void OnClick();
	bool OnCreate();
	bool OnDestroy();
	void HandleEvents(const SDL_Event& event, GameManager* game);
	void Update(float deltatime);
	void render(GameManager* game );





};
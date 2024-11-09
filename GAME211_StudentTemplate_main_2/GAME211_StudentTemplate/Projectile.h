#pragma once

#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <stdio.h>
#include <vector>
#include "Body.h"
#include "GameManager.h"
#include "PlayerBody.h"

class Projectile : public Body
{

    bool isActive = false;

protected:
    class GameManager* game;

public:
	/*Projectile() : Body{}
	{
        game = nullptr;
	}*/

    Projectile(Vec3 pos_, Vec3 vel_, Vec3 accel_,
        float mass_,
        float radius_,
        float orientation_,
        float rotation_,
        float angular_,
        GameManager* game_
    ) : Body{
          pos_
        , vel_
        , accel_
        , mass_
        , radius_
        , orientation_
        , rotation_
        , angular_
    } 
        , game{ game_ }
    {}

	bool OnCreate();
	void Render(float scale = 1.0f);
	void Update(float deltaTime);
    void OnDestroy();
	void setTexture(SDL_Texture* texture_) { texture = texture_; }
    void setActive(bool active) { isActive = active; }
    bool getActive() const { return isActive; }
    
    Vec3 getPos() const { return pos; }
};

#endif
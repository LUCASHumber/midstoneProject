#pragma once

#ifndef ENEMY_H
#define ENEMY_H

#include <stdio.h>
#include "Body.h"
#include "GameManager.h"
#include "PlayerBody.h"
#include "Projectile.h"

class Enemy : public Body
{

protected:
	class GameManager* game;

public:
    Enemy() : Body{}
    {
        game = nullptr;
    }

    Enemy(Vec3 pos_, Vec3 vel_, Vec3 accel_,
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
    void MoveTowardsPlayer(const Vec3& playerPos, float deltaTime);
    bool IsHitByProjectile(const Projectile& projectile, float collisionRadius);
	void Update(float deltaTime);
	void OnDestroy();
	void setTexture(SDL_Texture* texture_) { texture = texture_; }
};

#endif
//
//  PlayerBody.h
//  DemoAI
//
//  Created by Gail Harris on 2021-Dec-23.
//

#ifndef PLAYERBODY_H
#define PLAYERBODY_H

#include <stdio.h>
#include "Body.h"
#include "GameManager.h"

class Enemy;

using namespace std;

class PlayerBody : public Body
{
   
    bool isRotating = false;

    float boostSpeed = 100.0f;
    bool isBoosting = false;

    bool isShooting = false;

   

   
protected:
    class GameManager* game;

public:
    PlayerBody() : Body{}
    {
        game = nullptr;
    }

    // Note the last parameter in this constructor!
    // Look in GameManager.cpp to see how this is called.
    // Look in .cpp file, at Render(), to see why game_ is a parameter.
    PlayerBody(
        Vec3 pos_, Vec3 vel_, Vec3 accel_,
        float mass_,
        float radius_,
        float orientation_,
        float rotation_,
        float angular_,
        GameManager *game_
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
    float playerAngle = 0.0f;
    float boost = 10.0f;
    float impulse = 50.0f;

    float playerDirection;//this is here to make sure that projectile class gets access to playerDirection in order the rotate
    float radiusAngle = 0.0f;
   
    
    // use the base class versions of getters

    bool OnCreate();
    void Render( float scale = 1.0f );
    void HandleEvents( const SDL_Event& event );
    void shipMove(float deltaTime);
    void ShootProjectile(float deltaTime);
    bool IsHitByEnemy(const Enemy& enemy, float collisionRadius);
    void Update( float deltaTime );
    void OnDestroy();
    void setTexture( SDL_Texture* texture_ ) { texture = texture_; }
    
};

#endif /* PLAYERBODY_H */

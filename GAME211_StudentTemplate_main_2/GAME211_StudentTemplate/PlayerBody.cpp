//
//  PlayerBody.cpp
//  DemoAI
//
//  Created by Gail Harris on 2021-Dec-23.
//

#include "PlayerBody.h"
#include "Projectile.h"
#include "Enemy.h"



bool PlayerBody::OnCreate()
{
    image = IMG_Load( "Spaceship.png" );
    if (!image) {
        cerr << "Failed to load image: " << SDL_GetError() << endl;
        return false;
    }

    SDL_Renderer *renderer = game->getRenderer();
    texture = SDL_CreateTextureFromSurface( renderer, image );
    if (!texture) {
        cerr << "Failed to create texture: " << SDL_GetError() << endl;
        SDL_FreeSurface(image);
        image = nullptr;
        return false;
    }

    return true;
}

void PlayerBody::Render( float scale )
{
    // This is why we need game in the constructor, to get the renderer, etc.
    SDL_Renderer *renderer = game->getRenderer();
    Matrix4 projectionMatrix = game->getProjectionMatrix();

    // square represents the position and dimensions for where to draw the image
    SDL_Rect square;
    Vec3 screenCoords;
    float    w, h;

    // convert the position from game coords to screen coords.
    screenCoords = projectionMatrix * pos;

    // Scale the image, in case the .png file is too big or small
    w = image->w * scale;
    h = image->h * scale;

    // The square's x and y values represent the top left corner of 
    // where SDL will draw the .png image.
    // The 0.5f * w/h offset is to place the .png so that pos represents the center
    // (Note the y axis for screen coords points downward, hence subtraction!!!!)
    square.x = static_cast<int>(screenCoords.x - 0.5f * w);
    square.y = static_cast<int>(screenCoords.y - 0.5f * h);
    square.w = static_cast<int>(w);
    square.h = static_cast<int>(h);

    // Convert character orientation from radians to degrees.
    // did not work, caused player orientation to not rotate
    //float orientationDegrees = orientation * 180.0f / M_PI ;
   
    
    //made new variable playerDirection to rotate player image(replaces orientationDegree) 
    //render player ship at + 90 radians to make image match orientation
    //does not convert radians to degrees
    SDL_RenderCopyEx( renderer, texture, nullptr, &square,
        playerDirection + 90, nullptr, SDL_FLIP_NONE);

}

void PlayerBody::HandleEvents( const SDL_Event& event ){
    //When A or D is pressed, spaceship moves 90 degrees left (when A is pressed) or right (when D is pressed)
     //need to figure out rotation

    //User presses A or D
    if (event.type == SDL_KEYDOWN && event.key.repeat == 0){
        switch (event.key.keysym.scancode) {
        case SDL_SCANCODE_A: //will rotate 90 degrees left
            playerAngle -= 90.0f;
            break;
        case SDL_SCANCODE_D: //will rotate 90 degrees right
            playerAngle += 90.0f;
            break;
        case SDL_SCANCODE_W:
            isBoosting = true;
            break;
        case SDL_SCANCODE_SPACE:
            isShooting = true;
            break;
        }
    }
    //User release A or D
    if (event.type == SDL_KEYUP && event.key.repeat == 0) {
        switch (event.key.keysym.scancode) {
        case SDL_SCANCODE_A: //stops rotating after rotating 90 degrees left
            playerAngle = 0.0f;
            break;
        case SDL_SCANCODE_D: //stops rotating after rotating 90 degrees right
            playerAngle = 0.0f;
            break;
        case SDL_SCANCODE_W:
            isBoosting = false;
            break;
        case SDL_SCANCODE_SPACE:
            isShooting = false;
            break;
        }
    }

}

void PlayerBody::shipMove(float deltaTime) {
   
    //have player input change direction
    playerDirection += playerAngle * deltaTime;
    playerDirection = fmod(playerDirection, 360.0f);

    //make player move in direction its facing
    radiusAngle = -playerDirection * M_PI / 180.0F;
    if (isBoosting) {

        boost = 10.0f;
        vel.x = cos(radiusAngle) * boost * deltaTime;
        vel.y = sin(radiusAngle) * boost * deltaTime;
    }
    float dampeningForce = 0.98f;

    vel *= dampeningForce;
  
    //update pos with vel
    pos.x += vel.x * deltaTime;
    pos.y += vel.y * deltaTime;

    if (pos.x < 0) {
        pos.x = 0; // Reset to left boundary
    }
    else if (pos.x > 25.0f) {
        pos.x = 25.0f; // Reset to right boundary
    }

    if (pos.y < 0) {
        pos.y = 0; // Reset to top boundary
    }
    else if (pos.y > 15.0f) {
        pos.y = 15.0f; // Reset to bottom boundary
    }

    if (isShooting) {
        ShootProjectile(deltaTime);
        isShooting = false; // Ensure we only shoot once per press
    }

}

void PlayerBody::ShootProjectile(float deltaTime)
{
    Projectile* newProjectiles = new Projectile(pos, vel, accel, 1.0f, 0.5f, 0.0f, 0.0f, 0.0f, game); 
   

    if (!newProjectiles->OnCreate()) {
        std::cerr << "Projectile could not be created!" << std::endl;
        delete newProjectiles;
        return;
    }

    float shotSpeed = 10.0f; // Set the speed of the projectile
    float angleInRadians = -playerDirection * M_PI / 180.0f;

    // Set initial position and velocity of the projectile based on player direction

    Vec3 projectileVelocity;
    projectileVelocity.x = cos(angleInRadians) * shotSpeed;
    projectileVelocity.y = sin(angleInRadians) * shotSpeed;

    newProjectiles->setPos(pos); // Position same as the ship
    newProjectiles->setVel(projectileVelocity); // Apply velocity
    newProjectiles->setActive(true);

    if (newProjectiles->OnCreate()) {
        cout << "shots created" << endl;
    }

    game->getShots().push_back(newProjectiles);
    

    //make projectile face same direction as player
    radiusAngle = playerDirection * M_PI / 180.0F;
    if (isShooting) {

        //makes player go in oppesite direction when shooting
        impulse = 60.0f;
        vel.x = cos(radiusAngle) * -impulse * deltaTime;
        vel.y = sin(radiusAngle) * impulse * deltaTime;
    }
}

bool PlayerBody::IsHitByEnemy(const Enemy& enemy, float collisionRadius) 
{
    Vec3 distance = enemy.getPos() - pos;
    float distanceSquared = distance.x * distance.x + distance.y * distance.y;
    return distanceSquared <= collisionRadius * collisionRadius;
}



void PlayerBody::Update( float deltaTime )
{
    // Update position, call Update from base class
    // Note that would update velocity too, and rotation motion

    //calls shipMove
    shipMove(deltaTime);
    

    //prints playerAngle playerDirection and x y for debugging
    /*cout << playerAngle << endl;
    cout << playerDirection << endl;
    cout << pos.x << " " << pos.y << endl;*/

    Body::Update(deltaTime);

}

void PlayerBody::OnDestroy()
{
    if (image) {
        SDL_FreeSurface(image);
        image = nullptr; 
    }

    
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr; 
    }
}


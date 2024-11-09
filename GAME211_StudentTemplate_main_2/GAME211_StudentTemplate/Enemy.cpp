#include "Enemy.h"

bool Enemy::OnCreate()
{
    image = IMG_Load("Blinky.png");
    if (image == nullptr) {
        std::cerr << "Can't open the image: " << IMG_GetError() << std::endl;
        return false;
    }

    SDL_Renderer* renderer = game->getRenderer();
    texture = SDL_CreateTextureFromSurface(renderer, image);
    if (texture == nullptr) {
        std::cerr << "Can't create texture from surface: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(image);
        return false;
    }

    return true; // Return true if everything succeeds
}

void Enemy::Render(float scale)
{
    // This is why we need game in the constructor, to get the renderer, etc.
    SDL_Renderer* renderer = game->getRenderer();
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
    float orientationDegrees = orientation * 180.0f / M_PI ;


    //made new variable playerDirection to rotate player image(replaces orientationDegree) 
    //render player ship at + 90 radians to make image match orientation
    //does not convert radians to degrees
    SDL_RenderCopyEx(renderer, texture, nullptr, &square,
        orientationDegrees, nullptr, SDL_FLIP_NONE);

}

void Enemy::MoveTowardsPlayer(const Vec3& playerPos, float deltaTime)
{
    Vec3 direction = playerPos - pos;
    
    float distance = sqrt(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);
    if (distance > 0.0f) {  // Avoid division by zero
        direction /= distance; // Normalize the direction

        float speed = 1.0f; // You can adjust this value as needed
        vel = direction * speed;
        pos += vel * deltaTime; // Update position based on velocity and time
    }

}

bool Enemy::IsHitByProjectile(const Projectile& projectile, float collisionRadius)
{
    Vec3 diff = projectile.getPos() - pos;
    float distanceSquared = diff.x * diff.x + diff.y * diff.y;

    // Check if the distance between enemy and projectile is within the collision radius
    return distanceSquared <= collisionRadius * collisionRadius;
}

void Enemy::Update(float deltaTime)
{
    if (game != nullptr) {
        Vec3 playerPos = game->getPlayer()->getPos();
        MoveTowardsPlayer(playerPos, deltaTime);
    }

}

void Enemy::OnDestroy()
{
    SDL_FreeSurface(image);
    SDL_DestroyTexture(texture);
}

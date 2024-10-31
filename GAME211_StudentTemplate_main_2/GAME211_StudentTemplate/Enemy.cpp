#include "Enemy.h"

bool Enemy::OnCreate()
{
	image = IMG_Load("Blinky.png");
	SDL_Renderer* renderer = game->getRenderer();
	texture = SDL_CreateTextureFromSurface(renderer, image);
	if (image == nullptr) {
		std::cerr << "Can't open the image" << std::endl;
		return false;
	}

	return false;
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

void Enemy::Update(float deltaTime)
{
}

void Enemy::OnDestroy()
{
    SDL_FreeSurface(image);
    SDL_DestroyTexture(texture);
}

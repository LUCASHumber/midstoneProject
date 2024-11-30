#include "Projectile.h"

bool Projectile::OnCreate()
{
	image = IMG_Load( "Projectile_Basic.png" );
	SDL_Renderer* renderer = game->getRenderer();
	texture = SDL_CreateTextureFromSurface(renderer, image);
	if (texture == nullptr) {
		std::cerr << "Can't open the image" << std::endl;
		return false;
    } else{
        cout << "projectile shot" << endl;
    }

    se.loadSound("Shoot.wav");
    se.playSound(sound);
   
	return true;
}

void Projectile::Render(float scale)
{
    if (!isActive) {
        return;
    }

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

    float orientationDegrees = orientation * 180.0f / M_PI;

    SDL_RenderCopyEx(renderer, texture, nullptr, &square,
       orientationDegrees, nullptr, SDL_FLIP_NONE);
    
}

void Projectile::Update(float deltaTime)
{
    pos += vel * deltaTime;
   
    if (pos.x < 0 || pos.x > 25.0f || pos.y < 0 || pos.y > 15.0f) {
        OnDestroy();  // Call the destroy method when the projectile leaves the screen
      
    }

  /*  cout << pos.x << " " << pos.y << endl;*/

}

void Projectile::OnDestroy()
{
   
    isActive = false;
    SDL_FreeSurface(image);
    SDL_DestroyTexture(texture);

  cout << "Projectile destroyed!" << endl;
   
   
}

//
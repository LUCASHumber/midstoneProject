#include "button.h"
#include <SDL_image.h>
#include "Matrix.h"

button::~button()
{
}

button::button(SDL_Renderer* _ourRender, std::string _filepath, Vec3 _pos)
{
	ourRender = _ourRender;
	filePath = _filepath;
	pos = _pos;
}

void button::OnClick()
{
}

bool button::OnCreate()
{
	SDL_Surface* surface;
	surface = IMG_Load(filePath.c_str());
	texture = SDL_CreateTextureFromSurface(ourRender, surface);
	SDL_FreeSurface(surface);
	return true;
}

bool button::OnDestroy()
{
	//look up for SDL destroy texture 
	return true;
}

void button::HandleEvents(const SDL_Event& event, GameManager* game)
{
        /*if (event.type == SDL_KEYDOWN)
        {
            switch (event.key.keysym.scancode)
            {
            case SDL_SCANCODE_X:
                game->ChangeScene(1);
                break;
            
            default:
                break;
            }
        }*/

        //If mouse event happened
    if (event.type == SDL_MOUSEBUTTONDOWN )
    {
        Vec3 screenCoords;
        Matrix4 projectionMatrix = game->getProjectionMatrix();
        screenCoords = projectionMatrix * pos;
        

        //Get mouse position
        int x, y;
        SDL_GetMouseState(&x, &y);

        int w;
        int h;
        SDL_QueryTexture(texture, NULL, NULL, &w, &h); 
        w *= scale;
        h *= scale;

        //Check if mouse is in button
        bool inside = true;

        //Mouse is left of the button
        if (x < screenCoords.x)
        {
            inside = false;
        }
        //Mouse is right of the button
        else if (x > screenCoords.x + w)
        {
            inside = false;
        }
        //Mouse above the button
        else if (y < screenCoords.y)
        {
            inside = false;
        }
        //Mouse below the button
        else if (y > screenCoords.y + h)
        {
            inside = false;
        }

        if (inside == true)
        {
            game->ChangeScene(1);
        }
    }

 
}

void button::Update(float deltatime)
{
}

void button::render(GameManager* game)
{
    SDL_Renderer* renderer = game->getRenderer();
    Matrix4 projectionMatrix = game->getProjectionMatrix();

    // square represents the position and dimensions for where to draw the image
    SDL_Rect square;
    Vec3 screenCoords;
    int    w, h;

    // convert the position from game coords to screen coords.
    screenCoords = projectionMatrix * pos;
    
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    // Scale the image, in case the .png file is too big or small
    w *= scale;
    h *= scale;


    // The square's x and y values represent the top left corner of 
    // where SDL will draw the .png image.
    // The 0.5f * w/h offset is to place the .png so that pos represents the center
    // (Note the y axis for screen coords points downward, hence subtraction!!!!)
    square.x = static_cast<int>(screenCoords.x);
    square.y = static_cast<int>(screenCoords.y);
    square.w = static_cast<int>(w);
    square.h = static_cast<int>(h);

    // Convert character orientation from radians to degrees.
    // did not work, caused player orientation to not rotate
    //float orientationDegrees = orientation * 180.0f / M_PI ;


    //made new variable playerDirection to rotate player image(replaces orientationDegree) 
    //render player ship at + 90 radians to make image match orientation
    //does not convert radians to degrees
    SDL_RenderCopyEx(renderer, texture, nullptr, &square, 0.0, nullptr, SDL_FLIP_NONE);

}

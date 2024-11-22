#ifndef MUSIC_H
#define MUSIC_H
#include <SDL_mixer.h>
#include <vector>
#include <iostream>
#include <SDL.h>

using namespace std;

class Music{
private:
	std::vector<Mix_Chunk*> sounds;
	std::vector<Mix_Music*> music;
	
public:

	Music();
	int loadMusic(const char* filename);
	int loadSound(const char* filename);
	int playMusic(int m);
	int playSound(int s);
	int initMixer();
	int quitMixer();
};

#endif
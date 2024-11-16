#include "Music.h"

Music::Music(){}

int Music::loadMusic(const char* filename) //load music file
{
	Mix_Music* m = NULL;
	m = Mix_LoadMUS(filename);
	if (m == NULL) {
		std::cout << "Failed to load music. SDL_Mixer error: %s\n", Mix_GetError();
		return -1;
	}
	music.push_back(m);
	return music.size() - 1;
}

int Music::loadSound(const char* filename) //load sound file
{
	Mix_Chunk* s = NULL;
	s = Mix_LoadWAV(filename);
	if (s == NULL) {
		std::cout << "Failed to load music. SDL_Mixer error: %s\n", Mix_GetError();
		return -1;
	}
	sounds.push_back(s);
	return sounds.size() - 1;
}

int Music::initMixer() //initialize mixer
{
	Mix_Init(MIX_INIT_MP3);
	SDL_Init(SDL_INIT_AUDIO);
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		std::cout << "SDL_Mixer couldn't init. Err: %s\n", Mix_GetError();
		return -1;
	}

	return 0;
}

int Music::playMusic(int m) //play music
{
	if (Mix_PlayingMusic() == 0) {
		//Mix_Volume(1, volume);
		Mix_PlayMusic(music[m], -1);
	}
	return 0;
}

int Music::playSound(int s) //play sound
{
	//Mix_Volume(1, volume);
	Mix_PlayChannel(-1, sounds[s], 0);

	return 0;
}

int Music::quitMixer() //quit mixer
{
	for(int s = 0; s<sounds.size(); s++){
		Mix_FreeChunk(sounds[s]);
		sounds[s] = NULL;
	}
	for(int m = 0; m<music.size(); m++){
		Mix_FreeMusic(music[m]);
		music[m] = NULL;
	}
	Mix_Quit();
	return 0;
}
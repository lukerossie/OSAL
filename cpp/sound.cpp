#ifdef __APPLE__
#include <SDL2/SDL.h>
#include <SDL2_mixer/SDL_mixer.h>
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#endif

#include "../hpp/system.hpp"

namespace
{
	int soundCounter=0;	
}

void init_sound()
{
	int audioRate = 22050;
	Uint16 audioFormat = AUDIO_S16SYS;
	int audioChannels = 2;
	int audioBuffers = 4096;
	
	if( Mix_OpenAudio(audioRate, audioFormat, audioChannels, audioBuffers) != 0 ) 
	{
		print("Unable to initialize audio.\n");
		exit(1);
	}
}

struct sound
{
	bool loop;
	Mix_Chunk *m;
	int channel;
};
sound *ctor_sound( char *sound_file_path, bool loop )
{	
	sound *s=(sound *)amem(sizeof(sound));
	s->m=Mix_LoadWAV(sound_file_path);
	s->loop=loop;
	s->channel=soundCounter++;
}
void dtor_sound(sound *s)
{
	Mix_FreeChunk(s->m);
	fmem(s);
}
void play_sound(sound *s);
{
	Mix_PlayChannel( s->channel, s->m, 0 );
}
void pause_sound(sound *s);
{
	Mix_Pause( s->channel );
}
void resume_sound(sound *s);
{
	Mix_Resume( s->channel );
}

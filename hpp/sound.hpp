#ifndef SOUND_HPP
#define SOUND_HPP

void init_sound();

struct sound;
sound *ctor_sound(char *sound_file_path,bool loop=false);
void dtor_sound(sound *s);
void play_sound(sound *s);
void pause_sound(sound *s);
void resume_sound(sound *s);

#endif
#ifndef SOUND_H
#define SOUND_H

void init_sound();

struct sound;
sound *ctor_sound(char *sound_file_path,BOOL2 loop=false);
void dtor_sound(sound *s);
void play_sound(sound *s);
void pause_sound(sound *s);
void resume_sound(sound *s);

#endif
#ifndef AUDIO_H
#define AUDIO_H

#include <AL/al.h>
#include <AL/alc.h>

#ifdef __cplusplus
extern "C" {
#endif

// Initalize OpenAL system, has to be called before doing any audio work
void audio_init();

// Shuts down the AL subsystem and frees memory
void audio_shutdown();

// Loads a WAV file from the provided `*filename`
ALuint audio_load_wav(const char *filename);

// Loads an OGG file from provided `*filename`
ALuint audio_load_ogg(const char *filename);

// Loads audio from `*filename` - only WAVs and OGGs are supported
void audio_load(const char *filename);

#ifdef __cplusplus
}
#endif

#endif //audio.h
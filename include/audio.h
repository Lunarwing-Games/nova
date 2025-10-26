#ifndef AUDIO_H
#define AUDIO_H

#include <AL/al.h>
#include <AL/alc.h>

#ifdef __cplusplus
extern "C" {
#endif

void audio_init();
void audio_shutdown();
ALuint audio_load_wav(const char *filename);
ALuint audio_load_ogg(const char *filename);
void audio_load(const char *filename);

#ifdef __cplusplus
}
#endif

#endif //audio.h
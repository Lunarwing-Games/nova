#ifndef AUDIO_H
#define AUDIO_H

#include <AL/al.h>
#include <AL/alc.h>

#ifdef __cplusplus
extern "C" {
#endif

void audio_init();
void audio_shutdown();
ALuint audio_load_wav(const char *fn);
void audio_play(ALuint buffer);

#ifdef __cplusplus
}
#endif

#endif //audio.h
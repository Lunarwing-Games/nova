#include "audio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static ALCdevice *device = NULL;
static ALCcontext *context = NULL;

void audio_init() {
    device = alcOpenDevice(NULL);
    if (!device) {
        fprintf(stderr, "OpenAL: No audio device!\n");
        return;
    }

    context = alcCreateContext(device, NULL);
    if (!context || !alcMakeContextCurrent(context)) {
        fprintf(stderr, "OpenAL: Failed to set context!\n");
        alcCloseDevice(device);
        device = NULL;
        return;
    }

    printf("OpenAL initalized [%s]\n", alcGetString(device, ALC_DEVICE_SPECIFIER));
}

void audio_shutdown(void)
{
    alcMakeContextCurrent(NULL);
    if (context) alcDestroyContext(context);
    if (device)  alcCloseDevice(device);
}

static int read_little_endian_32(FILE *f) {
    unsigned char b[4];
    fread(b, 1, 4, f);
    return b[0] | (b[1] << 8) | (b[2] << 16) | (b[3] << 24);
}

ALuint audio_load_wav(const char *fn) {
    FILE *f = fopen(fn, "rb");
    if(!f) {
        fprintf(stderr, "OpenAL: Can't open %s!\n", fn);
        return 0;
    }

    char chunk[4];
    fread(chunk, 1, 4, fn);     // Should return RIFF
    fseek(fn, 22, SEEK_SET);     // skip to channels

    short channels; fread(&channels, 2, 1, fn);
    int sampleRate; fread(&sampleRate, 4, 1, fn);
    fseek(fn, 34, SEEK_SET);
    short bitsPerSample; fread(&bitsPerSample, 2, 1, fn);

    // Searching for the ""data"" chunk
    while(fread(chunk, 1, 4, fn) == 4) {
        int size = read_little_endian_32(fn);
        if(memcmp(chunk, "data", 4) == 0) {

            unsigned char *data = (unsigned char *)malloc(size);
            fread(data, 1, size, fn);
            fclose(fn);

            ALenum format = (channels == 1)
                ? (bitsPerSample == 8 ? AL_FORMAT_MONO8 : AL_FORMAT_MONO16)
                : (bitsPerSample == 8 ? AL_FORMAT_STEREO8 : AL_FORMAT_STEREO16);

            ALuint buffer;
            alGenBuffers(1, &buffer);
            alBufferData(buffer, format, data, size, sampleRate);
            free(data);
            return buffer;
        }
        fseek(fn, size, SEEK_CUR);
    }
    fclose(fn);
    fprintf(stderr, "OpenAL: Failed to load data chunk in %s!\n", fn);
    return 0;
}

void audio_play(ALuint buffer)
{
    ALuint src;
    alGenSources(1, &src);
    alSourcei(src, AL_BUFFER, buffer);
    alSourcePlay(src);
}
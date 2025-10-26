#include "audio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STB_VORBIS_HEADER_ONLY
#include "stb_vorbis.c"

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

ALuint audio_load_wav(const char *filename) {
    FILE *f = fopen(filename, "rb");
    if(!f) {
        fprintf(stderr, "OpenAL: Can't open %s!\n", filename);
        return 0;
    }

    char chunk[4];
    fread(chunk, 1, 4, f);      // Should return RIFF
    fseek(f, 22, SEEK_SET);     // skip to channels

    short channels; 
    fread(&channels, 2, 1, f);

    int sampleRate;
    fread(&sampleRate, 4, 1, f);

    fseek(f, 34, SEEK_SET);
    short bitsPerSample; fread(&bitsPerSample, 2, 1, f);

    // Searching for the ""data"" chunk
    while(fread(chunk, 1, 4, f) == 4) {
        int size = read_little_endian_32(f);
        if(memcmp(chunk, "data", 4) == 0) {
            unsigned char *data = (unsigned char *)malloc(size);
            if(!data) {
                fprintf(stderr, "OpenAL: Out of memory loading %s!\n", filename);
                fclose(f);
                return 0;
            }
            fread(data, 1, size, f);
            fclose(f);

            ALenum format = (channels == 1)
                ? (bitsPerSample == 8 ? AL_FORMAT_MONO8 : AL_FORMAT_MONO16)
                : (bitsPerSample == 8 ? AL_FORMAT_STEREO8 : AL_FORMAT_STEREO16);

            ALuint buffer;
            alGenBuffers(1, &buffer);
            alBufferData(buffer, format, data, size, sampleRate);
            free(data);
            return buffer;
        }
        fseek(f, size, SEEK_CUR);
    }
    fclose(f);
    fprintf(stderr, "OpenAL: Failed to load data chunk in %s!\n", filename);
    return 0;
}

ALuint audio_load_ogg(const char *filename) {
    int channels, sample_rate;
    short *output = NULL;
    int samples = stb_vorbis_decode_filename(filename, &channels, &sample_rate, &output);

    if(samples < 0) {
        fprintf(stderr, "OpenAL: Failed to decode OGG %s!\n", filename);
        return 0;
    }

    ALenum format = (channels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
    ALuint buffer;
    alGenBuffers(1, &buffer);
    alBufferData(buffer, format, output, samples * channels * sizeof(short), sample_rate);
    free(output);

    printf("Loaded OGG %s (%d Hz, %d ch)\n", filename, sample_rate, channels);
    return buffer;
}

void audio_load(const char *filename) {

}
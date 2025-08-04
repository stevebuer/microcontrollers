/*
 * Auto-generated with MS 360 Co-Pilot
 *
 * prompt: write a c program to play a sine wave through alsa on linux
 */

/* modified 
 *
 * SAB 8.2.2025
 *
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <alsa/asoundlib.h>

#define SAMPLE_RATE 44100
#define FREQUENCY 750.0
#define DURATION 1
#define AMPLITUDE 32000
#define CHANNELS 1

#define TESTPROG

#define DIT 0.050
#define DAH 0.150

int beep(float duration) 
{
    snd_pcm_t *pcm_handle;
    snd_pcm_hw_params_t *params;
    snd_pcm_uframes_t frames;
    int rc;
    int dir;
    unsigned int rate = SAMPLE_RATE;
    int size;
    short *buffer;
    int i;

    rc = snd_pcm_open(&pcm_handle, "default", SND_PCM_STREAM_PLAYBACK, 0);
    if (rc < 0) {
        fprintf(stderr, "Unable to open PCM device: %s\n", snd_strerror(rc));
        exit(1);
    }

    snd_pcm_hw_params_alloca(&params);
    snd_pcm_hw_params_any(pcm_handle, params);
    snd_pcm_hw_params_set_access(pcm_handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);
    snd_pcm_hw_params_set_format(pcm_handle, params, SND_PCM_FORMAT_S16_LE);
    snd_pcm_hw_params_set_channels(pcm_handle, params, CHANNELS);
    snd_pcm_hw_params_set_rate_near(pcm_handle, params, &rate, &dir);
    snd_pcm_hw_params(pcm_handle, params);

    snd_pcm_hw_params_get_period_size(params, &frames, &dir);
    size = frames * CHANNELS * sizeof(short);
    buffer = (short *) malloc(size);

//    int total_frames = SAMPLE_RATE * DURATION;
    int total_frames = SAMPLE_RATE * duration;
    for (i = 0; i < total_frames; i += frames) {
        for (int j = 0; j < frames; j++) {
            double t = (double)(i + j) / SAMPLE_RATE;
            buffer[j] = (short)(AMPLITUDE * sin(2 * M_PI * FREQUENCY * t));
        }

        rc = snd_pcm_writei(pcm_handle, buffer, frames);
        if (rc == -EPIPE) {
            fprintf(stderr, "Underrun occurred\n");
            snd_pcm_prepare(pcm_handle);
        } else if (rc < 0) {
            fprintf(stderr, "Error writing to PCM device: %s\n", snd_strerror(rc));
        }
    }

    snd_pcm_drain(pcm_handle);
    snd_pcm_close(pcm_handle);
    free(buffer);

    usleep(1000*100);

    return 0;
}

#ifdef TESTPROG

int main()
{
	beep(DAH); beep(DIT); usleep(500*1000);
	beep(DAH); beep(DAH); beep(DIT); beep(DIT); beep(DIT); usleep(500*1000);
	beep(DAH); beep(DAH); usleep(500*1000);
	beep(DAH); beep(DIT); beep(DAH); usleep(500*1000);
	beep(DAH); beep(DAH); beep(DAH); usleep(500*1000);

}

#endif

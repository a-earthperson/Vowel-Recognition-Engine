#include <stdio.h>
#include "helper.h"
#include "featureExtraction.h"
#include "fft.h"
#include "filterBank.h"
#include "kmeans.h"
#include "dwt.h"

#define LCDK 0

SAMPLE INPUT[1024];
double OMEGA[1024];
double PSD[1024];
double ENERGY_FROM_FILTERBANK[26];
double MFCC[26];

static void initialize();

int main(void)
{
    initialize();

    PaStreamParameters  inputParameters, outputParameters;
    PaStream*           stream;
    PaError             err = paNoError;
    paTestData          data;
    int                 i;
    int                 totalFrames;
    int                 numSamples;
    int                 numBytes;
    SAMPLE              max, val;
    double              average;

    fprintf(stderr, "patest_record.c\n"); fflush(stdout);

    data.maxFrameIndex = 1024 + 1;
    data.frameIndex = 0;
    numSamples = totalFrames * NUM_CHANNELS;
    numBytes = numSamples * sizeof(SAMPLE);

    data.recordedSamples = INPUT;

    err = Pa_Initialize();
    if( err != paNoError ) goto done;

    inputParameters.device = Pa_GetDefaultInputDevice(); /* default input device */
    if (inputParameters.device == paNoDevice) {
        fprintf(stderr,"Error: No default input device.\n");
        goto done;
    }
    inputParameters.channelCount = 1;                    /* stereo input */
    inputParameters.sampleFormat = PA_SAMPLE_TYPE;
    inputParameters.suggestedLatency = Pa_GetDeviceInfo( inputParameters.device )->defaultLowInputLatency;
    inputParameters.hostApiSpecificStreamInfo = NULL;

    /* Record some audio. -------------------------------------------- */
    err = Pa_OpenStream(
            &stream,
            &inputParameters,
            NULL,                  /* &outputParameters, */
            SAMPLE_RATE,
            FRAMES_PER_BUFFER,
            paClipOff,      /* we won't output out of range samples so don't bother clipping them */
            recordCallback,
            &data );
    if( err != paNoError ) goto done;

    err = Pa_StartStream( stream );
    if( err != paNoError ) goto done;
    fprintf(stderr, "\n=== Now recording!! Please speak into the microphone. ===\n"); fflush(stdout);

    int currentFrameIndex = 0;

    while( ( err = Pa_IsStreamActive( stream ) ) == 1 )
    {
        if(data.frameIndex != 1024) continue;
        data.frameIndex = 0;

        /** Step_1 :: Apply Hamming Window to raw input signal **/
        applyWindow(INPUT, OMEGA, 1024);

        fwt97(OMEGA, 1024);

        /** Step_2 :: Periodogram estimate of PSD of input signal **/
        //getPSD(OMEGA, PSD, 1024, (1024/2)+1);

        //getEnergyOutputFromFilterBank(ENERGY_FROM_FILTERBANK, PSD, 513);

        //getMFCC(MFCC, ENERGY_FROM_FILTERBANK);

        //predict(MFCC);

//        int i;
        if(abs(OMEGA[0]) < 1) continue;
        for(i = 0; i < 1024; i++) {
            //printf("DWT[%d],", i);
            printf("%f,",OMEGA[i]);
            //fprintf(stdout,"%d,%d,%f,%f\n", i, INPUT[i], OMEGA[i], PSD[i]); fflush(stdout);
            //printf("input(%d): %d, omega(%d): %f, psd(%d): %f\n", i, INPUT[i], i, OMEGA[i], i, PSD[i]); fflush(stdout);
        }
        printf("ah\n");
        fflush(stdout);
    }

    if( err < 0 ) goto done;

    err = Pa_CloseStream( stream );
    if( err != paNoError ) goto done;

    /* Write recorded data to a file. */
#if WRITE_TO_FILE
    {
        FILE  *fid;
        fid = fopen("recorded.raw", "wb");
        if( fid == NULL )
        {
            fprintf(stderr, "Could not open file.");
        }
        else
        {
            fwrite( data.recordedSamples, NUM_CHANNELS * sizeof(SAMPLE), totalFrames, fid );
            fclose( fid );
            fprintf(stderr, "Wrote data to 'recorded.raw'\n");
        }
    }
#endif

    /* Playback recorded data.  -------------------------------------------- */
    data.frameIndex = 0;

    outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
    if (outputParameters.device == paNoDevice) {
        fprintf(stderr,"Error: No default output device.\n");
        goto done;
    }
    outputParameters.channelCount = 1;                     /* stereo output */
    outputParameters.sampleFormat =  PA_SAMPLE_TYPE;
    outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;

    fprintf(stderr, "\n=== Now playing back. ===\n"); fflush(stdout);
    err = Pa_OpenStream(
            &stream,
            NULL, /* no input */
            &outputParameters,
            SAMPLE_RATE,
            FRAMES_PER_BUFFER,
            paClipOff,      /* we won't output out of range samples so don't bother clipping them */
            playCallback,
            &data );
    if( err != paNoError ) goto done;

    if( stream )
    {
        err = Pa_StartStream( stream );
        if( err != paNoError ) goto done;

        fprintf(stderr, "Waiting for playback to finish.\n"); fflush(stdout);

        while( ( err = Pa_IsStreamActive( stream ) ) == 1 ) Pa_Sleep(100);
        if( err < 0 ) goto done;

        err = Pa_CloseStream( stream );
        if( err != paNoError ) goto done;

        fprintf(stderr, "Done.\n"); fflush(stdout);
    }

    done:
    Pa_Terminate();
    if( data.recordedSamples )       /* Sure it is NULL or valid. */
        free( data.recordedSamples );
    if( err != paNoError )
    {
        fprintf( stderr, "An error occured while using the portaudio stream\n" );
        fprintf( stderr, "Error number: %d\n", err );
        fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
        err = 1;          /* Always return 0 or 1, but no other return codes. */
    }
    return err;
}


static void initialize() {

    // Set all vectors to 0.0
    unsigned int n;
    for(n = 0; n < 1024; n++) {
        INPUT[n] = 0;
        OMEGA[n] = 0;
        PSD[n] = 0;
    }

    // Calculate Hamming Window constants
    generateWindow(1024);

    // Calculate twiddle factors for FFT
    generateTwiddles(1024);

    generateFilters();
}
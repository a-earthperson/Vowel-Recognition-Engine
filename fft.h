//
// Created by Work Account on 5/6/17.
//

#ifndef VOWELRECOGNITION_FFT_H
#define VOWELRECOGNITION_FFT_H

// fft.h complex FFT function taken from Rulph's C31 book
// this file contains definition of complex data structure
#include <math.h>
#include <stdio.h>
#include "config.h"
#include "helper.h"

struct cmpx            //complex data structure used by FFT
{
    double real;
    double imag;
};

typedef struct cmpx COMPLEX;

static COMPLEX twiddle[1024];

static void fft(COMPLEX *Y, int M, COMPLEX *w)
{
    COMPLEX temp1,temp2;      // temporary storage
    int i,j,k;                // loop counters
    int upper_leg, lower_leg; // upper/lower butterfly leg
    int leg_diff;             // upper/lower leg difference
    int num_stages=0;         // number of FFT stages
    int index, step;          // twiddle favtor index and step
    i=1;                      // no. stages = log2(no. points)
    do
    {
        num_stages+=1;
        i=i*2;
    } while (i!=M);

    leg_diff=M/2;               // starting difference
    step=2;                     // twiddle step
    for (i=0;i<num_stages;i++)  // for M-point FFT
    {
        index=0;
        for (j=0;j<leg_diff;j++)
        {
            for (upper_leg=j;upper_leg<M;upper_leg+=(2*leg_diff))
            {
                lower_leg=upper_leg+leg_diff;
                temp1.real=(Y[upper_leg]).real + (Y[lower_leg]).real;
                temp1.imag=(Y[upper_leg]).imag + (Y[lower_leg]).imag;
                temp2.real=(Y[upper_leg]).real - (Y[lower_leg]).real;
                temp2.imag=(Y[upper_leg]).imag - (Y[lower_leg]).imag;
                (Y[lower_leg]).real=temp2.real*(w[index]).real
                                    -temp2.imag*(w[index]).imag;
                (Y[lower_leg]).imag=temp2.real*(w[index]).imag
                                    +temp2.imag*(w[index]).real;
                (Y[upper_leg]).real=temp1.real;
                (Y[upper_leg]).imag=temp1.imag;
            }
            index+=step;
        }
        leg_diff=leg_diff/2;
        step*=2;
    }
    j=0;
    for (i=1;i<(M-1);i++) // bit reversal for resequencing data
    {
        k=M/2;
        while (k<=j)
        {
            j=j-k;
            k=k/2;
        }
        j=j+k;
        if (i<j)
        {
            temp1.real=(Y[j]).real;
            temp1.imag=(Y[j]).imag;
            (Y[j]).real=(Y[i]).real;
            (Y[j]).imag=(Y[i]).imag;
            (Y[i]).real=temp1.real;
            (Y[i]).imag=temp1.imag;
        }
    }

    return;
}

static void generateTwiddles(const unsigned int N) {

    unsigned int n;

    for (n=0 ; n<N ; n++) //set up FFT twiddle factors
    {
        const double arg = fma(n, M_PI, 0) / N; // TODO :: Check twiddle here
        twiddle[n].real = cos(arg);
        twiddle[n].imag = -sin(arg);
        //if(VERBOSE) printf("twiddle[%d] : %f, %f\n", n, twiddle[n].real, twiddle[n].imag);
    }

}

static void getPSD(const double *input, double *out, const unsigned int N, const unsigned int K) {

    COMPLEX fft_vector[N];
    unsigned int n;
    for (n=0 ; n<N ; n++)
    {
        fft_vector[n].real = input[n];
        fft_vector[n].imag = 0.0;
    }

    fft(fft_vector, N, twiddle);

    for (n = 0 ; n < K; n++)
    {
        out[n] = (pow(fft_vector[n].real, 2.0) + pow(fft_vector[n].imag, 2.0)) / 1024.0 ;
        //fprintf(stderr, "psd[%d]:%f\n",n,out[n]);
    }
}

#endif VOWELRECOGNITION_FFT_H

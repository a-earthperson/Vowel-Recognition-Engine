//
// Created by Work Account on 5/6/17.
//

#include <math.h>
#include "helper.h"
#include "config.h"

#ifndef VOWELRECOGNITION_FEATUREEXTRACTION_H
#define VOWELRECOGNITION_FEATUREEXTRACTION_H

static double WINDOW[1024];

static void applyWindow(const SAMPLE *raw, double *out, const unsigned int N) {

    unsigned int n;
    for(n = 0; n < N; ++n) {
        out[n] = fma(raw[n], WINDOW[n], 0);
        //if(VERBOSE) printf("hamming(%d):%f,", n, out[n]);
    }
}

static double quickCosFMA(const double arg, const double multiple, const double add) {
    return fma(multiple, cos(arg), add);
}

/**
 * Initialize a flat-top window : (< 0.01 dB passband ripple), 2.5x larger bandwidth than hamming window
 * @param N window size
 */
static void generateFlatTop(const unsigned int N) {

    const double A[] = {0.215578950, -0.416631580, 0.277263158, -0.083578947, 0.006947368};
    const double N_MINUS_1 = (double)(N) - 1.0;

    unsigned int n;
    for(n = 0; n < N; ++n) {
        const double ARG_M_2PI = fma(M_PI, 2.0 * n, 0) / N_MINUS_1;
        const double A4_COS_A0 = fma(A[4], cos(4.0 * ARG_M_2PI), A[0]);
        const double A3_COS_A4 = fma(A[3], cos(3.0 * ARG_M_2PI), A4_COS_A0);
        const double A2_COS_A3 = fma(A[2], cos(2.0 * ARG_M_2PI), A3_COS_A4);
        const double A1_COS_A2 = fma(A[1], cos(      ARG_M_2PI), A2_COS_A3);
        WINDOW[n] = A1_COS_A2;
    }
}

static void generateHamming(const unsigned int N) {

    const double ALPHA = 0.46;
    const double BETA = -0.54;
    const double N_MINUS_1 = (double)(N) - 1.0;

    unsigned int n;
    for(n = 0; n < N; ++n) {
        const double arg = fma(M_PI, 2.0 * n, 0) / N_MINUS_1;
        WINDOW[n] = fma(BETA, cos(arg), ALPHA);
    }
}

static void generateWindow(const unsigned int N) {
    if(FLATTOP_WINDOW)
        generateFlatTop(N);
    else
        generateHamming(N);
}

#endif VOWELRECOGNITION_FEATUREEXTRACTION_H

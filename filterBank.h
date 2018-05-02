//
// Created by Work Account on 5/6/17.
//

#ifndef VOWELRECOGNITION_FILTERBANK_H
#define VOWELRECOGNITION_FILTERBANK_H

#include <math.h>
#include "helper.h"
#include "config.h"

static double MelPoints[28] = {0.00000000000, 105.186038770, 210.372077530, 315.558116300, 420.744155070,
                               525.930193830, 631.116232600, 736.302271370, 841.488310140, 946.674348900,
                               1051.86038767, 1157.04642644, 1262.23246520, 1367.41850397, 1472.60454274,
                               1577.79058150, 1682.97662027, 1788.16265904, 1893.34869781, 1998.53473657,
                               2103.72077534, 2208.90681411, 2314.09285287, 2419.27889164, 2524.46493041,
                               2629.65096917, 2734.83700794, 2840.02304671};

static double HzPoints[28] = {0.0};

static double fBins[28] = {0.000, 4.000, 9.000, 14.00, 20.00, 26.00,
                           33.00, 41.00, 49.00, 59.00, 69.00, 80.00,
                           92.00, 106.0, 120.0, 137.0, 154.0, 174.0,
                           195.0, 219.0, 245.0, 273.0, 304.0, 338.0,
                           376.0, 417.0, 462.0, 512.0};

static double fBank[513][26] = {{0}};
static double fBankTranspose[26][513] = {{0}};

static double HzToMel(const double inputHz) {
    return fma(2595.0, log10(1.0 + inputHz/700.0), 0);
}

static double MelToHz(const double inputMel) {
    return fma((pow(10, (inputMel/2595.0)) - 1.0), 700.0, 0.0);
}

static double dot_product(const double v[], const double u[], const unsigned int n)
{
    unsigned int i;
    double result = 0;
    for (i = 0; i < n; i++) {
        result = fma(v[i], u[i], result);
    }
    return result;
}

static void getEnergyOutputFromFilterBank(double out[], const double signalPSD[], const unsigned int N) {
    unsigned int k;
    for (k = 0; k < 26; k++) {
        out[k] = log10(dot_product(signalPSD, fBankTranspose[k], N));
    }

//    mean /= 26.0;
//    for (k = 0; k < 26; k++) {
//        // out[k] = (out[k]/mean);
//        //log10(pow((out[k] - mean),2));
//        //fprintf(stderr, "all filters %d give out same energy = %f\n", k, out[k]);
//    }
}

static void generateFilters() {

    // Calculate frequencies for every Mel-Point
    unsigned int k, m;
    for(k = 0; k < 28; k++) {
        HzPoints[k] = MelToHz(MelPoints[k]);
        if(VERBOSE) fprintf(stderr, "(Mel[%d] = %f) -> Hz[%d] = %f\n", k, MelPoints[k], k, HzPoints[k]);
    }

    for(m = 1; m < 27; m++) {
        const unsigned int f_m_minus_1 = (unsigned int) (fBins[m - 1]);
        const unsigned int f_m         = (unsigned int) (fBins[m]);
        const unsigned int f_m_plus_1  = (unsigned int) (fBins[m + 1]);
        if(VERBOSE) fprintf(stderr, "f_m_minus_1:%d, f_m:%d, f_m_plus_1:%d\n",f_m_minus_1, f_m, f_m_plus_1);

        for(k = f_m_minus_1; k < f_m; k++) {
            fBank[m - 1][k] = (((double) k) - fBins[m - 1]) / (fBins[m] - fBins[m - 1]);
            fprintf(stderr, "fBank[m - 1][k] = fbank[%d][%d] = %f\n",m-1,k,fBank[m - 1][k]);
        }

        for(k = f_m; k < f_m_plus_1; k++) {
            fBank[m - 1][k] = (fBins[m + 1] - (double) k) / (fBins[m + 1] - fBins[m]);
            fprintf(stderr, "fBank[m - 1][k] = fbank[%d][%d] = %f\n",m-1,k,fBank[m - 1][k]);
        }
    }

    // transpose the matrix
    for(k = 0; k < 513; k++) {
        for(m = 0; m < 26; m++) {
            fBankTranspose[m][k] = fBank[k][m];
            fprintf(stderr, "fBankT[row][col] = fbankT[%d][%d] = %f\n",m,k,fBankTranspose[m][k]);
        }
    }

}

static void getMFCC(double out[], const double energy[]) {
    unsigned int i, m;
    int print = energy[0] > 6;
    for(i = 0; i < 13; i++) {
        out[i] = 0.0;
        for(m = 0; m < 26; m++) {
            out[i] = fma(energy[m], cos((m - 0.5) * M_PI/26.0 * i), out[i]);
        }
        if(print) fprintf(stderr, "%f,", out[i]);
    }
    if(print) fprintf(stderr, "eh\n");

//    double mean = 0.0;
//
//
//    for (i = 0; i < 26; i++) {
//        mean += energy[i];
//    }
//
//    mean /= 26.0;
//
//    for(i = 0; i < 26; i++) {
//        out[i] = energy[i]/mean;
//        if(print) fprintf(stderr, "%f,", out[i]);
//    }
//    if(print) fprintf(stderr, "ie\n");
}

#endif VOWELRECOGNITION_FILTERBANK_H

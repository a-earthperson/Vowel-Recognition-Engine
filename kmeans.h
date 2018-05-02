//
// Created by Work Account on 5/6/17.
//

#ifndef VOWELRECOGNITION_DCT_H_H
#define VOWELRECOGNITION_DCT_H_H


#include <math.h>
#include <stdio.h>

static double MFCC_ah[13] = {123.596039, 121.726233, 116.221777, 107.390568, 95.7229880, 81.8594660, 66.548386, 50.5973620, 34.8213550, 19.9913000, 6.78682100, -4.2437090, -12.713132};
static double MFCC_eh[13] = {154.317989, 152.006831, 145.202239, 134.282303, 119.849315, 102.690070, 83.724364, 63.9453550, 44.3560300, 25.9062620, 9.43484000, -4.3795800, -15.055334};
static double MFCC_ee[13] = {170.951626, 168.401072, 160.891799, 148.841516, 132.915585, 113.983124, 93.060046, 71.2431060, 49.6396630, 29.2980970, 11.1437510, -4.0752470, -15.828659};
static double MFCC_oo[13] = {178.437631, 175.771262, 167.920831, 155.322499, 138.671056, 118.874117, 96.992704, 74.1724320, 51.5702120, 30.2816420, 11.2741250, -4.6696970, -16.994450};

static double getDistancexD(const double *MFCC1, const double *MFCC2, const unsigned int dimension) {

    unsigned int z;
    double squaredDistance = 0;
    for(z=0; z < 13; z++) {
        squaredDistance += pow((MFCC1[z] - MFCC2[z]),2);
    }
    return pow(squaredDistance, 0.5);
}

static unsigned int computeDistance(double out[], const double MFCC_x[]) {

    unsigned int idx = 0, x;

    const double distance_ah = out[0] = getDistancexD(MFCC_x, MFCC_ah, 13);
    const double distance_eh = out[1] = getDistancexD(MFCC_x, MFCC_eh, 13);
    const double distance_ee = out[2] = getDistancexD(MFCC_x, MFCC_ee, 13);
    const double distance_oo = out[3] = getDistancexD(MFCC_x, MFCC_oo, 13);

    double min = MAXFLOAT;
    for(x = 0; x < 4; x++) {
        if(out[x] < min) {
            min = out[x];
            idx = x;
        }
    }

    //fprintf(stdout, "ah:%f, eh:%f, ee:%f, oo:%f\n", distance_ah, distance_eh, distance_ee, distance_oo);
//    switch(idx) {
//
//        case(0):
//            fprintf(stdout, "Prediction: ah\n");
//            break;
//
//        case(1):
//            fprintf(stdout, "Prediction: eh\n");
//            break;
//
//        case(2):
//            fprintf(stdout, "Prediction: ee\n");
//            break;
//
//        case(3):
//            fprintf(stdout, "Prediction: oo\n");
//            break;
//
//        default:
//        case(4):
//            fprintf(stdout, "Prediction: unknown\n");
//            break;
//
//    }
    return idx;

}

static double predictions[4] = {0};

static double predict(const double MFCC_x[]) {
    computeDistance(predictions, MFCC_x);
}
#endif VOWELRECOGNITION_DCT_H_H

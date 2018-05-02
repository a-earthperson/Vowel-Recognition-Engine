//
// Created by Work Account on 5/6/17.
//

#ifndef VOWELRECOGNITION_DATATYPES_H
#define VOWELRECOGNITION_DATATYPES_H

#include <ntsid.h>
#include "fft.h"

typedef struct {
    size_t size;
    double *data;
} doubleVector;

typedef struct {
    size_t size;
    COMPLEX *data;
} complexVector;


#endif VOWELRECOGNITION_DATATYPES_H

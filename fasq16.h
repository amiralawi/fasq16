/*
MIT No Attribution

Copyright 2022 Amir Alawi

Permission is hereby granted, free of charge, to any person obtaining a copy of this
software and associated documentation files (the "Software"), to deal in the Software
without restriction, including without limitation the rights to use, copy, modify,
merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
    
#ifndef FASTQ16_H
#define FASTQ16_H
    
#include <stdint.h>

//type definitions
typedef int32_t fasq16_t;
typedef struct fasqMat3_t{
    //3x3 transformation matrix implementation.  The last row is implicitly
    //defined as [0, 0, 1].  This significantly reduces computational
    //complexity
    //
    // mat = [ [m[0], m[1], m[2]],
    //         [m[3], m[4], m[5]],
    //         [   0,    0,    1] ]
    
    fasq16_t m[6];
} fasqMat3_t;
typedef struct fasqMat4_t{
    //4x4 transformation matrix implementation.  The last row is implicitly
    //defined as [0, 0, 0, 1].  This significantly reduces computational
    //complexity
    //
    // mat = [ [m[0], m[1],  m[2],  m[3]],
    //         [m[4], m[5],  m[6],  m[7]],
    //         [m[8], m[9], m[10], m[11]],
    //         [   0,    0,     0,     1] ]
    
    fasq16_t m[12];
} fasqMat4_t;
typedef struct fasqVec2_t{
    union{
        fasq16_t v[2];
        struct{
            fasq16_t x;
            fasq16_t y;
        };
    };
} fasqVec2_t;
typedef struct fasqVec3_t{
    union{
        fasq16_t v[3];
        struct{
            fasq16_t x;
            fasq16_t y;
            fasq16_t z;
        };
    };
} fasqVec3_t;

//basic operations
fasq16_t fasq16_add(fasq16_t a, fasq16_t b);
fasq16_t fasq16_sub(fasq16_t a, fasq16_t b);
fasq16_t fasq16_mul(fasq16_t a, fasq16_t b);

fasq16_t fasq16_div(fasq16_t a, fasq16_t b);

//vector operations
fasq16_t fasq16_dot2(fasq16_t a0, fasq16_t a1, fasq16_t b0, fasq16_t b1);
fasq16_t fasqVec2_dot(fasqVec2_t *a, fasqVec2_t *b);
fasq16_t fasqVec3_dot(fasqVec3_t *a, fasqVec3_t *b);

//trig functions
fasq16_t fasq16_cos(fasq16_t a0, fasq16_t b0, fasq16_t a1, fasq16_t b1);

//matrix functions
void fasqMat3_transform(fasqMat3_t *m, fasqVec2_t *a, fasqVec2_t *b);
void fasqMat4_transform(fasqMat4_t *m, fasqVec3_t *a, fasqVec2_t *b);
void fasqMat3_multiply(fasqMat3_t *a, fasqMat3_t *b, fasqMat3_t *c);
void fasqMat4_multiply(fasqMat3_t *a, fasqMat3_t *b, fasqMat3_t *c);

//useful constants
#define FASQ16_ONE 0x00010000
#define FASQ16_PI 205887
#define FASQ16_E 178145
#define FASQ16_MAXIMUM 0x7FFFFFFF
#define FASQ16_MINIMUM 0x80000000
#define FASQ16_OVERFLOW 0x80000000

#define FASQVEC2_UNIT_X {{FASQ16_ONE, 0}}
#define FASQVEC2_UNIT_Y {{0, FASQ16_ONE}}
#define FASQVEC3_UNIT_X {{FASQ16_ONE, 0, 0}}
#define FASQVEC3_UNIT_Y {{0, FASQ16_ONE, 0}}
#define FASQVEC3_UNIT_Z {{0, 0, FASQ16_ONE}}

#define FASQMAT3_IDENTITY {{FASQ16_ONE, 0, 0, 0, FASQ16_ONE, 0}}
#define FASQMAT4_IDENTITY {{FASQ16_ONE, 0, 0, 0, 0, FASQ16_ONE, 0, 0, 0, 0, FASQ16_ONE, 0}}

//conversion functions
fasq16_t fasq16_from_int();
fasq16_t fasq16_from_float(float a);
fasq16_t fasq16_from_dbl();

int fasq16_to_int(fasq16_t a);
float fasq16_to_float(fasq16_t a);
double fasq16_to_double(fasq16_t a);


// This macro is useful for defining fasq16_t constant values.  This macro is
// evaluated with multiple times - this should only be used for literals in
// that you do run into unintended side effects.  Use the above functions for
// run-time conversions instead.
#define FASQ16(x) ((fasq16_t)(((x) >= 0) ? ((x) * 65536.0 + 0.5) : ((x) * 65536.0 - 0.5)))

//internal library functions



#endif //FASTQ16_H

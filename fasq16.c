
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

#include "fasq16.h"

fasq16_t fasqVec2_dot(fasqVec2_t *a, fasqVec2_t *b){
    return fasq16_dot2(a->x, a->y, b->x, b->y);
}
fasq16_t fasqVec3_dot(fasqVec3_t *a, fasqVec3_t *b){
    return fasq16_mul(a->x, b->x) + 
           fasq16_mul(a->y, b->y) + 
           fasq16_mul(a->z, b->z);
}
void fasqMat3_multiply(fasqMat3_t *a, fasqMat3_t *b, fasqMat3_t *c){
    // A * B = C
    c->m[0] = fasq16_dot2(a->m[0], a->m[1], b->m[0], b->m[3]);
    c->m[3] = fasq16_dot2(a->m[3], a->m[4], b->m[0], b->m[3]);

    c->m[1] = fasq16_dot2(a->m[0], a->m[1], b->m[1], b->m[4]);
    c->m[4] = fasq16_dot2(a->m[3], a->m[4], b->m[1], b->m[4]);

    c->m[2] = fasq16_dot2(a->m[0], a->m[1], b->m[2], b->m[5]) + a->m[2];
    c->m[5] = fasq16_dot2(a->m[0], a->m[1], b->m[2], b->m[5]) + a->m[5];
}
void fasqMat3_transform(fasqMat3_t *m, fasqVec2_t *a, fasqVec2_t *b){
    // m * a = b
    //
    // m = [ [m[0], m[1], m[2]],
    //       [m[3], m[4], m[5]],
    //       [   0,    0,    1] ]
    //
    // b.x = m[0]*a.x + m[1]*a.y + m[2]
    // b.y = m[3]*a.x + m[4]*a.y + m[2]
    
    //use an intermediate temp variable
    //this avoids a bug when the input and output pointers are the same
    fasqVec2_t temp;
    temp.v[0] = fasq16_mul(m->m[0], a->x) + m->m[1]*a->y + m->m[2];
    temp.v[1] = m->m[0]*a->x + m->m[1]*a->y + m->m[2];
    *b = temp;
}

inline fasq16_t fasq16_from_int(int x){ return x * FASQ16_ONE; }
inline fasq16_t fasq16_from_float(float x){
	x = x * FASQ16_ONE;
    
    // this ensures rounding
	x += (float)((x >= 0) ? 0.5f : -0.5f);
    
	return (fasq16_t)x;
}
inline fasq16_t fasq16_from_dbl(double x){
	x = x * FASQ16_ONE;
    
    // this ensures rounding
	x += (double)((x >= 0) ? 0.5 : -0.5);
    
	return (fasq16_t)x;
}

inline float fasq16_to_float(fasq16_t a) { return (float)a / (float)FASQ16_ONE; }
inline double fasq16_to_dbl(fasq16_t a) { return (double)a / (double)FASQ16_ONE; }
inline int fasq16_to_int(fasq16_t a){
    return (a >> 16);
}

//TODO:
void fasqMat4Multiply(fasqMat3_t *a, fasqMat3_t *b, fasqMat3_t *c);

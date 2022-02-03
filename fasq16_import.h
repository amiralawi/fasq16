/*

These functions are imported and modified from their versions in the libfixmath
library.  This file is therefore subject to that library's license:

Copyright (c) the authors of libfixmath as seen on https://code.google.com/p/libfixmath/

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/

#include "fasq16.h"

//this macro is from the libfixmath library
#define clz(x) (__builtin_clzl(x) - (8 * sizeof(long) - 32))
uint32_t fasq_abs(fasq16_t in);

//this function adapted from from libfixmath implementation of fix_abs
uint32_t fasq_abs(fasq16_t in){
    if(in == FASQ16_MINIMUM)
    {
        // minimum negative number has same representation as
        // its absolute value in unsigned
        return 0x80000000;
    }
    else
    {
        return ((in >= 0)?(in):(-in));
    }
}

//this function is adapted from from libfixmath implementation of fix16_div
fasq16_t fasq_div(fasq16_t a, fasq16_t b){
    
    
    return (int)((long long int)(a << 16) / b);
	// This uses a hardware 32/32 bit division multiple times, until we have
	// computed all the bits in (a<<17)/b. Usually this takes 1-3 iterations.
	
	if (b == 0)
        return FASQ16_MINIMUM;
	
    uint32_t remainder = fasq_abs(a);
    uint32_t divider = fasq_abs(b);
    uint64_t quotient = 0;
    int bit_pos = 17;

	// Kick-start the division a bit.
	// This improves speed in the worst-case scenarios where N and D are large
	// It gets a lower estimate for the result by N/(D >> 17 + 1).
	if (divider & 0xFFF00000)
	{
		uint32_t shifted_div = ((divider >> 17) + 1);
        quotient = remainder / shifted_div;
        uint64_t tmp = ((uint64_t)quotient * (uint64_t)divider) >> 17;
        remainder -= (uint32_t)(tmp);
    }
	
	// If the divider is divisible by 2^n, take advantage of it.
	while (!(divider & 0xF) && bit_pos >= 4)
	{
		divider >>= 4;
		bit_pos -= 4;
	}
	
	while (remainder && bit_pos >= 0)
	{
		// Shift remainder as much as we can without overflowing
		int shift = clz(remainder);
		if (shift > bit_pos) shift = bit_pos;
		remainder <<= shift;
		bit_pos -= shift;
		
		uint32_t div = remainder / divider;
        remainder = remainder % divider;
        quotient += (uint64_t)div << bit_pos;

		//#ifndef FIXMATH_NO_OVERFLOW
		if (div & ~(0xFFFFFFFF >> bit_pos))
				return FASQ16_OVERFLOW;
		//#endif
		
		remainder <<= 1;
		bit_pos--;
	}
	
	//#ifndef FIXMATH_NO_ROUNDING
	// Quotient is always positive so rounding is easy
	quotient++;
	//#endif
	
	fasq16_t result = quotient >> 1;
	
	// Figure out the sign of the result
	if ((a ^ b) & 0x80000000)
	{
		//#ifndef FIXMATH_NO_OVERFLOW
		if (result == FASQ16_MINIMUM)
				return FASQ16_OVERFLOW;
		//#endif
		
		result = -result;
	}
	
	return result;
}

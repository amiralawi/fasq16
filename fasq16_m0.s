@ 
@ MIT No Attribution
@ 
@ Copyright 2022 Amir Alawi
@ 
@ Permission is hereby granted, free of charge, to any person obtaining a copy of this
@ software and associated documentation files (the "Software"), to deal in the Software
@ without restriction, including without limitation the rights to use, copy, modify,
@ merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
@ permit persons to whom the Software is furnished to do so.
@ 
@ THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
@ INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
@ PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
@ HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
@ OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
@ SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
@

.syntax unified
.cpu cortex-m0


.text

@ exported symbols

.global fasq16_add
.global fasq16_sub
.global fasq16_mul
.global fasq16_dot2
   


fasq16_add:
    ADDS r0, r1
    bx r14

fasq16_sub:
    SUBS r0, r1
    bx r14

fasq16_mul:
    @ cortex-m0 does not have a 64-bit accumulator.  In order to avoid overflow,
    @ each operand is split into fractional and integer portions.  This ensures
    @ that there is no overflow: 16bit * 16bit = 32bit
    @
    @ AB*CD = (A  + B)*(C + D)
    @       = A*C +
    @         A*D +
    @         B*C +
    @         B*D


    @ r0 = 0xaaaabbbb
    @ r1 = 0xccccdddd
    
    PUSH {r4}
    UXTH r2, r0   @extract lower half-word of the value in r0 and write in r2
    LSRS r0, #16  @extract upper half-word of the value in r0 and write in r0
    SXTH r0, r0   @sign-extend the upper-word
    
    
    UXTH r3, r1   @extract lower half-word of the value in r1 and write in r3
    LSRS r1, #16  @extract upper half-word of the value in r1 and write in r1
    SXTH r1, r1   @sign-extend the upper-word
    
    
    @ r0 = 0x0000aaaa, ABint
    @ r2 = 0x0000bbbb, ABfrac
    @ r1 = 0x0000cccc, CDint
    @ r3 = 0x0000dddd, CDfrac
    
    @ ABint * CDint << 16
    MOVS r4, r0
    MULS r4, r1
    LSLS r4, #16
    
       
    @ ABint * CDfrac
    MULS r0, r3

    @A*C+A*D
    @ABint*CDint + ABint*CDfrac
    @this frees up r4
    ADDS r0, r4

    @ ABfrac * CDint
    MOV r4, r2
    MULS r4, r1
    ADDS r0, r4

    MOV r4, r2
    MULS r4, r3
    LSRS r4, #16
    ADDS r0, r4
    
    POP {r4}
    bx     lr
    
fasq16_dot2:
    @ adapted from fasq16_mul, except two pairs of arguments
    @ fasq16_dot2(a0, a1, b0, b1) = (a0, a1) . (b0, b1)
    @                             = a0*b0 + a1*b1


    @ r0 = 0xaaaabbbb
    @ r1 = 0xeeeeffff
    @ r2 = 0xccccdddd
    @ r3 = 0xgggghhhh
    
    
    PUSH {r4, r5, r6}
    UXTH r4, r0   @extract lower half-word of the value in r0 and write in r4
    LSRS r0, #16  @extract upper half-word of the value in r0 and write in r0
    SXTH r0, r0   @sign-extend the upper-word
    
    UXTH r5, r2   @extract lower half-word of the value in r2 and write in r5
    LSRS r2, #16  @extract upper half-word of the value in r2 and write in r2
    SXTH r2, r2   @sign-extend the upper-word
    
    
    @ r0 = 0x0000aaaa, ABint
    @ r1 = 0xeeeeffff, preserve
    @ r2 = 0x0000cccc, CDint
    @ r3 = 0xgggghhhh, preserve
    @ r4 = 0x0000bbbb, ABfrac
    @ r5 = 0x0000dddd, CDfrac
    @ r6 = free
    
    @ r6 = ABint * CDint << 16
    MOVS r6, r0
    MULS r6, r2
    LSLS r6, #16
    
    @ r0 = ABint * CDfrac
    MULS r0, r5

    @ r0 = ABint*CDfrac + (ABint*CDint << 16)
    ADD r0, r6

    @ r0 = ABint*CDfrac + (ABint*CDint << 16) + ABfrac*CDint
    MOVS    r6, r4
    MULS    r6, r2
    ADD    r0, r6

    @ r0 = ABint*CDfrac + (ABint*CDint << 16) + ABfrac*CDint + (ABfrac*CDfrac >> 16) = AB*CD
    MOVS    r6, r4
    MULS    r6, r5
    LSRS    r6, #16
    ADD    r0, r6
    
    @@@@@@@@@@@
    @@@@@@@@@@@
    @@@@@@@@@@@
    
    @ r0 = AB*CD
    @ r1 = 0xeeeeffff
    @ r2 = free
    @ r3 = 0xgggghhhh
    @ r4-r6 = free
    
    UXTH r2, r1   @extract lower half-word of the value in r1 and write in r2
    LSRS r1, #16  @extract upper half-word of the value in r1 and write in r1
    SXTH r1, r1   @sign-extend the upper-word
    
    UXTH r4, r3   @extract lower half-word of the value in r3 and write in r4
    LSRS r3, #16  @extract upper half-word of the value in r3 and write in r3
    SXTH r3, r3   @sign-extend the upper-word
    
    @ r0 = AB*CD, preserve
    @ r1 = 0x0000eeee, EFint
    @ r2 = 0x0000ffff, EFfrac
    @ r3 = 0x0000gggg, GHint
    @ r4 = 0x0000hhhh, GHfrac
    @ r5-r7 = free
    
    @ r0 = AB*CD + (EFint*GHint << 16)
    MOVS r5, r1
    MULS r5, r3
    LSLS r5, #16
    ADD r0, r5
    
    @ r0 = AB*CD + (EFint*GHint << 16) + EFint*GHfrac
    MULS r1, r4
    ADD r0, r1
    
    @ r0 = AB*CD + (EFint*GHint << 16) + EFint*GHfrac + EFfrac*GHint
    MOVS    r5, r2
    MULS    r5, r3
    ADD    r0, r5
    
    @ r0 = AB*CD + (EFint*GHint << 16) + EFint*GHfrac + EFfrac*GHint + (EFfrac*GHfrac >> 16)
    @ EFfrac * GHfrac >> 16
    MOVS    r5, r2
    MULS    r5, r4
    LSRS    r5, #16
    ADD    r0, r5
    
    POP     {r4, r5, r6}
    BX      LR

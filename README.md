# fasq16
Fast fixed-point (Q16.16) library for embedded applications.

# Design Philosophy
fasq16 is a fixed-point library intended as a replacement to floating-point for applications without an FPU.  Initial functionality will be implemented in C, these will be replaced/supplemented with optimized assembly routines starting with Armv6-M (cortex-m0).  Code design considers the following principles in order of priority:
1. Readability
2. Speed
3. Accuracy

# Available Functions

- Addition / Subtraction / Multiplication / Division
- Vector dot-product
- Matrix multiplication
- Matrix/Vector transformation

# Planned features
The following features are planned for implementation:
- Trig functions (cos, sin, tan)
- Squareroot

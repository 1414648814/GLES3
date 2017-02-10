//
//  CCMath.h
//  GLES
//
//  Created by staff on 16/12/16.
//  Copyright © 2016年 George1994. All rights reserved.
//

#ifndef CCMath_h
#define CCMath_h

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cassert>
#include <cstring>

#define MATH_DEG_TO_RAD(x)          ((x) * 0.0174532925f)
#define MATH_RAD_TO_DEG(x)          ((x)* 57.29577951f)
#define MATH_RANDOM_MINUS1_1()      ((2.0f*((float)rand()/RAND_MAX))-1.0f)      // Returns a random float between -1 and 1.
#define MATH_RANDOM_0_1()           ((float)rand()/RAND_MAX)                    // Returns a random float between 0 and 1.
#define MATH_FLOAT_SMALL            1.0e-37f
#define MATH_TOLERANCE              2e-37f
#define MATH_E                      2.71828182845904523536f
#define MATH_LOG10E                 0.4342944819032518f
#define MATH_LOG2E                  1.442695040888963387f
#define MATH_PI                     3.14159265358979323846f
#define MATH_PIOVER2                1.57079632679489661923f
#define MATH_PIOVER4                0.785398163397448309616f
#define MATH_PIX2                   6.28318530717958647693f
#define MATH_EPSILON                0.000001f
#define MATH_CLAMP(x, lo, hi)       ((x < lo) ? lo : ((x > hi) ? hi : x))
#define MATRIX_SIZE ( sizeof(float) * 16)

#define GP_ASSERT(expression) assert(expression)

#define GP_ERROR(...) do \
{ \
std::cout << __current__func__ << std::endl; \
std::cout << __VA_ARGS__ << std::endl; \
assert(0); \
std::exit(-1); \
} while (0)

// Warning macro.
#define GP_WARN(...) do \
{ \
std::cout << __current__func__ << std::endl; \
std::cout << __VA_ARGS__ << std::endl; \
} while (0)

#endif /* CCMath_h */

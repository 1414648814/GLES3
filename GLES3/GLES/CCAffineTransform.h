//
//  CCAffineTransform.hpp
//  GLES3
//
//  Created by staff on 17/2/3.
//  Copyright © 2017年 George. All rights reserved.
//

#ifndef CCAffineTransform_hpp
#define CCAffineTransform_hpp

#include <stdio.h>
#include "CCMath.h"
#include "CCVector2.h"
#include "CCVector3.h"
#include "CCVector4.h"
#include "CCPoint.h"
#include "CCMatrix.h"

//仿射矩阵
//单位矩阵

/**@{
 Affine transform
 a   b    0
 c   d    0
 tx  ty   1
 
 Identity
 1   0    0
 0   1    0
 0   0    1
 */

struct AffineTransform {
    float a, b, c, d;
    float tx, ty;
    
    static const AffineTransform IDENTITY;
};


#endif /* CCAffineTransform_hpp */

// 创建仿射矩阵
AffineTransform AffineTransformMake(float a, float b, float c, float d, float tx, float ty);

// 节点乘以仿射矩阵
Vector2 PointApplyAffineTransform(const Vector2& point, const AffineTransform& t);

// size乘以仿射矩阵
CCSize SizeApplyAffineTransform(const CCSize& size, const AffineTransform& t);

// rect乘以仿射矩阵
CCRect RectApplyAffineTransform(const CCRect& rect, const AffineTransform& anAffineTransform);

// rect乘以矩阵
CCRect RectApplyTransform(const CCRect& rect, const Matrix& transform);

// 节点乘以矩阵
Vector2 PointApplyTransform(const Vector2& point, const Matrix& transform);

/**
 Translation, equals
 1  0  1
 0  1  0   * affine transform
 tx ty 1
 */
// 平移
AffineTransform AffineTransformTranslate(const AffineTransform& t, float tx, float ty);
/**
 Rotation, equals
 cos(angle)   sin(angle)   0
 -sin(angle)  cos(angle)   0  * AffineTransform
 0            0            1
 */
// 旋转
AffineTransform AffineTransformRotate(const AffineTransform& aTransform, float anAngle);
/**
 Scale, equals
 sx   0   0
 0    sy  0  * affineTransform
 0    0   1
 */
// 缩放
AffineTransform AffineTransformScale(const AffineTransform& t, float sx, float sy);

/**Concat two affine transform, t1 * t2*/
AffineTransform AffineTransformConcat(const AffineTransform& t1, const AffineTransform& t2);

/**Compare affine transform.*/
// 仿射矩阵是否和矩阵相等
bool AffineTransformEqualToTransform(const AffineTransform& t1, const AffineTransform& t2);

/**Get the inverse of affine transform.*/
// 获取仿射矩阵的逆矩阵
AffineTransform AffineTransformInvert(const AffineTransform& t);

/**Concat Mat4, return t1 * t2.*/
// 矩阵相乘
Matrix TransformConcat(const Matrix& t1, const Matrix& t2);

// 单位矩阵
extern const AffineTransform AffineTransformIdentity;


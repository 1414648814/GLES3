//
//  CCOPGL.h
//  GLES3
//
//  Created by George on 17/1/4.
//  Copyright © 2017年 George. All rights reserved.
//

#ifndef CCOPGL_h
#define CCOPGL_h

#include <stdio.h>
#include "CCGL.h"

namespace OPGL {
    void activeTexture(GLenum texture);
    
    // 绑定纹理到纹理目标上
    void bindTexture2D(GLuint textureId);
    
    // 删除纹理
    void deleteTexture(GLenum textureId);
    // 设置混合
    static void SetBlending(GLenum sfactor, GLenum dfactor);
    
    // 设置混合
    void blendFunc(GLenum sfactor, GLenum dfactor);
    
    void bindVAO(GLuint vaoId);
}


#endif /* CCOPGL_h */

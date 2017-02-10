//
//  CCOPGL.cpp
//  GLES3
//
//  Created by staff on 17/1/20.
//  Copyright © 2017年 George. All rights reserved.
//

#include <stdio.h>
#include "CCOPGL.h"

// 将OpenGL的函数封装
static const int MAX_ATTRIBUTES = 16;
static const int MAX_ACTIVE_TEXTURE = 16;

namespace  {
    static GLuint    s_currentShaderProgram = -1;
    static GLuint    s_currentBoundTexture[MAX_ACTIVE_TEXTURE] =  {(GLuint)-1,(GLuint)-1,(GLuint)-1,(GLuint)-1, (GLuint)-1,(GLuint)-1,(GLuint)-1,(GLuint)-1, (GLuint)-1,(GLuint)-1,(GLuint)-1,(GLuint)-1, (GLuint)-1,(GLuint)-1,(GLuint)-1,(GLuint)-1, };
    static GLenum    s_blendingSource = -1;
    static GLenum    s_blendingDest = -1;
    static int       s_GLServerState = 0;
    static GLuint    s_VAO = 0;
    static GLenum    s_activeTexture = -1;
}

namespace OPGL {
    void activeTexture(GLenum texture)
    {
        if (s_activeTexture != texture) {
            s_activeTexture = texture;
            glActiveTexture(s_activeTexture);
        }
    }
    
    // 绑定纹理到纹理目标上
    void bindTexture2D(GLuint textureId) {
        if (s_currentBoundTexture[0] != textureId) {
            s_currentBoundTexture[0] = textureId;
            activeTexture(GL_TEXTURE0 + 0);
            glBindTexture(GL_TEXTURE_2D, textureId);
        }
    }
    
    // 删除纹理
    void deleteTexture(GLenum textureId) {
        for (size_t i = 0; i < MAX_ACTIVE_TEXTURE; ++i) {
            if (s_currentBoundTexture[i] == textureId) {
                s_currentBoundTexture[i] = -1;
            }
        }
        glDeleteTextures(1, &textureId);
    }
    
    // 设置混合
    static void SetBlending(GLenum sfactor, GLenum dfactor)
    {
        if (sfactor == GL_ONE && dfactor == GL_ZERO)
        {
            glDisable(GL_BLEND);
        }
        else
        {
            glEnable(GL_BLEND);
            glBlendFunc(sfactor, dfactor);
        }
    }
    
    // 设置混合
    void blendFunc(GLenum sfactor, GLenum dfactor)
    {
        if (sfactor != s_blendingSource || dfactor != s_blendingDest)
        {
            s_blendingSource = sfactor;
            s_blendingDest = dfactor;
            SetBlending(sfactor, dfactor);
        }
    }
    
    void bindVAO(GLuint vaoId)
    {
        if (s_VAO != vaoId)
        {
            s_VAO = vaoId;
            glBindVertexArray(vaoId);
        }
    }
}
//
//  CCGL.h
//  GLES
//
//  Created by staff on 16/12/12.
//  Copyright © 2016年 George1994. All rights reserved.
//

#ifndef CCGL_h
#define CCGL_h

#define CC_PLATFORM_UNKOWN   0
#define CC_PLATFORM_MAC      1
#define CC_PLATFORM_IPHONE   2

#define CC_TARGET_PLATFORM   2


//暂时先只加入mac和ios平台
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
#import <OpenGL/gl.h>
#import <OpenGL/glu.h>
#import <OpenGL/glext.h>
#import <OpenGL/gltypes.h>

// VAO（共用一个函数名称）
#define glBindVertexArray    glBindVertexArrayAPPLE
#define glDeleteVertexArrays glDeleteVertexArraysAPPLE
#define glGenVertexArrays    glGenVertexArraysAPPLE
#define glIsVertexArray      glIsVertexArrayAPPLE

#define glClearDepthf        glClearDepth
#define glDepthRangef        glDepthRange

// 安卓4.3以上才会支持es3.0，并且2.0是最基础的，也是最经典的
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IPHONE)
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#include <OpenGLES/gltypes.h>

// MapBuffer
#define glGetBufferPointerv  glGetBufferPointervOES
#define glMapBuffer          glMapBufferOES
#define glUnmapBuffer        glUnmapBufferOES
#define GL_WRITE_ONLY        GL_WRITE_ONLY_OES

// VAO
#define glBindVertexArray    glBindVertexArrayOES
#define glDeleteVertexArrays glDeleteVertexArraysOES
#define glGenVertexArrays    glGenVertexArraysOES
#define glIsVertexArray      glIsVertexArrayOES

#define glClearDepth         glClearDepthf

#endif

#endif /* CCGL_h */
//
//  CCShader.hpp
//  GLES
//
//  Created by staff on 16/12/13.
//  Copyright © 2016年 George1994. All rights reserved.
//

#ifndef CCShader_hpp
#define CCShader_hpp

#include "CCGL.h"
#include "CCConfig.h"
#include <stdio.h>
#include <string>

// shader中的vertexAttrib
struct VertexAttrib
{
    GLuint index; //属性下标，从0开始
    GLint size; //数据个数
    GLenum type; //数据类型
    std::string name; //顶点着色器的名字
};

// shader中的uniform
struct Uniform
{
    GLint location; //属性位置，从0开始
    GLint size;
    GLenum type;
    std::string name;
};

// 封装Uniform
class UniformValue {
public:
    UniformValue();
    ~UniformValue();
    
protected:
    Uniform _uniform;
};

// 封装VertexAttrib
class VertexAttribValue {
public:
    VertexAttribValue();
    ~VertexAttribValue();
    
protected:
    VertexAttrib _attrib;
};

// 着色器
class Shader {
public:
    Shader();
    ~Shader();
};


#endif /* CCShader_hpp */

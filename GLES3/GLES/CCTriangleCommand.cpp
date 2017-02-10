//
//  CCTriangleCommand.cpp
//  GLES
//
//  Created by staff on 16/12/14.
//  Copyright © 2016年 George1994. All rights reserved.
//

#include "CCTriangleCommand.h"
#include "CCOPGL.h"
#include "CCProgram.h"

TriangleCommand::TriangleCommand()
: _materialID(0)
, _textureID(0)
, _programState(NULL)
, _blendType(BlendFunc::DISABLE)
{
    this->_cmdType = COMMAND_TRIANGEL;
}

TriangleCommand::~TriangleCommand() {
    
}

bool TriangleCommand::init(float globalZOrder, GLuint textureID, ProgramState* state, BlendFunc blend, const Triangles& triangles, const Matrix& modelView, bool is3D) {
    RenderCommand::init(globalZOrder, modelView, is3D);
    
    this->_triangles = triangles;
    this->_modelView = modelView;
    
    //索引的数目必须是3的倍数，因为基本图形是三角形，包括顶点的三个信息
    if (this->_triangles.indexCount % 3 != 0) {
        ssize_t count = this->_triangles.indexCount;
        this->_triangles.indexCount = count / 3 * 3;
    }
    
    // 不需要重复渲染
    if (this->_textureID != textureID || this->_blendType.src != blend.src || this->_blendType.dst != blend.dst || this->_programState != state) {
        
        this->_textureID = textureID;
        this->_programState = state;
        this->_blendType = blend;
    }
    
    //this->generateMatrialID();
    
    return true;
}

// 生成纹理ID
void TriangleCommand::generateMatrialID() {
    if (this->_programState->getUniformsCount() > 0) {
        this->_materialID = MATIRIAL_ID_NOT_BATCH;
    }
}

// 使用纹理
void TriangleCommand::useMaterial() {
    // 设置纹理
    OPGL::bindTexture2D(this->_textureID);// 绑定，即可从_textureID中取出图像数据
    
    // 设置绑定模式
    OPGL::blendFunc(this->_blendType.src, this->_blendType.dst);
    
    // 设置模型矩阵
    this->_programState->apply(this->_modelView);
}


//
//  CCLayer.cpp
//  GLES3
//
//  Created by staff on 17/2/9.
//  Copyright © 2017年 George. All rights reserved.
//

#include "CCLayer.h"
#include "CCRenderer.h"
#include "CCProgram.h"


void Layer::draw(Renderer *renderer, const Matrix &transform)
{
    _customCommand.init(_globalZOrder, transform);
    _customCommand.func = CC_CALLBACK_0(Layer::onDraw, this, transform);
    renderer->addCommand(&_customCommand, (int)COMMAND_COMMON);
    
    for(int i = 0; i < 4; ++i)
    {
        Vector4 pos;
        pos.x = _squareVertices[i].x; pos.y = _squareVertices[i].y; pos.z = _positionZ;
        pos.w = 1;
        _modelViewTransform.transformVector(&pos);
        _noMVPVertices[i] = Vector3(pos.x,pos.y,pos.z)/pos.w;
    }
}

void Layer::onDraw(const Matrix& transform)
{
    auto program = getProgram();
    
    program->use();
    program->setUniformForBuilts(transform);
    
    program->setVertexAttribPointer(VERTEX_ATTRIB_POSITION);
    
    // 4.2颜色
    program->setVertexAttribPointer(VERTEX_ATTRIB_COLOR);
    
    // 4.3纹理坐标
    program->setVertexAttribPointer(VERTEX_ATTRIB_TEXCOOD);
    
    //
    // Attributes
    //
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribPointer(VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, 0, _noMVPVertices);
    glVertexAttribPointer(VERTEX_ATTRIB_COLOR, 4, GL_FLOAT, GL_FALSE, 0, _squareColors);
    
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    //CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1,4);
}

void Layer::setContentSize(const CCSize & size)
{
    _squareVertices[1].x = size.width;
    _squareVertices[2].y = size.height;
    _squareVertices[3].x = size.width;
    _squareVertices[3].y = size.height;
    
    Layer::setContentSize(size);
}

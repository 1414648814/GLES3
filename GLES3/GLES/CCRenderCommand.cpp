//
//  CCRenderCommand.cpp
//  GLES
//
//  Created by staff on 16/12/13.
//  Copyright © 2016年 George1994. All rights reserved.
//

#include "CCRenderCommand.h"


RenderCommand::RenderCommand() {
    this->_global_z_order = 0;
    this->_cmdType = COMMAND_UNKOWN;
}

RenderCommand::~RenderCommand() {
    
}

bool RenderCommand::init(float globalZOrder, const Matrix& transform) {
    _global_z_order = globalZOrder;
    
    
    return true;
}

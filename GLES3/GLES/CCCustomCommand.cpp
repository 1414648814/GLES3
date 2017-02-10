//
//  CustomCommand.cpp
//  GLES3
//
//  Created by staff on 17/2/10.
//  Copyright © 2017年 George. All rights reserved.
//

#include "CCCustomCommand.h"

CustomCommand::CustomCommand() {
    _cmdType = COMMAND_COMMON;
}

CustomCommand::~CustomCommand() {
    
}

void CustomCommand::init(float depth, const Matrix &modelViewTransform)
{
    RenderCommand::init(depth, modelViewTransform);
}

void CustomCommand::init(float globalOrder)
{
    _global_z_order = globalOrder;
}

void CustomCommand::execute()
{
    if(func)
    {
        func();
    }
}



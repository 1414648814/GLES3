//
//  CustomCommand.hpp
//  GLES3
//
//  Created by staff on 17/2/10.
//  Copyright © 2017年 George. All rights reserved.
//

#ifndef CustomCommand_hpp
#define CustomCommand_hpp

#include <stdio.h>
#include "CCRenderCommand.h"
#include "CCMatrix.h"
#include "CCPoint.h"

class CustomCommand : public RenderCommand {
public:
    CustomCommand();
    ~CustomCommand();
    
    
    void init(float globalZOrder, const Matrix& modelViewTransform);
    /**
     Init function. The render command will be in 2D mode.
     @param globalZOrder GlobalZOrder of the render command.
     */
    void init(float globalZOrder);
    void execute();
    
    /**Callback function.*/
    std::function<void()> func;
};


#endif /* CustomCommand_hpp */

//
//  CCLayer.hpp
//  GLES3
//
//  Created by staff on 17/2/9.
//  Copyright © 2017年 George. All rights reserved.
//

#ifndef CCLayer_hpp
#define CCLayer_hpp

#include <stdio.h>
#include <iostream>
#include <vector>
#include "CCMatrix.h"
#include "CCPoint.h"
#include "CCNode.h"
#include "CCCustomCommand.h"

class Layer : public Node {
public:
    static Layer* create();
    
    virtual void draw(Renderer *renderer, const Matrix &transform) override;
    virtual void setContentSize(const CCSize & var) override;
    
    Layer();
    virtual ~Layer() {}
    
protected:

    void onDraw(const Matrix& transform);
    
    Vector2 _squareVertices[4];
    Color4F  _squareColors[4];
    CustomCommand _customCommand;
    Vector3 _noMVPVertices[4];
};

#endif /* CCLayer_hpp */

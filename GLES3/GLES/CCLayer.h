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

class Layer {
public:
    static Layer* create();
    
protected:
    Layer();
    ~Layer();
};

#endif /* CCLayer_hpp */

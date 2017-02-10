//
//  HelloScene.hpp
//  GLES
//
//  Created by staff on 2016/12/27.
//  Copyright © 2016年 George1994. All rights reserved.
//

#ifndef HelloScene_hpp
#define HelloScene_hpp

#include <stdio.h>
#include "GLES.h"

class HelloScene : public Scene {
public:
    bool init();
    static Scene* scene();
    
protected:
    HelloScene();
    ~HelloScene();
};


#endif /* HelloScene_hpp */

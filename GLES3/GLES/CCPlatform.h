//
//  CCPlatform.hpp
//  GLES
//
//  Created by staff on 2016/12/27.
//  Copyright © 2016年 George1994. All rights reserved.
//

#ifndef CCPlatform_hpp
#define CCPlatform_hpp

#include <stdio.h>
#include "CCApplication.h"

class Platform : private Application {
public:
    Platform();
    virtual ~Platform() {}
    
    bool applicationDidFinishLaunching();
    
    void applicationDidEnterBackground();

    void applicationWillEnterForeground();
    
    void setAnimationInterval(double interval);
};

#endif /* CCPlatform_hpp */

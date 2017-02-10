//
//  CCGLView.hpp
//  GLES
//
//  Created by staff on 16/12/13.
//  Copyright © 2016年 George1994. All rights reserved.
//

#ifndef CCGLView_hpp
#define CCGLView_hpp
#include <iostream>
#include "CCPoint.h"

class GLView {
public:
    static GLView* getInstance();
    void destoryInstance();
    
    bool initWithRect(const std::string& viewName, CCRect rect, float frameZoomFactor);
    
    /**
     *  是否初始化glview
     */
    bool isGLViewReady();
    
    bool initEAGLView(void* glview);
    
    void swapBuffers();
    
    void* getEAGLView() const;
    
    void setViewPort(float x, float y);
    
    void setFrameSize(float w, float h);
    const CCSize& getFrameSize() const;
    
    CCSize getVisibleSize();
    
protected:
    GLView();
    ~GLView();
    
    CCSize _screenSize;
    float _scaleX;
    float _scaleY;
    
    void *_eaglview;
};


#endif /* CCGLView_hpp */

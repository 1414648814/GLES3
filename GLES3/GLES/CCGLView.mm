//
//  CCGLView.cpp
//  GLES
//
//  Created by staff on 16/12/13.
//  Copyright © 2016年 George1994. All rights reserved.
//

#import "CCGLView.h"
#import "EAGLView.h"

static GLView* view = nullptr;

GLView::GLView() :
_scaleX(1.0f),
_scaleY(1.0f)
{
    
}

GLView::~GLView() {
    
}

GLView* GLView::getInstance() {
    if (view == nullptr) {
        view = new GLView();
    }
    return view;
}

void GLView::destoryInstance() {
    delete view;
}

bool GLView::initWithRect(const std::string& viewName, CCRect rect, float frameZoomFactor)
{
    CGRect r = CGRectMake(rect.origin.x, rect.origin.y, rect.size.width, rect.size.height);
    
    EAGLView *eaglview = [EAGLView viewWithFrame: r];
    
    [eaglview setMultipleTouchEnabled:YES];
    
    _eaglview = (__bridge void *)eaglview;
    
    setFrameSize(rect.size.width, rect.size.height);
    
    return true;
}

bool GLView::initEAGLView(void *glview) {
    this->_eaglview = glview;
    EAGLView* view = (__bridge EAGLView*)this->_eaglview;
    setFrameSize([view getWidth], [view getHeight]);
    return true;
}

void GLView::swapBuffers() {
    EAGLView *eaglview = (__bridge EAGLView*) _eaglview;
    [eaglview swapBuffers];
}

void* GLView::getEAGLView() const {
    return _eaglview;
}

void GLView::setViewPort(float x, float y) {
    
}

void GLView::setFrameSize(float w, float h) {
    this->_screenSize = CCSize(w, h);
}

const CCSize& GLView::getFrameSize() const {
    return this->_screenSize;
}

CCSize GLView::getVisibleSize() {
    return CCSize(this->_screenSize.width/this->_scaleX, this->_screenSize.height/this->_scaleY);
}

bool GLView::isGLViewReady() {
    return this->_eaglview != nullptr;
}
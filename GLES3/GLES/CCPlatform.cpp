//
//  CCPlatform.cpp
//  GLES
//
//  Created by staff on 2016/12/27.
//  Copyright © 2016年 George1994. All rights reserved.
//

#include "CCPlatform.h"
#include "HelloScene.h"
#include "GLES.h"

Platform::Platform() : Application() {
    
}

bool Platform::applicationDidFinishLaunching() {
    // initialize director
    Game *game = Game::getInstance();
    GLView* glView = GLView::getInstance();
    
    game->setGLView(glView);

    
    // set FPS. the default value is 1.0/60 if you don't call this
    game->setAnimationInterval(1.0 / 60);
    
    // create a scene. it's an autorelease object
    Scene *pScene = HelloScene::scene();
    
    // run
    game->runScene(pScene);
    
    return true;
}

void Platform::applicationDidEnterBackground() {
    Game::getInstance()->stopAnimation();

}

void Platform::applicationWillEnterForeground() {
    Game::getInstance()->startAnimation();
}

void Platform::setAnimationInterval(double interval)
{
    Game::getInstance()->setAnimationInterval(interval);
}

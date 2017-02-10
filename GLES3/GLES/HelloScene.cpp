//
//  HelloScene.cpp
//  GLES
//
//  Created by staff on 2016/12/27.
//  Copyright © 2016年 George1994. All rights reserved.
//

#include "HelloScene.h"
#include "CCGame.h"

HelloScene::HelloScene() : Scene() {

}

HelloScene::~HelloScene() {
    
    
}

Scene* HelloScene::scene() {
    HelloScene* scene = new HelloScene();
    if (scene && scene->init()) {
        return scene;
    }
    
    return nullptr;
}

// insert code into here
bool HelloScene::init() {
    if (!Scene::init()) {
        return false;
    }
    
    auto visibleSize = Game::getInstance()->getVisibleSize();
    
    auto sp = Sprite::create("/Users/staff/Desktop/GLES3/GLES3/wall.jpg", 1);
    sp->setAnchorPoint(Vector2(0.5, 0.5));
    sp->setPosition(Vector2(visibleSize.width/2, visibleSize.height/2));
    this->addNode(sp);
    return true;
}
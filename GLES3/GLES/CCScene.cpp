//
//  CCScene.cpp
//  GLES
//
//  Created by staff on 2016/12/26.
//  Copyright © 2016年 George1994. All rights reserved.
//

#include "CCScene.h"
#include "CCRenderer.h"
#include "CCGame.h"
#include "CCCamera.h"

Scene::Scene() {
    this->_render = NULL;
    this->_defaultCamera = Camera::create();
    this->_modelMatrix.identity();
    
    this->_nodes.clear();
    this->addChild(this->_defaultCamera, 1);
}

bool Scene::init() {
    auto size = Game::getInstance()->getVisibleSize();
    return initWithSize(size);
}

bool Scene::initWithSize(const CCSize& size) {
    setContentSize(size);
    return true;
}

Scene* Scene::create() {
    Scene *scene = new Scene();
    if (scene && scene->init()) {
        return scene;
    }
    return nullptr;
}

// 场景的绘制
void Scene::render(Renderer* render) {
    if (render != NULL) {
        auto game = Game::getInstance();
        
        // 默认的摄像头，暂时只允许使用一个照相机，并且是默认的，不允许添加其它自定义照相机
        Camera::_visitingCamera = this->_defaultCamera;
        
        // 注意这个传递的是个矩阵地址，并且是个单位矩阵
        const auto& modelView = this->getSceneTransform();
        
        game->pushMatrix(MATRIX_STACK_PROJECTION);
        game->loadMatrix(Camera::_visitingCamera->getViewProjectionMatrix(), MATRIX_STACK_PROJECTION);
        
        this->_render = render;
        
        // 访问所有子结点
        visit(render, modelView);
        
        // 开始渲染
        this->_render->render();
        
        game->popMatrix(MATRIX_STACK_PROJECTION);
        
        Camera::_visitingCamera = nullptr;
    }
}

// 获取scene的位置
Matrix Scene::getSceneTransform() {
    Matrix transform;
    transform.identity();
    
    return transform;
}

Camera* Scene::getDefaultCamera() {
    return this->_defaultCamera;
}

//
//  CCScene.cpp
//  GLES
//
//  Created by staff on 2016/12/26.
//  Copyright © 2016年 George1994. All rights reserved.
//

#include "CCScene.h"
#include "CCNode.h"
#include "CCSprite.h"
#include "CCRenderer.h"
#include "CCGame.h"
#include "CCCamera.h"

Scene::Scene() {
    this->_render = NULL;
    this->_defaultCamera = Camera::create();
    this->addNode(this->_defaultCamera);
    this->_modelMatrix.identity();
    
    this->_nodes.clear();
}

bool Scene::init() {
    
    return true;
}

Scene* Scene::create() {
    Scene *scene = new Scene();
    if (scene && scene->init()) {
        return scene;
    }
    return nullptr;
}

// 添加子节点
void Scene::addNode(Node* node) {
    for (auto it = this->_nodes.begin(); it != this->_nodes.end(); it++) {
        if (*it == node) {
            std::cout << "the node already added." << std::endl;
            return ;
        }
    }
    this->_nodes.push_back(node);
}

/**
 *  递归访问结点并且进行渲染
 */
void Scene::visitNode() {
    for (auto it = this->_nodes.begin(); it != this->_nodes.end(); it++) {
        auto node = *it;
        if (node) {
            node->visit();
        }
    }
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
        game->loadMatrix(modelView, MATRIX_STACK_PROJECTION);
        
        this->_render = render;
        
        // 访问所有子结点
        this->visitNode();
        
        // 开始渲染
        this->_render->render();
        
        game->popMatrix(MATRIX_STACK_PROJECTION);
        
        Camera::_visitingCamera = nullptr;
    }
}

void Scene::removeAllNode() {
    this->_nodes.clear();
}

void Scene::removeNode(Node* node) {
    for (auto itr = this->_nodes.begin(); itr != this->_nodes.end(); itr++) {
        if (*itr == node) {
            if ((*itr)->getChildrenCnt() == 0) {
                return ;
            }
            else {
                (*itr)->removeChidren();
                return ;
            }
        }
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

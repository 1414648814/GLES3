//
//  CCScene.hpp
//  GLES
//
//  Created by staff on 2016/12/26.
//  Copyright © 2016年 George1994. All rights reserved.
//

#ifndef CCScene_hpp
#define CCScene_hpp

#include <stdio.h>
#include <iostream>
#include <vector>
#include "CCMatrix.h"
#include "CCPoint.h"

class Sprite;
class Node;
class Renderer;
class Camera;

class Scene {
public:
    /**
     *  初始化
     */
    virtual bool init();
    
    /**
     *  添加结点
     */
    void addNode(Node*);
    
    /**
     *  删除结点
     */
    void removeAllNode();
    void removeNode(Node*);
    
    /**
     *  绘制结点
     */
    void render(Renderer*);
    
    /**
     *  访问结点
     */
    void visitNode();
    
    static Scene* create();
    
    Matrix getSceneTransform();
    
    Camera* getDefaultCamera();
    
protected:
    friend class Node;
    friend class Sprite;
    friend class Renderer;
    
    Scene();
    virtual ~Scene() {};
    
private:
    Renderer* _render;
    Matrix _modelMatrix;
    std::vector<Node *> _nodes;
    Camera* _defaultCamera;
};



#endif /* CCScene_hpp */
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
#include "CCNode.h"

class Renderer;
class Camera;

class Scene : public Node {
public:
    
    static Scene* create();
    
    /**
     *  初始化
     */
    bool init() override;
    bool initWithSize(const CCSize& size);
    
    using Node::addChild;
    
    /**
     *  绘制结点
     */
    virtual void render(Renderer*);
    
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
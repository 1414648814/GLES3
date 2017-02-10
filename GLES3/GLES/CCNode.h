//
//  CCNode.hpp
//  GLES
//
//  Created by staff on 16/12/13.
//  Copyright © 2016年 George1994. All rights reserved.
//

#ifndef CCNode_hpp
#define CCNode_hpp

#include "CCConfig.h"
#include "CCVector2.h"
#include "CCVector3.h"
#include "CCVector4.h"
#include "CCQuaternion.h"
#include "CCMatrix.h"
#include <stdio.h>
#include <new>
#include <iostream>
#include <vector>
#include <string>

class Renderer;
class GLView;
class ProgramState;
class Program;
class Game;

class Node {
public:
    /**
     *  构造函数
     */
    Node();
    virtual ~Node() {};
    
    /**
     *  初始化
     */
    virtual bool init(void);
    
    static Node* create();
    
    /**
     *  绘制
     */
    virtual void draw(Renderer*, const Matrix&);
    
    /**
     *  访问节点
     */
    virtual void visit();
    virtual void visit(Renderer*, const Matrix&);
    
    // 计算模型矩阵
    const Matrix& getNodeToParentTransform() const;
    Matrix transform(const Matrix &parentTransform);
    void caculateTransform(const Matrix&);
    
    /**
     *  对program进行操作
     */
    Program* getProgram();
    void setProgram(Program*);
    
    /**
     *  对programstate进行操作
     */
    ProgramState* getProgramState();
    void setProgramState(ProgramState*);
    
    /**
     *  设置contentsize
     */
    virtual void setContentSize(const CCSize& size);
    virtual const CCSize& getContentSize() const;
    
    virtual const Vector2& getPosition() const;
    virtual void setPosition(const Vector2& position);
    
    virtual const Vector2& getAnchorPoint() const;
    virtual void setAnchorPoint(const Vector2&);
    
    virtual const Vector2& getAnchorPointInPoints() const;
    
    virtual void setNodeName(std::string name);
    virtual const std::string getNodeName() const;
    
    void addChild(Node*, ssize_t);
    
    inline std::vector<Node *> getChildren() const {
        return _childrens;
    }
    
    inline size_t getChildrenCnt() {
        return _childrens.size();
    }
    
    inline const Matrix& getModelView() const {
        return _modelViewTransform;
    }
    
    // 获取3d的位置
    virtual void setPosition3D(Vector3 pos);
    virtual const Vector3 getPosition3D() const;
    
    //
    virtual void updateRotation3D();
    
    // 设置Quat
    virtual Quaternion getRotationQuat() const;
    virtual void setRotationQuat(const Quaternion& quat);
    
    // 是否在照相机的视角内，决定要不要去绘制这个节点
    virtual bool isVisitableByVisitingCamera() const;
    
    virtual void removeChidren();
    
    virtual inline int getLocalZOrder() {
        return _localZOrder;
    }
    virtual inline int getGlocalZOrder() {
        return _globalZOrder;
    }
    
protected:
    Game * _game;
    Renderer * _renderer;
    ProgramState* _shaderState; //渲染管线的管理状态
    std::vector<Node*> _childrens; //子节点
    Matrix _modelViewTransform;
    mutable bool _isUpdateTransform; //是否有设置过位置
    Vector2 _position; //位置
    float _positionZ; //opengl中的深度
    bool _isVisible;
    std::string _nodeName;
    
    Vector2 _anchorPointInPoints; //存在于一个节点中的锚点
    Vector2 _anchorPoint; //不存在于任何节点中的锚点
    Quaternion _rotationQuat;
    mutable Matrix _transform;
    Vector3 _postion3D; //3d的位置
    
    float _rotationX; //x轴上旋转
    float _rotationY; //y轴上旋转
    
    // rotation Z is decomposed in 2 to simulate Skew for Flash animations
    float _rotationZ_X;             ///< rotation angle on Z-axis, component X
    float _rotationZ_Y;             ///< rotation angle on Z-axis, component Y
    
    float _scaleX;                  ///< scaling factor on x-axis
    float _scaleY;                  ///< scaling factor on y-axis
    float _scaleZ;                  ///< scaling factor on z-axis
    
    float _skewX;                   ///< skew angle on x-axis
    float _skewY;                   ///< skew angle on y-axis
    
    CCSize _contentSize;              ///< untransformed size of the node
    bool _isUpdateContentSize;
    
    int _localZOrder; //z轴上的顺序
    int _globalZOrder; //z轴上的顺序
};

#endif /* CCNode_hpp */

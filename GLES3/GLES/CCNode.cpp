//
//  CCNode.cpp
//  GLES
//
//  Created by staff on 16/12/13.
//  Copyright © 2016年 George1994. All rights reserved.
//

#include "CCNode.h"
#include "CCRenderer.h"
#include "CCProgram.h"
#include "CCGLView.h"
#include "CCGame.h"
#include "CCCamera.h"

Node::Node() :
  _renderer(NULL)
, _shaderState(NULL)
, _isUpdateTransform(false)
, _position(Vector2(0,0))
, _positionZ(0)
, _localZOrder(0)
, _globalZOrder(0)
, _isVisible(true)
, _rotationX(0.0f)
, _rotationY(0.0f)
, _rotationZ_X(0.0f)
, _rotationZ_Y(0.0f)
, _scaleX(1.0f)
, _scaleY(1.0f)
, _scaleZ(1.0f)
, _skewX(0.0f)
, _skewY(0.0f)
, _contentSize(CCSize::ZERO)
, _isUpdateContentSize(false)
{
    this->_game = Game::getInstance();
    this->_transform = Matrix::identity();
}


bool Node::init() {
    this->_renderer = new (std::nothrow) Renderer;
    return true;
}

Node* Node::create() {
    Node* node = new Node();
    if (node && node->init()) {
        return node;
    }
    return nullptr;
}

Program* Node::getProgram() {
    if (this->_shaderState) {
        return this->_shaderState->getProgram();
    }
    return NULL;
}

void Node::setProgram(Program* program) {
    if (this->_shaderState == NULL || (this->_shaderState && this->_shaderState->getProgram() != program)) {
        this->_shaderState = ProgramState::create(program);
    }
}

ProgramState* Node::getProgramState() {
    return this->_shaderState;
}

void Node::setProgramState(ProgramState* state) {
    if (this->_shaderState != state) {
        this->_shaderState = state;
    }
}

void Node::setContentSize(const CCSize& size) {
    if (!this->_contentSize.equals(size)) {
        this->_contentSize = size;
        this->_anchorPointInPoints.set(_contentSize.width * _anchorPoint.x, _contentSize.height * _anchorPoint.y);
        this->_isUpdateContentSize = this->_isUpdateTransform = true;
    }
}

const CCSize& Node::getContentSize() const{
    return this->_contentSize;
}

const Vector2& Node::getPosition() const {
    return this->_position;
}

void Node::setPosition(const Vector2& position) {
    this->_position.x = position.x;
    this->_position.y = position.y;
    
    this->_isUpdateTransform = true;
}

void Node::setPosition3D(Vector3 pos) {
    this->_positionZ = pos.z;
    this->_position.set(pos.x, pos.y);
}

const Vector3 Node::getPosition3D() const {
    return this->_postion3D;
}

void Node::setNodeName(std::string name) {
    this->_nodeName = name;
}

const std::string Node::getNodeName() const {
    return this->_nodeName;
}

const Vector2& Node::getAnchorPoint() const {
    return this->_anchorPoint;
}

void Node::setAnchorPoint(const Vector2& point) {
    if (point != _anchorPoint) {
        this->_anchorPoint = point;
        this->_anchorPointInPoints.set(this->_contentSize.width * this->_anchorPoint.x, this->_contentSize.height * this->_anchorPoint.y);
        this->_isUpdateTransform = true;
    }
}

const Vector2& Node::getAnchorPointInPoints() const {
    return this->_anchorPointInPoints;
}

void Node::updateRotation3D()
{
    //convert quaternion to Euler angle
    float x = _rotationQuat.x, y = _rotationQuat.y, z = _rotationQuat.z, w = _rotationQuat.w;
    _rotationX = atan2f(2.f * (w * x + y * z), 1.f - 2.f * (x * x + y * y));
    float sy = 2.f * (w * y - z * x);
    sy = clampf(sy, -1, 1);
    _rotationY = asinf(sy);
    _rotationZ_X = atan2f(2.f * (w * z + x * y), 1.f - 2.f * (y * y + z * z));
    
    _rotationX = CC_RADIANS_TO_DEGREES(_rotationX);
    _rotationY = CC_RADIANS_TO_DEGREES(_rotationY);
    _rotationZ_X = _rotationZ_Y = -CC_RADIANS_TO_DEGREES(_rotationZ_X);
}

void Node::setRotationQuat(const Quaternion& quat)
{
    _rotationQuat = quat;
    updateRotation3D();
    this->_isUpdateTransform = true;
}

Quaternion Node::getRotationQuat() const
{
    return _rotationQuat;
}

void Node::visit() {
    Renderer* render = this->_game->getRenderer();
    // 模型视图矩阵
    auto& parent_modelview = this->_game->getMatrix(MATRIX_STACK_MODELVIEW);
    this->visit(render, parent_modelview);
}

void Node::visit(Renderer* render, const Matrix& parent_modelview) {
    // 不可见则不进行绘制
    if (!this->_isVisible) {
        return;
    }
    
    // 根据父亲节点矩阵计算出该节点的矩阵位置
    this->caculateTransform(parent_modelview);
    
    bool visibleByCamera = this->isVisitableByVisitingCamera();
    
    // 节点都是模型矩阵
    this->_game->pushMatrix(MATRIX_STACK_MODELVIEW);
    this->_game->loadMatrix(this->_modelViewTransform, MATRIX_STACK_MODELVIEW);
    
    if (!this->_childrens.empty()) {
        // 绘制自己
        if (visibleByCamera) {
            this->draw(render, this->_modelViewTransform);
        }
        
        // 访问子节点
        for (auto itr = this->_childrens.begin(); itr != this->_childrens.end(); itr++) {
            (*itr)->visit(render, _modelViewTransform);
        }
    }
    else if(visibleByCamera) {
        this->draw(render, this->_modelViewTransform);
    }
    
    this->_game->popMatrix(MATRIX_STACK_MODELVIEW);
}

// 这是一个虚函数，子类用来实现
void Node::draw(Renderer* render, const Matrix& transform) {
    
    
}

// 求出子节点相对于父亲节点的坐标
Matrix Node::transform(const Matrix &parentTransform) {
    return parentTransform * this->getNodeToParentTransform();
}

void Node::caculateTransform(const Matrix& parentTransform) {
    if (this->_isUpdateTransform || this->_isUpdateContentSize) {
        this->_modelViewTransform = this->transform(parentTransform);
        
        this->_isUpdateTransform = false;
        this->_isUpdateContentSize = false;
    }
}

// 获取相对于父亲的节点位置
const Matrix& Node::getNodeToParentTransform() const {
    /**
     *  是否有更新位置，只要调用过setPositons，其就会变为false
     */
    if (this->_isUpdateTransform) {
        
        float x = this->_position.x;
        float y = this->_position.y;
        float z = this->_positionZ;
        
        // 是否旋转
        bool needsSkewMatrix = (_skewX || _skewY);
        // 计算缩放
        Vector2 anchorPoint(this->_anchorPointInPoints.x * this->_scaleX, this->_anchorPointInPoints.y * this->_scaleY);
        
        // calculate real position
        // 计算真正的位置
        if (! needsSkewMatrix && !_anchorPointInPoints.isZero())
        {
            x += -anchorPoint.x;
            y += -anchorPoint.y;
        }
    
        // Build Transform Matrix = translation * rotation * scale
        Matrix translation;
        //move to anchor point first, then rotate
        // 移动到锚点的位置
        Matrix::createTranslation(x + anchorPoint.x, y + anchorPoint.y, z, &translation);
        // 旋转
        Matrix::createRotation(_rotationQuat, &_transform);
        
        if (_rotationZ_X != _rotationZ_Y)
        {
            // Rotation values
            // Change rotation code to handle X and Y
            // If we skew with the exact same value for both x and y then we're simply just rotating
            float radiansX = -CC_DEGREES_TO_RADIANS(_rotationZ_X);
            float radiansY = -CC_DEGREES_TO_RADIANS(_rotationZ_Y);
            float cx = cosf(radiansX);
            float sx = sinf(radiansX);
            float cy = cosf(radiansY);
            float sy = sinf(radiansY);
            
            float m0 = _transform.m[0], m1 = _transform.m[1], m4 = _transform.m[4], m5 = _transform.m[5], m8 = _transform.m[8], m9 = _transform.m[9];
            _transform.m[0] = cy * m0 - sx * m1, _transform.m[4] = cy * m4 - sx * m5, _transform.m[8] = cy * m8 - sx * m9;
            _transform.m[1] = sy * m0 + cx * m1, _transform.m[5] = sy * m4 + cx * m5, _transform.m[9] = sy * m8 + cx * m9;
        }
        
        _transform = translation * _transform;
        //move by (-anchorPoint.x, -anchorPoint.y, 0) after rotation
        _transform.translate(-anchorPoint.x, -anchorPoint.y, 0);
        
        //
        if (_scaleX != 1.f)
        {
            _transform.m[0] *= _scaleX, _transform.m[1] *= _scaleX, _transform.m[2] *= _scaleX;
        }
        if (_scaleY != 1.f)
        {
            _transform.m[4] *= _scaleY, _transform.m[5] *= _scaleY, _transform.m[6] *= _scaleY;
        }
        if (_scaleZ != 1.f)
        {
            _transform.m[8] *= _scaleZ, _transform.m[9] *= _scaleZ, _transform.m[10] *= _scaleZ;
        }

        if (needsSkewMatrix)
        {
            float skewMatArray[16] =
            {
                1, (float)tanf(CC_DEGREES_TO_RADIANS(_skewY)), 0, 0,
                (float)tanf(CC_DEGREES_TO_RADIANS(_skewX)), 1, 0, 0,
                0,  0,  1, 0,
                0,  0,  0, 1
            };
            Matrix skewMatrix(skewMatArray);
            
            _transform = _transform * skewMatrix;
            
            // adjust anchor point
            if (!_anchorPointInPoints.isZero())
            {
                // FIXME:: Argh, Mat4 needs a "translate" method.
                // FIXME:: Although this is faster than multiplying a vec4 * mat4
                _transform.m[12] += _transform.m[0] * -_anchorPointInPoints.x + _transform.m[4] * -_anchorPointInPoints.y;
                _transform.m[13] += _transform.m[1] * -_anchorPointInPoints.x + _transform.m[5] * -_anchorPointInPoints.y;
            }
        }
        
        _isUpdateTransform = false;
    }
    
    return _transform;
}

bool Node::isVisitableByVisitingCamera() const
{
    auto camera = Camera::getVisitingCamera();
    //bool visibleByCamera = camera ? ((unsigned short)camera->getCameraFlag() & _cameraMask) != 0 : true;
    bool visibleByCamera = true;
    return visibleByCamera;
}

void Node::addChild(Node* node, ssize_t tag) {
    // 避免重复添加子节点
    for (auto itr = this->_childrens.begin(); itr != this->_childrens.end(); itr++) {
        if (*itr && *itr == node) {
            std::cout << "add duplicate child node..." << std::endl;
            return ;
        }
    }
    
    this->_childrens.push_back(node);
}

void Node::removeChidren() {
    if (this->_childrens.empty()) {
        return ;
    }
    else {
        this->_childrens.clear();
    }
}

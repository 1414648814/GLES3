//
//  CCCamera.cpp
//  GLES3
//
//  Created by staff on 17/2/6.
//  Copyright © 2017年 George. All rights reserved.
//

#include "CCCamera.h"
#include "CCScene.h"
#include "CCGame.h"

Camera* Camera::_visitingCamera = nullptr;

Camera::Camera() :
_scene(nullptr)
, _viewProjectionDirty(true)
{
    _view.identity();
    _viewInv.identity();
    _viewProjection.identity();
}

Camera::~Camera() {
    
    
}

// 三种创建相机的方式
Camera* Camera::createPerspective(float fieldOfView, float aspectRatio, float nearPlane, float farPlane) {
    auto ret = new (std::nothrow) Camera();
    if (ret)
    {
        ret->initPerspective(fieldOfView, aspectRatio, nearPlane, farPlane);
        return ret;
    }
    return nullptr;
}

Camera* Camera::createOrthographic(float zoomX, float zoomY, float nearPlane, float farPlane) {
    auto ret = new (std::nothrow) Camera();
    if (ret)
    {
        ret->initOrthographic(zoomX, zoomY, nearPlane, farPlane);
        return ret;
    }
    return nullptr;
}

Camera* Camera::create() {
    Camera* camera = new (std::nothrow) Camera();
    if (camera && camera->initDefault()) {
        return camera;
    }
    
    return nullptr;
}

bool Camera::initDefault() {
    auto size = Game::getInstance()->getVisibleSize();
    // 创建默认的照相机
    auto projection = Game::getInstance()->getProjection();
    switch (projection)
    {
        case Game::Projection::_3D:
        {
            float zeye = Game::getInstance()->getZEye();
            initPerspective(60, (GLfloat)size.width / size.height, 10, zeye + size.height / 2.0f);
            Vector3 eye(size.width/2, size.height/2.0f, zeye), center(size.width/2, size.height/2, 0.0f), up(0.0f, 1.0f, 0.0f);
            setPosition3D(eye);
            lookAt(center, up);
            break;
        }
        default:
            std::cout << "unrecognized projection" << std::endl;
            break;
    }
    return true;
}

bool Camera::initPerspective(float fieldOfView, float aspectRatio, float nearPlane, float farPlane) {
    Matrix::createPerspective(fieldOfView, aspectRatio, nearPlane, farPlane, &_projection);
    return true;
}

bool Camera::initOrthographic(float zoomX, float zoomY, float nearPlane, float farPlane) {
    Matrix::createOrthographicOffCenter(0, zoomX, 0, zoomY, nearPlane, farPlane, &_projection);
    return true;
}

// 设置场景以及相关的照相机
void Camera::setScene(Scene* scene) {
    if (_scene != scene)
    {
        //remove old scene
        if (_scene)
        {
            _scene = nullptr;
        }
        //set new scene
        if (scene)
        {
            _scene = scene;
        }
    }
}

Camera* Camera::getDefaultCamera() {
    auto scene = Game::getInstance()->getRunningScene();
    if(scene)
    {
        return scene->getDefaultCamera();
    }
    
    return nullptr;
}

void Camera::lookAt(const Vector3& lookAtPos, const Vector3& up) {
    // z轴
    //camera->lookAt必须在camera->setPostion3D之后，因为其在运行过程中调用了getPosition3D()
    //定义y方向的归一化向量。
    Vector3 upv = up;
    upv.normalize();
    
    //计算x、y、z、方向上的向量。
    Vector3 zaxis;
    Vector3::subtract(this->getPosition3D(), lookAtPos, &zaxis);
    zaxis.normalize();
    
    Vector3 xaxis;
    Vector3::cross(upv, zaxis, &xaxis);
    xaxis.normalize();
    
    Vector3 yaxis;
    Vector3::cross(zaxis, xaxis, &yaxis);
    yaxis.normalize();
    
    //将上面计算的向量值构造旋转矩阵
    Matrix  rotation;
    rotation.m[0] = xaxis.x;
    rotation.m[1] = xaxis.y;
    rotation.m[2] = xaxis.z;
    rotation.m[3] = 0;
    rotation.m[4] = yaxis.x;
    rotation.m[5] = yaxis.y;
    rotation.m[6] = yaxis.z;
    rotation.m[7] = 0;
    rotation.m[8] = zaxis.x;
    rotation.m[9] = zaxis.y;
    rotation.m[10] = zaxis.z;
    rotation.m[11] = 0;
    
    /*
     定义四元数，将旋转矩阵转换为四元数。
     通过四元数来设置3D空间中的旋转角度。要保证四元数是经过归一化的。
     */
    Quaternion  quaternion;
    Quaternion::createFromRotationMatrix(rotation,&quaternion);
    quaternion.normalize();
    setRotationQuat(quaternion);
}

const Matrix& Camera::getProjectionMatrix() const {
    return this->_projection;
}

const Matrix& Camera::getViewMatrix() const {
    Matrix viewInv;
    viewInv.identity();
    
    Matrix trans = getNodeToParentTransform();
    viewInv.multiply(trans);
    
    static int count = sizeof(float) * 16;
    if (memcmp(viewInv.m, _viewInv.m, count) != 0)
    {
        _viewProjectionDirty = true;
        //_frustumDirty = true;
        _viewInv = viewInv;
        _view = viewInv.getInversed();
    }
    return _view;
}

const Matrix& Camera::getViewProjectionMatrix() const {
    getViewMatrix();
    if (_viewProjectionDirty)
    {
        _viewProjectionDirty = false;
        Matrix::multiply(_projection, _view, &_viewProjection);
    }
    
    return this->_viewProjection;
}

const Camera* Camera::getVisitingCamera() {
    return _visitingCamera;
}


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

Camera::Camera() : _scene(nullptr){
    
    
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
            float zeye = 1.0f;//Game::getInstance()->getZEye();
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
    Vector3 upv = up;
    upv.normalize();
    Vector3 zaxis;
    Vector3::subtract(this->getPosition3D(), lookAtPos, &zaxis);
    zaxis.normalize();
    
    // x轴
    Vector3 xaxis;
    Vector3::cross(upv, zaxis, &xaxis);
    xaxis.normalize();
    
    // y轴
    Vector3 yaxis;
    Vector3::cross(zaxis, xaxis, &yaxis);
    yaxis.normalize();
    
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
    
    Quaternion  quaternion;
    Quaternion::createFromRotationMatrix(rotation,&quaternion);
    quaternion.normalize();
    setRotationQuat(quaternion);
}

const Matrix& Camera::getProjectionMatrix() const {
    return this->_projection;
}

const Matrix& Camera::getViewMatrix() const {
    return this->_view;
}

const Matrix& Camera::getViewProjectionMatrix() const {
    return this->_viewProjection;
}

const Camera* Camera::getVisitingCamera() {
    return _visitingCamera;
}


//
//  CCCamera.hpp
//  GLES3
//
//  Created by staff on 17/2/6.
//  Copyright © 2017年 George. All rights reserved.
//

#ifndef CCCamera_hpp
#define CCCamera_hpp

#include <stdio.h>
#include <iostream>
#include "CCConfig.h"
#include "CCPoint.h"
#include "CCQuaternion.h"
#include "CCVector2.h"
#include "CCVector3.h"
#include "CCMatrix.h"
#include "CCNode.h"

class Scene;
class Game;

class Camera : public Node {
    friend class Scene;
    friend class Game;
    
public:
    enum class Type
    {
        PERSPECTIVE = 1,
        ORTHOGRAPHIC = 2
    };
    
    /**
     * Creates a perspective camera.
     *
     * @param fieldOfView The field of view for the perspective camera (normally in the range of 40-60 degrees).
     * @param aspectRatio The aspect ratio of the camera (normally the width of the viewport divided by the height of the viewport).
     * @param nearPlane The near plane distance.
     * @param farPlane The far plane distance.
     */
    // 创建投影相机
    static Camera* createPerspective(float fieldOfView, float aspectRatio, float nearPlane, float farPlane);
    /**
     * Creates an orthographic camera.
     *
     * @param zoomX The zoom factor along the X-axis of the orthographic projection (the width of the ortho projection).
     * @param zoomY The zoom factor along the Y-axis of the orthographic projection (the height of the ortho projection).
     * @param nearPlane The near plane distance.
     * @param farPlane The far plane distance.
     */
    // 创建正交相机
    static Camera* createOrthographic(float zoomX, float zoomY, float nearPlane, float farPlane);
    
    /** create default camera, the camera type depends on Director::getProjection, the depth of the default camera is 0 */
    // 创建默认的相机
    static Camera* create();
    
    Camera::Type getType() const { return _type; }
    
    // 初始化相机
    bool initDefault();
    bool initPerspective(float fieldOfView, float aspectRatio, float nearPlane, float farPlane);
    bool initOrthographic(float zoomX, float zoomY, float nearPlane, float farPlane);
    
    void setScene(Scene* scene);
    
    // 获取默认的相机
    static Camera* getDefaultCamera();
    
    /**
     * Make Camera looks at target
     *
     * @param target The target camera is point at
     * @param up The up vector, usually it's Y axis
     */
    virtual void lookAt(const Vector3& target, const Vector3& up);
    
    /**
     * Gets the camera's projection matrix.
     *
     * @return The camera projection matrix.
     */
    // 获取投影矩阵
    const Matrix& getProjectionMatrix() const;
    /**
     * Gets the camera's view matrix.
     *
     * @return The camera view matrix.
     */
    // 获取视图矩阵
    const Matrix& getViewMatrix() const;
    
    /**get view projection matrix*/
    // 获取视图投影矩阵
    const Matrix& getViewProjectionMatrix() const;
    
    static const Camera* getVisitingCamera();
    
    Camera();
    ~Camera();
    
public:
    Camera::Type _type;
    static Camera* _visitingCamera;
    Matrix _projection;
    mutable Matrix _view;
    mutable Matrix _viewInv;
    mutable Matrix _viewProjection;
    Scene* _scene; // 记录当前场景
    
    mutable bool  _viewProjectionDirty;
    bool _viewProjectionUpdated; //Whether or not the viewprojection matrix was updated since the last frame.
};

#endif /* CCCamera_hpp */

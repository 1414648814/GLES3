//
//  CCGame.hpp
//  GLES
//
//  Created by staff on 2016/12/26.
//  Copyright © 2016年 George1994. All rights reserved.
//

#ifndef CCGame_hpp
#define CCGame_hpp

#include <stdio.h>
#include <vector>
#include <stack>
#include "CCScene.h"
#include "CCRenderer.h"
#include "CCGLView.h"
#include "CCMatrix.h"
#include "CCConfig.h"

#include "SOIL.h"
#include "CCTriangleCommand.h"

class Game {
public:
    /**
     * @brief Possible OpenGL projections used by director
     */
    enum class Projection
    {
        /// Sets a 2D projection (orthogonal projection).
        _2D,
        
        /// Sets a 3D projection with a fovy=60, znear=0.5f and zfar=1500.
        _3D,
        
        /// It calls "updateProjection" on the projection delegate.
        CUSTOM,
        
        /// Default projection is 3D projection.
        DEFAULT = _3D,
    };

    
    static Game* getInstance();
    static void destroyInstance();
    
    // 设置关于GL相关的值
    void setGLDefaultValues();
    
    bool initGame();
    
    void drawScene();
    
    inline GLView* getGLView() const {
        return _glView;
    }
    
    void setGLView(GLView* view);
    
    void pushScene(Scene* scene);
    void popScene();
    void runScene(Scene*);
    
    void mainLoop();
    
    void startAnimation();
    void stopAnimation();
    
    /**
     *  设置时间间隔
     */
    void setAnimationInterval(double dValue);
    
    /**
     *  获取场景个数
     */
    size_t getSceneCnt();
    
    // 使用单位矩阵来初始化矩阵栈
    bool initMatrixStack();
    
    // 弹出矩阵
    void popMatrix(MATRIX_STACK_TYPE type);
    
    // 压入矩阵
    void pushMatrix(MATRIX_STACK_TYPE type);
    
    // 获取矩阵
    const Matrix& getMatrix(MATRIX_STACK_TYPE);
    
    // 压入一个矩阵
    void loadMatrix(const Matrix& mat, MATRIX_STACK_TYPE type);
    // 压入一个单位矩阵
    void loadIdentityMatrix(MATRIX_STACK_TYPE type);
    
    // 将矩阵相乘
    void multiplyMatrix(MATRIX_STACK_TYPE type, const Matrix& mat);
    
    Renderer* getRenderer();
    
    // 获取size
    CCSize getVisibleSize();
    
    // 设置视口
    void setViewport();
    
    CCSize getVisibleSize() const;
    
    Scene* getRunningScene();
    
    inline Projection getProjection() { return _projection; }
    /** Sets OpenGL projection. */
    void setProjection(Projection projection);

    float getZEye();
    
    void test();
    
protected:
    Renderer* _render;
    GLView* _glView;
    Scene* _curscene;
    std::vector<Scene *> _scenes;
    bool _isValid;
    double _interval;
    std::stack<Matrix> _modelViewStack;
    std::stack<Matrix> _projectionStack;
    CCSize _viewSize;
    Projection _projection;
    
protected:
    Game();
    ~Game();

};

#endif /* CCGame_hpp */

//
//  CCRenderCommand.hpp
//  GLES
//
//  Created by staff on 16/12/13.
//  Copyright © 2016年 George1994. All rights reserved.
//

#ifndef CCRenderCommand_hpp
#define CCRenderCommand_hpp

#include "CCMath.h"
#include "CCConfig.h"
#include <stdio.h>

class RenderCommand {
public:

    RenderCommand();
    virtual ~RenderCommand(); //用来作为基类的继承
    
    // 所有的渲染的指令会被调用，暂时默认使用的2d，不考虑3d，所以只使用第一个参数
    bool init(float, const Matrix&, bool);
    
    /**
     * 获取命令的类型
     */
    inline COMMAND_TYPE getCmdType() const {
        return _cmdType;
    }
    
    /**
     * 获取z轴的值
     */
    unsigned int getGlobalZOrder() const {
        return _global_z_order;
    }
    
    /**
     * 设置是否跳过批量绘制
     */
    inline void setSkipRendering(bool isSkipRender) {
        _isSkipRendering = isSkipRender;
    }
    
    inline bool getSkipRendering() const {
        return _isSkipRendering;
    }
    
public:
    COMMAND_TYPE _cmdType;
    
protected:
    unsigned int _global_z_order; //z轴值
    bool _isSkipRendering; //是否跳过绘制
};

#endif /* CCRenderCommand_hpp */
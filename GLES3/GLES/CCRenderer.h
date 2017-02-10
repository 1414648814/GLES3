//
//  CCRenderer.hpp
//  GLES
//
//  Created by staff on 16/12/13.
//  Copyright © 2016年 George1994. All rights reserved.
//

#ifndef CCRenderer_hpp
#define CCRenderer_hpp

#include "CCConfig.h"
#include "CCMatrix.h"
#include <stdio.h>
#include <string>
#include <iostream>
#include <map>
#include <cassert>
#include <vector>
    
class RenderCommand;
class TriangleCommand;
class Program;

class RendererQueue {
public:
    RendererQueue();
    ~RendererQueue();
    
    void push_back(RenderCommand *);
    
    RenderCommand* pop_back();
    
    void clear();

    size_t size();
    
    /**
     *  获取子队列
     *
     *  @param RENDER_QUEUE_TYPE 类型
     */
    std::vector<RenderCommand *> getSubQueue(RENDER_QUEUE_TYPE);
    
    /**
     *  保存渲染的状态
     */
    void saveRenderState();
    
    /**
     *  是否开启深度测试
     */
    bool getDepthEnabled();
    
    /**
     *  是否开启隐藏功能
     */
    bool getCullEnabled();
    
    /**
     *  是否允许写进深度缓存
     */
    bool getDepthWrited();
    
protected:
    std::vector<RenderCommand *> _renderCmds[MAX_RENDER_QUEUE_COUNT];
    GLboolean _isDepthEnabled; //是否开启深度测试
    GLboolean _isCullEnabled; //是否启用隐藏
    GLboolean _isDepthWrited; //是否允许写进深度缓存
};

class RenderState {
public:
    RenderState* getInstance();
    void destroyInstance();
    
private:
    RenderState();
    ~RenderState();
};


/// 渲染对象
class Renderer {
public:
    Renderer();
    ~Renderer();
    
    /**
     * 初始化view
     *
     * - returns: 是否初始化成功
     */
    bool initGLView();
    
    /**
     *  填充顶点数据
     */
    void filledVertexData(TriangleCommand *command);
    
    /**
     *  绘制三角形
     *
     *  @return 是否绘制成功
     */
    bool drawTriangleCmd();
    
    /**
     *  处理命令队列
     */
    void visitRenderQueue(RendererQueue &queue);
    
    /**
     *  处理命令队列
     */
    void handleRenderCommand(RenderCommand *command);
    
    /**
     *  设置没有VBO的情况下
     */
    void setVAOAndVBO();
    
    /**
     *  设置在有VBO的情况下
     */
    void setWithVBO();
    
    /**
     *  设置深度测试
     */
    void setDepthFor2DTest(bool enable);
    
    /**
     *  添加命令
     *
     *  @param RenderCommand 渲染命令
     *  @param renderType    渲染队列的类型
     */
    void addCommand(RenderCommand*, int);
    
    /**
     *  渲染
     */
    void render();
    
    /**
     *  重置绘制状态
     */
    void reset();
    
    /**
     *  清除绘制状态
     */
    void clean();
    
protected:
    // 用于绘制精灵
    V3F_C4B_T2F _triangleVertexs[MAX_VERTICES_SIZE];
    GLushort _triangleIndices[MAX_INDICES_SIZE];
    GLuint _triangleVAO; //VAO
    GLuint _triangleVBO[2]; //两个VBO， 一个用来存储顶点相关的数据，一个用来存储顶点的索引
    std::vector<TriangleCommand*> _batchedCommands;
    unsigned int _filledVertex; //已经被填充的顶点
    unsigned int _filledIndex; //已经被填充的索引
    
    Program *_program;
    bool _isRendering; //是否正在渲染
    bool _isDepthTestFor2D; //是否用来专门d的深度测刷
    
    Color4F _clearColor; //清除的颜色
    
    bool _initedView; //是否初始化
    
    std::vector<RendererQueue> _renderQueues;
};

#endif /* CCRenderer_hpp */

//
//  CCRenderer.cpp
//  GLES
//
//  Created by staff on 16/12/13.
//  Copyright © 2016年 George1994. All rights reserved.
//
#include "CCGL.h"
#include "CCRenderer.h"
#include "CCGLView.h"
#include "CCTexture.h"
#include "CCTriangleCommand.h"
#include "CCOPGL.h"
#include "CCProgram.h"

/*
 * --------------------------------RendererQueue--------------------------------
 */
RendererQueue::RendererQueue() {
    this->_isCullEnabled = GL_FALSE;
    this->_isDepthEnabled = GL_FALSE;
}

RendererQueue::~RendererQueue() {

}

void RendererQueue::push_back(RenderCommand *command) {
    if (command->getGlobalZOrder() > 0) {
        this->_renderCmds[RENDER_QUEUE_MORE_ZORDER].push_back(command);
    }
}

size_t RendererQueue::size() {
    size_t result = 0;
    
    for (int i = 0 ; i < MAX_RENDER_QUEUE_COUNT; i++) {
        result += this->_renderCmds[i].size();
    }
    
    return result;
}

void RendererQueue::clear() {
    for (int i = 0 ; i < MAX_RENDER_QUEUE_COUNT; i++) {
        this->_renderCmds[i]. clear();
    }
}

void RendererQueue::saveRenderState() {
    this->_isDepthEnabled = glIsEnabled(GL_DEPTH_TEST);
    this->_isCullEnabled = glIsEnabled(GL_CULL_FACE);
    
    glGetBooleanv(GL_DEPTH_WRITEMASK, &this->_isDepthWrited);
}

/**
 *  是否开启深度测试
 */
bool RendererQueue::getDepthEnabled() {
    return this->_isDepthEnabled;
}

/**
 *  是否开启隐藏功能
 */
bool RendererQueue::getCullEnabled() {
    return this->_isCullEnabled;
}

std::vector<RenderCommand *> RendererQueue::getSubQueue(RENDER_QUEUE_TYPE type) {
    return this->_renderCmds[type];
}

/*
 * --------------------------------Renderer--------------------------------
 */
Renderer::Renderer() :
_triangleVAO(-1)
, _isRendering(false)
, _isDepthTestFor2D(false)
, _filledVertex(0)
, _filledIndex(0)
, _initedView(false){
    
    RendererQueue queue;
    this->_renderQueues.push_back(queue);
    this->_clearColor = Color4F::WHITE;
    this->_program = new Program();
    this->reset();
}

Renderer::~Renderer() {
    this->_renderQueues.clear();
    
    // 删除VBO
    glDeleteBuffers(2, this->_triangleVBO);
//#if (SUPPORT_USE_VAO == 1)
//    // 删除VAO
//    glDeleteVertexArrays(1, &this->_triangleVAO);
//    glBindVertexArray(0);
//#endif
    
}

bool Renderer::initGLView() {
    this->setVAOAndVBO();
    this->_initedView = true;
    return true;
}

/**
 *  设置同时存在VAO和VBO的情况下
 */
void Renderer::setVAOAndVBO() {
    // 1. 生成VAO
    glGenVertexArrays(1, &this->_triangleVAO);
    // 2.绑定VAO，避免修改
    OPGL::bindVAO(this->_triangleVAO);
    // 3.在VAO中生成两个VBO
    glGenBuffers(2, &this->_triangleVBO[0]);
    // 4. 将顶点信息存放进第一个buffer中
    glBindBuffer(GL_ARRAY_BUFFER, this->_triangleVBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->_triangleVertexs[0]) * MAX_VERTICES_SIZE, this->_triangleVertexs, GL_DYNAMIC_DRAW);
    
    // 分别填充顶点信息到数组中
    // 4.1顶点
    this->_program->setVertexAttribPointer(VERTEX_ATTRIB_POSITION);
    
    // 4.2颜色
    this->_program->setVertexAttribPointer(VERTEX_ATTRIB_COLOR);
    
    // 4.3纹理坐标
    this->_program->setVertexAttribPointer(VERTEX_ATTRIB_TEXCOOD);
    
    // 5.更改到第二个buffer中
    glBindVertexArray(0); //必须先unbind
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_triangleVBO[1]);
    // GL_STATIC_DRAW数据不会或几乎不会发生改变
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(this->_triangleIndices[0]) * MAX_INDICES_SIZE, this->_triangleIndices, GL_STATIC_DRAW);
    
    OPGL::bindVAO(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    std::cout << "1.设置顶点数组缓冲区..." << std::endl;
}

/**
 *  设置在只有VBO，而没有VAO的情况下
 */
void Renderer::setWithVBO() {
    // 先生成两个VBO
    glGenBuffers(2, &this->_triangleVBO[0]);
    
    // 2.重置VAO，避免修改
    glBindVertexArray(0);
    // 3.在VAO中生成两个VBO
    // 4. 将顶点信息存放进第一个buffer中
    glBindBuffer(GL_ARRAY_BUFFER, this->_triangleVBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->_triangleVertexs[0]) * MAX_VERTICES_SIZE, this->_triangleVertexs, GL_DYNAMIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_triangleVBO[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(this->_triangleIndices[0]) * MAX_INDICES_SIZE, this->_triangleIndices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Renderer::addCommand(RenderCommand *command, int renderQueueType) {
    if (command->getCmdType() == COMMAND_UNKOWN) {
        std::cout << "this command type is unkown!" << std::endl;
        return ;
    }
    if (renderQueueType < 0) {
        std::cout << "this command type is unkown!" << std::endl;
        return ;
    }
    
    this->_renderQueues[renderQueueType].push_back(command);
}

void Renderer::filledVertexData(TriangleCommand *command) {
    // 填充顶点数据
    memcpy(this->_triangleVertexs + _filledVertex, command->getVertics(), sizeof(V3F_C4B_T2F) * command->getVertexCount());
    
    // 获取模型矩阵
    const Matrix& modelView = command->getModelView();
    // 根据顶点信息将矩阵平移
    for(ssize_t i=0; i< command->getVertexCount(); ++i)
    {
        V3F_C4B_T2F *q = &_triangleVertexs[i + _filledVertex];
        Vector3 *vec1 = (Vector3*)&q->vertices;
        modelView.transformPoint(vec1);
    }

    // 获取索引信息
    const unsigned short* indices = command->getIndices();
    for (ssize_t i = 0; i < command->getIndexCount(); ++i) {
        this->_triangleIndices[i + this->_filledIndex] = this->_filledVertex + indices[i];
    }
    
    this->_filledVertex += command->getVertexCount();
    this->_filledIndex += command->getIndexCount();
    //std::cout << "2.填充顶点数据..." << std::endl;
}

bool Renderer::drawTriangleCmd() {
    int indexToDraw = 0;
    
    //Upload buffer to VBO
    if(_filledVertex <= 0 || _filledIndex <= 0 || _batchedCommands.empty())
    {
        std::cout << "image data is null" << std::endl;
        return false;
    }
    // 设置默认的顶点对象数组
    OPGL::bindVAO(this->_triangleVAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, this->_triangleVBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->_triangleVertexs[0]) * this->_filledVertex, nullptr, GL_STATIC_DRAW);
    // 更新子缓冲数据
    // 第一种方法
    // 映射缓存
//    void *buf = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
//    memcpy(buf, this->_triangleVertexs, sizeof(this->_triangleVertexs[0]) * this->_filledVertex);
//    glUnmapBuffer(GL_ARRAY_BUFFER);
    
    //第二种方法
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(this->_triangleVertexs[0]) * this->_filledVertex, this->_triangleVertexs);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_triangleVBO[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(this->_triangleIndices[0]) * this->_filledIndex, this->_triangleIndices, GL_STATIC_DRAW);
    
    // 计算有多少个索引，也就是有多少个四边形
    for (const auto& cmd : _batchedCommands) {
        cmd->useMaterial();
        indexToDraw += cmd->getIndexCount();
    }
    
    if (indexToDraw > 0) {
        glDrawElements(GL_TRIANGLES, (GLsizei)indexToDraw, GL_UNSIGNED_SHORT, 0);
        //std::cout << "3.完成绘制..." << std::endl;
    }
    
    OPGL::bindVAO(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    return true;
}


// 开启深度缓存
void Renderer::setDepthFor2DTest(bool enable) {
    if (enable) {
        glClearDepth(1.0f);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
    }
    else {
        glDisable(GL_DEPTH_TEST);
    }
    
    this->_isDepthTestFor2D = enable;
}

// 渲染的中心函数
void Renderer::render() {
    this->_isRendering = true;
    if (this->_initedView) {
        visitRenderQueue(this->_renderQueues[0]);
    }
    
    // 清除绘制命令!!!!!
    this->clean();
    this->_isRendering = false;
}

void Renderer::visitRenderQueue(RendererQueue &queue) {
    if (queue.size() == 0) {
        std::cout << "there is no render command to renderer." << std::endl;
        return ;
    }
    
    queue.saveRenderState();
    
    //目前只处理z轴上大于0的
    const auto& posQueue = queue.getSubQueue(RENDER_QUEUE_MORE_ZORDER);
    
    if (posQueue.size() > 0) {
        if (this->_isDepthTestFor2D) {
            glEnable(GL_DEPTH_TEST);
            glDepthMask(true);
        }
        glEnable(GL_BLEND); //启用混合
        // 循环遍历队列中的命令，并且处理
        for (auto it = posQueue.begin(); it != posQueue.end(); it++) {
            this->handleRenderCommand(*it);
        }
    }
}

void Renderer::handleRenderCommand(RenderCommand *command) {
    if (command->getCmdType() == COMMAND_TRIANGEL) {
        auto cmd = static_cast<TriangleCommand*>(command);
        _batchedCommands.push_back(cmd);
        this->reset();
        this->filledVertexData(cmd);
        this->drawTriangleCmd();
    }
    else if (command->getCmdType() == COMMAND_QUAD) {
        std::cout << "the command's type is QUAD." << std::endl;
    }
    else {
        std::cout << "the command's type is unkown." << std::endl;
    }
}

void Renderer::reset() {
    // 清除颜色缓存
    glClearColor(this->_clearColor.r, this->_clearColor.g, this->_clearColor.b, this->_clearColor.a);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::clean() {
    // 清除每个类型的绘制命令
    for (size_t i = 0; i < this->_renderQueues.size(); i++) {
        this->_renderQueues[i].clear();
    }
    
    // 重置批量绘制
    this->_filledIndex = 0;
    this->_filledVertex = 0;
    this->_batchedCommands.clear();
}
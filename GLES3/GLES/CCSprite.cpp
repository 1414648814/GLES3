//
//  CCSprite.cpp
//  GLES
//
//  Created by staff on 16/12/13.
//  Copyright © 2016年 George1994. All rights reserved.
//

#include "CCSprite.h"
#include "CCRenderer.h"
#include "CCTexture.h"
#include "CCProgram.h"

Sprite::Sprite() {
    this->_blendType = BlendFunc::ALPHA_PREMULTIPLIED;
    this->_triangleCommand = new TriangleCommand();
    this->_tag = 0;
}

/*
 * 绘制函数
 */
void Sprite::draw(Renderer* render, const Matrix& transform) {
    // reset
    render->reset();
    
    // 避免重复添加，只需要添加一次command指令，后续只需要用到draw绘制函数
    // 要先对cmd进行初始化
    this->_triangleCommand->init(this->_tag, this->_texture->getTextureID(), this->getProgramState(), this->_blendType, this->_triangles, transform);

    render->addCommand(this->_triangleCommand, COMMAND_TRIANGEL);
    
    //std::cout << "1.初始化精灵绘制命令..." << std::endl;
}

Sprite* Sprite::create(const std::string path, ssize_t tag) {
    Sprite* sprite = new (std::nothrow)Sprite();
    if (sprite && sprite->initWithFile(path)) {
        sprite->setTag(tag);
        return sprite;
    }
    return NULL;
}

/**
 *  根据路径生成纹理
 */
bool Sprite::initWithFile(const std::string path) {
    // 如果内存中存在纹理
    auto texture = TextureManager::getInstance()->getTexture(path);
    if (texture && this->initWithTexture(texture)) {
        return true;
    }
    Image* imgae = Image::create(path.c_str());
    texture = TextureManager::getInstance()->addImage(imgae);
    if (texture && this->initWithTexture(texture)) {
        return true;
    }
    return false;
}

/**
 *  初始化纹理
 */
bool Sprite::initWithTexture(Texture* texture) {
    bool result;
    if (texture == NULL) {
        return false;
    }
    
    if (!Node::init()) {
        result = false;
    }
    else {
        // 创建program，同时创建programState
        this->setProgramState(ProgramState::getOrCreateWithGLProgramName(Program::SHADER_NAME_POSITION_TEXTURE_COLOR));
        
        CCRect rect = CCRect::ZERO;
        if (texture) {
            rect.size = texture->getContentSize();
            this->_texture = texture;
        }
        this->setTexture(texture, rect, rect.size);
        result = true;
    }
    return result;
}

/**
 *  设置纹理
 */
void Sprite::setTexture(Texture* texture, const CCRect& rect, const CCSize& size) {
    this->updateVertex(rect);
    this->setContentSize(size);
}

/**
 *  更新顶点相关信息
 */
void Sprite::updateVertex(const CCRect& rect) {
    memset(&this->_quad, 0, sizeof(this->_quad));
    
    this->setTrianglesColorsInfo();
    this->setTrianglesTextureCoodInfo(rect);
    this->setTrianglesVertexInfo(rect);
    this->updateQuad();
}

void Sprite::setTrianglesColorsInfo() {
    this->_quad.bl.colors = Color4B::WHITE;
    this->_quad.br.colors = Color4B::WHITE;
    this->_quad.tl.colors = Color4B::WHITE;
    this->_quad.tr.colors = Color4B::WHITE;
}

void Sprite::setTrianglesVertexInfo(const CCRect& rect) {
    
    float x1 = 0.0f;
    float y1 = 0.0f;
    float x2 = x1 + rect.size.width;
    float y2 = y1 + rect.size.height;
    
    // 左下角
    this->_quad.bl.vertices.set(x1, y1, 0.0f);
    // 右下角
    this->_quad.br.vertices.set(x2, y1, 0.0f);
    // 左上角
    this->_quad.tl.vertices.set(x1, y2, 0.0f);
    // 右下角
    this->_quad.tr.vertices.set(x2, y2, 0.0f);
    
}

void Sprite::setTrianglesTextureCoodInfo(const CCRect& rect) {
    // 左上角为原点
    float left = rect.origin.x/this->_texture->getPixelWidth();
    float right = (rect.origin.x + rect.size.width)/this->_texture->getPixelWidth();
    float top = rect.origin.y/this->_texture->getPixelHeight();
    float bottom = (rect.origin.y + rect.size.height)/this->_texture->getPixelHeight();
    
    // 左下角
    this->_quad.bl.texCoords.u = left;
    this->_quad.bl.texCoords.v = bottom;
    
    // 右下角
    this->_quad.br.texCoords.u = right;
    this->_quad.br.texCoords.v = bottom;
    
    // 左上角
    this->_quad.tl.texCoords.u = left;
    this->_quad.tl.texCoords.v = top;
    
    // 右上角
    this->_quad.tr.texCoords.u = right;
    this->_quad.tr.texCoords.v = top;
}

void Sprite::updateQuad() {
    // 更新到三角形上
    this->_triangles.vertCount = 4; //顶点个数
    this->_triangles.verts = (V3F_C4B_T2F*)&this->_quad; //顶点数据
    this->_triangles.indexCount = 6; //索引个数
    this->_triangles.indices = quadIndices;
}

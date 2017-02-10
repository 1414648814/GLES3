//
//  CCTexture.cpp
//  GLES
//
//  Created by staff on 16/12/13.
//  Copyright © 2016年 George1994. All rights reserved.
//

#include "CCTexture.h"
#include "CCOPGL.h"

/*
 * --------------------------------Texture--------------------------------
 */
Texture::Texture() {
    this->_image = NULL;
    this->_type = Pixel_Format_UNKOWN;
    this->_textureID = 0;
}

Texture::~Texture() {
    
    
}

void Texture::initImage(Image* image, Image_Pixel_Format_Type type) {
    if (image != NULL && image->getData() != NULL && image->getDataLen() > 0) {
        this->_type = type;
        this->_image = image;
        this->_name = image->getPath();
        
        this->clearBuffer();
        this->setBlendMode();
        this->drawImage();
    }
}

void Texture::initImage(const std::string imgPath) {
    Image *image = Image::create(imgPath.c_str());
    this->initImage(image, Pixel_Format_RGBA8888);
}

void Texture::clearBuffer() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Texture::setBlendMode() {
    glEnable(GL_BLEND);
    // 只有源
    glBlendFunc(GL_ONE, GL_ZERO);
}

void Texture::drawImage() {
    glEnable(GL_TEXTURE_2D);
    
    //绑定
    glGenTextures(1, &this->_textureID);
//    glBindTexture(GL_TEXTURE_2D, this->_textureID);
    OPGL::bindTexture2D(this->_textureID);
    
    //纹理过滤函数，纹理图像空间映射到帧缓冲图像空间
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    //线性过滤函数
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    //生成2d纹理
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->_image->getImageWidth(), this->_image->getImageHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, this->_image->getData());
    glGenerateMipmap(GL_TEXTURE_2D);
    
    std::cout << "3.设置纹理样式成功..." << std::endl;
    
//    glBindTexture(GL_TEXTURE_2D, 0);
    OPGL::bindTexture2D(0);
}

void Texture::setTextureName(const std::string name) {
    this->_name = name;
}

CCSize Texture::getContentSize() {
    CCSize size(this->_image->getImageWidth(), this->_image->getImageHeight());
    return size;
}

float Texture::getWidth() {
    return this->_image->getImageWidth();
}

float Texture::getHeight() {
    return this->_image->getImageHeight();
}

CCSize Texture::getPixelContentSize() {
    return this->_image->getPixelContentSize();
}

float Texture::getPixelWidth() {
    return this->_image->getPixelWidth();
}

float Texture::getPixelHeight() {
    return this->_image->getPixelHeight();
}

/*
 * --------------------------------TextureManager--------------------------------
 */
static TextureManager* manager = NULL;

TextureManager* TextureManager::getInstance() {
    if (manager == NULL) {
        manager = new (std::nothrow) TextureManager();
        return manager;
    }
    else {
        return manager;
    }
}

void TextureManager::destoryInstance() {
    
    
}

TextureManager::TextureManager() {
    
    
}

TextureManager::~TextureManager() {
    
    
}

Texture* TextureManager::addImage(Image* image) {
    // 如果存在则不需要再创建
    if (this->_textures.find(image->getPath()) != this->_textures.end()) {
        return this->_textures.at(image->getPath());
    }
    
    Texture* texture = new (std::nothrow)Texture();
    texture->initImage(image, Pixel_Format_RGBA8888); // 默认像素格式
    this->_textures.insert(std::make_pair(image->getPath(), texture));
    
    return texture;
}

// 通过路径获取texture
Texture* TextureManager::getTexture(const std::string& path) {
    if (this->_textures.find(path) != this->_textures.end()) {
        return this->_textures.at(path);
    }
    
    return NULL;
}

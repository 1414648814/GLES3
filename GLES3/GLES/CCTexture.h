//
//  CCTexture.hpp
//  GLES
//
//  Created by staff on 16/12/13.
//  Copyright © 2016年 George1994. All rights reserved.
//

#ifndef CCTexture_hpp
#define CCTexture_hpp

#include "CCGL.h"
#include "CCConfig.h"
#include "CCImage.h"
#include "CCPoint.h"
#include <stdio.h>
#include <string>
#include <map>
#include <iostream>

/**
 *  纹理
 */
class Texture {
public:
    Texture();
    ~Texture();
    
    /**
     * 处理图像的数据
     */
    void initImage(Image*, Image_Pixel_Format_Type);
    
    void initImage(const std::string imgPath);
    
    void clearBuffer();
    void setBlendMode();
    
    void drawImage();
    
    void setTextureName(const std::string);
    
    /**
     *  返回大小
     */
    CCSize getContentSize();
    float getWidth();
    float getHeight();
    
    CCSize getPixelContentSize();
    float getPixelWidth();
    float getPixelHeight();
    
    inline GLuint getTextureID() {
        return _textureID;
    }
    
protected:
    std::string _name;
    GLuint _textureID;
    
    Image* _image;
    Image_Pixel_Format_Type _type;
};

class TextureManager {
public:
    static TextureManager* getInstance();
    static void destoryInstance();
    
    Texture* addImage(Image*);
    Texture* getTexture(const std::string&);
    
protected:
    TextureManager();
    ~TextureManager();
    
protected:
    std::map<std::string, Texture*> _textures;
};
    


#endif /* CCTexture_hpp */

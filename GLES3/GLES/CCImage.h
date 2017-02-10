//
//  CCImage.hpp
//  GLES
//
//  Created by staff on 16/12/19.
//  Copyright © 2016年 George1994. All rights reserved.
//

#ifndef CCImage_hpp
#define CCImage_hpp

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>

#include "SOIL.h"
#include "CCPoint.h"
    
// 图像类型
typedef enum {
    Image_Type_PNG = 0,
    Image_Type_JPG,
    Image_Type_BITMAP,
    
}ImageType;


typedef enum {
    PNG_Image_Color_RGBA = 0,
    PNG_Image_Color_RGB,
}PNGImageColorType;

class Image {
public:
    Image();
    ~Image();
    
    static Image* create(const char* path);
    
    /**
     * 根据图像文件初始化数据
     */
    bool initWithImageFile(const char* path);
    
    /**
     * 根据图像数据初始化数据
     */
    bool initWithImgeData(unsigned char* data, unsigned int width, unsigned int height, PNGImageColorType format);
    
    ssize_t getDataLen();
    
    unsigned char* getData();
    
    ImageType getImageType() const;
    
    float getImageWidth() const;
    float getImageHeight() const;
    CCSize getiImageContentSize() const;
    
    float getPixelWidth() const;
    float getPixelHeight() const;
    CCSize getPixelContentSize() const;
    
    std::string getPath() const;
    
    
    
protected:
    ImageType _type;
    PNGImageColorType _colorType; //目前只用png的图片，其他后面支持
    
    unsigned char* _data;
    size_t _datalen;
    
    float _img_width;
    float _img_height;
    
    float _pixel_width;
    float _pixel_height;
    
    std::string _path;
};

#endif /* CCImage_hpp */
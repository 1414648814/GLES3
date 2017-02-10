//
//  CCImage.cpp
//  GLES
//
//  Created by staff on 16/12/19.
//  Copyright © 2016年 George1994. All rights reserved.
//

#include "CCImage.h"
#include "CCPoint.h"
#include "CCGL.h"
    
// 用流读取图片数据的回调函数
typedef struct
{
    const unsigned char * data;
    ssize_t size;
    int offset;
}tImageSource;

Image* Image::create(const char* path) {
    Image* image = new (std::nothrow)Image();
    if (image && image->initWithImageFile(path)) {
        return image;
    }
    return NULL;
}

Image::Image() {
    // 对象必须要进行初始化一次
    
}

Image::~Image() {
    // 销毁时必须要撤销初始化一次
    SOIL_free_image_data(this->_data);
}

bool Image::initWithImageFile(const char* path) {
    // 根据路径获取图片数据
    int width, height;
    unsigned char* imgdata = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGBA);
    
    if (width) {
        this->_img_width = width;
        this->_pixel_width = width;
    }
    
    if (height) {
        this->_img_height = height;
        this->_pixel_height = height;
    }
    
    if (imgdata != nullptr) {
        this->_data = imgdata;
        this->_datalen = strlen((const char*)imgdata);
        
//        //测试图片数据
//        FILE * file;
//        file = fopen("/Users/staff/Desktop/abc.png","wb");
//        if (file) {
//            fwrite(this->_data, 1, this->_datalen, file);
//        }
//        fclose(file);
    }
    
    this->_colorType = PNG_Image_Color_RGBA;
    this->_path = path;
    
    std::cout << "2.获取图像数据成功..." << std::endl;
    return true;
}

// 根据数据
bool Image::initWithImgeData(unsigned char* data, unsigned int width, unsigned int height, PNGImageColorType format) {
    this->_data = data;
    this->_img_width = width;
    this->_img_height = height;
    this->_pixel_width = width;
    this->_pixel_height = height;
    this->_colorType = format;
    return true;
}

ssize_t Image::getDataLen() {
    return this->_datalen;
}

unsigned char* Image::getData() {
    return _data;
}

ImageType Image::getImageType() const {
    return _type;
}

// 在手机上除以2
CCSize Image::getiImageContentSize() const {
    return CCSize(this->_img_width, this->_img_height) / 1;
}

float Image::getImageWidth() const {
    return _img_width/1;
}

float Image::getImageHeight() const {
    return _img_height/1;
}

std::string Image::getPath() const {
    return _path;
}

float Image::getPixelWidth() const {
    return this->_pixel_width;
}

float Image::getPixelHeight() const {
    return this->_pixel_height;
}

CCSize Image::getPixelContentSize() const {
    return CCSize(this->_pixel_width, this->_pixel_height);
}

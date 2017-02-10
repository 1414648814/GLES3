//
//  CCSprite.hpp
//  GLES
//
//  Created by staff on 16/12/13.
//  Copyright © 2016年 George1994. All rights reserved.
//

#ifndef CCSprite_hpp
#define CCSprite_hpp

//#include "CCConfig.hpp"

#include "CCNode.h"
#include "CCPoint.h"
#include "CCType.h"
#include "CCTriangleCommand.h"
#include <stdio.h>
#include <vector>
#include <string>

// 索引
static unsigned short quadIndices[] = {
    0, 1, 2,
    1, 2, 3
};

class Renderer;
class Texture;
class Matrix;
class TriangleCommand;

class Sprite : public Node {
public:
    
    static Sprite* create(const std::string path, ssize_t tag);
    
    /**
     *  绘制
     */
    virtual void draw(Renderer*, const Matrix&) override;
    
    /**
     *  根据路径生成纹理
     */
    virtual bool initWithFile(const std::string path);
    
    /**
     *  初始化纹理
     */
    virtual bool initWithTexture(Texture*);
    
    /**
     *  设置纹理
     */
    virtual void setTexture(Texture*, const CCRect& , const CCSize&);
    
    /**
     *  更新顶点信息
     */
    virtual void updateVertex(const CCRect&);
    
    inline virtual void setTag(ssize_t tag) {
        _tag = tag;
    }
    
protected:
    V3F_C4B_T2F_Quad _quad; //保存四边形上四个点的相关信息（位置，颜色，纹理坐标）
    BlendFunc _blendType; //混合类型
    Texture *_texture;
    TriangleCommand *_triangleCommand; //绘制命令
    ssize_t _tag;
    GLubyte _displayOpacity; //显示的透明度
    GLubyte _realOpacity; //真实的透明度
    Color3B _displayColor; //显示的颜色
    Color3B _realColor; //真实的颜色
    
    Sprite();
    virtual ~Sprite(){};
    
private:
    Triangles _triangles; //三角形
    
    virtual void updateQuad();
    
    virtual void setTrianglesColorsInfo();
    
    // 更新三角形的顶点信息
    virtual void setTrianglesVertexInfo(const CCRect&);
    
    // 更新三角形的纹理坐标信息 
    virtual void setTrianglesTextureCoodInfo(const CCRect&);
};

#endif /* CCSprite_hpp */

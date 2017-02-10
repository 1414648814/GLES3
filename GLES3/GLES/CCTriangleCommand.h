//
//  CCTriangleCommand.hpp
//  GLES
//
//  Created by staff on 16/12/14.
//  Copyright © 2016年 George1994. All rights reserved.
//

#ifndef CCTriangleCommand_hpp
#define CCTriangleCommand_hpp

#include "CCConfig.h"
#include "CCMatrix.h"
#include "CCRenderCommand.h"
#include <stdio.h>
#include <iostream>

class ProgramState;

struct Triangles
{
    V3F_C4B_T2F *verts; //顶点数据
    unsigned short* indices;
    ssize_t vertCount;
    ssize_t indexCount;
    
    void outVerts() {
        for (ssize_t i = 0; i < vertCount; i++) {
            std::cout << "第" << i + 1 << "个顶点信息" << std::endl;
            std::cout << "坐标：" << verts[i].vertices.x << "," << verts[i].vertices.y << "," << verts[i].vertices.z << std::endl;
            std::cout << "颜色：" << verts[i].colors.r << "," << verts[i].colors.g << "," << verts[i].colors.b << std::endl;
            std::cout << "纹理：" << verts[i].texCoords.u << "," << verts[i].texCoords.v << std::endl;
        }
    }
    
    void outIndics() {
        for (ssize_t i = 0; i < indexCount; i++) {
            std::cout << "第" << i + 1 << "个纹理信息" << std::endl;
            std::cout << indices[i] << std::endl;
        }
    }
};

class TriangleCommand : public RenderCommand {
public:
    TriangleCommand();
    ~TriangleCommand();
    
    /**
     * 初始化
     */
    bool init(float, GLuint, ProgramState*, BlendFunc, const Triangles&, const Matrix&, bool);
    
    // 生成材质ID
    void generateMatrialID();
    
    // 使用材质
    void useMaterial();
    
    // 获取三角形
    inline const Triangles& getTriangles() const {
        return _triangles;
    }
    
    // 纹理ID
    inline GLuint getTextureID() const {
        return _textureID;
    }
    
    // 材质ID
    inline uint32_t getMaterialID() const {
        return _materialID;
    }
    
    inline V3F_C4B_T2F* getVertics() const {
        return _triangles.verts;
    }
    
    inline unsigned short* getIndices() const {
        return _triangles.indices;
    }
    
    // 获取顶点数目
    inline ssize_t getVertexCount() const {
        return _triangles.vertCount;
    }
    
    // 获取索引数目
    inline ssize_t getIndexCount() const {
        return _triangles.indexCount;
    }
    
    inline const Matrix& getModelView() const {
        return _modelView;
    }
    
protected:
    // 材质id
    uint32_t _materialID;
    // 纹理id
    GLuint   _textureID;
    // shader状态
    ProgramState* _programState;
    // 混合函数
    BlendFunc _blendType;
    // 渲染的三角形
    Triangles _triangles;
    // 模型矩阵
    Matrix _modelView;
};



#endif /* CCTriangleCommand_hpp */
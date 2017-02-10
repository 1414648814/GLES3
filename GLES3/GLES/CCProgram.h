//
//  CCProgram.hpp
//  GLES
//
//  Created by staff on 16/12/13.
//  Copyright © 2016年 George1994. All rights reserved.
//

#ifndef CCProgram_hpp
#define CCProgram_hpp

#include "CCGL.h"
#include "CCConfig.h"
#include "CCShader.h"
#include "CCGame.h"

#include <stdio.h>
#include <string>
#include <iostream>
#include <map>
#include <cassert>

/*
 * 对着色器的进行封装
 * 在openglES中，每个program对象有且仅有一个顶点着色器对象和一个片段着色器对象连接到它
 */
class Program {
public:
    Program();
    ~Program();
    
    // 通过从内存的字符串流导入着色器程序
    bool initWithByteArrays(const GLchar* shaderArr, const GLchar* fragArr);
    static Program* createWithByteArrays(const GLchar* shaderArr, const GLchar* fragArr);
    
    // 通过文件读取
    bool initWithFileName(const std::string& shaderName, const std::string& fragName);
    static Program* createWithFileName(const std::string& shaderName, const std::string& fragName);
    
    // 自定义把顶点属性索引绑定到顶点属性名
    void bindAttribLoacation(GLuint program, GLuint index,const GLchar *name);
    
    // 获取属性索引（也就是着色器程序中attribute类型的id）
    GLint getAttribLocation(const GLchar *name);
    // 获取uniform索引（也就是着色器程序中uniform类型的id）
    GLint getUniformLocation(const GLchar *name);
    
    // 将顶点数据传入着色器程序
    void setVertexAttribPointer(VERTEX_ATTRIB_TYPE type);
    
    // 更新统一变量
    void updateBuiltUniforms();
    
    // 设置已经存在
    void setUniformForBuilts(const Matrix& matMV);
    /**
     *  编译shader
     *
     *  @param shader shader文件
     *  @param type   shader类型
     *  @param source 文件流
     */
    bool compileShader(GLuint* shader, GLenum type, const GLchar* source);
    
    /**
     *  把顶点属性索引绑定到顶点属性名上
     *  (注意：需要在link之前执行)
     */
    void bindVertAttrib();
    
    /**
     *  对着色器对象执行链接操作
     *
     *  @return 是否成功
     */
    bool link();
    
    /**
     *  将Opengl渲染管道切换到着色器模式，并使用刚才做好的着色器对象
     *
     *  @return 是否成功
     */
    bool use();
    
    /**
     *  清除shader，释放资源
     */
    void clearShader();

public:
    static const char* SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP;
    static const char* SHADER_NAME_POSITION_TEXTURE_COLOR;

protected:
    Game * _game;
    
    GLuint _program;
    GLint  _builtInUniforms[UNIFORM_MAX_NUM];
    GLuint _vertShader; //操作顶点着色器
    GLuint _fragShader; //操作片段着色器
};

/*
 * 管理所有的program，每个program对应一个shader
 */
class ProgramManager {
public:
    enum ShaderType {
        ShaderType_PositionTextureColor,
        ShaderType_Unkown
    };
    
    static ProgramManager* getInstance();
    static void destroyInstance();
    
    // 通过key获取program
    Program* getProgram(const std::string& key);
    
    // 添加program
    void addProgram(const std::string& key, Program* program);
    
protected:
    bool init();
    void loadAllShaders();
    void loadSingleShader(Program*, ShaderType);
protected:
    std::map<std::string, Program*> _programs;
    
private:
    ProgramManager();
    ~ProgramManager();
};

/*
 * 管理program中的状态（uniforms 和 attributes）
 * 其中uniforms用于保存只读常量值的变量，attributes主要用于定义输入的每个点属性
 * 它们在shader中通过location和name来对应的
 * 
 */
class ProgramState {
    friend class ProgramStateManager;
public:
    // 直接创建
    static ProgramState* create(Program* program);
    
    // 通过名字创建
    static ProgramState* getOrCreateWithGLProgramName(const std::string& programname);
    
    // 通过shader名字创建
    static ProgramState* getOrCreateWithShaders(const std::string& vertexShader, const std::string& fragShader);
    
    void setProgram(Program*);
    Program* getProgram() const;
    
    bool init(Program* program);
    
    // 整体更新
    void apply(const Matrix&);
    
    // 更新program
    void applyProgram(const Matrix&);
    // 更新attribute
    void applyAttributes(bool);
    // 更新uniform
    void applyUniforms();
    
    inline ssize_t getUniformsCount() const {
        return _uniforms.size();
    }
    
    inline ssize_t getVertexAttribsCount() const {
        return _vertexAttribs.size();
    }
    
protected:
    ProgramState();
    ~ProgramState();
    
protected:
    Program * _program;
    std::map<GLint, Uniform> _uniforms;
    std::map<GLint, VertexAttrib> _vertexAttribs;
};

/*
 * 管理所有的programstate，每个program对应一个shader状态
 */
class ProgramStateManager {
public:
    static ProgramStateManager* getInstance();
    static void destroyInstance();
    
    // 通过key获取program
    ProgramState* getProgramState(Program*);
    
protected:
    std::map<Program*, ProgramState*> _programStates;
    
private:
    ProgramStateManager();
    ~ProgramStateManager();
};


#endif /* CCProgram_hpp */

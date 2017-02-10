//

//  CCGame.cpp
//  GLES
//
//  Created by staff on 2016/12/26.
//  Copyright © 2016年 George1994. All rights reserved.
//
#define STRINGIFY(A)  #A
#include "CCGame.h"
#include "CCOPGL.h"

// 没有使用单例模式，导致每次都要之前对象设置的都为空
static Game* game = nullptr;
Game* Game::getInstance() {
    if (game == nullptr) {
        game = new (std::nothrow)Game();
        if (game && game->initGame()) {
            return game;
        }
    }
    return game;
}

void Game::destroyInstance() {
    
}

Game::Game() : _render(NULL), _glView(NULL), _curscene(NULL), _isValid(false), _viewSize(0, 0) {
    
}

Game::~Game() {
    
}

bool Game::initGame() {
    this->_projection = Projection::_3D;
    this->_render = new Renderer();
    
    this->_render->reset();
    this->initMatrixStack();
    return true;
}

void Game::setGLDefaultValues() {
    this->setProjection(_projection);
    
}

void Game::setGLView(GLView* openView) {
    if (_glView != openView) {
        this->_glView = openView;
        this->_render->initGLView();
        
        this->_viewSize = this->_glView->getVisibleSize();
        
        if (_glView) {
            this->setGLDefaultValues();
        }
    }
}

// 设置投影矩阵
void Game::setProjection(Projection projection) {
    CCSize size = _viewSize;
    
    switch (projection) {
        case Projection::_3D:
        {
            float zeye = this->getZEye();

            Matrix matrixPerspective, matrixLookup;

            loadIdentityMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);

            // issue #1334
            Matrix::createPerspective(60, (GLfloat)size.width/ size.height, 10, zeye+ size.height/2, &matrixPerspective);

            multiplyMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION, matrixPerspective);

            Vector3 eye(size.width/2, size.height/2, zeye), center(size.width/2, size.height/2, 0.0f), up(0.0f, 1.0f, 0.0f);
            Matrix::createLookAt(eye, center, up, &matrixLookup);
            multiplyMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION, matrixLookup);
            
            loadIdentityMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
            break;
        }
        default:
            break;
    }
    
    this->_projection = projection;
    this->setViewport();
}

void Game::pushScene(Scene* scene) {
    this->_scenes.push_back(scene);
}

void Game::popScene() {
    this->_scenes.pop_back();
}

void Game::runScene(Scene* scene) {
    if (scene != NULL) {
        this->_curscene = scene;
        this->_scenes.push_back(scene);
    }
}

void Game::drawScene() {
    this->_render->reset();
    
    if (this->_curscene) {
        // 设置当前渲染
        this->_curscene->render(this->_render);
    }
    
    if (this->_glView) {
        this->_glView->swapBuffers();
    }
}

GLuint _sp_textureID;
Triangles _sp_triangles;
V3F_C4B_T2F_Quad _sp_quad;
Matrix _sp_modelView;
GLuint _triangleVAO;
GLuint _triangleVBO[2]; //默认支持VBO
V3F_C4B_T2F Vertexs[MAX_VERTICES_SIZE]; //绘制的节点信息
GLushort Indices[MAX_INDICES_SIZE]; // 绘制所需的索引信息
GLuint _program; //渲染程序
GLuint _vertexShader; //顶点着色器
GLuint _fragShader; //片元着色器
GLint _uniforms[12]; //统一变量
unsigned int _filledVertex = 0; //填充的顶点偏移
unsigned int _filledIndex = 0; //填充的索引偏移
const char* CCPositionTextureColorVert2 =
"                               \n\
attribute vec4 a_position;      \n\
attribute vec4 a_color;         \n\
attribute vec2 a_texCoord;      \n\
varying lowp vec4 ourColor;     \n\
varying mediump vec2 TexCoord;  \n\
void main() {                   \n\
gl_Position = a_position * CC_PMatrix;\n\
ourColor = a_color;             \n\
TexCoord = a_texCoord;          \n\
}\n\
";

const char* CCPositionTextureColorFrag2 =
"precision lowp float;\n\
varying vec4 ourColor;\n\
varying vec2 TexCoord;\n\
void main() {\n\
gl_FragColor = ourColor * texture2D(CC_Texture0, TexCoord);\n\
}\n\
";

unsigned short quadIndices2[] = {
    0, 1, 2,
    3, 2, 1
};

void Game::test() {
    std::cout << "测试状态下..." << std::endl;
    
    //_sp_triangles.outVerts();
    //_sp_triangles.outIndics();
    
    glGenVertexArrays(1, &_triangleVAO);
    // 绑定VAO
    OPGL::bindVAO(_triangleVAO);
    // 创建生成两个VBO
    glGenBuffers(2, &_triangleVBO[0]);
    // 顶点Buffer
    glBindBuffer(GL_ARRAY_BUFFER, _triangleVBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(V3F_C4B_T2F) * _sp_triangles.vertCount, _sp_triangles.verts, GL_DYNAMIC_DRAW);
    
    //GLuint _positionSlot = glGetAttribLocation(_program, ATTRIBUTE_NAME_POSITION);
    //GLuint _colorSlot = glGetAttribLocation(_program, ATTRIBUTE_NAME_COLOR);
    //GLuint _textureCoordsSlot = glGetAttribLocation(_program, ATTRIBUTE_NAME_TEXCOOD);
    
    // vertices
    // 位置
    glEnableVertexAttribArray(VERTEX_ATTRIB_POSITION);
    glVertexAttribPointer(VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(V3F_C4B_T2F), (GLvoid*) offsetof( V3F_C4B_T2F, vertices));
    
    // colors
    // 颜色
    glEnableVertexAttribArray(VERTEX_ATTRIB_COLOR);
    glVertexAttribPointer(VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(V3F_C4B_T2F), (GLvoid*) offsetof( V3F_C4B_T2F, colors));
    
    // tex coords
    // 纹理坐标数据
    glEnableVertexAttribArray(VERTEX_ATTRIB_TEXCOOD);
    glVertexAttribPointer(VERTEX_ATTRIB_TEXCOOD, 2, GL_FLOAT, GL_FALSE, sizeof(V3F_C4B_T2F), (GLvoid*) offsetof( V3F_C4B_T2F, texCoords));
    
    // 索引Buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _triangleVBO[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * _sp_triangles.indexCount, _sp_triangles.indices, GL_STATIC_DRAW);
    
    OPGL::bindVAO(0);
    
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    int width, height;
    unsigned char* image_data = SOIL_load_image("/Users/staff/Desktop/GLES3/GLES3/wall.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
    size_t _datalen = strlen((const char*)image_data);
    
    
    SOIL_free_image_data(image_data);
    
    glEnable(GL_TEXTURE_2D);
    //OPGL::activeTexture(GL_TEXTURE0);
    
    //绑定
    glGenTextures(1, &_sp_textureID);
    glBindTexture(GL_TEXTURE_2D, _sp_textureID);
    
    //纹理过滤函数，纹理图像空间映射到帧缓冲图像空间
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    //线性过滤函数
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    //生成2d纹理
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    glBindTexture(GL_TEXTURE_2D, 0);
    
    _program = glCreateProgram();
    _vertexShader = _fragShader = 0;
    
    std::string str = "precision highp float;\n precision highp int;\n";
    
    _vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const GLchar *sources[] = {
        str.c_str(),
        DEFAULT_SHADER_UNIFORMS,
        CCPositionTextureColorVert2
    };
    glShaderSource(_vertexShader, sizeof(sources)/sizeof(*sources), sources, NULL);
    glCompileShader(_vertexShader);
    GLint status;
    glGetShaderiv(_vertexShader, GL_COMPILE_STATUS, &status);
    if (!status) {
        GLint infoLen = 0;
        glGetShaderiv(_vertexShader, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen) {
            char* infoLog = static_cast<char *>(malloc(sizeof(char) * infoLen));
            glGetShaderInfoLog(_vertexShader, infoLen, NULL, infoLog);
            std::cout << "compile vertex shader wrong..." << infoLog << std::endl;
        }
    }
    else {
        std::cout << "compile vertex shader success..." << std::endl;
    }
    
    str = "precision mediump float;\n precision mediump int;\n";
    _fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    // 片元着色器中没有指定的精度，所以必须要指定一个精度
    const GLchar *sources2[] = {
        str.c_str(),
        DEFAULT_SHADER_UNIFORMS,
        CCPositionTextureColorFrag2
    };
    glShaderSource(_fragShader, sizeof(sources2)/sizeof(*sources2), sources2, NULL);
    glCompileShader(_fragShader);
    
    glGetShaderiv(_fragShader, GL_COMPILE_STATUS, &status);
    if (!status) {
        GLint infoLen = 0;
        glGetShaderiv(_fragShader, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen) {
            char* infoLog = static_cast<char *>(malloc(sizeof(char) * infoLen));
            glGetShaderInfoLog(_fragShader, infoLen, NULL, infoLog);
            std::cout << "compile fragment shader wrong..." << infoLog << std::endl;
        }
    }
    else {
        std::cout << "compile fragment shader success..." << std::endl;
    }
    
    // 创建和链接程序
    if (_vertexShader) {
        glAttachShader(_program, _vertexShader);
    }
    
    if (_fragShader) {
        glAttachShader(_program, _fragShader);
    }
    
    // 验证程序对象（比较慢，不推荐使用）
    //glValidateProgram(_program);
    
    // 将通用顶点属性索引和指定的属性变量关联起来
    // 用来将Program指定的程序对象中的一个用户定义的属性变量和一个通用的顶点属性索引绑定
    // 属性数组
    static const struct {
        const char* attributeName;
        VERTEX_ATTRIB_TYPE type;
    }
    attribute_locations[] = {
        {ATTRIBUTE_NAME_POSITION, VERTEX_ATTRIB_POSITION}, //顶点位置
        {ATTRIBUTE_NAME_COLOR, VERTEX_ATTRIB_COLOR}, //顶点颜色
        {ATTRIBUTE_NAME_TEXCOOD, VERTEX_ATTRIB_TEXCOOD}, //顶点纹理坐标
    };
    
    // 把顶点属性索引绑定属性名
    for (int i = 0; i < sizeof(attribute_locations)/sizeof(attribute_locations[0]); i++) {
        glBindAttribLocation(_program, attribute_locations[i].type, attribute_locations[i].attributeName);
    }
    
    glLinkProgram(_program);
    glUseProgram(_program);
    
    //需要在program创建了以后才可以获取
    _uniforms[UNIFORM_TYPE_P_MATRIX] = glGetUniformLocation(_program, UNIFORM_NAME_P_MATRIX);
    _uniforms[UNIFORM_TYPE_MV_MATRIX] = glGetUniformLocation(_program, UNIFORM_NAME_MV_MATRIX);
    _uniforms[UNIFORM_TYPE_MVP_MATRIX] = glGetUniformLocation(_program, UNIFORM_NAME_MVP_MATRIX);
    _uniforms[UNIFORM_TYPE_TEXTURE0] = glGetUniformLocation(_program, UNIFORM_NAME_SAMPLE0);
    _uniforms[UNIFORM_TYPE_TEXTURE1] = glGetUniformLocation(_program, UNIFORM_NAME_SAMPLE1);
    _uniforms[UNIFORM_TYPE_TEXTURE2] = glGetUniformLocation(_program, UNIFORM_NAME_SAMPLE2);
    _uniforms[UNIFORM_TYPE_TEXTURE3] = glGetUniformLocation(_program, UNIFORM_NAME_SAMPLE3);
    
    if (_uniforms[UNIFORM_TYPE_TEXTURE0] != -1) {
        glUniform1i(_uniforms[UNIFORM_TYPE_TEXTURE0], 0);
    }
    if (_uniforms[UNIFORM_TYPE_TEXTURE1] != -1) {
        glUniform1i(_uniforms[UNIFORM_TYPE_TEXTURE1], 1);
    }
    if (_uniforms[UNIFORM_TYPE_TEXTURE2] != -1) {
        glUniform1i(_uniforms[UNIFORM_TYPE_TEXTURE2], 2);
    }
    if (_uniforms[UNIFORM_TYPE_TEXTURE3] != -1) {
        glUniform1i(_uniforms[UNIFORM_TYPE_TEXTURE3], 3);
    }
    
    // 已经使用完shader以后，则重置shader
    if (_vertexShader) {
        glDeleteShader(_vertexShader);
    }
    
    if (_fragShader) {
        glDeleteShader(_fragShader);
    }
    
    _vertexShader = _fragShader = 0;
    
    glGetProgramiv(_program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        std::cout << "the program link failed." << std::endl;
        glDeleteProgram(_program);
        _program = 0;
        assert(status == GL_FALSE);
    }
    else {
        std::cout << "the program link success..." << std::endl;
    }
    
    memset(&_sp_quad, 0, sizeof(_sp_quad));
    
    _sp_quad.bl.colors = Color4B::WHITE;
    _sp_quad.br.colors = Color4B::WHITE;
    _sp_quad.tl.colors = Color4B::WHITE;
    _sp_quad.tr.colors = Color4B::WHITE;
    
    // 1.指的是要处理纹理的部分
    float left = 0.0f;
    float right = 1.0f;
    float top = 0.0f;
    float bottom = 1.0f;
    
    // 左下角
    _sp_quad.bl.texCoords.u = left;
    _sp_quad.bl.texCoords.v = bottom;
    
    // 右下角
    _sp_quad.br.texCoords.u = right;
    _sp_quad.br.texCoords.v = bottom;
    
    // 左上角
    _sp_quad.tl.texCoords.u = left;
    _sp_quad.tl.texCoords.v = top;
    
    // 右上角
    _sp_quad.tr.texCoords.u = right;
    _sp_quad.tr.texCoords.v = top;
    
    // 2.处理四边形的顶点坐标
    float x1 = 0.0;
    float y1 = 0.0f;
    float x2 = 512.0f;
    float y2 = 512.0f;
    
    // 左下角
    _sp_quad.bl.vertices.set(x1, y1, 0.0f);
    // 右下角
    _sp_quad.br.vertices.set(x2, y1, 0.0f);
    // 左上角
    _sp_quad.tl.vertices.set(x1, y2, 0.0f);
    // 右上角
    _sp_quad.tr.vertices.set(x2, y2, 0.0f);
    
    // 3.更新到三角形上
    _sp_triangles.vertCount = 4; //顶点个数
    _sp_triangles.verts = (V3F_C4B_T2F*)&_sp_quad; //一个三角形命令的四个顶点数据
    _sp_triangles.indexCount = 6; //索引个数
    _sp_triangles.indices = quadIndices2;
    
    // 分配顶点属性数据
    memcpy(Vertexs + _filledVertex, _sp_triangles.verts, sizeof(V3F_C4B_T2F) * _sp_triangles.vertCount);
    
    _sp_modelView.setIdentity();
    
    // 更新每一个顶点的模型矩阵，根据这个去设置顶点着色器的值
    for (ssize_t i = 0; i < _sp_triangles.vertCount; ++i) {
        V3F_C4B_T2F *v_i = &Vertexs[i + _filledVertex];
        Vector3 *v_p = (Vector3 *)&v_i->vertices;
        _sp_modelView.transformPoint(v_p);
    }
    
    // 获取索引信息
    const unsigned short* indices = _sp_triangles.indices;
    for (ssize_t i = 0; i < _sp_triangles.indexCount; ++i) {
        Indices[i + _filledIndex] = _filledVertex + indices[i];
    }
    
    // 已处理的顶点下标
    _filledVertex += _sp_triangles.vertCount;
    _filledIndex += _sp_triangles.indexCount;
    
    if (_sp_triangles.indexCount % 3 != 0) {
        ssize_t count = _sp_triangles.indexCount;
        _sp_triangles.indexCount = count / 3 * 3;
    }
    
    int indexToDraw = 0;
    
    //Upload buffer to VBO
    if(_filledVertex <= 0 || _filledIndex <= 0)
    {
        std::cout << "image data is null" << std::endl;
        return;
    }
    // 设置默认的顶点对象数组
    OPGL::bindVAO(_triangleVAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, _triangleVBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertexs[0]) * _filledVertex, nullptr, GL_DYNAMIC_DRAW);
    //    void *buf = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    //    memcpy(buf, Vertexs, sizeof(Vertexs[0])* _filledVertex);
    //    // 映射缓存
    //    glUnmapBuffer(GL_ARRAY_BUFFER);
    
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertexs[0]) * _filledVertex, Vertexs);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _triangleVBO[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices[0]) * _filledIndex, Indices, GL_STATIC_DRAW);
    
    OPGL::bindVAO(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    indexToDraw += _sp_triangles.indexCount;
    
//    Vector3 trans(1.0f, 1.0f, 0.0f);
//    _sp_modelView.transformPoint(&trans);
    
    _sp_modelView.m[0] = 0.975803375;
    _sp_modelView.m[5] = 1.7320509;
    _sp_modelView.m[10] = -1.0231657;
    _sp_modelView.m[11] = -1;
    _sp_modelView.m[12] = -554.256348;
    _sp_modelView.m[13] = -554.256287;
    _sp_modelView.m[14] = 545.933044;
    _sp_modelView.m[15] = 553.346008;
    
    std::cout << ".....m...." << std::endl;
    for (int i = 0; i < 16; i++) {
        if (i % 4 == 0) {
            std::cout << std::endl;
        }
        std::cout << _sp_modelView.m[i] << "  ";
    }
    
    if (indexToDraw > 0) {
        //glEnable(GL_BLEND);
        //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBindTexture(GL_TEXTURE_2D, _sp_textureID); // 绑定，即可从_textureID中取出图像数据
        glUniformMatrix4fv(_uniforms[UNIFORM_TYPE_P_MATRIX], 1, GL_FALSE, _sp_modelView.m);
        
        OPGL::bindVAO(_triangleVAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
        std::cout << "drawing..." << std::endl;
        OPGL::bindVAO(0);
    }
    
    // 恢复默认的顶点对象数组
    _filledVertex = 0;
    _filledIndex = 0;
    
    if (this->_glView) {
        this->_glView->swapBuffers();
    }
}

void Game::mainLoop() {
//    if (this->_isValid) {
//        //std::cout << "有效状态，正在进行绘制..." << std::endl;
//        this->drawScene();
//    }
//    else {
//        std::cout << "无效状态下不进行绘制..." << std::endl;
//    }
    
    this->test();
}

void Game::startAnimation() {
    this->_isValid =true;
}

void Game::stopAnimation() {
    this->_isValid =false;
}

// 每隔一段时间才会被调用，不必要每一帧都要绘制，导致内存紧张
void Game::setAnimationInterval(double dValue) {
    this->_interval = dValue;
    if (this->_isValid) {
        stopAnimation();
    }
    else {
        startAnimation();
    }
}

size_t Game::getSceneCnt() {
    return this->_scenes.size();
}

bool Game::initMatrixStack() {
    while (!this->_modelViewStack.empty()) {
        this->_modelViewStack.pop();
    }
    
    while (!this->_projectionStack.empty()) {
        this->_projectionStack.pop();
    }
    
    // 添加单位矩阵
    this->_modelViewStack.push(Matrix::identity());
    this->_projectionStack.push(Matrix::identity());
    
    return true;
}

// 弹出矩阵
const Matrix& Game::popMatrix(MATRIX_STACK_TYPE type) {
    if (type == MATRIX_STACK_MODELVIEW) {
        if (this->_modelViewStack.size() == 0) {
            std::cout << "modelViewStack size is 0..." << std::endl;
            return nullptr;
        }
        return this->_modelViewStack.top();
    }
    else if (type == MATRIX_STACK_PROJECTION) {
        if (this->_projectionStack.size() == 0) {
            std::cout << "modelViewStack size is 0..." << std::endl;
            return nullptr;
        }
        return this->_projectionStack.top();
    }
    else {
        std::cout << "unkown matrix type..." << std::endl;
        return nullptr;
    }
}

// 压入矩阵
void Game::pushMatrix(MATRIX_STACK_TYPE type) {
    if (type == MATRIX_STACK_MODELVIEW) {
        this->_modelViewStack.push(this->_modelViewStack.top());
    }
    else if (type == MATRIX_STACK_PROJECTION) {
        this->_projectionStack.push(this->_projectionStack.top());
    }
    else {
        std::cout << "unkown matrix type..." << std::endl;
    }
}

//存储一个矩阵
void Game::loadMatrix(const Matrix& mat, MATRIX_STACK_TYPE type) {
    if (type == MATRIX_STACK_MODELVIEW) {
        this->_modelViewStack.top() = mat;
    }
    else if (type == MATRIX_STACK_PROJECTION) {
        this->_projectionStack.top() = mat;
    }
    else {
        std::cout << "unkown matrix type..." << std::endl;
    }
}

const Matrix& Game::getMatrix(MATRIX_STACK_TYPE type) {
    if (type == MATRIX_STACK_MODELVIEW) {
        if (this->_modelViewStack.size() == 0) {
            std::cout << "modelViewStack size is 0..." << std::endl;
            return nullptr;
        }
        return this->_modelViewStack.top();
    }
    else if (type == MATRIX_STACK_PROJECTION) {
        if (this->_projectionStack.size() == 0) {
            std::cout << "projectionStack size is 0..." << std::endl;
            return nullptr;
        }
        return this->_projectionStack.top();
    }
    else {
        std::cout << "unkown matrix type..." << std::endl;
        return nullptr;
    }
}

void Game::loadIdentityMatrix(MATRIX_STACK_TYPE type) {
    if (type == MATRIX_STACK_MODELVIEW) {
        this->_modelViewStack.top() = Matrix::identity();
    }
    else if (type == MATRIX_STACK_PROJECTION) {
        this->_projectionStack.top() = Matrix::identity();
    }
    else {
        std::cout << "unkown matrix type..." << std::endl;
    }
}

void Game::multiplyMatrix(MATRIX_STACK_TYPE type, const Matrix& mat) {
    if(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW == type)
    {
        this->_modelViewStack.top() *= mat;
    }
    else if(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION == type)
    {
        this->_projectionStack.top() *= mat;
    }
    else
    {
        std::cout << "unknow matrix stack type" << std::endl;
    }
}

CCSize Game::getVisibleSize(){
    if (this->_glView) {
        return this->_glView->getVisibleSize();
    }
    else {
        return CCSize(0, 0);
    }
}

Renderer* Game::getRenderer() {
    return this->_render;
}

Scene* Game::getRunningScene() {
    return this->_curscene;
}

float Game::getZEye() {
    return (_viewSize.height / 1.1566f);
}

void Game::setViewport() {
    if (this->_glView) {
        this->_glView->setViewPort(this->_viewSize.width, this->_viewSize.height);
    }
}

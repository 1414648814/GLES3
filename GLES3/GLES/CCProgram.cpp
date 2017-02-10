//
//  CCProgram.cpp
//  GLES
//
//  Created by staff on 16/12/13.
//  Copyright © 2016年 George1994. All rights reserved.
//


#include "CCShaderFile.h"
#include "CCProgram.h"

/*
 * --------------------------------Program--------------------------------
 */
const char* Program::SHADER_NAME_POSITION_TEXTURE_COLOR = "ShaderPositionTextureColor";
const char* Program::SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP = "ShaderPositionTextureColor_noMVP";

Program::Program() :
_vertShader(0),
_fragShader(0),
_program(-1) {
    this->_game = Game::getInstance();
    
}

Program::~Program() {
    
}

// 对shader进行编译
bool Program::compileShader(GLuint* shader, GLenum type, const GLchar* source) {
    std::string str = type == GL_VERTEX_SHADER ? "precision highp float;\n precision highp int;\n" : "precision mediump float;\n precision mediump int;\n";
    //1.将默认定义代码以及用户写的着色器程序的源代码分别保存到字符数组中
    const GLchar *sources[] = {
        str.c_str(),
        DEFAULT_SHADER_UNIFORMS,
        source
    };
    
    //2.创建着色器对象
    *shader = glCreateShader(type);
    
    //3.将着色器的源代码字符数组绑定到顶点着色器对象
    glShaderSource(*shader, sizeof(sources)/sizeof(*sources), sources, NULL);
    
    //4.编译顶点着色器对象
    glCompileShader(*shader);
    
    //5.获取shader的编译状态
    GLint status;
    glGetShaderiv(*shader, GL_COMPILE_STATUS, &status);
    if (!status) {
        GLint infoLen = 0;
        glGetShaderiv(*shader, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen) {
            char* infoLog = static_cast<char *>(malloc(sizeof(char) * infoLen));
            glGetShaderInfoLog(*shader, infoLen, NULL, infoLog);
            std::cout << "compile "<<  (type == GL_VERTEX_SHADER  ? "vertex " : " fragment ") << " shader wrong..." << infoLog << std::endl;
        }
    }
    else {
        std::cout << "compile "<< (type == GL_VERTEX_SHADER  ? "vertex " : " fragment ") << "shader success..." << std::endl;
    }
    
    return (status == GL_TRUE);
}

/*
 * 通过从内存的字符串流导入着色器程序
 * 需要按照顺序进行初始化
 */
bool Program::initWithByteArrays(const GLchar* shaderArr, const GLchar* fragArr) {
    //1.创建program
    this->_program = glCreateProgram();
    
    this->_vertShader = this->_fragShader = 0;
    
    // 数据不会空
    // 2. 分别编译shader
    if (shaderArr != NULL) {
        if (!this->compileShader(&this->_vertShader, GL_VERTEX_SHADER, shaderArr)) {
            std::cout << "compile the vertex shader failed." << std::endl;
            return false;
        }
    }
    
    if (fragArr != NULL) {
        if (!this->compileShader(&this->_fragShader, GL_FRAGMENT_SHADER, fragArr)) {
            std::cout << "compile the fragment shader failed." << std::endl;
            return false;
        }
    }
    
    // 3.分别将顶点着色器对象和片段着色器对象附加到着色器对象对象上
    if (this->_vertShader) {
        glAttachShader(this->_program, this->_vertShader);
    }
    
    if (this->_fragShader) {
        glAttachShader(this->_program, this->_fragShader);
    }
    
    return true;
}

Program* Program::createWithByteArrays(const GLchar* shaderArr, const GLchar* fragArr) {
    Program *program = new (std::nothrow) Program();
    if (program && program->initWithByteArrays(shaderArr, fragArr)) {
        program->bindVertAttrib();
        program->link();
        program->use();
        program->updateBuiltUniforms();
        program->clearShader();
        std::cout << "初始化shader成功..." << std::endl;
        return program;
    }
    
    return NULL;
}

// 通过文件读取
bool Program::initWithFileName(const std::string& shaderName, const std::string& fragName) {
    
    return true;
}

Program* Program::createWithFileName(const std::string& shaderName, const std::string& fragName) {
    Program *program = new (std::nothrow) Program();
    if (program && program->initWithFileName(shaderName, fragName)) {
        program->bindVertAttrib();
        program->link();
        program->use();
        program->updateBuiltUniforms();
        program->clearShader();
        std::cout << "初始化shader成功..." << std::endl;
        return program;
    }
    
    return NULL;
}

// 自定义把顶点属性索引绑定到顶点属性名
void Program::bindAttribLoacation(GLuint program, GLuint index,const GLchar *name) {
    glBindAttribLocation(this->_program, index, name);
}

// 获取属性索引（也就是着色器程序中attribute类型的id）
GLint Program::getAttribLocation(const GLchar *name) {
    return glGetAttribLocation(this->_program, name);
}

// 获取uniform索引（也就是着色器程序中uniform类型的id）
GLint Program::getUniformLocation(const GLchar *name) {
    return glGetUniformLocation(this->_program, name);
}

// 更新数组中的uniform位置的值
const std::string uniform_names[] = {
    UNIFORM_NAME_P_MATRIX,
    UNIFORM_NAME_MV_MATRIX,
    UNIFORM_NAME_MVP_MATRIX,
    UNIFORM_NAME_NOR_MATRIX,
    UNIFORM_NAME_TIME,
    UNIFORM_NAME_SIN_TIME,
    UNIFORM_NAME_COS_TIME,
    UNIFORM_NAME_RANDOM01,
    UNIFORM_NAME_SAMPLE0,
    UNIFORM_NAME_SAMPLE1,
    UNIFORM_NAME_SAMPLE2,
    UNIFORM_NAME_SAMPLE3
};

void Program::updateBuiltUniforms() {
    this->_builtInUniforms[UNIFORM_TYPE_P_MATRIX] = getUniformLocation(UNIFORM_NAME_P_MATRIX);
    this->_builtInUniforms[UNIFORM_TYPE_MV_MATRIX] = getUniformLocation(UNIFORM_NAME_MV_MATRIX);
    this->_builtInUniforms[UNIFORM_TYPE_MVP_MATRIX] = getUniformLocation(UNIFORM_NAME_MVP_MATRIX);
    this->_builtInUniforms[UNIFORM_TYPE_NORMAL_MATRIX] = getUniformLocation(UNIFORM_NAME_NOR_MATRIX);
    this->_builtInUniforms[UNIFORM_TYPE_TIME] = getUniformLocation(UNIFORM_NAME_TIME);
    this->_builtInUniforms[UNIFORM_TYPE_SIN_TIME] = getUniformLocation(UNIFORM_NAME_SIN_TIME);
    this->_builtInUniforms[UNIFORM_TYPE_COS_TIME] = getUniformLocation(UNIFORM_NAME_COS_TIME);
    this->_builtInUniforms[UNIFORM_TYPE_RANDOM01] = getUniformLocation(UNIFORM_NAME_RANDOM01);
    this->_builtInUniforms[UNIFORM_TYPE_TEXTURE0] = getUniformLocation(UNIFORM_NAME_SAMPLE0);
    this->_builtInUniforms[UNIFORM_TYPE_TEXTURE1] = getUniformLocation(UNIFORM_NAME_SAMPLE1);
    this->_builtInUniforms[UNIFORM_TYPE_TEXTURE2] = getUniformLocation(UNIFORM_NAME_SAMPLE2);
    this->_builtInUniforms[UNIFORM_TYPE_TEXTURE3] = getUniformLocation(UNIFORM_NAME_SAMPLE3);
    
    // 因为unfiorm中的纹理采样值不会变化，所以可以先设置
    if (this->_builtInUniforms[UNIFORM_TYPE_TEXTURE0] != -1) {
        glUniform1i(this->_builtInUniforms[UNIFORM_TYPE_TEXTURE0], 0);
    }
    if (this->_builtInUniforms[UNIFORM_TYPE_TEXTURE1] != -1) {
        glUniform1i(this->_builtInUniforms[UNIFORM_TYPE_TEXTURE1], 1);
    }
    if (this->_builtInUniforms[UNIFORM_TYPE_TEXTURE2] != -1) {
        glUniform1i(this->_builtInUniforms[UNIFORM_TYPE_TEXTURE2], 2);
    }
    if (this->_builtInUniforms[UNIFORM_TYPE_TEXTURE3] != -1) {
        glUniform1i(this->_builtInUniforms[UNIFORM_TYPE_TEXTURE3], 3);
    }
}

// 或者使用bind固定位置后，通过固定位置进行获取
void Program::bindVertAttrib() {
    static const struct {
        const char* attributeName;
        VERTEX_ATTRIB_TYPE type;
    }
    attribute_locations[] = {
        {ATTRIBUTE_NAME_POSITION, VERTEX_ATTRIB_POSITION},
        {ATTRIBUTE_NAME_COLOR, VERTEX_ATTRIB_COLOR},
        {ATTRIBUTE_NAME_TEXCOOD, VERTEX_ATTRIB_TEXCOOD},
    };
    
    // 把顶点属性索引绑定属性名
    for (int i = 0; i < sizeof(attribute_locations)/sizeof(attribute_locations[0]); i++) {
        glBindAttribLocation(this->_program, attribute_locations[i].type, attribute_locations[i].attributeName);
    }
}

/*＊
 *  1.启用
 *  2.传递设置的值到shader程序中，前提要先绑定属性位置
 */
void Program::setVertexAttribPointer(VERTEX_ATTRIB_TYPE type) {
    switch (type) {
        case VERTEX_ATTRIB_POSITION:
            glEnableVertexAttribArray(VERTEX_ATTRIB_POSITION);
            glVertexAttribPointer(VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(V3F_C4B_T2F), (GLvoid *)offsetof(V3F_C4B_T2F, vertices));
            break;
            
        case VERTEX_ATTRIB_COLOR:
            glEnableVertexAttribArray(VERTEX_ATTRIB_COLOR);
            glVertexAttribPointer(VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(V3F_C4B_T2F), (GLvoid *)offsetof(V3F_C4B_T2F, colors));
            break;
            
        case VERTEX_ATTRIB_TEXCOOD:
            glEnableVertexAttribArray(VERTEX_ATTRIB_TEXCOOD);
            glVertexAttribPointer(VERTEX_ATTRIB_TEXCOOD, 2, GL_FLOAT, GL_FALSE, sizeof(V3F_C4B_T2F), (GLvoid *)offsetof(V3F_C4B_T2F, texCoords));
            break;
    
        default:
            break;
    }
}

void Program::setUniformForBuilts(const Matrix& matMV) {
    // 获取模型视图矩阵
    auto& mat = this->_game->getMatrix(MATRIX_STACK_PROJECTION);
    // 设置Uniform类型
    if (this->_builtInUniforms[UNIFORM_TYPE_P_MATRIX] != -1) {
        glUniformMatrix4fv(this->_builtInUniforms[UNIFORM_TYPE_P_MATRIX], 1, GL_FALSE, mat.m);
    }
}

bool Program::link() {
    if (this->_program == -1) {
        std::cout << "the program is null..." << std::endl;
        return false;
    }
    
    glLinkProgram(this->_program);
    
    // 判断是否链接成功
    GLint status = GL_TRUE;
    glGetProgramiv(_program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        GLint infoLen = 0;
        glGetProgramiv(_program, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen) {
            char* infoLog = static_cast<char *>(malloc(sizeof(char) * infoLen));
            glGetProgramInfoLog(_program, infoLen, NULL, infoLog);
            glDeleteProgram(_program);
            std::cout << "link program wrong..." << infoLog << std::endl;
        }
    }
    else {
        std::cout << "the program link success..." << std::endl;
    }
    
    return (status == GL_TRUE);
}

bool Program::use() {
    if (this->_program == -1) {
        std::cout << "the program is null..." << std::endl;
        return false;
    }
    
    glUseProgram(this->_program);
    
    return true;
}

void Program::clearShader() {
    if (this->_vertShader) {
        glDeleteShader(this->_vertShader);
    }
    
    if (this->_fragShader) {
        glDeleteShader(this->_fragShader);
    }
    
    // 初始化
    this->_vertShader = this->_fragShader = 0;
}

/*
 * --------------------------------ProgramManager--------------------------------
 */

static ProgramManager* _manager = NULL;

ProgramManager::ProgramManager() {
    
    
}

ProgramManager::~ProgramManager() {
    
    
}

ProgramManager* ProgramManager::getInstance() {
    if (_manager == NULL) {
        ProgramManager * manager = new (std::nothrow) ProgramManager();
        if (manager && manager->init()) {
            _manager = manager;
            _manager->loadAllShaders();
            return _manager;
        }
        else {
            return NULL;
        }
    }
    else {
        return _manager;
    }
}

void ProgramManager::destroyInstance() {
    delete _manager;
    _manager = NULL;
}

bool ProgramManager::init() {
    
    
    return true;
}

void ProgramManager::loadAllShaders() {
    // 作为事例，只写一个
//    Program* program = new (std::nothrow) Program();
//    loadSingleShader(program, ShaderType_PositionTextureColor);
    
    Program* program = Program::createWithByteArrays(CCPositionTextureColorVert, CCPositionTextureColorFrag);
    this->_programs.insert(std::make_pair(Program::SHADER_NAME_POSITION_TEXTURE_COLOR, program));
}

void ProgramManager::loadSingleShader(Program* program, ShaderType type) {
    switch (type) {
        case ShaderType_PositionTextureColor:
            program->initWithByteArrays(CCPositionTextureColorVert, CCPositionTextureColorFrag);
            break;
            
        case ShaderType_Unkown:
            break;
            
        default:
            std::cout << "unkown type..." << std::endl;
            break;
    }
}

// 通过key获取program
Program* ProgramManager::getProgram(const std::string& key) {
    if (this->_programs[key]) {
        return this->_programs[key];
    }
    else {
        return NULL;
    }
}

void ProgramManager::addProgram(const std::string& key, Program* program) {
    auto prev = this->getProgram(key);
    if (prev == program) {
        return;
    }
    
    this->_programs.erase(key);
    this->_programs.insert(std::make_pair(key, program));
}

/*
 * --------------------------------ProgramState--------------------------------
 */

ProgramState::ProgramState() {
    
    
}

ProgramState::~ProgramState() {
    
    
}

ProgramState* ProgramState::create(Program* program) {
    ProgramState* ret = new (std::nothrow) ProgramState;
    if (ret && ret->init(program)) {
        return ret;
    }
    return NULL;
}

ProgramState* ProgramState::getOrCreateWithGLProgramName(const std::string& programname) {
    // 通过key获取到混存下来的program
    Program* program = ProgramManager::getInstance()->getProgram(programname);
    if (program != NULL) {
        ProgramState* ret = ProgramStateManager::getInstance()->getProgramState(program);
        return ret;
    }
    return NULL;
}

ProgramState* ProgramState::getOrCreateWithShaders(const std::string& vertexShader, const std::string& fragShader) {
    std::string key = vertexShader + "+" + fragShader;
    Program* program = ProgramManager::getInstance()->getProgram(key);
    if (!program) {
        program = Program::createWithFileName(vertexShader, fragShader);
        ProgramManager::getInstance()->addProgram(key, program);
    }
    
    return create(program);
}

void ProgramState::setProgram(Program* program) {
    this->_program = program;
}

Program* ProgramState::getProgram() const {
    return this->_program;
}

bool ProgramState::init(Program* program) {
    this->_program = program;
    return true;
}

void ProgramState::apply(const Matrix& modelView) {
    this->applyProgram(modelView);
    this->applyAttributes(true);
    this->applyUniforms();
}

void ProgramState::applyProgram(const Matrix& modelView) {
    if (this->_program == NULL) {
        std::cout << "invalid priogram!" << std::endl;
        return ;
    }
    
    //设置shader
    this->_program->setUniformForBuilts(modelView);
}

void ProgramState::applyAttributes(bool applyAttribFlags) {
    
    
}

void ProgramState::applyUniforms() {
    
    
}

/*
 * --------------------------------ProgramManager--------------------------------
 */

static ProgramStateManager* _stateManager = NULL;

ProgramStateManager::ProgramStateManager() {
    this->_programStates.clear();
    
}

ProgramStateManager::~ProgramStateManager() {
    
}

ProgramStateManager* ProgramStateManager::getInstance() {
    if (_stateManager == NULL) {
        ProgramStateManager * state = new (std::nothrow) ProgramStateManager();
        if (state) {
            _stateManager = state;
            return _stateManager;
        }
        return NULL;
    }
    else {
        return _stateManager;
    }
}

void ProgramStateManager::destroyInstance() {
    delete _stateManager;
    _stateManager = NULL;
}

// 通过key获取program
ProgramState* ProgramStateManager::getProgramState(Program* program) {
    auto itr = this->_programStates.find(program);
    if (itr != this->_programStates.end())
    {
        return itr->second;
    }
    
    auto ret = new (std::nothrow) ProgramState;
    if(ret && ret->init(program)) {
        this->_programStates.insert(std::make_pair(program, ret));
        return ret;
    }
    
    return ret;
}

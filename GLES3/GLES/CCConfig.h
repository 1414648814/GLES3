//
//  CCConfig.h
//  GLES
//
//  Created by staff on 16/12/12.
//  Copyright © 2016年 George1994. All rights reserved.
//

#ifndef CCConfig_h
#define CCConfig_h

#include "CCType.h"
#include "CCGL.h"
#include "CCPoint.h"

// 平台类型
enum GAME_PLATFORM_TYPE {
    GAME_PLATFORM_UNKOWN = 0,
    GAME_PLATFORM_MAC,
    GAME_PLATFORM_IPHONE
};

/**
 *  无法渲染 0
 *  渲染bounding box 1
 *  渲染texture box 2
 */
typedef enum {
    SPRITE_DRAW_DISABLE = 0,
    SPRITE_DRAW_BOUNDING_BOX,
    SPRITE_DRAW_TEXTURE_BOX
} SPRITE_DRAW_TYPE;

#define CC_SPRITE_DRAW 0 //默认是无法渲染


// Attribute顶点属性变量名字（需要和shader中的对应起来，否则无法传值）
typedef enum {
    VERTEX_ATTRIB_POSITION = 0, // 位置
    VERTEX_ATTRIB_COLOR,        // 颜色
    VERTEX_ATTRIB_TEXCOOD       // 纹理坐标
} VERTEX_ATTRIB_TYPE;

static const char* ATTRIBUTE_NAME_POSITION = "a_position";
static const char* ATTRIBUTE_NAME_COLOR    = "a_color";
static const char* ATTRIBUTE_NAME_TEXCOOD    = "a_texCood";

// Uniform顶点
static const char* UNIFORM_NAME_P_MATRIX = "CC_PMatrix";
static const char* UNIFORM_NAME_MV_MATRIX = "CC_MVMatrix";
static const char* UNIFORM_NAME_MVP_MATRIX = "CC_MVPMatrix";
static const char* UNIFORM_NAME_NOR_MATRIX = "CC_NormalMatrix";
static const char* UNIFORM_NAME_TIME = "CC_Time";
static const char* UNIFORM_NAME_SIN_TIME = "CC_SinTime";
static const char* UNIFORM_NAME_COS_TIME = "CC_CosTime";
static const char* UNIFORM_NAME_RANDOM01 = "CC_Random01";
static const char* UNIFORM_NAME_SAMPLE0 = "CC_Texture0";
static const char* UNIFORM_NAME_SAMPLE1 = "CC_Texture1";
static const char* UNIFORM_NAME_SAMPLE2 = "CC_Texture2";
static const char* UNIFORM_NAME_SAMPLE3 = "CC_Texture3";

// 默认在shader程序中增加的代码
static const char* DEFAULT_SHADER_UNIFORMS =
"uniform mat4 CC_PMatrix;\n"
"uniform mat4 CC_MVMatrix;\n"
"uniform mat4 CC_MVPMatrix;\n"
"uniform mat3 CC_NormalMatrix;\n"
"uniform vec4 CC_Time;\n"
"uniform vec4 CC_SinTime;\n"
"uniform vec4 CC_CosTime;\n"
"uniform vec4 CC_Random01;\n"
"uniform sampler2D CC_Texture0;\n"
"uniform sampler2D CC_Texture1;\n"
"uniform sampler2D CC_Texture2;\n"
"uniform sampler2D CC_Texture3;\n"
"//CC INCLUDES END\n\n";

#define MAX_VERTICES_SIZE 600000 //Buffer中一次存储进顶点数据的最大数目
#define MAX_INDICES_SIZE MAX_VERTICES_SIZE * 6 / 4 //前者的索引

// 只有在mac和ipone的情况下才可以使用VAO
#ifdef SUPPORT_USE_VAO
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_IPHONE)
        #define SUPPORT_USE_VAO 1
    #else
        #define SUPPORT_USE_VAO 0
    #endif
#endif

// 绘制命令类型，每种绘制命令都代表一种基本图形
typedef enum {
    COMMAND_TRIANGEL = 0, //三角形
    COMMAND_QUAD = 1, //长方形
    COMMAND_UNKOWN = 2, //未知
} COMMAND_TYPE;

// 渲染的类型
typedef enum {
    RENDER_QUEUE_MORE_ZORDER = 0, //z轴大于0的下标
    
} RENDER_QUEUE_TYPE;

// 绘制队列最大个数
#define MAX_RENDER_QUEUE_COUNT 5

// 材料是否批量
#define MATIRIAL_ID_NOT_BATCH 0

// 纹理的像素格式类型
typedef enum {
    Pixel_Format_RGBA8888, // 目前png和jpg都支持这种格式，所以目前只处理这一种
    Pixel_Format_UNKOWN
    
} Image_Pixel_Format_Type;


#define UNIFORM_MAX_NUM 12

typedef enum {
    UNIFORM_TYPE_P_MATRIX = 0,
    UNIFORM_TYPE_MV_MATRIX,
    UNIFORM_TYPE_MVP_MATRIX,
    UNIFORM_TYPE_NORMAL_MATRIX,
    UNIFORM_TYPE_TIME,
    UNIFORM_TYPE_SIN_TIME,
    UNIFORM_TYPE_COS_TIME,
    UNIFORM_TYPE_RANDOM01,
    UNIFORM_TYPE_TEXTURE0,
    UNIFORM_TYPE_TEXTURE1,
    UNIFORM_TYPE_TEXTURE2,
    UNIFORM_TYPE_TEXTURE3
    
} UNIFORM_TYPE;

typedef enum {
    MATRIX_STACK_MODELVIEW = 0, //模型视图矩阵
    MATRIX_STACK_PROJECTION, //投影矩阵
    
} MATRIX_STACK_TYPE;

/** @def CC_DEGREES_TO_RADIANS
 converts degrees to radians
 */
#define CC_DEGREES_TO_RADIANS(__ANGLE__) ((__ANGLE__) * 0.01745329252f) // PI / 180

/** @def CC_RADIANS_TO_DEGREES
 converts radians to degrees
 */
#define CC_RADIANS_TO_DEGREES(__ANGLE__) ((__ANGLE__) * 57.29577951f) // PI * 180

#endif /* CCConfig_h */

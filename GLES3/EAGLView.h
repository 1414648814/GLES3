//
//  EAGLView.h
//  GLES3
//
//  Created by staff on 17/1/6.
//  Copyright © 2017年 George. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "GLES/CCGL.h"

@interface EAGLView : UIView
{
@private
    EAGLContext* context;
    CADisplayLink* displayLink;
    BOOL updateFrameBuffer; //是否刷新帧缓存
    GLuint defaultFrameBuffer; //默认的帧缓存
    GLuint colorRenderBuffer; //颜色缓存
    GLuint depthRenderBuffer; //深度缓存
    
    GLint backingWidth;
    GLint backingHeight;
    
    CGRect originalRect;
    CGSize size;
}

@property (nonatomic, readonly) EAGLContext *context;

+ (id) viewWithFrame:(CGRect)frame;
- (id) initWithFrame:(CGRect)frame;

- (void)createFrameBuffer;
- (void)deleteFrameBuffer;
- (void)setFrameBuffer;
- (BOOL)presentFrameBuffer;

- (CGSize)backingSize;
- (float)getWidth;
- (float)getHeight;

- (void)swapBuffers;

+(id) sharedEGLView;

@end

//
//  EAGLView.m
//  GLES3
//
//  Created by staff on 17/1/6.
//  Copyright © 2017年 George. All rights reserved.
//

#import "EAGLView.h"
#include <iostream>

static EAGLView *view = 0;

@implementation EAGLView

@dynamic context;

+ (Class) layerClass
{
    return [CAEAGLLayer class];
}

+(id) sharedEGLView {
    if (view == 0) {
        std::cout << "view 为空..." << std::endl;
        return 0;
    }
    return view;
}

+ (id) viewWithFrame:(CGRect)frame {
    return [[self alloc] initWithFrame:frame];
}

- (id) initWithFrame:(CGRect)frame {
    if ((self = [super initWithFrame:frame]))
    {
        //初始化
        displayLink = nil;
        updateFrameBuffer = YES;
        defaultFrameBuffer = 0;
        colorRenderBuffer = 0;
        depthRenderBuffer = 0;
        
        CGFloat scale = [[UIScreen mainScreen] scale];
        CAEAGLLayer* layer = (CAEAGLLayer *)self.layer;
        layer.opaque = YES;
        layer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                    [NSNumber numberWithBool:FALSE],
                                    kEAGLDrawablePropertyRetainedBacking,
                                    kEAGLColorFormatRGBA8,
                                    kEAGLDrawablePropertyColorFormat,
                                    nil];
        self.contentScaleFactor = scale;
        layer.contentsScale = scale;
        
        // 初始化上下文并且设置成当前上下文
        context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
        if (!context || ![EAGLContext setCurrentContext:context])
        {
            return nil;
        }
        
        // Create default framebuffer object. The backing will be allocated for the current layer in -resizeFromLayer
        // 创建默认缓冲区
        glGenFramebuffers(1, &defaultFrameBuffer);
        NSAssert( defaultFrameBuffer, @"Can't create default frame buffer");
        
        // 创建颜色缓冲区
        glGenRenderbuffers(1, &colorRenderBuffer);
        NSAssert(colorRenderBuffer, @"Can't create default render buffer");
        
        // 绑定帧缓冲区和渲染缓冲区
        glBindFramebuffer(GL_FRAMEBUFFER, defaultFrameBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, colorRenderBuffer);
        
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderBuffer);
        
        view = self;
        originalRect = self.frame;
        
        if ([view respondsToSelector:@selector(setContentScaleFactor:)])
        {
            view.contentScaleFactor = [[UIScreen mainScreen] scale];
        }
    }
    
    return self;
}

//The EAGL view is stored in the nib file. When it's unarchived it's sent -initWithCoder:.
- (id)initWithCoder:(NSCoder*)coder
{
    self = [super initWithCoder:coder];
    if (self)
    {
        CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
        
        // kEAGLColorFormatRGBA8
        // kEAGLColorFormatRGB565
        eaglLayer.opaque = TRUE;
        eaglLayer.drawableProperties = [ NSDictionary dictionaryWithObjectsAndKeys:
                                        [ NSNumber numberWithBool:FALSE ],
                                        kEAGLDrawablePropertyRetainedBacking,
                                        kEAGLColorFormatRGBA8,
                                        kEAGLDrawablePropertyColorFormat,
                                        nil ];
        
        // Enable multitouch
        [ self setMultipleTouchEnabled:YES ];
    }
    
    view = self;
    return self;
}

- (void) layoutSubviews {
    CAEAGLLayer* layer = (CAEAGLLayer*)self.layer;
    
    // Allocate color buffer backing based on the current layer size
    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderBuffer);
    
    if( ! [context renderbufferStorage:GL_RENDERBUFFER fromDrawable:layer] )
    {
        NSLog(@"failed to call context");
    }

    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &backingWidth);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &backingHeight);
    
    //NSLog(@"cocos2d: surface size: %dx%d", (int)backingWidth, (int)backingHeight);
    
    GLenum error;
    if( (error=glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE)
    {
        NSLog(@"Failed to make complete framebuffer object 0x%X", error);
    }
    
    glViewport(0, 0, backingWidth, backingHeight);
}

- (EAGLContext *)context
{
    return context;
}

- (void)createFrameBuffer {
    if (context && !defaultFrameBuffer) {
        [EAGLContext setCurrentContext:context];
        
        // Create default framebuffer object. The backing will be allocated for the current layer in -resizeFromLayer
        // 创建默认缓冲区
        glGenFramebuffers(1, &defaultFrameBuffer);
        NSAssert( defaultFrameBuffer, @"Can't create default frame buffer");
        
        // 创建颜色缓冲区
        glGenRenderbuffers(1, &colorRenderBuffer);
        NSAssert(colorRenderBuffer, @"Can't create default render buffer");

        glBindFramebuffer(GL_FRAMEBUFFER, defaultFrameBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, colorRenderBuffer);
        
        // GL_DEPTH24_STENCIL8_OES: Set a depth buffer and a stencil buffer.
        // GL_DEPTH_COMPONENT16: Set a 16bits depth buffer.
        // GL_DEPTH_COMPONENT24_OES: Set a 24bits depth buffer.
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, backingWidth, backingHeight);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer);
        
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            NSLog(@"Failed to make complete framebuffer object %x", glCheckFramebufferStatus(GL_FRAMEBUFFER));
    }
}

- (void)deleteFrameBuffer {
    if (context) {
        [EAGLContext setCurrentContext:context];
        
        if (defaultFrameBuffer) {
            glDeleteFramebuffers(1, &defaultFrameBuffer);
            defaultFrameBuffer = 0;
        }
        
        if (colorRenderBuffer) {
            glDeleteFramebuffers(1, &colorRenderBuffer);
            colorRenderBuffer = 0;
        }
        
        if (depthRenderBuffer) {
            glDeleteFramebuffers(1, &depthRenderBuffer);
            depthRenderBuffer = 0;
        }
    }
    
}

- (void)setFrameBuffer {
    if (context) {
        [EAGLContext setCurrentContext:context];
        
        if (!defaultFrameBuffer) {
            [self createFrameBuffer];
        }
        
        glBindFramebuffer(GL_FRAMEBUFFER, defaultFrameBuffer);
    }
    
}

- (void)swapBuffers {
    if(![context presentRenderbuffer:GL_RENDERBUFFER])
    {
        std::cout << "presentRenderBuffer failed..." << std::endl;
    }
}

- (BOOL)presentFrameBuffer {
    BOOL success = FALSE;
    
    if (context)
    {
        [EAGLContext setCurrentContext:context];
        
        glBindRenderbuffer(GL_RENDERBUFFER, colorRenderBuffer);
        
        success = [context presentRenderbuffer:GL_RENDERBUFFER];
    }
    
    return success;
}

- (float)getWidth {
    CGSize bound = [self bounds].size;
    return (float)bound.width * self.contentScaleFactor;
}

- (float)getHeight {
    CGSize bound = [self bounds].size;
    return (float)bound.height * self.contentScaleFactor;
}

-(CGSize) backingSize
{
    return CGSizeMake(backingWidth, backingHeight);
}

- (void) dealloc {
    if ([EAGLContext currentContext] == context)
    {
        [EAGLContext setCurrentContext:nil];
    }
}

@end

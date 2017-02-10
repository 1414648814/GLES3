//
//  GameCaller.m
//  GLES3
//
//  Created by staff on 17/1/8.
//  Copyright © 2017年 George. All rights reserved.
//

#import "GameCaller.h"
#import "GLES/CCPoint.h"
#import "GLES/CCGame.h"
#import "EAGLView.h"
#import "GLES/CCGL.h"
static id s_sharedGameCaller;

//@interface NSObject(CADisplayLink)
//+(id) displayLinkWithTarget: (id)arg1 selector:(SEL)arg2;
//-(void) addToRunLoop: (id)arg1 forMode: (id)arg2;
//-(void) setFrameInterval: (int)interval;
//-(void) invalidate;
//@end

@implementation GameCaller

@synthesize interval;

+(id) getInstance
{
    if (s_sharedGameCaller == nil)
    {
        s_sharedGameCaller = [GameCaller new];
    }
    
    return s_sharedGameCaller;
}

+(void) destroy
{
    s_sharedGameCaller = nil;
}

-(void) alloc
{
    interval = 1;
    isLoop = FALSE;
}

-(void) startMainLoop
{
    // CCDirector::setAnimationInterval() is called, we should invalidate it first
    if (!isLoop) {
        [displayLink invalidate];
        displayLink = nil;
        
        displayLink = [NSClassFromString(@"CADisplayLink") displayLinkWithTarget:self selector:@selector(doGameCaller:)];
        [displayLink setFrameInterval: self.interval];
        [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
        
        isLoop = TRUE;
    }
}

-(void) stopMainLoop
{
    if (isLoop) {
        [displayLink invalidate];
        displayLink = nil;
        isLoop = FALSE;
    }
}

-(void) setAnimationInterval:(double)intervalNew
{
    // CCDirector::setAnimationInterval() is called, we should invalidate it first
    [displayLink invalidate];
    displayLink = nil;
    
    self.interval = 60.0 * intervalNew; //设置每秒绘制60帧
    
    displayLink = [NSClassFromString(@"CADisplayLink") displayLinkWithTarget:self selector:@selector(doGameCaller:)];
    [displayLink setFrameInterval: self.interval];
    [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
}

-(void) doGameCaller: (id) sender
{
    // 这里会一直运行，所以需要弄成单例模式
    Game *game = Game::getInstance();
    [EAGLContext setCurrentContext: [(__bridge EAGLView*)game->getGLView()->getEAGLView() context]];
    Game::getInstance()->mainLoop();
}

@end
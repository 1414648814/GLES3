//
//  GameCaller.h
//  GLES3
//
//  Created by staff on 17/1/8.
//  Copyright © 2017年 George. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <OpenGLES/EAGL.h>

// 调用Game
@interface GameCaller : NSObject {
    CADisplayLink *displayLink;
    int interval;
    BOOL isLoop;
}
@property (readwrite) int interval;
+(id) getInstance;
-(void) startMainLoop;
-(void) stopMainLoop;
-(void) doGameCaller: (id) sender;
-(void) setAnimationInterval:(double)interval;
+(void) destroy;
@end


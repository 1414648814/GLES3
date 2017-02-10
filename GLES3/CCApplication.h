//
//  CCApplication.h
//  GLES3
//
//  Created by staff on 17/1/6.
//  Copyright © 2017年 George. All rights reserved.
//

#ifndef CCApplication_h
#define CCApplication_h

// 调用GameCaller
class Application {
public:
    static Application* getInstance();
    void deleteInstance();
    
    void Run();
    
    virtual bool applicationDidFinishLaunching() {
        return true;
    }
    
    virtual void applicationDidEnterBackground() {}

    virtual void applicationWillEnterForeground() {}
    
    virtual void setAnimationInterval(double interval) {}
    
protected:
    Application();
    virtual ~Application() {}
    
    static Application * application;
};


#endif /* CCApplication_h */
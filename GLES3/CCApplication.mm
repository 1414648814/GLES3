//
//  CCApplication.mm
//  GLES3
//
//  Created by staff on 17/1/6.
//  Copyright © 2017年 George. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <cassert>
#import <iostream>
#import "CCApplication.h"
#import "GameCaller.h"

// 封装Application
Application* Application::application = nullptr;

Application::Application() {
    application = this;
}

Application* Application::getInstance() {
    if (application == nullptr) {
        assert(application == nullptr);
        application = new Application();
    }
    return application;
}

void Application::deleteInstance() {
    delete application;
}

void Application::Run() {
    // 加入scene以后才能进入循环
    if (this->applicationDidFinishLaunching()) {
        [[GameCaller getInstance] startMainLoop];
    }
}
#include "AppDelegate.h"
#include <codecvt>
#include "DocumentManager.h"
#include "UICreator.h"

// uncomment this if you want to use the audio engine, the usage of audio engine is in the cpp_test
// #define USE_AUDIO_ENGINE 1

#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;
#endif

USING_NS_CC;
using namespace rapidjson;

// set the size of the window
static cocos2d::Size designResolutionSize = cocos2d::Size(1280, 720);
static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate() 
{
#if USE_AUDIO_ENGINE
    AudioEngine::end();
#endif
}

// if you want a different context, modify the value of glContextAttrs
// it will affect all platforms
void AppDelegate::initGLContextAttrs()
{
    // set OpenGL context attributes: red,green,blue,alpha,depth,stencil,multisamplesCount
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8, 0};

    GLView::setGLContextAttrs(glContextAttrs);
}

// if you want to use the package manager to install more packages,  
// don't modify or remove this function
// It's designed for Android platform.
static int register_all_packages()
{
    return 0; //flag for packages manager
}


bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        // Get a Window on PC, and set the resolution of the window
        glview = GLViewImpl::createWithRect("FarmingGame", cocos2d::Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
        // glview = GLViewImpl::createWithFullScreen("FarmingGame");
#else
        glview = GLViewImpl::create("FarmingGame");
#endif
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0f / 60);

    // Set the design resolution of the scene. It is different from the resolution of the window.
    glview->setDesignResolutionSize(smallResolutionSize.width, smallResolutionSize.height, ResolutionPolicy::NO_BORDER);

#if 0 
    auto frameSize = glview->getFrameSize();
    // if the frame's height is larger than the height of medium size.
    if (frameSize.height > mediumResolutionSize.height)
    {        
        director->setContentScaleFactor(MIN(largeResolutionSize.height/designResolutionSize.height, largeResolutionSize.width/designResolutionSize.width));
    }
    // if the frame's height is larger than the height of small size.
    else if (frameSize.height > smallResolutionSize.height)
    {        
        director->setContentScaleFactor(MIN(mediumResolutionSize.height/designResolutionSize.height, mediumResolutionSize.width/designResolutionSize.width));
    }
    // if the frame's height is smaller than the height of medium size.
    else
    {        
        director->setContentScaleFactor(MIN(smallResolutionSize.height/designResolutionSize.height, smallResolutionSize.width/designResolutionSize.width));
    }
#endif

    register_all_packages();

    try
    {
        // creator to cocos2dx scene load
        auto manager = DocumentManager::getInstance();
        const Document* global_document = manager->getDocument("global");
        auto main_scene_path = (*global_document)["UI"]["MainUI"].GetString();
        Scene* scene = Scene::create();
        manager->loadDocument(main_scene_path);
    	auto main_UI = manager->getDocument("MainUI");
    	Layer* map_layer = UICreator::createWithFile(main_UI);
    	scene->addChild(map_layer);
        // manager->createArchiveDocument(18);
        // manager->saveArchiveDocument();
        // manager->saveConfigDocument();

        // Layer UI_layer = UICreator::createwithfile("MainScene");
        // scene->addChild(UI_layer);
        // run
        // director->runWithScene(scene_with_cpp);
        // director->runWithScene(scene_with_creator);

        manager->loadDocument("Usr");
        // manager->createArchiveDocument(12);
        // manager -> freeArchiveDocument();



		


    }
    catch (const std::exception& e)
    {
        auto manager = DocumentManager::getInstance();
        manager->saveArchiveDocument();
        manager->saveConfigDocument();

        MessageBoxA(
            NULL,
            (LPCSTR) e.what(),
            (LPCSTR)L"Error Details",
            MB_ICONWARNING | MB_DEFBUTTON2
        );
    }
    

    return true;
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::pauseAll();
#endif
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::resumeAll();
#endif
}

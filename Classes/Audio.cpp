#include "Audio.h"

USING_NS_CC;
using namespace cocos2d::experimental;

// 初始化静态成员
MusicManager* MusicManager::instance = nullptr;

MusicManager::MusicManager()
    : backgroundMusicId(-1) {
}

MusicManager::~MusicManager() {
    // 在析构时停止背景音乐和音效
    stopBackgroundMusic();
    for (int effectId : effectIds) {
        AudioEngine::stop(effectId);
    }
}

// 获取单例实例
MusicManager* MusicManager::getInstance() {
    if (!instance) {
        instance = new MusicManager();
    }
    return instance;
}

void MusicManager::playBackgroundMusic(const std::string& filename, bool loop) {
    if (backgroundMusicId != -1) {
        stopBackgroundMusic();  // 如果已经在播放背景音乐，先停止它
    }
    backgroundMusicId = AudioEngine::play2d(filename, loop);
}

void MusicManager::stopBackgroundMusic() {
    if (backgroundMusicId != -1) {
        AudioEngine::stop(backgroundMusicId);
        backgroundMusicId = -1;
    }
}

void MusicManager::pauseBackgroundMusic() {
    if (backgroundMusicId != -1) {
        AudioEngine::pause(backgroundMusicId);
    }
}

void MusicManager::resumeBackgroundMusic() {
    if (backgroundMusicId != -1) {
        AudioEngine::resume(backgroundMusicId);
    }
}

void MusicManager::playEffect(const std::string& filename, bool loop) {
    int effectId = AudioEngine::play2d(filename, loop);
    effectIds.push_back(effectId);
}

void MusicManager::stopEffect(int effectId) {
    AudioEngine::stop(effectId);
}

void MusicManager::setBackgroundVolume(float volume) {
    AudioEngine::setVolume(backgroundMusicId, volume);
}

void MusicManager::setEffectVolume(float volume) {
    for (int effectId : effectIds) {
        AudioEngine::setVolume(effectId, volume);
    }
}

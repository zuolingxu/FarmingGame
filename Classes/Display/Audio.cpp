#include "Audio.h"

USING_NS_CC;
using namespace cocos2d::experimental;

// ��ʼ����̬��Ա
MusicManager* MusicManager::instance = nullptr;

MusicManager::MusicManager()
    : backgroundMusicId(-1) {
}

MusicManager::~MusicManager() {
    // ������ʱֹͣ�������ֺ���Ч
    stopBackgroundMusic();
    for (int effectId : effectIds) {
        AudioEngine::stop(effectId);
    }
}

// ��ȡ����ʵ��
MusicManager* MusicManager::getInstance() {
    if (!instance) {
        instance = new MusicManager();
    }
    return instance;
}

void MusicManager::playBackgroundMusic(const std::string& filename, bool loop) {
    if (backgroundMusicId != -1) {
        stopBackgroundMusic();  // ����Ѿ��ڲ��ű������֣���ֹͣ��
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

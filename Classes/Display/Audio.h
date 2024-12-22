#ifndef MUSICMANAGER_H
#define MUSICMANAGER_H

#include <string>
#include "cocos2d.h"
#include "audio/include/AudioEngine.h"

class MusicManager {
public:
    // 构造函数、析构函数
    MusicManager();
    ~MusicManager();

    // 单例模式获取实例
    static MusicManager* getInstance();

    // 播放背景音乐
    void playBackgroundMusic(const std::string& filename, bool loop = true);

    //根据场景播放音乐
    void playMusicOfScene(const std::string& scene);

    // 停止背景音乐
    void stopBackgroundMusic();

    // 暂停背景音乐
    void pauseBackgroundMusic();

    // 恢复背景音乐
    void resumeBackgroundMusic();

    // 播放音效
    void playEffect(const std::string& filename, bool loop = false);

    // 停止音效
    void stopEffect(int effectId);

    // 设置音量
    void setBackgroundVolume(float volume);

    void setEffectVolume(float volume);

    void playMusicOfScene(const std::string& scene);

private:
    // 单例实例
    static MusicManager* instance;

    // 背景音乐ID和音效ID
    int backgroundMusicId;
    std::vector<int> effectIds;
    std::string cur_music_;
};

#endif // MUSICMANAGER_H
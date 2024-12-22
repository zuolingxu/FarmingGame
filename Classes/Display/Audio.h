#ifndef MUSICMANAGER_H
#define MUSICMANAGER_H

#include <string>
#include "cocos2d.h"
#include "audio/include/AudioEngine.h"

class MusicManager {
public:
    // ���캯������������
    MusicManager();
    ~MusicManager();

    // ����ģʽ��ȡʵ��
    static MusicManager* getInstance();

    // ���ű�������
    void playBackgroundMusic(const std::string& filename, bool loop = true);

    //���ݳ�����������
    void playMusicOfScene(const std::string& scene);

    // ֹͣ��������
    void stopBackgroundMusic();

    // ��ͣ��������
    void pauseBackgroundMusic();

    // �ָ���������
    void resumeBackgroundMusic();

    // ������Ч
    void playEffect(const std::string& filename, bool loop = false);

    // ֹͣ��Ч
    void stopEffect(int effectId);

    // ��������
    void setBackgroundVolume(float volume);

    void setEffectVolume(float volume);

    void playMusicOfScene(const std::string& scene);

private:
    // ����ʵ��
    static MusicManager* instance;

    // ��������ID����ЧID
    int backgroundMusicId;
    std::vector<int> effectIds;
    std::string cur_music_;
};

#endif // MUSICMANAGER_H
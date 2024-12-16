#pragma once
#pragma once
#include "cocos2d.h"
#include "MapObject.h"
#include "DocumentManager.h"
#include "json/document.h"
#include "MapLayer.h"

class Mineral : public MapObject {
private:

    MapLayer* parent;


    bool isMined;  // 是否已被挖掘
    std::string mineralType;  // 矿石类型


public:
    // 构造函数
    explicit Mineral(const Vec<int>& pos ,MapLayer* parent, bool im,const std::string& type);

    // 析构函数
    ~Mineral() override;

    // 创建矿石实例（从存档读取）
    static MapObject* create(rapidjson::Value& val, MapLayer* parent, const Vec<int>& pos);

    // 初始化矿石（比如加载对应的资源）
    void init() override;

    // 玩家交互（挖掘矿石）
    void interact() override;

    // 清理矿石（清理资源）
    void clear() override;

    // 暂停
    void pause() override;

    // 恢复
    void resume() override;

    void settle() override;

    // 是否与其他对象发生碰撞
    bool hasCollision() override;

    // 处理矿石挖掘后的存档更新
    void update_ismined_Archive(const Vec<int>& position, bool isMinedValue);
};


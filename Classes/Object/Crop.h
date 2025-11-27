#pragma once
#include "cocos2d.h"
#include "DocumentManager.h"
#include "json/document.h"
#include "MapLayer.h"
#include <string>

USING_NS_CC;

/**
 * BaseCrop
 * --------
 * 抽象组件：定义所有作物共同的接口。
 */
class BaseCrop {
public:
    virtual ~BaseCrop() = default;

    // 作物创建并挂载到某个格子时调用
    virtual void init(cocos2d::Node* parent, const cocos2d::Vec2& tilePos) = 0;

    // 每个游戏日结算一次
    virtual void dailySettle() = 0;

    // 尝试收获，成功则返回 true（一般意味着可以从地图上移除）
    virtual bool harvest() = 0;

    // 清理在场景上的所有可视对象
    virtual void clear() = 0;

    // 基本状态访问
    virtual const std::string& getName() const = 0;
    virtual int  getLiveDay() const = 0;
    virtual int  getMaturationDay() const = 0;
    virtual bool isWatered() const = 0;
    virtual bool isWithered() const = 0;
    virtual void setWithered(bool withered) = 0;

    // 玩家或环境对作物的操作
    virtual void water() = 0;
    virtual void applyPesticide() = 0;
};

/**
 * BasicCrop
 * ---------
 * 具体组件：正常生长的作物逻辑，不考虑干旱、虫害等额外状态。
 */
class BasicCrop : public BaseCrop {
public:
    BasicCrop(const std::string& name,
              int maturationDay);

    ~BasicCrop() override = default;

    void init(cocos2d::Node* parent, const cocos2d::Vec2& tilePos) override;
    void dailySettle() override;
    bool harvest() override;
    void clear() override;

    const std::string& getName() const override { return _name; }
    int  getLiveDay() const override { return _liveDay; }
    int  getMaturationDay() const override { return _maturationDay; }
    bool isWatered() const override { return _watered; }
    bool isWithered() const override { return _withered; }
    void setWithered(bool withered) override { _withered = withered; }

    void water() override { _watered = true; }
    void applyPesticide() override { /* BasicCrop 默认不处理农药逻辑 */ }

protected:
    std::string _name;
    int  _liveDay;         // 生存天数
    int  _maturationDay;   // 成熟所需天数
    bool _watered;         // 当天是否浇水
    bool _withered;        // 是否已经枯萎

    // 可视化信息（简单示例）
    cocos2d::Node*  _parent;
    cocos2d::Vec2   _tilePos;
    cocos2d::Sprite* _sprite;

    // 根据当前状态更新贴图/颜色
    void updateSpriteFrame();
};

/**
 * CropDecorator
 * -------------
 * 抽象装饰器：实现 BaseCrop，并持有一个 BaseCrop 指针，大部分工作转发给它。
 */
class CropDecorator : public BaseCrop {
public:
    explicit CropDecorator(std::shared_ptr<BaseCrop> component);
    ~CropDecorator() override = default;

    void init(cocos2d::Node* parent, const cocos2d::Vec2& tilePos) override;
    void dailySettle() override;
    bool harvest() override;
    void clear() override;

    const std::string& getName() const override;
    int  getLiveDay() const override;
    int  getMaturationDay() const override;
    bool isWatered() const override;
    bool isWithered() const override;
    void setWithered(bool withered) override;

    void water() override;
    void applyPesticide() override;

protected:
    std::shared_ptr<BaseCrop> _component;
};

/**
 * ImmatureCropDecorator
 * ---------------------
 * 未成熟：主要限制收获。
 */
class ImmatureCropDecorator : public CropDecorator {
public:
    explicit ImmatureCropDecorator(std::shared_ptr<BaseCrop> component);

    bool harvest() override;   // 未成熟时禁止收获
    void dailySettle() override;
};

/**
 * MatureCropDecorator
 * -------------------
 * 成熟：允许收获，可在这里增加额外收益或特效。
 */
class MatureCropDecorator : public CropDecorator {
public:
    explicit MatureCropDecorator(std::shared_ptr<BaseCrop> component);

    bool harvest() override;   // 可加收获奖励
    void dailySettle() override;
};

/**
 * DroughtCropDecorator
 * --------------------
 * 干旱：连续多天不浇水会导致枯萎。
 */
class DroughtCropDecorator : public CropDecorator {
public:
    explicit DroughtCropDecorator(std::shared_ptr<BaseCrop> component);

    void dailySettle() override;
    void water() override;   // 浇水会清除连续干旱计数
private:
    int _continuousDryDays;
};

/**
 * PestCropDecorator
 * -----------------
 * 虫害：每天有一定概率让作物枯萎，可以用农药清除虫害状态。
 */
class PestCropDecorator : public CropDecorator {
public:
    explicit PestCropDecorator(std::shared_ptr<BaseCrop> component);

    void dailySettle() override;
    void applyPesticide() override; // 清除虫害
private:
    bool _underPest;
};

/**
 * WitheredCropDecorator
 * ---------------------
 * 枯萎：强制作物死亡，不再生长，收获只用于清地。
 */
class WitheredCropDecorator : public CropDecorator {
public:
    explicit WitheredCropDecorator(std::shared_ptr<BaseCrop> component);

    void dailySettle() override;
    bool harvest() override;      // 允许清理但不产出
    bool isWithered() const override { return true; }
};



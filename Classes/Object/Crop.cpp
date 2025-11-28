#include "Crop.h"
#include "Land.h"
#include "MainCharacter.h"
#include "TimeManager.h"
#include "HelperClasses.h"
#include <random>

// =======================
// BasicCrop implementation
// =======================

BasicCrop::BasicCrop(const std::string& name,
                     int maturationDay)
: _name(name),
  _liveDay(0),
  _maturationDay(maturationDay),
  _watered(false),
  _withered(false),
  _parent(nullptr),
  _tilePos(Vec2::ZERO),
  _sprite(nullptr) {
}

void BasicCrop::init(cocos2d::Node* parent, const cocos2d::Vec2& tilePos) {
    _parent  = parent;
    _tilePos = tilePos;

    // 简单示例：实际项目中可以按 _name 加载对应的 plist / png
    _sprite = Sprite::create();
    if (_sprite) {
        _sprite->setPosition(tilePos);
        if (_parent) {
            _parent->addChild(_sprite);
        }
    }
    updateSpriteFrame();
}

void BasicCrop::dailySettle() {
    if (_withered) {
        return;
    }

    if (_watered) {
        ++_liveDay;
        _watered = false; // 浇水只作用一天
    } else {
        // 不浇水：这里简单地让作物不增长
    }

    if (_liveDay < 0) {
        _liveDay = 0;
    }

    // 简化规则：如果一直是“种下但没长”的状态，可以认为枯萎
    if (_liveDay == 0 && !_watered) {
        _withered = true;
    }

    updateSpriteFrame();
}

bool BasicCrop::harvest() {
    // 只有未枯萎且已经成熟才允许收获
    if (!_withered && _liveDay >= _maturationDay) {
        // 这里本来应该生成物品、加到背包中，这里只做示意
        clear();
        return true;
    }
    return false;
}

void BasicCrop::clear() {
    if (_sprite && _sprite->getParent()) {
        _sprite->removeFromParent();
    }
    _sprite = nullptr;
}

void BasicCrop::updateSpriteFrame() {
    if (!_sprite) return;

    // 非真实美术，仅用颜色表示不同阶段
    if (_withered) {
        _sprite->setColor(Color3B::GRAY);
        return;
    }

    float ratio = _maturationDay > 0
                  ? static_cast<float>(_liveDay) / _maturationDay
                  : 1.0f;
    if (ratio < 0.33f) {
        _sprite->setColor(Color3B::GREEN);   // 幼苗
    } else if (ratio < 0.66f) {
        _sprite->setColor(Color3B::YELLOW);  // 生长期
    } else {
        _sprite->setColor(Color3B::ORANGE);  // 成熟
    }
}

// =======================
// CropDecorator
// =======================

CropDecorator::CropDecorator(std::shared_ptr<BaseCrop> component)
: _component(std::move(component)) {
}

void CropDecorator::init(cocos2d::Node* parent, const cocos2d::Vec2& tilePos) {
    _component->init(parent, tilePos);
}

void CropDecorator::dailySettle() {
    _component->dailySettle();
}

bool CropDecorator::harvest() {
    return _component->harvest();
}

void CropDecorator::clear() {
    _component->clear();
}

const std::string& CropDecorator::getName() const {
    return _component->getName();
}

int CropDecorator::getLiveDay() const {
    return _component->getLiveDay();
}

int CropDecorator::getMaturationDay() const {
    return _component->getMaturationDay();
}

bool CropDecorator::isWatered() const {
    return _component->isWatered();
}

bool CropDecorator::isWithered() const {
    return _component->isWithered();
}

void CropDecorator::setWithered(bool withered) {
    _component->setWithered(withered);
}

void CropDecorator::water() {
    _component->water();
}

void CropDecorator::applyPesticide() {
    _component->applyPesticide();
}

// =======================
// ImmatureCropDecorator
// =======================

ImmatureCropDecorator::ImmatureCropDecorator(std::shared_ptr<BaseCrop> component)
: CropDecorator(std::move(component)) {
}

bool ImmatureCropDecorator::harvest() {
    // 未成熟且未枯萎时禁止收获
    if (getLiveDay() < getMaturationDay() && !isWithered()) {
        // 实际项目中可以弹出提示：“作物尚未成熟”
        return false;
    }
    return CropDecorator::harvest();
}

void ImmatureCropDecorator::dailySettle() {
    CropDecorator::dailySettle();
    // 可以在这里加“未成熟中的特效”，当前保持空实现
}

// =======================
// MatureCropDecorator
// =======================

MatureCropDecorator::MatureCropDecorator(std::shared_ptr<BaseCrop> component)
: CropDecorator(std::move(component)) {
}

bool MatureCropDecorator::harvest() {
    bool success = CropDecorator::harvest();
    if (success) {
        // 这里可以增加“完美成熟奖励”等逻辑
    }
    return success;
}

void MatureCropDecorator::dailySettle() {
    CropDecorator::dailySettle();
    // 可以在这里做高亮、闪烁等提示“可收获”
}

// =======================
// DroughtCropDecorator
// =======================

DroughtCropDecorator::DroughtCropDecorator(std::shared_ptr<BaseCrop> component)
: CropDecorator(std::move(component)),
  _continuousDryDays(0) {
}

void DroughtCropDecorator::dailySettle() {
    // 先根据浇水情况更新连续干旱天数
    if (!isWatered() && !isWithered()) {
        ++_continuousDryDays;
        if (_continuousDryDays >= 3) {
            // 连续 3 天不浇水 → 枯萎
            setWithered(true);
        }
    } else {
        _continuousDryDays = 0;
    }

    CropDecorator::dailySettle();
}

void DroughtCropDecorator::water() {
    _continuousDryDays = 0;
    CropDecorator::water();
}

// =======================
// PestCropDecorator
// =======================

PestCropDecorator::PestCropDecorator(std::shared_ptr<BaseCrop> component)
: CropDecorator(std::move(component)),
  _underPest(true) {
}

void PestCropDecorator::dailySettle() {
    CropDecorator::dailySettle();

    if (!_underPest || isWithered()) return;

    // 每天有一定概率直接让作物枯萎
    static std::default_random_engine engine(std::random_device{}());
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    float r = dist(engine);
    if (r < 0.1f) { // 10% 概率
        setWithered(true);
    }
}

void PestCropDecorator::applyPesticide() {
    _underPest = false;
    CropDecorator::applyPesticide();
}

// =======================
// WitheredCropDecorator
// =======================

WitheredCropDecorator::WitheredCropDecorator(std::shared_ptr<BaseCrop> component)
: CropDecorator(std::move(component)) {
    // 构造时立即标记底层作物枯萎
    setWithered(true);
}

void WitheredCropDecorator::dailySettle() {
    // 一旦枯萎就不再生长，因此不调用基类逻辑
}

bool WitheredCropDecorator::harvest() {
    // 允许玩家“清地”，但不产出
    clear();
    return true;
}


![alt text](./images/cover.jpeg)

<div style="page-break-after: always;"></div>

# Software Design Patterns -- Refactoring Report

**Catelogue**
<!-- vscode-markdown-toc -->
* 1. [Project Intorduction](#ProjectIntorduction)
	* 1.1. [Project Background](#ProjectBackground)
	* 1.2. [Project Function](#ProjectFunction)
		* 1.2.1. [Player and Farm Features](#PlayerandFarmFeatures)
		* 1.2.2. [Community and NPC Features](#CommunityandNPCFeatures)
		* 1.2.3. [Exploration and Adventure Features](#ExplorationandAdventureFeatures)
		* 1.2.4. [Character Progression and System Features](#CharacterProgressionandSystemFeatures)
* 2. [Creational Patterns](#CreationalPatterns)
	* 2.1. [Factory Pattern](#FactoryPattern)
		* 2.1.1. [Brief Introduction](#BriefIntroduction)
		* 2.1.2. [Reason for Refactoring](#ReasonforRefactoring)
		* 2.1.3. [Refactoring Details](#RefactoringDetails)
		* 2.1.4. [UML Class Diagram](#UMLClassDiagram)
		* 2.1.5. [Benefits of Refactoring](#BenefitsofRefactoring)
* 3. [Structural Patterns](#StructuralPatterns)
	* 3.1. [Decorator Pattern](#DecoratorPattern)
		* 3.1.1. [Brief Introduction](#BriefIntroduction-1)
		* 3.1.2. [Reason for Refactoring](#ReasonforRefactoring-1)
		* 3.1.3. [Refactoring Details](#RefactoringDetails-1)
		* 3.1.4. [UML Class Diagram](#UMLClassDiagram-1)
		* 3.1.5. [Benefits of Refactoring](#BenefitsofRefactoring-1)
	* 3.2. [Facade Pattern](#FacadePattern)
		* 3.2.1. [Brief Introduction](#BriefIntroduction-1)
		* 3.2.2. [Reason for Refactoring](#ReasonforRefactoring-1)
		* 3.2.3. [Refactoring Details](#RefactoringDetails-1)
		* 3.2.4. [UML Class Diagram](#UMLClassDiagram-1)
		* 3.2.5. [Benefits of Refactoring](#BenefitsofRefactoring-1)
* 4. [Behavioral Patterns](#BehavioralPatterns)
	* 4.1. [Chain of Responsibility Pattern](#ChainofResponsibilityPattern)
		* 4.1.1. [Brief Introduction](#BriefIntroduction-1)
		* 4.1.2. [Reason for Refactoring](#ReasonforRefactoring-1)
		* 4.1.3. [Refactoring Details](#RefactoringDetails-1)
		* 4.1.4. [UML Class Diagram](#UMLClassDiagram-1)
		* 4.1.5. [Benefits of Refactoring](#BenefitsofRefactoring-1)
	* 4.2. [Strategy Pattern](#StrategyPattern)
		* 4.2.1. [Brief Introduction](#BriefIntroduction-1)
		* 4.2.2. [Reason for Refactoring](#ReasonforRefactoring-1)
		* 4.2.3. [Refactoring Details](#RefactoringDetails-1)
		* 4.2.4. [UML Class Diagram](#UMLClassDiagram-1)
		* 4.2.5. [Benefits of Refactoring](#BenefitsofRefactoring-1)
* 5. [Additional Patterns](#AdditionalPatterns)
	* 5.1. [Null Object Pattern](#NullObjectPattern)
		* 5.1.1. [Brief Introduction](#BriefIntroduction-1)
		* 5.1.2. [Reason for Refactoring](#ReasonforRefactoring-1)
		* 5.1.3. [Refactoring Details](#RefactoringDetails-1)
		* 5.1.4. [UML Class Diagram](#UMLClassDiagram-1)
		* 5.1.5. [Benefits of Refactoring](#BenefitsofRefactoring-1)
* 6. [AI Tools Usage in Refactoring](#AIToolsUsageinRefactoring)
	* 6.1. [Phase 1: Initial Concept Exploration and Broad Literature Review](#Phase1:InitialConceptExplorationandBroadLiteratureReview)
	* 6.2. [Phase 2: Targeted Case Study and Implementation Research](#Phase2:TargetedCaseStudyandImplementationResearch)
	* 6.3. [Phase 3: Critical Analysis and Filtering of AI-Sourced Information](#Phase3:CriticalAnalysisandFilteringofAI-SourcedInformation)
	* 6.4. [Reflection](#Reflection)
* 7. [Reference](#Reference)

<!-- vscode-markdown-toc-config
	numbering=true
	autoSave=true
	/vscode-markdown-toc-config -->
<!-- /vscode-markdown-toc -->

<div style="page-break-after: always;"></div>

##  1. <a name='ProjectIntorduction'></a>Project Intorduction

###  1.1. <a name='ProjectBackground'></a>Project Background
This project implements a Stardew Valley–style farming life simulation game. The player acts as a new farmer who has just arrived in a small town. Starting from an undeveloped piece of land, the player cultivates fields, plants crops, raises animals, and builds relationships with local villagers. The game features four seasons, festival events, and daily quests. With limited stamina and time each day, the player must carefully plan their activities and balance between farm management, social interaction, and exploration. 

The game adopts a frontend–backend architecture. The frontend, built on Cocos2d-x, is responsible for rendering tile-based maps and user interfaces, while the backend handles time progression, save/load management, and the states of the main character and all map objects. All configuration and scene data are stored in JSON files and accessed via a dedicated DocumentManager, which keeps game logic and resource management clean and extensible.

<img src="images/Background.png" width="800">

###  1.2. <a name='ProjectFunction'></a>Project Function

The core features of this project include farm management, animal husbandry, community interaction, exploration and adventure, as well as character progression and a time system. With limited stamina and time each day, the player must plan their actions carefully and balance farming, social activities, gathering, and exploration to gradually build a prosperous farm and rich social network.

####  1.2.1. <a name='PlayerandFarmFeatures'></a>Player and Farm Features
**Farm Management**
The player can till the soil, plant seeds, water fields, and harvest a variety of crops on designated farm tiles.

Crop growth and maturity depend on both crop type and season, and some crops are only available in specific seasons.
![Farm1](images/Farm1.png)

**Animal Husbandry**
The player can raise multiple kinds of livestock such as cows and chickens. Animals have attributes like satiety, value, and reproduction rate.

By feeding different types of fodder, the player can speed up growth and improve product quality. During night-time settlement, animals may grow, reproduce, or die if not properly fed.
![Farm2](images/Farm2.png)

**Resource & Product Management**
The player can buy seeds, animal feed, and other resources from shops, and sell mature crops and animal products to earn money.

The inventory UI allows the player to view and manage all items, serving as the basis for crafting, trading, and gifting.

####  1.2.2. <a name='CommunityandNPCFeatures'></a>Community and NPC Features

**Community Interaction**
The player can talk to town residents and give them gifts to increase friendship, unlocking more dialogue and interactive events.

Each NPC has preferred gifts that provide additional affection bonuses when given.
![Farm3](images/Farm3.png)

**Quest System**
The quest panel displays available commissions from residents, such as collecting specific items or helping repair buildings.

Completing quests rewards the player with money, items, or reputation and drives both character growth and town development.

####  1.2.3. <a name='ExplorationandAdventureFeatures'></a>Exploration and Adventure Features

**Map Exploration**
The player can leave the farm and explore surrounding regions, including forests, mountains, mines, and the seaside.

Different regions provide distinct resources and interactions, enhancing openness and the sense of adventure.

**Mining & Gathering**
In mines, the player can excavate ores and rare materials used for crafting and trading.

In the wild, the player can gather wood, herbs, and other resources that are required for recipes or quests.
![Farm4](images/Farm4.png)

**Fishing System**

At specific water areas, the player can enter a fishing UI. Each cast yields fish or items with a certain degree of randomness.

Rare fish can be sold at high prices or used as special cooking ingredients.
![Farm5](images/Farm5.png)


####  1.2.4. <a name='CharacterProgressionandSystemFeatures'></a>Character Progression and System Features

**Time & Day-Night System**
The game features an in-game clock and calendar, mapping real time to in-game days with a fixed ratio.

At night or when the player chooses to sleep, the system settles daily events such as crop growth, animal updates, and quest refresh, and then automatically saves progress.

**UI & Scene Management**
The game provides various UI screens, including start menu, map scenes, inventory, shop, crafting, fishing, quest bar, NPC dialogue, and save/load interfaces.

A unified scene and UI management module handles map switching, layer stacking, and data persistence to ensure a smooth and cohesive gameplay experience.

<div style="page-break-after: always;"></div>

##  2. <a name='CreationalPatterns'></a>Creational Patterns

###  2.1. <a name='FactoryPattern'></a>Factory Pattern

####  2.1.1. <a name='BriefIntroduction'></a>Brief Introduction
The Factory Pattern is a creational design pattern that centralizes object creation behind an abstraction. Client code requests products via a factory instead of instantiating concrete classes directly. This separates "what" to create from "how" to create, improving decoupling, adherence to the Single Responsibility Principle (SRP), and enabling the Open/Closed Principle (OCP) for introducing new product types.

In this project, map entities such as `Animal`, `NPC`, `Land`, `Mineral`, and `Gate` are concrete products that derive from the abstract `MapObject`. The creation of these products is handled by a dedicated factory module (`MapObjectFactory`) instead of being inside `MapObject`.

![Factory Pattern](./images/FactoryPattern.png)

####  2.1.2. <a name='ReasonforRefactoring'></a>Reason for Refactoring
Originally, `MapObject` exposed a static `create(...)` and contained an if-else chain to instantiate different concrete classes based on a type string. This had several drawbacks:

- Tight coupling: `MapObject` became responsible for both behavior and creation, violating SRP.
- Poor extensibility: adding a new type required modifying `MapObject::create(...)`, breaking OCP and increasing regression risk.
- Scattered lifecycle policy: memory management (e.g., `autorelease`) and error handling were mixed with product selection logic.

the original code are as follows:
```cpp
if (type == "Animal")
{
ref = Animal::create(SubVal, parents, pos);
}
else if (type == "NPC")
{
ref = NPC::create(SubVal, parents, pos);
}
else if (type == "Land")
{
ref = Land::create(SubVal,parents, pos);
}
else if (type == "Mineral")
{
ref = Mineral::create(SubVal, parents, pos);
}
else if (type == "Gate")
{
ref = Gate::create(SubVal, parents, pos);
}
else
{
throw std::runtime_error("Archive has been corrupted");
}

```

Given the number of map object types under `Classes/Object` (`Animal`, `NPC`, `Land`, `Mineral`, `Gate`), centralizing creation in a factory module makes the codebase easier to extend and maintain.

####  2.1.3. <a name='RefactoringDetails'></a>Refactoring Details
We moved object creation out of `MapObject` and into `MapObjectFactory` (`Classes/Object/MapObjectFactory.h/.cpp`). The refactoring followed these principles:

- `MapObject` no longer owns creation logic; it remains the abstract product base (`Classes/Object/MapObject.h/.cpp`).
- Each concrete product (`Animal`, `NPC`, `Land`, `Mineral`, `Gate`) keeps a static `create()` as its construction entry point, encapsulating class-specific initialization.
- `MapObjectFactory` directs construction by dispatching the requested type to the corresponding concrete `create()` method, providing a single place to manage creation policy and error handling.
- The new code obeys SRP by separating creation from behavior, and OCP by allowing new types to be added without modifying existing code.

Files involved:
- `Classes/Object/MapObjectFactory.h` defines the factory interface and dispatch mechanism.
- `Classes/Object/MapObjectFactory.cpp` implements creation routing and centralizes lifecycle policy (e.g., unified error handling; optional `autorelease` coordination).
- `Classes/Object/Animal.h/.cpp`, `NPC.h/.cpp`, `Land.h/.cpp`, `Mineral.h/.cpp`, `Gate.h/.cpp` each provide `create()` and product-specific logic.
- `Classes/Object/MapObject.cpp` no longer contains the old static `create(...)` chain.

Creation flow after refactoring:
- Client code requests an object by type (e.g., "Animal").
- `MapObjectFactory::create()` maps the type to the corresponding concrete class and invokes its `create()`.
- The factory returns a `MapObject*` to the client, while keeping lifecycle and error handling consistent.

The new creation code is as follows:
```cpp
void MapObjectFactory::registerCreator(const std::string& type, Creator c) {
    registry()[type] = std::move(c);
}

MapObject* MapObjectFactory::create(const std::string& type, rapidjson::Value& val, MapLayer* parent, const Vec<int>& pos) {
    auto it = registry().find(type);
    if (it == registry().end()) {
        throw std::runtime_error("Unknown MapObject type: " + type);
    }
    return it->second(val, parent, pos);
}
```

####  2.1.4. <a name='UMLClassDiagram'></a>UML Class Diagram
We provide a brief structure image and a detailed UML reflecting the factory-centric creation model:

![Factory UML](./images/FactoryPatternUML.png)

Notes:
- The Product package includes `MapObject` (abstract) and five concrete classes (`Animal`, `NPC`, `Land`, `Mineral`, `Gate`).
- The Factory package includes `MapObjectFactory` as the creation controller, along with conceptual creator roles for the five concrete products to indicate factory-side construction responsibilities.

####  2.1.5. <a name='BenefitsofRefactoring'></a>Benefits of Refactoring
This refactoring delivers the following benefits:

- Separation of concerns: `MapObject` focuses on product behavior, while `MapObjectFactory` handles creation and policy.
- Extensibility: adding new map object types only requires extending factory dispatch (or introducing registration) without touching product base classes.
- Consistent lifecycle: memory management and error handling are centralized, reducing duplication and leaks.
- Testability: construction paths are isolated in one place, making it easier to mock or stub creation during tests.
- Maintainability: fewer cross-cutting changes and clearer module boundaries within `Classes/Object`.

<div style="page-break-after: always;"></div>

##  3. <a name='StructuralPatterns'></a>Structural Patterns

###  3.1. <a name='DecoratorPattern'></a>Decorator Pattern

####  3.1.1. <a name='BriefIntroduction-1'></a>Brief Introduction
The Decorator Pattern is a structural design pattern that allows behavior to be added to individual objects dynamically without changing their original class definitions. A decorator wraps a component object that implements the same abstract interface, and forwards most requests to this wrapped component. Additional responsibilities or states are then injected before or after the delegated calls. Client code interacts with the abstract interface and does not need to distinguish whether the object is a plain component or a component wrapped by multiple decorators.
![Decorator](images/Decorator.png)
In this project, the Decorator Pattern is applied to model various crop states in the game, such as immature, mature, drought, pest, and withered. Each of these states is implemented as a concrete decorator. At runtime, different decorators can be composed on top of the same crop instance (e.g., “immature + pest + drought”), which avoids putting a large number of flags and conditional branches into a single `Crop` class.

####  3.1.2. <a name='ReasonforRefactoring-1'></a>Reason for Refactoring
Before refactoring, a single Crop class typically represented all aspects of a crop, including:
* Life cycle (planted, growing, mature, withered);

* Daily watering status and pest infestation;

* Yield calculation and harvesting behavior;

* Binding and updating of the visual sprite on the tile-based map.

As more features were added, the Crop class became over-responsible. New states such as drought, pest, fertilizer effects, and special events were continuously added, leading to several problems:

1. God class problem: The Crop class violated the Single Responsibility Principle. Methods like dailySettle() and harvest() contained many intertwined conditional branches.

2. Poor extensibility for new states: Introducing a new state (e.g., disease or frost) required modifications to multiple data fields and methods inside Crop, increasing the risk of regression.

3. State explosion: When crops could simultaneously be “mature + drought + pest”, it became difficult to express such combinations clearly within a single class. Readability and maintainability were severely harmed.

4. Hard-to-test logic: Logic related to drought and pests was scattered across several methods, making it hard to test or reuse environmental effects in isolation.

Therefore, the refactoring goals were:

* Decouple “normal growth” behavior from “special state” behavior;

* Express complex state combinations via the composition of decorators;

* Reduce the invasiveness of new crop states on existing code, and improve extensibility and testability.
####  3.1.3. <a name='RefactoringDetails-1'></a>Refactoring Details
After refactoring, the original Crop class is decomposed into several roles:

##### Abstract Component – `BaseCrop`

`BaseCrop` defines the unified interface for crops: lifecycle methods such as init(...), dailySettle(), harvest(), and clear(), as well as state access methods like getLiveDay(), getMaturationDay(), isWatered(), and isWithered().
```cpp
class BaseCrop {
public:
    virtual ~BaseCrop() = default;

    // Life cycle
    virtual void init(cocos2d::Node* parent, const cocos2d::Vec2& tilePos) = 0;
    virtual void dailySettle() = 0;
    virtual bool harvest() = 0;
    virtual void clear() = 0;

    // State query
    virtual const std::string& getName() const = 0;
    virtual int  getLiveDay() const = 0;
    virtual int  getMaturationDay() const = 0;
    virtual bool isWatered() const = 0;
    virtual bool isWithered() const = 0;
    virtual void setWithered(bool withered) = 0;

    // Player / environment operations
    virtual void water() = 0;
    virtual void applyPesticide() = 0;
};
```
Client code (e.g., tiles and time manager) depends only on the BaseCrop abstraction, and does not care about the concrete implementation.

##### Concrete Component – `BasicCrop`

`BasicCrop` implements the `BaseCrop` interface and encapsulates the core “normal growth” logic: crop name, current live days, maturation days, daily watering flag, and withered flag.

In dailySettle(), the crop grows if it was watered; otherwise, it stays at the same day count. Reaching the maturation day only affects the appearance but does not directly encode drought, pest, or other special rules.

updateSpriteFrame() updates the sprite color or frame according to the growth stage, providing basic visual feedback.
```cpp
class BasicCrop : public BaseCrop {
public:
    BasicCrop(const std::string& name, int maturationDay)
    : _name(name),
      _liveDay(0),
      _maturationDay(maturationDay),
      _watered(false),
      _withered(false),
      _parent(nullptr),
      _tilePos(Vec2::ZERO),
      _sprite(nullptr) {}

    void init(Node* parent, const Vec2& tilePos) override {
        _parent  = parent;
        _tilePos = tilePos;

        _sprite = Sprite::create();
        if (_sprite) {
            _sprite->setPosition(tilePos);
            if (_parent) _parent->addChild(_sprite);
        }
        updateSpriteFrame();
    }

    void dailySettle() override {
        if (_withered) return;

        if (_watered) {
            ++_liveDay;
            _watered = false;
        }

        if (_liveDay == 0 && !_watered) {
            _withered = true;
        }

        updateSpriteFrame();
    }

    bool harvest() override {
        if (!_withered && _liveDay >= _maturationDay) {
            clear();
            return true;
        }
        return false;
    }

    void clear() override {
        if (_sprite && _sprite->getParent())
            _sprite->removeFromParent();
        _sprite = nullptr;
    }

    // State access & operations
    const std::string& getName() const override { return _name; }
    int  getLiveDay() const override { return _liveDay; }
    int  getMaturationDay() const override { return _maturationDay; }
    bool isWatered() const override { return _watered; }
    bool isWithered() const override { return _withered; }
    void setWithered(bool w) override { _withered = w; }

    void water() override { _watered = true; }
    void applyPesticide() override { /* no-op for basic crop */ }

protected:
    std::string _name;
    int  _liveDay;
    int  _maturationDay;
    bool _watered;
    bool _withered;

    Node*   _parent;
    Vec2    _tilePos;
    Sprite* _sprite;

    void updateSpriteFrame() {
        if (!_sprite) return;

        if (_withered) {
            _sprite->setColor(Color3B::GRAY);
            return;
        }

        float ratio = _maturationDay > 0
                      ? static_cast<float>(_liveDay) / _maturationDay
                      : 1.0f;
        if (ratio < 0.33f)       _sprite->setColor(Color3B::GREEN);
        else if (ratio < 0.66f)  _sprite->setColor(Color3B::YELLOW);
        else                     _sprite->setColor(Color3B::ORANGE);
    }
};
```

##### Abstract Decorator – `CropDecorator`

`CropDecorator` also implements `BaseCrop` and holds a `std::shared_ptr<BaseCrop>` named `_component`.

The default implementation simply forwards all calls to `_component`, such as dailySettle(), harvest(), water(), and applyPesticide().
```cpp
class CropDecorator : public BaseCrop {
public:
    explicit CropDecorator(std::shared_ptr<BaseCrop> component)
    : _component(std::move(component)) {}

    void init(Node* parent, const Vec2& tilePos) override {
        _component->init(parent, tilePos);
    }

    void dailySettle() override       { _component->dailySettle(); }
    bool harvest() override           { return _component->harvest(); }
    void clear() override             { _component->clear(); }

    const std::string& getName() const override   { return _component->getName(); }
    int  getLiveDay() const override              { return _component->getLiveDay(); }
    int  getMaturationDay() const override        { return _component->getMaturationDay(); }
    bool isWatered() const override               { return _component->isWatered(); }
    bool isWithered() const override              { return _component->isWithered(); }
    void setWithered(bool w) override             { _component->setWithered(w); }

    void water() override             { _component->water(); }
    void applyPesticide() override    { _component->applyPesticide(); }

protected:
    std::shared_ptr<BaseCrop> _component;
};
```
Concrete decorators override a subset of these methods and inject additional behavior before or after calling the base implementation.

##### Concrete Decorators – Five Crop State Classes

**ImmatureCropDecorator**:
Overrides harvest() to disallow harvesting when liveDay < maturationDay and the crop is not withered, preventing accidental harvesting of immature crops.
```cpp
class ImmatureCropDecorator : public CropDecorator {
public:
    explicit ImmatureCropDecorator(std::shared_ptr<BaseCrop> component)
    : CropDecorator(std::move(component)) {}

    bool harvest() override {
        if (!isWithered() && getLiveDay() < getMaturationDay()) {
            return false; // prevent harvesting immature crops
        }
        return CropDecorator::harvest();
    }

    void dailySettle() override {
        CropDecorator::dailySettle();
        // optional: add “immature” effects here
    }
};

```
**MatureCropDecorator**:
Represents crops that have reached the maturation day. It can add visual hints in dailySettle() and optionally grant extra rewards in harvest() (e.g., additional yield).
```cpp
class MatureCropDecorator : public CropDecorator {
public:
    explicit MatureCropDecorator(std::shared_ptr<BaseCrop> component)
    : CropDecorator(std::move(component)) {}

    bool harvest() override {
        bool success = CropDecorator::harvest();
        if (success) {
            // optional: add extra rewards for perfectly mature crops
        }
        return success;
    }

    void dailySettle() override {
        CropDecorator::dailySettle();
        // optional: highlight mature crops
    }
};
```
**DroughtCropDecorator**:
Maintains a _continuousDryDays counter and updates it in dailySettle().

If the crop is not watered for several consecutive days, it calls setWithered(true) to mark the crop as withered.

water() clears the drought counter and forwards the call to the wrapped component.
```cpp
class DroughtCropDecorator : public CropDecorator {
public:
    explicit DroughtCropDecorator(std::shared_ptr<BaseCrop> component)
    : CropDecorator(std::move(component)),
      _continuousDryDays(0) {}

    void dailySettle() override {
        if (!isWatered() && !isWithered()) {
            ++_continuousDryDays;
            if (_continuousDryDays >= 3) {
                setWithered(true);
            }
        } else {
            _continuousDryDays = 0;
        }
        CropDecorator::dailySettle();
    }

    void water() override {
        _continuousDryDays = 0;
        CropDecorator::water();
    }

private:
    int _continuousDryDays;
};
```

**PestCropDecorator**:
Simulates pest damage in dailySettle() using random numbers. When _underPest is true, each day there is a certain probability that the crop becomes withered.

applyPesticide() clears the pest state by setting _underPest to false.
```cpp
class PestCropDecorator : public CropDecorator {
public:
    explicit PestCropDecorator(std::shared_ptr<BaseCrop> component)
    : CropDecorator(std::move(component)),
      _underPest(true) {}

    void dailySettle() override {
        CropDecorator::dailySettle();
        if (!_underPest || isWithered()) return;

        static std::default_random_engine engine(std::random_device{}());
        std::uniform_real_distribution<float> dist(0.0f, 1.0f);

        if (dist(engine) < 0.1f) {
            setWithered(true);
        }
    }

    void applyPesticide() override {
        _underPest = false;
        CropDecorator::applyPesticide();
    }

private:
    bool _underPest;
};
```

**WitheredCropDecorator**:

Calls setWithered(true) in the constructor to mark the underlying crop as withered.

dailySettle() no longer delegates to the inner component, stopping any further growth.

harvest() allows the player to clear the crop from the map but produces no yield.
```cpp
class WitheredCropDecorator : public CropDecorator {
public:
    explicit WitheredCropDecorator(std::shared_ptr<BaseCrop> component)
    : CropDecorator(std::move(component)) {
        setWithered(true);
    }

    void dailySettle() override {
        // Do nothing: no more growth after withering
    }

    bool harvest() override {
        clear();       // remove from the map, no yield
        return true;
    }

    bool isWithered() const override {
        return true;
    }
};
```

##### Runtime Composition and Usage
At runtime, the game first creates a base crop:
```
std::shared_ptr<BaseCrop> crop =
    std::make_shared<BasicCrop>("Turnip", 4);
```
Then, depending on the current environment and state, it wraps the crop with different decorators:

* Newly planted crops are wrapped with ImmatureCropDecorator.
`crop = std::make_shared<ImmatureCropDecorator>(crop);`

* When the crop reaches the maturation day, it is wrapped with MatureCropDecorator.
`crop = std::make_shared<MatureCropDecorator>(crop);`
* If it is not watered during a day, a DroughtCropDecorator is added.
`crop = std::make_shared<DroughtCropDecorator>(crop);`
* Pest events attach a PestCropDecorator.
`crop = std::make_shared<PestCropDecorator>(crop);`
* Once the crop is determined to be withered, it is wrapped with WitheredCropDecorator to enforce withered semantics.
`crop = std::make_shared<WitheredCropDecorator>(crop);`

The client maintains only a `std::shared_ptr<BaseCrop>` and simply calls `crop->dailySettle()`, `crop->water()`, `and crop->harvest()` without needing to know how many decorators are stacked on top.

####  3.1.4. <a name='UMLClassDiagram-1'></a>UML Class Diagram
The following figure illustrates the UML class diagram of the crop refactoring using the Decorator Pattern.

* `BaseCrop` is the abstract component that defines the common interface for all crop objects.

* `BasicCrop` inherits from `BaseCrop` and is the only concrete component. It encapsulates the normal growth behavior and basic state management.

* `CropDecorator` also inherits from `BaseCrop`, but internally holds a BaseCrop reference via composition (- `component` : `BaseCrop`). It serves as the base class for all decorators.

* `ImmatureCropDecorator`, `MatureCropDecorator`, `DroughtCropDecorator`, `PestCropDecorator`, and `WitheredCropDecorator` all inherit from `CropDecorator`. Each of them overrides methods like `harvest()`, `dailySettle()`, `water()`, or `applyPesticide()` to inject state-specific logic.
![DecoratorUML](images/DecoratorUML.png)

####  3.1.5. <a name='BenefitsofRefactoring-1'></a>Benefits of Refactoring
By refactoring crops into the “base component + decorators” structure, the project gains several benefits:

##### Clear separation of concerns
Normal growth behavior is encapsulated inside BasicCrop, while environmental and state-specific behaviors (drought, pest, withered, etc.) are isolated into their own decorator classes. This eliminates the need for a single class filled with numerous flags and conditional branches.

##### Improved extensibility for new states
When new crop states are required (e.g., disease, frost, or fertilizer boost), developers only need to implement a new decorator that inherits from CropDecorator and override the necessary methods. The BasicCrop and existing decorators remain unchanged, which follows the Open–Closed Principle.

##### Support for flexible state composition
Multiple decorators can be stacked on the same crop instance:

* A mature crop can simultaneously suffer from pests and drought.

* A newly planted crop can be instantly withered due to a special event. 

These combinations are expressed by nested wrappers instead of a huge method that handles all possible state combinations.

##### Better readability and maintainability
Each decorator focuses on one concern, and its class name directly conveys its semantics (e.g., DroughtCropDecorator, PestCropDecorator). This makes the implementation easier to read, understand, and maintain, especially in a team setting.

##### Easier testing and reuse
Each decorator can be tested independently. For example, one can specifically verify that “a crop becomes withered after three continuous dry days” or “pest infestation causes withering with a certain probability per day”. The same decorators can also be reused for multiple crop types without duplicating logic.

<div style="page-break-after: always;"></div>

###  3.2. <a name='FacadePattern'></a>Facade Pattern

####  3.2.1. <a name='BriefIntroduction-1'></a>Brief Introduction

The Facade pattern is a structural design pattern that provides a simplified, unified interface to a larger and more complex body of code, such as a subsystem or a library. It defines a higher-level interface that makes the subsystem easier to use by hiding its complexity. The primary intent is to decouple the client from the intricate workings of the subsystem, promoting a cleaner architecture and improving maintainability.

![alt text](images/facade.png)

####  3.2.2. <a name='ReasonforRefactoring-1'></a>Reason for Refactoring

Before the refactoring, the project's architecture suffered from two main issues:

1.  **High Coupling and Low Cohesion**: Client classes like `AppDelegate`, `UILogic`, and `Gate` were directly coupled with multiple manager classes (`SceneManager`, `DocumentManager`, `TimeManager`). This forced clients to understand and manage complex interactions between different subsystems. For example, loading a game required the client to first call `DocumentManager` to load data and then `SceneManager` to switch the scene. This scattered logic made the client code hard to read, maintain, and extend.

2.  **Violation of the Single Responsibility Principle (SRP)**: The `SceneManager` class was overburdened with responsibilities. It not only managed scene transitions and lifecycle but also handled the creation, display, and hiding of all UI layers. This violated the SRP, making `SceneManager` a large and complex class that was difficult to manage.

The goal of the refactoring was to address these issues by introducing a facade to simplify the client interface and by separating concerns to improve class design.

####  3.2.3. <a name='RefactoringDetails-1'></a>Refactoring Details

The refactoring process was performed in two main stages: introducing the `GameFacade` and then separating UI management into a new `UIManager` class.

**1. Introducing the `GameFacade` Class**

A singleton class `GameFacade` was created to serve as a single entry point for all major game operations. It encapsulates the complex interactions between the various manager subsystems.

The interface of `GameFacade` exposes high-level methods that correspond to business logic, hiding the underlying implementation details.

*Code Snippet: `GameFacade.h` Interface*
```cpp
class GameFacade {
public:
    static GameFacade* getInstance();

    // --- High-level business process interfaces ---
    void startGame();
    void loadGame(int archiveNumber);
    void changeScene(const std::string& mapName, const std::string& position = "default");
    void showUI(const std::string& uiName, bool isBaseLayer = false);
    void hideUI(const std::string& uiName);
    void goToNextDay();

private:
    // References to core subsystems
    SceneManager* sceneManager_;
    TimeManager* timeManager_;
    DocumentManager* docManager_;
    UIManager* uiManager_;
};
```

Client code was then refactored to use this facade. For example, the application startup logic in `AppDelegate` was simplified significantly.

*Code Snippet: Client Code Refactoring in `AppDelegate.cpp`*
```cpp
// Before Refactoring (Conceptual)
// SceneManager::getInstance()->NextMap("introduction");

// After Refactoring
GameFacade::getInstance()->startGame();
```

**2. Creating `UIManager` to Separate Concerns**

To fix the SRP violation in `SceneManager`, a new `UIManager` class was created. All UI-related logic and responsibilities were moved from `SceneManager` to `UIManager`.

*Code Snippet: `UIManager.h` Interface*
```cpp
class UIManager {
public:
    static UIManager* getInstance();

    void initUIs();
    cocos2d::Node* getUINode();
    void showUILayer(const std::string& UI_name, bool base = false);
    void hideUILayer(const std::string& UI_name);
    // ...
};
```

The `GameFacade` was then updated to hold a reference to `UIManager` and delegate all UI-related calls to it, while `SceneManager` was cleansed of all UI management code, making its responsibilities purely focused on scene management.

####  3.2.4. <a name='UMLClassDiagram-1'></a>UML Class Diagram

The following diagrams illustrate the structural changes before and after applying the Facade pattern.

**Before Refactoring**

The diagram below shows the high coupling, where client classes directly interact with multiple manager classes.

![Facade Pattern Before Refactoring](./images/facade_before.png)

**After Refactoring**

This diagram shows the decoupled architecture. Clients only communicate with the `GameFacade`, which coordinates the subsystems. The responsibilities are also better separated with the introduction of `UIManager`.

![Facade Pattern After Refactoring](./images/facade_after.png)

####  3.2.5. <a name='BenefitsofRefactoring-1'></a>Benefits of Refactoring

The application of the Facade pattern yielded several significant benefits:

1.  **Simplified Client Interface**: Clients no longer need to manage complex interactions between subsystems. They can perform high-level operations like `loadGame()` with a single method call.
2.  **Reduced Coupling**: The client code is now decoupled from the internal implementation of the subsystems. Changes within the subsystems (e.g., modifying how scenes are loaded) have no impact on the client code as long as the facade's interface remains stable.
3.  **Improved Readability and Maintainability**: The code's intent is much clearer. A call to `GameFacade::getInstance()->startGame()` is more expressive and understandable than a series of calls to different managers. This makes the codebase easier to maintain and for new developers to understand.
4.  **Better Adherence to SRP**: By separating UI management into a dedicated `UIManager`, both `SceneManager` and `UIManager` now have clear, single responsibilities. This leads to a more organized and robust class structure.

<div style="page-break-after: always;"></div>

##  4. <a name='BehavioralPatterns'></a>Behavioral Patterns

###  4.1. <a name='ChainofResponsibilityPattern'></a>Chain of Responsibility Pattern

####  4.1.1. <a name='BriefIntroduction-1'></a>Brief Introduction

The Chain of Responsibility (CoR) Pattern is a behavioral design pattern that decouples a sender from multiple potential receivers by passing a request along a chain of handler objects. Each handler decides either (1) to process the request or (2) to forward it to the next handler in the chain. This promotes loose coupling, localizes stage-specific logic, and supports flexible assembly, insertion, removal, or reordering of processing steps without changing client code.

In our scene transition workflow the request is implicit: “advance the scene loading pipeline one step”. Prior to refactoring, this progression logic was encoded via hard-coded percentage thresholds inside `NextMapCallBack::operator()`. The CoR refactoring converts those percentage-gated conditional blocks into four discrete handler classes representing semantically meaningful phases: `StartHandler`, `CreateHandler`, `RenderHandler`, and `AssembleHandler`. Each invocation advances exactly one stage. The caller (the scheduler in `SceneManager::NextMap`) remains oblivious to internal phase boundaries.

Key Intent Applied:
1. Replace monolithic conditional dispatch with composable objects.
2. Eliminate magic numeric thresholds (1 / 10 / 80 / 100) as control flow drivers.
3. Provide an extensible pipeline for future scene transition steps (e.g., prefetch assets, fade-out audio, analytics logging) without altering existing handlers.

Design Motifs:
- Request Granularity: One scheduler tick equals one handler execution.
- Single Advancement: No handler loops or recursion; deterministic progression improves traceability.
- Handler Responsibility: Each handler encapsulates exactly one cohesive operation aligned with SRP.

![Chain of Responsibility](images/ChainOfResponsibility.png)

####  4.1.2. <a name='ReasonforRefactoring-1'></a>Reason for Refactoring

Prior implementation issues in `NextMapCallBack`:

1. Hard-Coded Threshold Logic: Four sequential `if/else` branches keyed to `loading_per` percentage values tightly coupled phase semantics to arbitrary numbers. Adjusting or inserting a new phase required renormalizing those thresholds and touching shared conditional code.
2. Mixed Concerns: Progress calculation, UI bar updates, map creation, player instantiation, scene replacement, and UI layer visibility resided in a single method—violating SRP and reducing readability.
3. Poor Extensibility: Adding an intermediate phase (e.g., asynchronous resource streaming) meant editing a brittle chain of comparisons, increasing regression risk.
4. Limited Testability: Individual phases could not be unit-tested in isolation; tests had to drive numeric percentage inputs to hit the intended branch.
5. Hidden Temporal Ordering: Ordering was implicit in numeric ranges rather than explicit in object structure, making intent obscure to new contributors.

Refactoring Goals:
1. Make sequence explicit via object chain ordering instead of numeric ranges.
2. Localize side-effects (scene creation, map manipulation, UI toggling) per handler for focused testing.
3. Enable insertion/removal/reordering with constant client surface.
4. Prepare for potential asynchronous or conditional skips (future: ability for a handler to decide “not ready—retry”).
5. Reduce cognitive load by isolating stage semantics.

The original code snippet from `SceneManager.cpp`:

```cpp
void SceneManager::NextMapCallBack::operator()()
{
    if (loading_per < 1.0f) {
        start();
    } else if (loading_per < 10.0f) {
        create();
    } else if (loading_per < 80.0f) {
        render();
    } else if (loading_per < 100.0f) {
        assemble();
    }
    loading_bar->setPercent(loading_per);
}
```

####  4.1.3. <a name='RefactoringDetails-1'></a>Refactoring Details

Scope of Change:
- Extracted callback and handlers from `SceneManager` into new compilation unit `HandleNextScene.h/.cpp`.
- Removed in-class `NextMapCallBack` definition and prior threshold-based logic from `SceneManager.cpp`.
- Introduced an abstract base `NextSceneHandler` declaring `handle(NextMapCallBack&)` and chain linkage via `setNext` / `next`.
- Implemented four concrete handlers:
    - `StartHandler`: Builds loading scene & progress UI.
    - `CreateHandler`: Creates or clears maps, conditionally loads introduction map.
    - `RenderHandler`: Moves target map to front; instantiates player sprite if position specified; updates current map name.
    - `AssembleHandler`: Constructs final scene, attaches UI node, shows relevant layers, performs transition.

Execution Model:
1. `SceneManager::NextMap` schedules the callback lambda four times (repeat = 3 → 4 executions).
2. On each invocation `NextMapCallBack::operator()` invokes current handler’s `handle`, then advances `current_ = current_->next()`.
3. When `current_` becomes `nullptr` the chain is exhausted—subsequent scheduler ticks (if any) are inert (can be optimized by unscheduling early in future).

Transition from Percent-Driven Flow:
- `loading_per` is retained only for UI progress bar continuity, but no longer determines control flow; each handler sets a meaningful milestone value (5, 45, 85, 100). These are presentation details isolated per handler.

Potential Enhancements:
1. Memory Management: Replace raw handler pointers with `std::unique_ptr` chain for automatic lifetime.
2. Cancellation: Provide early exit (e.g., user abort) by clearing `current_` or adding a guard.
3. Async Integration: Allow handlers to post async tasks (asset streaming) and defer advancement until completion callback triggers.
4. Logging / Telemetry: Add a decorator or observer to emit timing metrics per phase.
5. Error Isolation: Wrap each `handle` body with localized exception handling; escalate aggregated failure context to a recovery handler.

The refactored `NextMapCallBack` now looks like this:

```cpp
NextMapCallBack::NextMapCallBack(std::string map_name_, std::string pos_)
    : map_name(std::move(map_name_))
    , pos(std::move(pos_))
{
    head_ = new StartHandler();
    NextSceneHandler* createH = new CreateHandler();
    NextSceneHandler* renderH = new RenderHandler();
    NextSceneHandler* assembleH = new AssembleHandler();

    head_->setNext(createH);
    createH->setNext(renderH);
    renderH->setNext(assembleH);

    current_ = head_;
}

void NextMapCallBack::operator()()
{
    if (current_) {
        current_->handle(*this);
        current_ = current_->next();
    }
    if (loading_bar) {
        loading_bar->setPercent(loading_per);
    }
}
```

####  4.1.4. <a name='UMLClassDiagram-1'></a>UML Class Diagram

![Chain of Responsibility UML](images/ChainOfResponsibilityUML.png)

Sequence Diagram Notes (for inclusion):
- Emphasize deterministic single-step advancement per scheduler tick.
- Show progress bar update after each handler.
- Highlight map and UI adjustments localized to their respective handlers.

####  4.1.5. <a name='BenefitsofRefactoring-1'></a>Benefits of Refactoring

Primary Benefits:
1. Explicit Phase Modeling: Handler classes make scene transition stages first-class citizens, improving discoverability and documentation potential.
2. Extensibility: Adding a new phase becomes a localized operation (create new handler + link) without modifying existing logic—aligns with Open/Closed Principle.
3. Readability & Maintainability: Eliminates cryptic percentage branching; responsibilities are self-descriptive by class name.
4. Testability: Each handler can be unit-tested by constructing a tailored `NextMapCallBack` context and invoking `handle()` directly.
5. Reduced Risk Surface: Changes to one phase no longer risk unintended side effects in unrelated phases.
6. Separation of Concerns: Progress UI updates, map lifecycle management, player spawning, and scene assembly are segregated.
7. Future Async Readiness: Chain architecture provides a natural anchor point for introducing asynchronous handlers with continuation semantics.

Secondary Benefits:
1. Metrics / Profiling Hooks: Handlers offer fine-grained instrumentation points for load-time analytics.
2. Conditional Phase Skipping: A handler could internally decide to bypass itself or insert fallback phases (e.g., degraded assets) without caller changes.
3. Easier Rollback Strategy: Potential to implement a reverse chain for error recovery (unwind partially created resources).
4. UI Variation: The `StartHandler` could be swapped for themed variants (festival loading screens) without touching other stages.

Residual Technical Debt & Next Steps:
1. Macro-Based Privacy Breakage: Replace `#define private public` with structured access (friend or accessor methods).
2. Raw Pointer Lifecycle: Migrate handler chain to RAII-managed smart pointers.
3. Unscheduling Optimization: After `current_ == nullptr`, cancel scheduler to reclaim cycles.
4. Error Channel: Introduce a lightweight result type or status enum to propagate recoverable failures.
5. Configuration-Driven Assembly: Externalize handler ordering into a configuration file for data-driven customization (e.g., mod support, DLC phases).

Strategic Outlook:
The CoR foundation transforms scene transitions from rigid numeric gating into a scalable pipeline. This unlocks architectural flexibility for advanced features—asset prefetching, adaptive loading UI, network synchronization, or analytics instrumentation—without destabilizing existing logic.

<div style="page-break-after: always;"></div>

###  4.2. <a name='StrategyPattern'></a>Strategy Pattern

####  4.2.1. <a name='BriefIntroduction-1'></a>Brief Introduction

####  4.2.2. <a name='ReasonforRefactoring-1'></a>Reason for Refactoring

####  4.2.3. <a name='RefactoringDetails-1'></a>Refactoring Details

####  4.2.4. <a name='UMLClassDiagram-1'></a>UML Class Diagram

####  4.2.5. <a name='BenefitsofRefactoring-1'></a>Benefits of Refactoring

<div style="page-break-after: always;"></div>

##  5. <a name='AdditionalPatterns'></a>Additional Patterns

###  5.1. <a name='NullObjectPattern'></a>Null Object Pattern

####  5.1.1. <a name='BriefIntroduction-1'></a>Brief Introduction
The Null Object Pattern is a behavioral design pattern that defines an object representing “absence” or “do-nothing” behavior, instead of using a null pointer/reference. The null object implements the same interface as real objects and provides safe, meaningful default behavior. Client code can uniformly invoke methods on the interface without checking for null, eliminating defensive null-checks scattered throughout the codebase.


In this project, the Null Object Pattern is applied to the player’s **currently held item** system. The original design used a raw pointer `Item* currentItem` that frequently became `nullptr` when the player held nothing. This forced more than 15 null-checks across interaction, UI, and inventory code (e.g., `if (currentItem != nullptr && currentItem->type == PICKAXE)`). By introducing a `NullItem` singleton that implements the full `Item` interface with safe no-op behavior, all null-checks are completely eliminated.

####  5.1.2. <a name='ReasonforRefactoring-1'></a>Reason for Refactoring
Before refactoring, the item system suffered from classic “null pointer hell”:

1. **Frequent null checks** – Almost every interaction (mining, tilling, watering, eating, fishing) required `currentItem != nullptr` checks.
2. **Inconsistent return values** – `getCurrentItemType()` had to return `ItemType::NONE` via a ternary operator when `currentItem` was null.
3. **Error-prone pointer management** – When an item’s quantity reached zero, developers had to manually set `currentItem = nullptr`, risking dangling pointers.
4. **Poor readability and extensibility** – Adding a new tool (e.g., axe, sickle) required modifying every null-checking site.
5. **Violation of “Tell, Don’t Ask”** – Code constantly asked “do you have an item?” instead of simply telling the item to perform an action.

These problems became especially severe because **“holding nothing” is the most common player state** (approximately 70% of gameplay time), yet it was represented by the absence of an object rather than a proper object with defined behavior.

####  5.1.3. <a name='RefactoringDetails-1'></a>Refactoring Details
The original value-type `struct Item` was transformed into a polymorphic hierarchy using the Null Object Pattern:

##### Abstract Component – `Item`
```cpp
class Item {
public:
    virtual ~Item() = default;
    virtual ItemType    getType() const = 0;
    virtual std::string getIconPath() const = 0;
    virtual int         getQuantity() const { return 0; }
    virtual void        setQuantity(int) {}

    // Behavior queries (default: false/no-op)
    virtual bool isTool() const       { return false; }
    virtual bool canMine() const      { return false; }
    virtual bool canHoe() const       { return false; }
    virtual bool canWater() const     { return false; }
    virtual bool canFish() const      { return false; }
    virtual bool isConsumable() const { return false; }

    // Safe actions
    virtual void use() const {}
    virtual void consume() const {}
};
```

##### Concrete Component – `RealItem`
Implements actual item logic (icon loading, quantity, tool/consumable checks, consumption that deducts quantity and restores energy).
```cpp
class RealItem final : public Item {
    // ... implements all meaningful behavior
    void consume() const override {
        if (MainCharacter::getInstance()->modifyItemQuantity(type_, -1)) {
            MainCharacter::getInstance()->modifyEnergy(getEnergyValue(type_));
        }
    }
};
```

##### Null Object – `NullItem` (Singleton)
```cpp
class NullItem final : public Item {
private:
    NullItem() = default;
public:
    static NullItem* getInstance() {
        static NullItem instance;
        return &instance;
    }

    ItemType    getType() const override     { return ItemType::NONE; }
    std::string getIconPath() const override { return "ui/empty_hand.png"; }

    void use() const override {
        CCLOG("Nothing in hand");   // Optional feedback
    }
};
```

##### MainCharacter Changes
```cpp
class MainCharacter {
private:
    std::vector<std::unique_ptr<RealItem>> inventory;   // Owns real items
    Item* currentItem = NullItem::getInstance();        // Never nullptr
};
```
- Constructor initializes `currentItem` to `NullItem::getInstance()`
- `setCurrentItem()` switches to a `RealItem*` or falls back to `NullItem`
- All inventory operations use `std::unique_ptr<RealItem>` for automatic memory management
- `getCurrentItem()` and `getCurrentItemType()` now have no null checks

####  5.1.4. <a name='UMLClassDiagram-1'></a>UML Class Diagram
The refactored structure forms a classic Null Object hierarchy:
- `Item` serves as the common abstract interface that defines all item-related operations.

- `RealItem` provides the full, meaningful implementation for actual tools and consumables.

- `NullItem` (implemented as a Meyers’ singleton) acts as the null object, returning safe default values (`ItemType::NONE`, empty icon path) and performing harmless no-op actions.

All client code — including `MainCharacter` character logic, tile interaction systems (mining, tilling, watering, fishing), inventory management, and UI rendering — now depends **solely on the `Item*` (or `const Item*`) abstraction. Crucially, **none of these clients ever perform a null-pointer check again**. Whether the player is holding a real pickaxe or nothing at all, the pointer is always valid and the polymorphic call is safe:


This transformation completely eliminates the null-checks that previously littered the codebase, achieving the ultimate goal of the Null Object Pattern: **the "absence" of an item is represented by a real object that knows how to behave correctly when absent**.

![NullObjectUML](images/NullObjectUML.png)

####  5.1.5. <a name='BenefitsofRefactoring-1'></a>Benefits of Refactoring
The application of the Null Object Pattern yielded significant improvements:

1. **Complete elimination of null checks**  
   Over 15 instances of `currentItem != nullptr` and ternary operators were removed.

2. **Safer and more robust code**  
   Impossible to accidentally dereference a null `currentItem`. The program is now immune to crashes caused by forgotten null checks.

3. **Cleaner, intention-revealing API**  
   Code changed from defensive “Ask” style:
   ```cpp
   if (currentItem && currentItem->type == PICKAXE) mine();
   ```
   to declarative “Tell” style:
   ```cpp
   if (currentItem->canMine()) mine();
   ```

4. **Greatly improved extensibility**  
   Adding a new tool (e.g., axe) only requires implementing `canChop()` in one `RealItem` case — no changes needed in any interaction site.

5. **Natural representation of the most common state**  
   “Empty hand” is now a first-class object with defined behavior (display empty-hand icon, safe no-op actions), perfectly matching real gameplay.

6. **Modern C++ memory safety**  
   Combined with `std::unique_ptr<RealItem>` in inventory, all item lifetime management is now fully RAII-compliant with zero manual deletes.

<div style="page-break-after: always;"></div>

##  6. <a name='AIToolsUsageinRefactoring'></a>AI Tools Usage in Refactoring

In our refactoring process, AI tools were employed strictly in an auxiliary capacity, functioning as an advanced research assistant rather than a direct contributor to the codebase. This strategic choice allowed us to leverage AI's powerful information retrieval capabilities while ensuring that all design decisions and code implementations remained human-led. Our usage can be detailed in three distinct phases:

###  6.1. <a name='Phase1:InitialConceptExplorationandBroadLiteratureReview'></a>Phase 1: Initial Concept Exploration and Broad Literature Review

At the outset, our goal was to build a solid theoretical foundation for the chosen design patterns. We used AI to accelerate this learning process.

Instead of simple queries like "What is Facade Pattern?", we engaged the AI in a dialogue to explore the pattern's nuances. For instance, we prompted: *"Search for the explain of the Facade Pattern's role in game engine architecture and compare its typical use cases against the Mediator Pattern."* This provided a comparative analysis that was more insightful than a standard web search.

![](./images/SearchResultsA1.png)
![](./images/SearchResultsA2.png)

###  6.2. <a name='Phase2:TargetedCaseStudyandImplementationResearch'></a>Phase 2: Targeted Case Study and Implementation Research

Once we had a firm grasp of the theory, we needed practical, context-specific examples.

We narrowed our queries to find information relevant to our specific technology stack. For example: *"Find tutorials or GitHub repositories demonstrating Facade Pattern usage specifically within a Cocos2d-x project for managing game states and subsystems."*

![](./images/SearchResultsB1.png)
![](./images/SearchResultsB2.png)

###  6.3. <a name='Phase3:CriticalAnalysisandFilteringofAI-SourcedInformation'></a>Phase 3: Critical Analysis and Filtering of AI-Sourced Information

Crucially, our process involved a manual verification and filtering stage, reinforcing the human-in-the-loop principle.

*   **Information Validation**: We discovered that AI, while powerful, could sometimes provide outdated or overly complex information. For example, one AI-suggested implementation for a pattern used advanced C++17 features that were incompatible with our project's C++11 standard. This required us to critically evaluate the AI's output and manually adapt the concepts to our constraints.
*   **Synthesizing Knowledge**: The AI provided a wealth of raw information, but the task of synthesizing this information into a coherent design plan was entirely our own. We reviewed the collected articles, code examples, and discussions, debated the trade-offs, and then designed our `GameFacade` from scratch, informed by—but not dictated by—the AI-sourced materials.

###  6.4. <a name='Reflection'></a>Reflection

This multi-phase approach proved highly effective. By defining the AI's role as a research assistant, we streamlined the often time-consuming process of literature review and technical research. The AI handled the "legwork" of finding relevant data, which freed up our cognitive resources to focus on the higher-level tasks of analysis, design, and implementation. This demonstrates a balanced and responsible use of AI in software engineering, where the tool enhances human intellect rather than replacing it.

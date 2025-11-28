![alt text](./images/cover.jpeg)

# Software Design Patterns -- Refactoring Report

<!-- TOC -->
- [Software Design Patterns -- Refactoring Report](#software-design-patterns----refactoring-report)
  - [Project Intorduction](#project-intorduction)
    - [Project Background](#project-background)
    - [Project Function](#project-function)
  - [Creational Patterns](#creational-patterns)
    - [Factory Pattern](#factory-pattern)
      - [Brief Introduction](#brief-introduction)
      - [Reason for Refactoring](#reason-for-refactoring)
      - [Refactoring Details](#refactoring-details)
      - [UML Class Diagram](#uml-class-diagram)
      - [Benefits of Refactoring](#benefits-of-refactoring)
  - [Structural Patterns](#structural-patterns)
      - [Brief Introduction](#brief-introduction-1)
      - [Reason for Refactoring](#reason-for-refactoring-1)
      - [Refactoring Details](#refactoring-details-1)
      - [UML Class Diagram](#uml-class-diagram-1)
      - [Benefits of Refactoring](#benefits-of-refactoring-1)
    - [Decorator Pattern](#decorator-pattern)
      - [Brief Introduction](#brief-introduction-2)
      - [Reason for Refactoring](#reason-for-refactoring-2)
      - [Refactoring Details](#refactoring-details-2)
      - [UML Class Diagram](#uml-class-diagram-2)
      - [Benefits of Refactoring](#benefits-of-refactoring-2)
    - [Facade Pattern](#facade-pattern)
      - [Brief Introduction](#brief-introduction-3)
      - [Reason for Refactoring](#reason-for-refactoring-3)
      - [Refactoring Details](#refactoring-details-3)
      - [UML Class Diagram](#uml-class-diagram-3)
      - [Benefits of Refactoring](#benefits-of-refactoring-3)
  - [Behavioral Patterns](#behavioral-patterns)
    - [Chain of Responsibility Pattern](#chain-of-responsibility-pattern)
      - [Brief Introduction](#brief-introduction-4)
      - [Reason for Refactoring](#reason-for-refactoring-4)
      - [Refactoring Details](#refactoring-details-4)
      - [UML Class Diagram](#uml-class-diagram-4)
      - [Benefits of Refactoring](#benefits-of-refactoring-4)
    - [Strategy Pattern](#strategy-pattern)
      - [Brief Introduction](#brief-introduction-5)
      - [Reason for Refactoring](#reason-for-refactoring-5)
      - [Refactoring Details](#refactoring-details-5)
      - [UML Class Diagram](#uml-class-diagram-5)
      - [Benefits of Refactoring](#benefits-of-refactoring-5)
  - [Additional Patterns](#additional-patterns)
    - [Null Object Pattern](#null-object-pattern)
      - [Brief Introduction](#brief-introduction-6)
      - [Reason for Refactoring](#reason-for-refactoring-6)
      - [Refactoring Details](#refactoring-details-6)
      - [UML Class Diagram](#uml-class-diagram-6)
      - [Benefits of Refactoring](#benefits-of-refactoring-6)
  - [Reference](#reference)
<!-- TOC -->

## Project Intorduction

### Project Background

### Project Function

## Creational Patterns

### Factory Pattern

#### Brief Introduction

#### Reason for Refactoring

#### Refactoring Details

#### UML Class Diagram

#### Benefits of Refactoring

## Structural Patterns

#### Brief Introduction

#### Reason for Refactoring

#### Refactoring Details

#### UML Class Diagram

#### Benefits of Refactoring

### Decorator Pattern

#### Brief Introduction

#### Reason for Refactoring

#### Refactoring Details

#### UML Class Diagram

#### Benefits of Refactoring

### Facade Pattern

#### Brief Introduction

The Facade pattern is a structural design pattern that provides a simplified, unified interface to a larger and more complex body of code, such as a subsystem or a library. It defines a higher-level interface that makes the subsystem easier to use by hiding its complexity. The primary intent is to decouple the client from the intricate workings of the subsystem, promoting a cleaner architecture and improving maintainability.

#### Reason for Refactoring

Before the refactoring, the project's architecture suffered from two main issues:

1.  **High Coupling and Low Cohesion**: Client classes like `AppDelegate`, `UILogic`, and `Gate` were directly coupled with multiple manager classes (`SceneManager`, `DocumentManager`, `TimeManager`). This forced clients to understand and manage complex interactions between different subsystems. For example, loading a game required the client to first call `DocumentManager` to load data and then `SceneManager` to switch the scene. This scattered logic made the client code hard to read, maintain, and extend.

2.  **Violation of the Single Responsibility Principle (SRP)**: The `SceneManager` class was overburdened with responsibilities. It not only managed scene transitions and lifecycle but also handled the creation, display, and hiding of all UI layers. This violated the SRP, making `SceneManager` a large and complex class that was difficult to manage.

The goal of the refactoring was to address these issues by introducing a facade to simplify the client interface and by separating concerns to improve class design.

#### Refactoring Details

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

#### UML Class Diagram

The following diagrams illustrate the structural changes before and after applying the Facade pattern.

**Before Refactoring**

The diagram below shows the high coupling, where client classes directly interact with multiple manager classes.

![Facade Pattern Before Refactoring](./images/facade_before.png)

**After Refactoring**

This diagram shows the decoupled architecture. Clients only communicate with the `GameFacade`, which coordinates the subsystems. The responsibilities are also better separated with the introduction of `UIManager`.

![Facade Pattern After Refactoring](./images/facade_after.png)

#### Benefits of Refactoring

The application of the Facade pattern yielded several significant benefits:

1.  **Simplified Client Interface**: Clients no longer need to manage complex interactions between subsystems. They can perform high-level operations like `loadGame()` with a single method call.
2.  **Reduced Coupling**: The client code is now decoupled from the internal implementation of the subsystems. Changes within the subsystems (e.g., modifying how scenes are loaded) have no impact on the client code as long as the facade's interface remains stable.
3.  **Improved Readability and Maintainability**: The code's intent is much clearer. A call to `GameFacade::getInstance()->startGame()` is more expressive and understandable than a series of calls to different managers. This makes the codebase easier to maintain and for new developers to understand.
4.  **Better Adherence to SRP**: By separating UI management into a dedicated `UIManager`, both `SceneManager` and `UIManager` now have clear, single responsibilities. This leads to a more organized and robust class structure.

## Behavioral Patterns

### Chain of Responsibility Pattern

#### Brief Introduction

#### Reason for Refactoring

#### Refactoring Details

#### UML Class Diagram

#### Benefits of Refactoring

### Strategy Pattern

#### Brief Introduction

#### Reason for Refactoring

#### Refactoring Details

#### UML Class Diagram

#### Benefits of Refactoring

## Additional Patterns

### Null Object Pattern

#### Brief Introduction

#### Reason for Refactoring

#### Refactoring Details

#### UML Class Diagram

#### Benefits of Refactoring

## Reference

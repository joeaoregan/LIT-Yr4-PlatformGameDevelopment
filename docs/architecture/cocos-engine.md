# Cocos2d-x Engine & Scene Graph

![Cocos2d-x Scene Graph & Composite Node Hierarchy](../images/svg/scene_graph.svg)

Space Quest is built upon the C++ **Cocos2d-x** engine, adopting its node hierarchy and action pipeline.

---

## 1. Director & Scene Lifecycle

The game lifecycle is driven by `cocos2d::Director::getInstance()`, which manages the main loop (updating game logic, running actions, and rasterising nodes to OpenGL at 60 FPS):

* **`runWithScene(cocos2d::Scene* scene)`**: Called in `AppDelegate::applicationDidFinishLaunching()` to load the initial scene (`SignInScene` on Android or `SplashScene` on desktop).
* **`replaceScene(cocos2d::TransitionScene* transition)`**: Replaces the active scene using animated scene transition controllers (such as `TransitionFade`, `TransitionSlideInB`, `TransitionZoomFlipX`, and `TransitionRotoZoom`), ensuring old scene layers and textures are disposed cleanly.

---

## 2. Node Hierarchy & Layering

Every active state in the game is represented as a `cocos2d::Scene` composed of specialized `cocos2d::Node` and `cocos2d::Layer` instances:

1. **Parallax Background Layer (`ParallaxNodeExtras`)**: Renders multi-tiered starfields scrolling at varying velocity multipliers to create optical depth, supplemented by `ParticleSystemQuad` star emitters.
2. **Batched Entity Layer (`SpriteBatchNode`)**: Houses active entities (`Player`, `EnemyShip`, `Laser`, `Asteroid`) blitted from shared texture atlases.
3. **Control Layer (`DPad`)**: Contains the on-screen capacitive virtual D-Pad (`EasyArrowsx-3.0`) and touch listeners for mobile play.
4. **HUD & UI Layer (`HUD`, `MusicPlayer`)**: Displays real-time status bars, timers, score counters, and audio jukebox controls.

```mermaid
graph TD
    Director["cocos2d::Director"] --> Scene["cocos2d::Scene"]
    Scene --> LevelLayer["Level (cocos2d::Layer)"]

    LevelLayer --> Parallax["ParallaxNodeExtras &amp; Star Particles"]
    LevelLayer --> Batch1["SpriteBatchNode ('Sprites.pvr.ccz')"]
    LevelLayer --> Batch2["SpriteBatchNode ('JoeSprites.png')"]
    LevelLayer --> Entities["Dynamic Entities (Player, Enemies, Power-ups)"]
    LevelLayer --> InputLayer["Input Layer (DPad EasyArrows &amp; MultiTouch)"]
    LevelLayer --> UILayer["HUD &amp; MusicPlayer Layer"]

    classDef default fill:#1e293b,stroke:#475569,stroke-width:1px,color:#f8fafc;
    classDef highlight fill:#1e3a8a,stroke:#3b82f6,stroke-width:2px,color:#93c5fd;
    class Director,Scene,LevelLayer highlight;
```

!!! tip "Performance Optimisation: Sprite Batching"  
    Using a shared texture atlas with `SpriteBatchNode::create("Sprites.pvr.ccz")` and `SpriteBatchNode::create("JoeSprites.png")` allows dozens of simultaneous laser beams, particles, and enemy sprites to be rasterised in a single OpenGL draw call.

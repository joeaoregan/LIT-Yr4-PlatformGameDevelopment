# System Architecture

![Space Quest Architecture](../images/svg/architecture.svg)

The **Space Quest** architecture leverages the Cocos2d-x 2D scene graph model, combining modular scene management with cross-platform input handlers, batched GPU sprite rendering, and cloud services via SDKBox.

!!! tip "Architectural Highlights"  
    * **Director Pattern**: `cocos2d::Director` manages scene lifecycles (`pushScene`, `replaceScene`) and drives the 60 FPS main loop.
    * **Input Abstraction**: Desktop keyboard events (`EventListenerKeyboard`) and mobile multi-touch events (`EventListenerTouchAllAtOnce`) are normalized into identical kinematic impulse vectors.
    * **Batch Node Rendering**: All ship and bullet sprites are rendered through a single `SpriteBatchNode`, minimizing GPU draw calls.
    * **Cloud Telemetry**: Integrated with SDKBox to broadcast gameplay analytics and persist cloud leaderboards.

---

## Subsystem Documentation

* **[Cocos2d-x Engine](cocos-engine.md)**: Director, scene graph hierarchy, and batch rendering.
* **[Input & Controls](input-systems.md)**: Desktop keyboard mappings and mobile virtual D-Pad.
* **[SDKBox & Cloud Services](sdkbox-services.md)**: Google Analytics and Google Play Services.

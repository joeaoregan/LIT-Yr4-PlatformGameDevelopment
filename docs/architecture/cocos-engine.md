# Cocos2d-x Engine & Scene Graph

Space Quest is built upon the C++ **Cocos2d-x** engine, adopting its node hierarchy and action pipeline.

---

## Scene Graph Hierarchy

Every active state in the game is represented as a `cocos2d::Scene` composed of specialized `Layer` instances:

1. **Parallax Background Layer**: Renders multi-tiered starfields scrolling at varying speeds to create optical depth.
2. **Game World Layer**: Houses active entities (`PlayerShip`, `Enemy`, `Laser`, `Asteroid`) managed via `SpriteBatchNode`.
3. **Control Layer**: Contains the on-screen virtual D-Pad and mobile touch buttons.
4. **HUD & UI Layer**: Displays real-time status bars, timers, score counters, and music controls.

!!! note "Performance Optimisation"  
    Using a shared texture atlas with `SpriteBatchNode` allows dozens of simultaneous laser beams and enemy sprites to be rasterised in a single OpenGL draw call.

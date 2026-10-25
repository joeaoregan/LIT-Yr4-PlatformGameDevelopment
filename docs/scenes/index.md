# Scenes & Level Progression

![Space Quest Game Lifecycle & Scene Transition Flow](../images/svg/scene_flow.svg)

The gameplay flow in Space Quest progresses across distinct narrative and interactive stages coordinated by the Cocos2d-x `cocos2d::Director`.

---

## 1. Scene Transition Flow

Scene transitions utilise animated Cocos2d-x transition controllers (`TransitionFade`, `TransitionSlideInB`, `TransitionZoomFlipX`, and `TransitionRotoZoom`) to move between stages:

```
[Android Startup] ──> SignInScene ──(Authenticated)──┐
                                                     ▼
[Desktop Startup] ───────────────────────────> SplashScene
                                                     │
                                             TransitionFade
                                                     ▼
┌──────────────────┐  TransitionSlideInB    ┌──────────────────┐
│ Utility Screens  │ <════════════════════> │  MainMenu Scene  │
│ (Settings/Scores)│                        └─────────┬────────┘
└──────────────────┘                                  │
                                             TransitionSlideInB
                                                      ▼
                                                 StoryScene (Briefing)
                                                      │
                                                      ├────────────────────────┐
                                                      ▼                        ▼
                                              Level 1: Asteroids      Level 2: DerpStar
                                                      │                        │
                                            (Target Destroyed)           (Boss Cleared)
                                                      ▼                        ▼
                                                 StoryScene              CreditsScene
                                                      │
                                              (Lives == 0)
                                                      ▼
                                                GameOverScene
```

---

## 2. Scene Breakdown

### Splash & Authentication
* **`SignInScene`**: Invoked on Android platforms to initialise Google Play Games services and authenticate player credentials via SDKBox.
* **`SplashScene`**: Displays studio branding, precaches sprite atlases (`Sprites.pvr.ccz`, `JoeSprites.png`), and initialises audio subsystems.

### Main Menu & Utility Hub
* **`MainMenu`**: Central navigation hub providing animated buttons for Start Game, High Scores, Settings, and Instructions.
* **`Settings`**: Configures sound effect and music volume channels.
* **`HighScores` & `EnterName`**: Manages local and cloud leaderboard standings.

### Gameplay Stages
* **[Level 1](level-1.md)**: Asteroid Belt navigation, scouting drones, and weapon upgrade pick-ups.
* **[Level 2 & Boss](level-2.md)**: Deep Space combat encountering alien battle cruisers and the multi-phased DerpStar capital ship.

### Narrative & Outcome Screens
* **`StoryScene`**: Delivers mission briefings and narrative dialogue between stages.
* **`GameOverScene`**: Activated via `TransitionRotoZoom` when player lives reach zero, providing instant replay or menu return options.
* **`CreditsScene`**: Activated upon defeating the DerpStar, running credits and triggering leaderboard score submissions.

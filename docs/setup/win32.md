# Win32 Build Setup (Visual Studio)

This guide outlines how to build and debug Space Quest on Windows using Visual Studio.

---

## Prerequisites

* Windows 10 / 11
* Visual Studio 2017 or 2019 (with Desktop Development with C++ workload)
* CMake 3.10+

## Build Steps

1. Open the Visual Studio solution file located at:
   ```text
   proj.win32/SpaceGame.sln
   ```
2. Set the build configuration to **Debug** or **Release** and platform to **Win32**.
3. Right-click the `SpaceGame` project in Solution Explorer and select **Set as Startup Project**.
4. Press `F5` to compile and launch the game.

!!! tip "Asset Directory"  
    Ensure that the `Resources/` folder is copied or symlinked to the executable output folder so textures and audio files load successfully at runtime.

# Android Studio Build Setup

This guide details building Space Quest for Android devices using Android Studio and the Android NDK.

---

## Prerequisites

* Android Studio (Chipmunk or newer)
* Android SDK (API 26+)
* Android NDK (r19c or compatible version)
* CMake

## Build Steps

1. Launch Android Studio and open the project directory:
   ```text
   proj.android-studio
   ```
2. Wait for Gradle sync to complete. Ensure `local.properties` specifies your local SDK and NDK paths:
   ```properties
   sdk.dir=C:\\Users\\<Username>\\AppData\\Local\\Android\\Sdk
   ndk.dir=C:\\Users\\<Username>\\AppData\\Local\\Android\\Sdk\\ndk\\<ndk-version>
   ```
3. Connect an Android device with USB Debugging enabled, or launch an x86/ARM emulator.
4. Select **Run** -> **Run 'app'** (`Shift + F10`) to build the APK and deploy to the device.

!!! important "SDKBox Signing"  
    For Google Play Leaderboards and Achievements to authenticate in production builds, sign the APK with the registered release keystore matching your Google Play Console credentials.

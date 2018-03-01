LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos/audio/include)

LOCAL_MODULE := MyGame_shared

LOCAL_MODULE_FILENAME := libMyGame

LOCAL_SRC_FILES := $(LOCAL_PATH)/hellocpp/main.cpp \
                   $(LOCAL_PATH)/../../../Classes/AppDelegate.cpp \
                   $(LOCAL_PATH)/../../../Classes/Audio.cpp \
                   $(LOCAL_PATH)/../../../Classes/AudioMenu.cpp \
                   $(LOCAL_PATH)/../../../Classes/DPad.cpp \
                   $(LOCAL_PATH)/../../../Classes/EnemyShip.cpp \
                   $(LOCAL_PATH)/../../../Classes/EnemyShipKling.cpp \
                   $(LOCAL_PATH)/../../../Classes/EnterName.cpp \
                   $(LOCAL_PATH)/../../../Classes/Game.cpp\
                   $(LOCAL_PATH)/../../../Classes/HighScores.cpp \
                   $(LOCAL_PATH)/../../../Classes/HUD.cpp \
                   $(LOCAL_PATH)/../../../Classes/Input.cpp \
                   $(LOCAL_PATH)/../../../Classes/Level.cpp \
                   $(LOCAL_PATH)/../../../Classes/Level1.cpp \
                   $(LOCAL_PATH)/../../../Classes/Level2.cpp \
                   $(LOCAL_PATH)/../../../Classes/Level3.cpp \
                   $(LOCAL_PATH)/../../../Classes/Level4.cpp \
                   $(LOCAL_PATH)/../../../Classes/MainMenu.cpp \
                   $(LOCAL_PATH)/../../../Classes/MenuScene.cpp \
                   $(LOCAL_PATH)/../../../Classes/MusicPlayer.cpp \
                   $(LOCAL_PATH)/../../../Classes/ParallaxNodeExtras.cpp \
                   $(LOCAL_PATH)/../../../Classes/Player.cpp \
                   $(LOCAL_PATH)/../../../Classes/Settings.cpp \
                   $(LOCAL_PATH)/../../../Classes/SplashScene.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../Classes

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static

# Additional code for UI
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_ui_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)
# 21/02/2018 Additional for UI
$(call import-module,extensions)
$(call import-module,ui)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END

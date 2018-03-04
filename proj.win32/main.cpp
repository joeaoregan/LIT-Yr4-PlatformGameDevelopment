/*
	main.h

	Joe O'Regan
	K00203642

	Changed window title
*/

#include "main.h"
#include "AppDelegate.h"
#include "cocos2d.h"
USING_NS_CC;

int WINAPI _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // create the application instance
    AppDelegate app;

	// Overrides appdelegate window creation
	auto director = Director::getInstance();
	director->setDisplayStats(true);
	auto glview = director->getOpenGLView();
	if (!glview) {
		glview = GLViewImpl::createWithRect("Space Game: Joe O'Regan K00203642", Rect(0, 0, 1280, 720), 1.0f);
		director->setOpenGLView(glview);
	}

    return Application::getInstance()->run();
}

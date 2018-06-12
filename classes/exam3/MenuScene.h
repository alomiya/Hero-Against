// 第6章例子2 -- 综合动画例子

#ifndef __MENU_SCENE_H__
#define __MENU_SCENE_H__

#include "config_set.h"
#include "Joystick.h"
#include "Hero.h"
#include "Enemy.h"
#include "MyContactListener.h"
#include "AIManager.h"
#include "AnimationScene.h"
#include "SimpleAudioEngine.h"
#include "SettingScene.h"
using namespace CocosDenshion;

class MenuScene : public Layer
{
public:
	MenuScene();
	~MenuScene();
	virtual bool init();
	CREATE_FUNC(MenuScene);
	static Scene* createScene();

	void update(float delta);

	void startCallback(Ref* pSender);
	void setCallback(Ref* pSender);
	void quitCallback(Ref* pSender);
private:

};

#endif
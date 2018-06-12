// 第6章例子2 -- 综合动画例子

#ifndef __Setting_SCENE_H__
#define __Setting_SCENE_H__

#include "config_set.h"
#include "Joystick.h"
#include "Hero.h"
#include "Enemy.h"
#include "MyContactListener.h"
#include "AIManager.h"
#include "MenuScene.h"
#include "SimpleAudioEngine.h"
#include "cocos-ext.h"              //包含cocos-ext.h头文件
using namespace cocos2d::extension; //引用cocos2d::extension命名空间
using namespace CocosDenshion;

class SettingScene : public Layer
{
public:
	SettingScene();
	~SettingScene();
	virtual bool init();
	CREATE_FUNC(SettingScene);
	static Scene* createScene();

	void update(float delta);

	void returnCallback(Ref* pSender);
	void switch1Changed(Ref* sender, Control::EventType controlEvent); //当值改变时触发的控件事件
	void switch2Changed(Ref* sender, Control::EventType controlEvent); //当值改变时触发的控件事件
private:
	ControlSwitch* controlSwitch1;
	ControlSwitch* controlSwitch2;
};

#endif
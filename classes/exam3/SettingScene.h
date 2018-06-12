// ��6������2 -- �ۺ϶�������

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
#include "cocos-ext.h"              //����cocos-ext.hͷ�ļ�
using namespace cocos2d::extension; //����cocos2d::extension�����ռ�
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
	void switch1Changed(Ref* sender, Control::EventType controlEvent); //��ֵ�ı�ʱ�����Ŀؼ��¼�
	void switch2Changed(Ref* sender, Control::EventType controlEvent); //��ֵ�ı�ʱ�����Ŀؼ��¼�
private:
	ControlSwitch* controlSwitch1;
	ControlSwitch* controlSwitch2;
};

#endif
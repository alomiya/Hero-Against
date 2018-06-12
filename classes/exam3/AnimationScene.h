// ��6������2 -- �ۺ϶�������

#ifndef __ANIMATION_SCENE_H__
#define __ANIMATION_SCENE_H__

#include "config_set.h"
#include "Joystick.h"
#include "Hero.h"
#include "Enemy.h"
#include "MyContactListener.h"
#include "AIManager.h"
#include "ResultScene.h" 
#include "SimpleAudioEngine.h"
#include "Manager.h"
using namespace CocosDenshion;

class AnimationScene : public Layer
{
public:
	AnimationScene();
	~AnimationScene();
	virtual bool init();
	CREATE_FUNC(AnimationScene);
	static Scene* createScene();

	void update(float delta);

	void attackCallback(Ref* pSender);
	void defendCallback(Ref* pSender);
	void enemyStand();
	void heroStand();
	void enemyMove();
	void heroMove();
	void player1wins();
	void player2wins();
	void drawgame(); //ƽ��
	void round1();
	void round2();
	void round3();
	void round4();
	void finalround();
	void ready();
	void go();
	void ko();
	void waiting();
	void backToResult();
	void timeover();
	void perfect();
private:
	Joystick* m_joystick;   // ҡ��
	MyContactListener* m_contactListener;  // ��ײ���
	AIManager* m_aimanager;  // AI manager
	Manager* m_manager;   //Ӣ����������������
	Hero* m_player;
	Enemy* m_enemy;

	Sprite* m_bloodBar_h;
	Sprite* m_bloodBar_e;
	Sprite* m_blueBar_h;
	Sprite* m_blueBar_e;
	
};

#endif
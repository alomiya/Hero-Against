#ifndef __RESULT_SCENE_H__
#define __RESULT_SCENE_H__

#include "config_set.h"
#include "Joystick.h"
#include "Hero.h"
#include "Enemy.h"
#include "MyContactListener.h"
#include "AIManager.h"
#include "MenuScene.h"
class ResultScene : public Layer
{
public:
	ResultScene();
	//ResultScene(int, int, int, int, int, int, int, int, bool);
	~ResultScene();
	virtual bool init();
	virtual void onEnter();
	CREATE_FUNC(ResultScene);
	int m_critNum_h;      //����������
	int m_attackNum_h;    //��������
	int m_attackedNum_h;    //����������
	int m_sufferedDamage_h; //�����˺�
	int m_critNum_e;      //����������
	int m_attackNum_e;    //��������
	int m_attackedNum_e;    //����������
	int m_sufferedDamage_e; //�����˺�
	int m_gameresult; //1 is player1 win,2 is player 2wins,0 is both lose
	int timecause;
	void quitCallback(Ref* pSender);
	//static Scene* createScene(int,int,int,int,int,int,int,int,bool);
	//static Scene* createScene();
private:
	float critRate_e;
	float DPS_e;
	float critRate_h;
	float DPS_h;
	
};
#endif
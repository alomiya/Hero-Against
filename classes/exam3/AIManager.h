#ifndef __AIMANAGER_H__
#define __AIMANAGER_H__

#include "config_set.h"
#include "Enemy.h"
#include "Hero.h"
#include "MyContactListener.h"
class AIManager : public Node
{
public:
	AIManager();
	~AIManager();

	static AIManager* create(Node* parent);
	void setAI(Enemy* enemy, Hero* hero);
	/*
	void moveLeft();
	void moveRight();
	void attack();
	void stand();
	*/
	void setIsGo(bool isgo) { is_go = isgo; }
private:
	virtual bool init(Node* parent);
	void update(float delta);
	MyContactListener* m_contactListener1;  // Åö×²¼ì²â
private:
	Enemy* m_enemy;
	Hero* m_hero;
	int cnt;

	State m_enemy_state; 
	bool is_go;
};

#endif
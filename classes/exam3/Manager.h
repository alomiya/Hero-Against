// ��6������2 -- �ۺ϶�������

#ifndef __MANAGER_H__
#define __MANAGER_H__

#include "config_set.h"

class Manager: public Layer
{
public:
	Manager();
	~Manager();
	virtual bool init(Node* parent);

	void update(float delta);
	static Manager* create(Node* parent);

	void updateHeroNum() { m_heroNum--; }
	void updateEnemyNum() { m_enemyNum--; }
	int getHeroNum() { return m_heroNum; }
	int getEnemyNum() { return m_enemyNum; }

	int getWholeTime() { return m_whole_time; }
	void updateTime() { m_whole_time = m_whole_time + 60 - m_scene_time; }

	int getRoundNum() { return m_roundNum; }
	void updateRound() { m_roundNum++; }

	void setOldTime(int time) { m_oldTime = time; }
	int getOldTime() { return m_oldTime; }

	int getSceneTime() { return m_scene_time; }
	void updateSceneTime() { m_scene_time--; }
	void resetSceneTime() { m_scene_time = 60; }

	bool isTimeOver() { return m_istimeover; }
	void setTimeOver(bool isOver) { m_istimeover = isOver; }
private:
	int m_heroNum;     // Ӣ����
	int m_enemyNum;    // ������
	int m_whole_time;  // ��ʱ��
	int m_scene_time;  // ����ʱ��
	int m_oldTime;     // �ϴμ�ʱ
	bool m_istimeover; // �Ƿ�ʱ�䵽
	int m_roundNum;    // �غ���
};

#endif
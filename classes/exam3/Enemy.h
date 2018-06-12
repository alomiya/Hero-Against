#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "config_set.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;
class Enemy : public Sprite
{
public:
	Enemy();
	~Enemy();

	static Enemy* create(Vec2 position);

	void update(float delta);

	void play(State state); // ���Ŷ���
	void hurt();  // ������

	void showBloodTips(int s);  // ��ʾ��Ѫ����
	void flyend(Label* label);  // ��Ѫ�����ƶ�����ʧ

	virtual bool init(Vec2 position);
	void onFrameEvent(cocostudio::Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex);  

	// set and get
	Armature* getArmature() { return m_armature; }

	bool isAttack() { return m_isAttack; }
	void setAttack(bool attack) { m_isAttack = attack; }

	int getLife() { return m_life; }
	void setLife(int life) { m_life = life; }

	int getBlue() { return m_blue; }
	void setBlue(int blue) { m_blue = blue; }

	void setLeft(bool isleft) { m_isleft = isleft; }
	bool getIsLeft() { return m_isleft; }

	bool isDeath() { return m_isdead; }

	int getMaxBlue() { return m_max_blue; }
	int getMaxLife() { return m_max_life; }
	void setreLife() { m_isrelife = true; }
	bool getIsReLife() { return m_isrelife; }
	void reLife();
	void updateLife() { 
		m_life = (1.3*m_life<m_max_life) ? 1.3*m_life : m_max_life;
		m_blue = (1.3*m_blue<m_max_life) ? 1.3*m_blue : m_max_blue;
	}
	int getCritNum() { return critNum; }
	int getAttackNum() { return attackNum; }
	int getAttackedNum() { return attackedNum; }
	int getSufferedDamage() { return sufferedDamage; }
	bool ishurt() { return m_ishurt; }
	bool getisAttacked() { return isattacked; }
	void setisAttacked(bool attacked) { isattacked = attacked; }
private:
	Armature* m_armature;
	State m_state;    // ���ﶯ��״̬
	bool m_isrunning; // �Ƿ������ܶ�
	bool m_isdead;    // �Ƿ�����
	bool m_isAttack;  // ����״̬

	bool m_isleft;    // �Ƿ��������

	bool m_ishurt;    // ����״̬
	bool m_isrelife;  // �Ƿ�����
	int m_life;       // ����ֵ

	
	int m_blue;       //����ֵ
	int m_max_life;   // �������ֵ
	int m_max_blue;   // �����ֵ
	
	int critNum;      //��������
	int attackNum;    //��������
	int attackedNum;    //����������
	int sufferedDamage; //�����˺�

	bool isattacked;
};
#endif
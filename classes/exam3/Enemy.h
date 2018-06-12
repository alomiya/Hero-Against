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

	void play(State state); // 播放动画
	void hurt();  // 被击中

	void showBloodTips(int s);  // 显示扣血数字
	void flyend(Label* label);  // 扣血数字移动并消失

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
	State m_state;    // 人物动画状态
	bool m_isrunning; // 是否正在跑动
	bool m_isdead;    // 是否死亡
	bool m_isAttack;  // 攻击状态

	bool m_isleft;    // 是否面向左边

	bool m_ishurt;    // 被击状态
	bool m_isrelife;  // 是否重生
	int m_life;       // 生命值

	
	int m_blue;       //法力值
	int m_max_life;   // 最大生命值
	int m_max_blue;   // 最大法力值
	
	int critNum;      //暴击次数
	int attackNum;    //攻击次数
	int attackedNum;    //被攻击次数
	int sufferedDamage; //承受伤害

	bool isattacked;
};
#endif
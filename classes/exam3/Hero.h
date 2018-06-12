#ifndef __HERO_H__
#define __HERO_H__

#include "config_set.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

class Hero : public Sprite
{
public:
	Hero();
	~Hero();

	static Hero* create(Vec2 position);

	void update(float delta);

	void play(State state);
	void hurt();  // 被击中

	void showTips(int s);  // 显示战斗信息
	void flyend(Label* label);  // 扣血数字移动并消失

	virtual bool init(Vec2 position);
	void onFrameEvent(cocostudio::Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex);  

	// set and get
	Armature* getArmature() { return m_armature; }
	
	bool isAttack() { return m_isAttack; }
	void setAttack(bool attack) { m_isAttack = attack; }
	void setDefend(bool defend) { m_isDefend = defend; }

	int getLife() { return m_life; }
	void setLife(int life) { m_life = life; }

	int getBlue() { return m_blue; }
	void setBlue(int blue) { m_blue = blue; }

	bool isDeath() { return m_isdead; }
	bool isDefend() { return m_isDefend; }

	int getMaxLife() { return m_max_life; }
	int getMaxBlue() { return m_max_blue; }

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
	State m_state;
	bool m_isrunning;
	bool m_isdead;
	bool m_isAttack;  // 是否正在攻击
	bool m_isDefend;  // 是否正在防御
	bool m_ishurt;    // 被击状态
	bool m_isrelife;  // 是否重生

	int m_life;       // 生命值
	int m_blue;       //法力值
	int m_max_life;   // 最大生命值
	int m_max_blue;   // 最大法力值

	int m_defend_blue; //抵挡技能法力消耗

	int critNum;      //暴击次数
	int attackNum;    //攻击次数
	int attackedNum;    //被攻击次数
	int sufferedDamage; //承受伤害

	bool isattacked;   //是否已攻击
};

#endif
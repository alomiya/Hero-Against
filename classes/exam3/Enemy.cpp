#include "Enemy.h"

Enemy::Enemy()
{
	m_isrunning = false;
	m_isdead = false;
	m_isAttack = false;
	m_isleft = true;
	m_ishurt = false;
	m_max_life = 500;
	m_max_blue = 350;
	m_life = m_max_life;
	m_blue = m_max_blue;
	m_isrelife = false;
	critNum = 0;      //暴击次数
	attackNum = 0;    //攻击次数
	attackedNum = 0;  //被攻击次数
	sufferedDamage = 0; //承受伤害
	isattacked = false;  
}

Enemy::~Enemy()
{

}

Enemy* Enemy::create(Vec2 position)
{
	Enemy *pRet = new(std::nothrow) Enemy();
	if (pRet && pRet->init(position))
	{
		pRet->autorelease();
		return pRet;
	} else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

bool Enemy::init(Vec2 position)
{
	if (!Sprite::init())
	{
		return false;
	}

	ArmatureDataManager::getInstance()->addArmatureFileInfo("exam3/AnimationScene/animation/tauren/tauren.ExportJson");
	m_armature = Armature::create("tauren");
	if (m_armature == NULL)
	{
		CCLOG("tauren load error!");
		return false;
	}
	m_armature->setPosition(Vec2::ZERO);
	m_armature->getAnimation()->play("loading");
	m_armature->getAnimation()->setFrameEventCallFunc(CC_CALLBACK_0(Enemy::onFrameEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
	this->addChild(m_armature);
	this->setPosition(position);

	this->scheduleUpdate();

	return true;
}

void Enemy::update(float delta)
{
	if (m_life <= 0)
	{
		play(DEATH);
	}
	switch (m_state)
	{
	case STAND:
		if ((m_isAttack == false) && (m_isrunning == true) && (m_isdead == false)) // 跑动结束只执行一次loading动画，避免点击attack动作无法播放
		{
			m_armature->getAnimation()->play("loading");
			m_isrunning = false;
		}
		break;
	case MOVELEFT:
		if ((m_isAttack == false) && (this->getPositionX() > 30) && (m_isdead == false) )
		{
			if (m_isrunning == false)
			{
				m_armature->getAnimation()->play("run");
				m_isrunning = true;
				setLeft(true);
			}
			if (m_armature->getScaleX() != 1)
			{
				m_armature->setScaleX(1);
			}
			this->setPositionX(this->getPositionX() - 3);
			
		}
		break;
	case MOVERIGHT:
		if ((m_isAttack == false) && (this->getPositionX() < Director::getInstance()->getVisibleSize().width-30) && (m_isdead == false) )
		{
			if (m_isrunning == false) // m_isrunning控制变量防止update时不断执行play("run")则永远显示run动画第一帧
			{
				m_armature->getAnimation()->play("run"); 
				m_isrunning = true;
				setLeft(false);
			}

			if (m_armature->getScaleX() != -1.0)
			{
				m_armature->setScaleX(-1);
			}
			this->setPositionX(this->getPositionX() + 3);
			
		}
		break;
	case ATTACK:
		if (m_isdead == false )
		{
			isattacked = true;
			attackNum++;
			m_isAttack = true;
			SimpleAudioEngine::sharedEngine()->playEffect("exam3/AnimationScene/bgm/knife.wav");
			m_armature->getAnimation()->play("attack");
		}
		break;
	case DEATH:
		if (m_isdead == false)
		{
			m_isrunning = false;
			m_isAttack = false;
			m_isdead = true;
			m_armature->getAnimation()->play("death");
		}
		break;
	case SMITTEN:
		if (m_isdead == false)
		{
			if (m_ishurt == true)
			{
				m_armature->getAnimation()->play("smitten");
				m_ishurt = false;
			}
		}
		break;
	}
}

void Enemy::onFrameEvent(cocostudio::Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex)
{
	log(evt.c_str());
	if (strcmp(evt.c_str(), "attack_end") == 0)
	{
		
		m_blue = (m_blue + 15 < m_max_blue) ? m_blue + 15 : m_max_blue;
		m_armature->getAnimation()->play("loading");
		m_isAttack = false;
	}
	if (strcmp(evt.c_str(), "smitten_end") == 0)
	{
		m_armature->getAnimation()->play("loading");
	}
	if (strcmp(evt.c_str(), "death_end") == 0)
	{
		if (m_isrelife == true)
		{
			reLife();
		}
		else {
			m_armature->getAnimation()->pause();
		}
	}
}


void Enemy::reLife() {
	m_isrelife = false;
	m_isdead = false;
	Size visibleSize = Director::getInstance()->getVisibleSize();
	this->setPosition(Vec2(visibleSize.width - 120, 150));
	m_life = m_max_life; m_blue = m_max_blue;
	m_armature->getAnimation()->play("loading");
}

void Enemy::play(State state)
{
	m_state = state;
}

//************************************
// Method:    hurt
// FullName:  Enemy::hurt
// Access:    public 
// Returns:   void
// Qualifier:
//************************************
void Enemy::hurt()
{
	// 根据基础伤害造成随机伤害
	showBloodTips(30); 
	m_ishurt = true;
	
}

// 显示扣血 
void Enemy::showBloodTips(int s)
{
	attackedNum++;
	int hitCount = 1;
	int hitRand = rand()%10;
	if (hitRand > 3 && hitRand < 8)
	{
		hitCount = 2;
		critNum++;
	} else if (hitRand > 7)
	{
		hitCount = 3;
		critNum++;
	}

	for (int i = 0; i < hitCount; i ++)
	{
		int hurt_blood = s + rand()%8;
		sufferedDamage = sufferedDamage + hurt_blood;
		setLife(m_life - hurt_blood); // 扣血
		auto label = Label::createWithBMFont("fonts/futura-48.fnt", StringUtils::format("-%d",hurt_blood));
		this->addChild(label, 5);
		label->setPosition(Vec2(0, 0) + Vec2(20 + rand()%80, 10 + rand()%80));
		label->runAction(Sequence::create(
			MoveBy::create(0.7f,Vec2(0,30)),
			CallFunc::create(CC_CALLBACK_0(Enemy::flyend, this, label)),
			NULL
			));
	}
}

void Enemy::flyend(Label* label)
{
	label->setVisible(false);
	label->removeFromParent();
}

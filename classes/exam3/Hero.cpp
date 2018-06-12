#include "Hero.h"

Hero::Hero()
{
	m_isDefend = false;
	m_isrunning = false;
	m_isdead = false;
	m_isAttack = false;
	m_ishurt = false;
	m_max_life = 500;
	m_max_blue = 350;
	m_life = m_max_life;
	m_blue = m_max_blue;
	m_defend_blue = 25;
	m_isrelife = false;
	critNum = 0;      //暴击次数
	attackNum = 0;    //攻击次数
	attackedNum = 0;  //被攻击次数
	sufferedDamage = 0; //承受伤害
	isattacked = false;
}

Hero::~Hero()
{

}

Hero* Hero::create(Vec2 position)
{
	Hero *pRet = new(std::nothrow) Hero();
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

bool Hero::init(Vec2 position)
{
	if (!Sprite::init())
	{
		return false;
	}

	ArmatureDataManager::getInstance()->addArmatureFileInfo("exam3/AnimationScene/animation/Hero/Hero.ExportJson");
	m_armature = Armature::create("Hero");
	if (m_armature == NULL)
	{
		CCLOG("hero load error!");
		return false;
	}
	m_armature->setPosition(Vec2::ZERO);
	m_armature->getAnimation()->play("loading");
	m_armature->getAnimation()->setFrameEventCallFunc(CC_CALLBACK_0(Hero::onFrameEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
	this->addChild(m_armature);
	this->setPosition(position);

	this->scheduleUpdate();

	return true;
}

void Hero::update(float delta)
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
		if ((this->getPositionX() > 30) && (m_isdead == false) && (m_isAttack == false))
		{
			if (m_isrunning == false)
			{
				m_armature->getAnimation()->play("run");
				m_isrunning = true;
			}
			if (m_armature->getScaleX() != -1)
			{
				m_armature->setScaleX(-1);
			}
			this->setPositionX(this->getPositionX() - 4);
		}
		break;
	case MOVERIGHT:
		if ((m_isAttack == false) && (this->getPositionX() < Director::getInstance()->getVisibleSize().width-30) && (m_isdead == false))
		{
			if (m_isrunning == false) // m_isrunning控制变量防止update时不断执行play("run")则永远显示run动画第一帧
			{
				m_armature->getAnimation()->play("run");
				m_isrunning = true;
			}

			if (m_armature->getScaleX() != 1.0)
			{
				m_armature->setScaleX(1);
			}
			this->setPositionX(this->getPositionX() + 4);
		}
		break;
	case ATTACK:
		if (m_isdead == false&& m_ishurt == false)
		{
			isattacked = true;
			attackNum++;  //攻击次数计算
			m_isAttack = true;
			SimpleAudioEngine::sharedEngine()->playEffect("exam3/AnimationScene/bgm/sword.wav");
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
	case DEFEND:
		if (m_isdead == false )
		{
			//有足够法力值释放抵挡技能
			if (m_blue > m_defend_blue)
			{
				SimpleAudioEngine::sharedEngine()->playEffect("exam3/AnimationScene/bgm/defend.wav");
				m_isDefend = true;                 // 处于防御中
				m_armature->getAnimation()->play("defend");
				setBlue(m_blue - m_defend_blue);  // 更新蓝量
			}
			//法力值不足
			else
			{
				showTips(-1);   // -1表示不够蓝防御
			}
		}
		break;
	}
}

void Hero::onFrameEvent(cocostudio::Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex)
{
	if (strcmp(evt.c_str(), "attack_end") == 0)
	{
		//平A回15蓝
		m_blue = (m_blue + 15 < m_max_blue) ? m_blue + 15 : m_max_blue;
		m_armature->getAnimation()->play("loading");
		m_isAttack = false;
	}
	if (strcmp(evt.c_str(), "smitten_end") == 0)
	{
		m_armature->getAnimation()->play("loading");
	}
	if (strcmp(evt.c_str(), "defend_end") == 0)
	{
		m_armature->getAnimation()->play("loading");
		m_isDefend = false;
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


void Hero::reLife() {
	m_isrelife = false;
	m_isdead = false;
	Size visibleSize = Director::getInstance()->getVisibleSize();
	this->setPosition(Vec2(visibleSize.width / 5, 35));
	m_life = m_max_life; m_blue = m_max_blue;
	m_armature->getAnimation()->play("loading");
}

void Hero::play(State state)
{
	m_state = state;
}

void Hero::hurt()
{
	// 根据基础伤害造成随机伤害
	if(isDefend())
		showTips(0);
	else
	{
		showTips(30);
		m_ishurt = true;
	}
}

// 显示战斗信息 
// 参数为0则为抵抗状态，为-1则表示法力不足
void Hero::showTips(int s)
{
	int tipsCount = 1;
	int tipsRand = rand()%10;
	if (tipsRand > 3 && tipsRand < 8&&!(s == 0 || s == -1))
	{
		tipsCount = 2;
		critNum++;        //被暴击次数
	} else if (tipsRand > 7)
	{
		tipsCount = 3; 
		critNum++;        //被暴击次数
	}
	//法力不足或防御状态则只显示一次tips
	if (s == 0 || s == -1)
		tipsCount = 1;
	else
		attackedNum++;   //被攻击次数

	for (int i = 0; i < tipsCount; i ++)
	{
		Label* label;
		int hurt_blood;
		//防御状态
		if (s == 0)
		{
			hurt_blood = 0;
			label = Label::createWithBMFont("fonts/futura-48.fnt", "miss");
			label->setColor(Color3B::GREEN);
		}
		//法力不足状态
		else if (s == -1)
		{
			hurt_blood = 0;
			label = Label::createWithBMFont("fonts/futura-48.fnt", "no mana");
			label->setColor(Color3B::ORANGE);
		}
		else
		{
			hurt_blood = s + rand() % 8;
			label = Label::createWithBMFont("fonts/futura-48.fnt", StringUtils::format("-%d", hurt_blood));
			label->setColor(Color3B::RED);
		}
		setLife(m_life - hurt_blood);
		sufferedDamage = sufferedDamage + hurt_blood;   //被造成伤害
		this->addChild(label, 5);
		label->setPosition(Vec2(0, 0) + Vec2(20 + rand()%80, 10 + rand()%80));
		label->runAction(Sequence::create(
			MoveBy::create(0.7f,Vec2(0,30)),
			CallFunc::create(CC_CALLBACK_0(Hero::flyend, this, label)),
			NULL
			));
	}
}

void Hero::flyend(Label* label)
{
	label->setVisible(false);
	label->removeFromParent();
}

#include "AIManager.h"

//************************************
// Method:    create
// FullName:  AIManager::create
// Access:    public static 
// Returns:   AIManager*
// Qualifier:
// Parameter: Node * parent
//************************************
AIManager* AIManager::create(Node* parent)
{
	AIManager* pRet = new(std::nothrow) AIManager();
	if (pRet && pRet->init(parent))
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

//************************************
// Method:    setAI
// FullName:  AIManager::setAI
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: Enemy * enemy
//************************************
void AIManager::setAI(Enemy* enemy, Hero* hero)
{
	m_enemy = enemy;
	m_hero = hero;
	m_enemy->play(STAND);
	this->scheduleUpdate();
	
	/*
	//AI方案一
	//AI动作依次为向左-》攻击-》停下-》向右-》攻击-》停下，并一直循环执行
	
	auto _left = CallFunc::create(CC_CALLBACK_0(AIManager::moveLeft, this));
	auto _attack = CallFunc::create(CC_CALLBACK_0(AIManager::attack, this));
	auto _right = CallFunc::create(CC_CALLBACK_0(AIManager::moveRight, this));
	auto _stand = CallFunc::create(CC_CALLBACK_0(AIManager::stand, this));
	auto sss = Sequence::create(
		_left,DelayTime::create(1.5f),_attack,DelayTime::create(0.75f),_stand,DelayTime::create(2.0f),
		_right,DelayTime::create(0.5f),_attack, DelayTime::create(0.75f), _stand, DelayTime::create(2.0f),
		NULL
		);
	auto act = RepeatForever::create(sss);
	this->runAction(act);
	*/
	//AI方案二
	//AI依次搜索主角的坐标，判断向左向右或直接攻击，移动至打击范围，并循环进行攻击和停下的动作，直到主角离开范围
	//上述动作在update()循环进行
}

//************************************
// Method:    init
// FullName:  AIManager::init
// Access:    virtual private 
// Returns:   bool
// Qualifier:
// Parameter: Node * parent
//************************************
bool AIManager::init(Node* parent)
{
	if (!Node::init())
	{
		return false;
	}
	parent->addChild(this);

	return true;
}

//************************************
// Method:    update
// FullName:  AIManager::update
// Access:    private 
// Returns:   void
// Qualifier:
// Parameter: float delta
//************************************
void AIManager::update(float delta)
{
	// 敌人动作
	float m_distance = m_enemy->getPositionX() - m_hero->getPositionX();
	//如果处于过场时间则处于等待状态
	if (is_go == false)
	{
		m_enemy->play(STAND);
	}
	//不加上&& !m_enemy->isAttack()会出现没攻击完成就强行转身的bug
	else if ((m_distance > 120 || m_distance > 0 && m_enemy->getIsLeft() == false)&& !m_enemy->isAttack())
	{
		m_enemy->play(MOVELEFT);
	}
	else if ((m_distance < -110 || m_distance <0 && m_enemy->getIsLeft() == true )&& !m_enemy->isAttack())
	{
		m_enemy->play(MOVERIGHT);
	}
	//避免攻击的时候同时被攻击而进入假死状态
	else if (m_enemy->ishurt() && !m_enemy->isAttack())
	{
		m_enemy->play(SMITTEN);
	}
	/*else if (!m_enemy->isAttack()&&!(m_distance < 90&& m_distance >-80 )) //<-难度会非常高
	{
		m_enemy->play(ATTACK);
	}*/
	//模拟delay
	else if(cnt<45)    //<-难度较低
	{
		m_enemy->play(ATTACK);
		cnt++;
	}
	else 
	{
		m_enemy->play(STAND);
		cnt++;
		cnt = cnt % 105;
	}
}

//************************************
// Method:    AIManager
// FullName:  AIManager::AIManager
// Access:    public 
// Returns:   
// Qualifier:
//************************************
AIManager::AIManager()
{
	cnt = 0;
	is_go = false;
}

//************************************
// Method:    ~AIManager
// FullName:  AIManager::~AIManager
// Access:    public 
// Returns:   
// Qualifier:
//************************************
AIManager::~AIManager()
{

}
/*
//************************************
// Method:    moveLeft
// FullName:  AIManager::moveLeft
// Access:    public 
// Returns:   void
// Qualifier:
//************************************
void AIManager::moveLeft()
{
	m_enemy->play(MOVELEFT);
}

//************************************
// Method:    moveRight
// FullName:  AIManager::moveRight
// Access:    public 
// Returns:   void
// Qualifier:
//************************************
void AIManager::moveRight()
{
	m_enemy->play(MOVERIGHT);
}

//************************************
// Method:    attack
// FullName:  AIManager::attack
// Access:    public 
// Returns:   void
// Qualifier:
//************************************
void AIManager::attack()
{
	m_enemy->play(ATTACK);
}

//************************************
// Method:    stand
// FullName:  AIManager::stand
// Access:    public 
// Returns:   void
// Qualifier:
//************************************
void AIManager::stand()
{
	m_enemy->play(STAND);
}
*/

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
	//AI����һ
	//AI��������Ϊ����-������-��ͣ��-������-������-��ͣ�£���һֱѭ��ִ��
	
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
	//AI������
	//AI�����������ǵ����꣬�ж��������һ�ֱ�ӹ������ƶ��������Χ����ѭ�����й�����ͣ�µĶ�����ֱ�������뿪��Χ
	//����������update()ѭ������
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
	// ���˶���
	float m_distance = m_enemy->getPositionX() - m_hero->getPositionX();
	//������ڹ���ʱ�����ڵȴ�״̬
	if (is_go == false)
	{
		m_enemy->play(STAND);
	}
	//������&& !m_enemy->isAttack()�����û������ɾ�ǿ��ת���bug
	else if ((m_distance > 120 || m_distance > 0 && m_enemy->getIsLeft() == false)&& !m_enemy->isAttack())
	{
		m_enemy->play(MOVELEFT);
	}
	else if ((m_distance < -110 || m_distance <0 && m_enemy->getIsLeft() == true )&& !m_enemy->isAttack())
	{
		m_enemy->play(MOVERIGHT);
	}
	//���⹥����ʱ��ͬʱ���������������״̬
	else if (m_enemy->ishurt() && !m_enemy->isAttack())
	{
		m_enemy->play(SMITTEN);
	}
	/*else if (!m_enemy->isAttack()&&!(m_distance < 90&& m_distance >-80 )) //<-�ѶȻ�ǳ���
	{
		m_enemy->play(ATTACK);
	}*/
	//ģ��delay
	else if(cnt<45)    //<-�ѶȽϵ�
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

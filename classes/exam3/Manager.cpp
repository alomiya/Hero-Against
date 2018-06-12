#include "Manager.h"
Manager::Manager()
{
	m_enemyNum = 3;
	m_heroNum = 3;
	m_whole_time = 0;  // 总时间
	m_scene_time = 60;  // 场景时间
	m_istimeover = false;
	m_roundNum = 1;
}

Manager::~Manager()
{

}

Manager* Manager::create(Node* parent)
{
	Manager* pRet = new(std::nothrow) Manager();
	if (pRet && pRet->init(parent))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

bool Manager::init(Node* parent)
{
	if (!Node::init())
	{
		return false;
	}
	parent->addChild(this);

	// update
	this->scheduleUpdate();
	return true;
}

void Manager::update(float delta)
{
}

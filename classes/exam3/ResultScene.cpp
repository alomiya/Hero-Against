#include "ResultScene.h"

ResultScene::ResultScene()
{
	
}
/*
ResultScene::ResultScene(int critNum_h, int attackNum_h, int attackedNum_h, int suffedDamage_h,
	int critNum_e, int attackNum_e, int attackedNum_e, int suffedDamage_e, bool isplayer1win)
{
	m_attackedNum_e = attackedNum_e; m_critNum_e = critNum_e; m_sufferedDamage_e = suffedDamage_e; m_attackNum_e = attackedNum_e;
	m_attackedNum_h = attackedNum_h; m_critNum_h = critNum_h; m_sufferedDamage_h = suffedDamage_h; m_attackNum_h = attackedNum_h;
	log("%d %d %d %d %d %d %d %d", m_attackedNum_e, m_critNum_e, m_sufferedDamage_e, m_attackNum_e,
		m_attackedNum_h, m_critNum_h, m_sufferedDamage_h, m_attackNum_h);
}*/
ResultScene::~ResultScene()
{

}

bool ResultScene::init()
{

	if (!Layer::init())
	{
		return false;
	}
	
	m_critNum_h = 0;      //暴击次数
	m_attackNum_h = 0;    //攻击次数
	m_attackedNum_h = 0;    //被攻击次数
	m_sufferedDamage_h = 0; //承受伤害
	m_critNum_e = 0;      //暴击次数
	m_attackNum_e = 0;    //攻击次数
	m_attackedNum_e = 0;    //被攻击次数
	m_sufferedDamage_e = 0; //承受伤害
	bool isPlayerOneWin = false;  //是否玩家一获胜
	auto quitItem = MenuItemImage::create(
		"exam3/AnimationScene/animation/control/quit.png",
		"exam3/AnimationScene/animation/control/quit.png",
		CC_CALLBACK_1(ResultScene::quitCallback, this));
	Size visibleSize = Director::getInstance()->getVisibleSize();
	quitItem->setPosition(Vec2(visibleSize.width *7/ 8, 75));
	quitItem->setScale(0.8);
	auto menu2 = Menu::create(quitItem, NULL);
	menu2->setPosition(Vec2::ZERO);
	this->addChild(menu2, 5);
	

	return true;
}
/*Scene* ResultScene::createScene(int critNum_h,int attackNum_h,int attackedNum_h,int suffedDamage_h,
	int critNum_e,int attackNum_e,int attackedNum_e,int suffedDamage_e,bool isplayer1win)
{
	m_attackedNum_e = attackedNum_e; m_critNum_e = critNum_e; m_suffedDamage_e = suffedDamage_e; m_attackNum_e = attackedNum_e;
	m_attackedNum_h = attackedNum_h; m_critNum_h = critNum_h; m_suffedDamage_h = suffedDamage_h; m_attackNum_h = attackedNum_h;
	log("%d %d %d %d %d %d %d %d", m_attackedNum_e, m_critNum_e, m_suffedDamage_e, m_attackNum_e,
		m_attackedNum_h, m_critNum_h, m_suffedDamage_h, m_attackNum_h);
	auto scene = Scene::create();
	auto layer = ResultScene::create();
	scene->addChild(layer);
	return scene;
}*/
/*
Scene* ResultScene::createScene()
{
	auto scene = Scene::create();
	auto layer = ResultScene::create();
	scene->addChild(layer);
	return scene;
}*/
void ResultScene::onEnter()
{
	CCLayer::onEnter();
	Size visibleSize = Director::getInstance()->getVisibleSize();

	// 背景
	auto background = Sprite::create("exam3/AnimationScene/animation/background/rebg.jpg");
	background->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	background->setScale(0.5);
	background->setTag(110);
	this->addChild(background);
	log("%d %d %d %d %d %d %d %d", m_attackedNum_e, m_critNum_e, m_sufferedDamage_e, m_attackNum_e,
		m_attackedNum_h, m_critNum_h, m_sufferedDamage_h, m_attackNum_h);
	
    
	critRate_e = (m_attackedNum_h==0)?0:1.0*m_critNum_h/ m_attackedNum_h;
	critRate_h = (m_attackedNum_e == 0) ? 0 : 1.0*m_critNum_e / m_attackedNum_e;
	DPS_e = 1.0*m_sufferedDamage_h / timecause;
	DPS_h = 1.0*m_sufferedDamage_e / timecause;

	Dictionary* strings = Dictionary::createWithContentsOfFile("fonts/my.xml");
	const char *dmg_text=((String*)strings->objectForKey("dmg"))->getCString();
	const char *dps_text = ((String*)strings->objectForKey("dps"))->getCString();
	const char *crit_text = ((String*)strings->objectForKey("crit"))->getCString();

	auto tips_dmg_1 = Label::createWithBMFont("fonts/u.fnt", dmg_text);
	tips_dmg_1->setPosition(Vec2(visibleSize.width / 14, visibleSize.height - 160));
	tips_dmg_1->setScale(0.5);
	this->addChild(tips_dmg_1, 5);
	
	auto tips_dps_1 = Label::createWithBMFont("fonts/u.fnt", dps_text);
	tips_dps_1->setPosition(Vec2(visibleSize.width / 14, visibleSize.height - 270));
	tips_dps_1->setScale(0.5);
	this->addChild(tips_dps_1, 5);
	
	auto tips_crit_1 = Label::createWithBMFont("fonts/u.fnt", crit_text);
	tips_crit_1->setPosition(Vec2(visibleSize.width / 14, visibleSize.height - 370));
	tips_crit_1->setScale(0.5);
	this->addChild(tips_crit_1, 5);
	

	auto tips_dmg_2 = Label::createWithBMFont("fonts/u.fnt", dmg_text);
	tips_dmg_2->setPosition(Vec2(visibleSize.width * 13 / 14, visibleSize.height - 170));
	tips_dmg_2->setScale(0.5); 
	this->addChild(tips_dmg_2, 5);
	
	auto tips_dps_2 = Label::createWithBMFont("fonts/u.fnt", dps_text);
	tips_dps_2->setPosition(Vec2(visibleSize.width *13/ 14, visibleSize.height - 270));
	tips_dps_2->setScale(0.5); 
	this->addChild(tips_dps_2, 5);
	
	auto tips_crit_2 = Label::createWithBMFont("fonts/u.fnt", crit_text);
	tips_crit_2->setPosition(Vec2(visibleSize.width * 13/ 14, visibleSize.height - 370));
	tips_crit_2->setScale(0.5);
	this->addChild(tips_crit_2, 5);

	auto tips_dmg_h = Label::createWithBMFont("fonts/u.fnt", StringUtils::format("%8d", m_sufferedDamage_e));
	tips_dmg_h->setPosition(Vec2(visibleSize.width / 4, visibleSize.height - 200));
	tips_dmg_h->setScale(1.2);
	this->addChild(tips_dmg_h, 5);
	auto tips_dps_h = Label::createWithBMFont("fonts/u.fnt", StringUtils::format("%7.2f", DPS_h));
	tips_dps_h->setPosition(Vec2(visibleSize.width / 4, visibleSize.height - 300));
	tips_dps_h->setScale(1.2);
	this->addChild(tips_dps_h, 5);
	auto tips_crit_h = Label::createWithBMFont("fonts/u.fnt", StringUtils::format("%3.2f%%", critRate_h*100));
	tips_crit_h->setPosition(Vec2(visibleSize.width / 4, visibleSize.height - 400));
	tips_crit_h->setScale(1.2);
	this->addChild(tips_crit_h, 5);


	auto tips_dmg_e = Label::createWithBMFont("fonts/u.fnt", StringUtils::format("%-10d", m_sufferedDamage_h));
	tips_dmg_e->setPosition(Vec2(visibleSize.width *3/4, visibleSize.height - 200));
	tips_dmg_e->setScale(1.2);
	this->addChild(tips_dmg_e, 5);

	auto tips_dps_e = Label::createWithBMFont("fonts/u.fnt", StringUtils::format("%-9.2f", DPS_e));
	tips_dps_e->setPosition(Vec2(visibleSize.width * 3 /4, visibleSize.height - 300));
	tips_dps_e->setScale(1.2);
	this->addChild(tips_dps_e, 5);

	auto tips_crit_e = Label::createWithBMFont("fonts/u.fnt", StringUtils::format("%-3.2f%%", critRate_e * 100.0));
	tips_crit_e->setPosition(Vec2(visibleSize.width * 3/ 4, visibleSize.height - 400));
	tips_crit_e->setScale(1.2);
	this->addChild(tips_crit_e, 5);


	auto tips_time= Label::createWithBMFont("fonts/u.fnt", StringUtils::format("TIME  %02d:%02d",timecause/60,timecause%60));
	tips_time->setPosition(Vec2(visibleSize.width / 10, visibleSize.height - 40));
	tips_time->setScale(0.7);
	this->addChild(tips_time, 5);

	auto tips_win = Sprite::create("exam3/AnimationScene/animation/tips/win.png");
	auto tips_lose = Sprite::create("exam3/AnimationScene/animation/tips/lose.png");
	auto tips_lose1 = Sprite::create("exam3/AnimationScene/animation/tips/lose.png");
	if (m_gameresult == 1)
	{
		tips_win->setPosition(Vec2(visibleSize.width / 7, visibleSize.height - 500));
		tips_lose->setPosition(Vec2(visibleSize.width * 6 / 7, visibleSize.height - 500));
		tips_win->setScale(0.8);
		tips_lose->setScale(0.8);
		this->addChild(tips_win);
		this->addChild(tips_lose);
	}
	else if (m_gameresult == 2)
	{
		tips_lose->setPosition(Vec2(visibleSize.width / 6, visibleSize.height - 500));
		tips_win->setPosition(Vec2(visibleSize.width * 6 / 7, visibleSize.height - 500));
		tips_win->setScale(0.8);
		tips_lose->setScale(0.8);
		this->addChild(tips_win);
		this->addChild(tips_lose);
	}
	else
	{
		tips_lose->setPosition(Vec2(visibleSize.width / 6, visibleSize.height - 500));
		tips_lose1->setPosition(Vec2(visibleSize.width * 6 / 7, visibleSize.height - 500));
		tips_lose->setScale(0.8);
		tips_lose1->setScale(0.8);
		this->addChild(tips_lose);
		this->addChild(tips_lose1);
	}


	auto tips_player1 = Sprite::create("exam3/AnimationScene/animation/tips/player1.png");
	auto tips_player2 = Sprite::create("exam3/AnimationScene/animation/tips/player2.png");
	tips_player1->setPosition(Vec2(visibleSize.width / 7, visibleSize.height - 80));
	tips_player2->setPosition(Vec2(visibleSize.width * 6 / 7, visibleSize.height - 80));
	tips_player1->setScale(0.25);
	tips_player2->setScale(0.25);
	this->addChild(tips_player1);
	this->addChild(tips_player2);

	auto tips_leftchart= Sprite::create("exam3/AnimationScene/animation/tips/leftchart.png");
	auto tips_rightchart = Sprite::create("exam3/AnimationScene/animation/tips/rightchart.png");
	tips_leftchart->setPosition(Vec2(visibleSize.width / 6, visibleSize.height - 110));
	tips_rightchart->setPosition(Vec2(visibleSize.width * 5 / 6, visibleSize.height - 110));
	//tips_leftchart->setScale(0);
	//tips_rightchart->setScale(0.8);
	this->addChild(tips_leftchart);
	this->addChild(tips_rightchart);
	
	
}

void ResultScene::quitCallback(Ref* pSender)
{
	auto scene = MenuScene::createScene();
	Director::getInstance()->pushScene(scene);
}
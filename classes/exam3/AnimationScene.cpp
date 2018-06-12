#include "AnimationScene.h"


AnimationScene::AnimationScene()
{
	
}

AnimationScene::~AnimationScene()
{

}

bool AnimationScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();

	//时间、回合数、生命等游戏机制控制器
	m_manager = Manager::create(this);

	// 背景
	auto background = LayerColor::create(Color4B(128,128,128,255), 960, 640);
	background->setPosition(Vec2(0, 0));
	background->setTag(110);
	this->addChild(background);
	auto cloud_1 = Sprite::create("exam3/AnimationScene/animation/background/cloud.png");
	cloud_1->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
	cloud_1->setTag(112);
	this->addChild(cloud_1);
	auto cloud_2 = Sprite::create("exam3/AnimationScene/animation/background/cloud.png");
	cloud_2->setPosition(Vec2(cloud_1->getPositionX() + cloud_1->getContentSize().width, visibleSize.height/2));
	cloud_2->setTag(113);
	this->addChild(cloud_2);
	
	// 场景计时器
	auto tips_times = Label::createWithBMFont("fonts/futura-48.fnt", StringUtils::format("%02d",m_manager->getSceneTime()));
	tips_times->setPosition(Vec2(visibleSize.width/2,visibleSize.height - 50));
	tips_times->setTag(115);
	this->addChild(tips_times, 5);
	struct timeval now;
	gettimeofday(&now, NULL);
	m_manager->setOldTime(now.tv_sec);

	// 摇杆
	m_joystick = Joystick::create(Vec2(100,100), 50.0f, "exam3/AnimationScene/animation/control/j-btn.png", "exam3/AnimationScene/animation/control/j-bg.png");
	this->addChild(m_joystick, 4);

	// 主攻击按钮
	auto attackItem = MenuItemImage::create(
		"exam3/AnimationScene/animation/control/attackitem.png",
		"exam3/AnimationScene/animation/control/attackitem.png",
		CC_CALLBACK_1(AnimationScene::attackCallback, this));

	attackItem->setPosition(Vec2(visibleSize.width - 50, 80));  //按钮位置
	attackItem->setScale(0.7);    //设置大小
	auto menu = Menu::create(attackItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 5);

	// 防御技能按钮
	auto defendItem = MenuItemImage::create(
		"exam3/AnimationScene/animation/control/defenditem.png",
		"exam3/AnimationScene/animation/control/defenditem.png",
		CC_CALLBACK_1(AnimationScene::defendCallback, this));

	defendItem->setPosition(Vec2(visibleSize.width - 160, 60)); //按钮位置
	defendItem->setScale(0.9);    //设置大小
	auto menu1 = Menu::create(defendItem, NULL);
	menu1->setPosition(Vec2::ZERO);
	this->addChild(menu1, 5);

	// 地图
	auto map = TMXTiledMap::create("exam3/AnimationScene/animation/background/background.tmx");
	if (map == NULL)
	{
		CCLOG("map load failed!");
		return false;
	}
	Size mapSize = map->getMapSize();
	Size tiledSize = map->getTileSize();
	map->setPosition(Vec2(0, 0));
	map->setTag(111);
	this->addChild(map, 2);
	// hero
	m_player = Hero::create(Vec2(visibleSize.width / 5, 35));
	this->addChild(m_player, 3);


	// enemy
	m_enemy = Enemy::create(Vec2(visibleSize.width - 120, 150));
	this->addChild(m_enemy, 3);

	auto _round = CallFunc::create(CC_CALLBACK_0(AnimationScene::round1, this));
	auto _ready = CallFunc::create(CC_CALLBACK_0(AnimationScene::ready, this));
	auto _go = CallFunc::create(CC_CALLBACK_0(AnimationScene::go, this));
	auto gametips = Sequence::create(_round, DelayTime::create(1.5f), _ready, DelayTime::create(1.0f), _go, DelayTime::create(0.7f), NULL);
	this->runAction(gametips);

	// AI 
	m_aimanager = AIManager::create(this);
	m_aimanager->setAI(m_enemy, m_player);

	// 碰撞检测
	m_contactListener = MyContactListener::create(this, m_player, m_enemy);

	// 血条
	m_bloodBar_h = Sprite::create("exam3/AnimationScene/animation/background/1-new.png");
	m_bloodBar_h->setTag(10000);
	m_bloodBar_h->setPosition(Vec2(m_bloodBar_h->getContentSize().width/2 + 5, visibleSize.height - 50));
	
	m_bloodBar_e = Sprite::create("exam3/AnimationScene/animation/background/2-new.png");
	m_bloodBar_e->setTag(10001);
	m_bloodBar_e->setPosition(Vec2(visibleSize.width - m_bloodBar_e->getContentSize().width/2 - 5, visibleSize.height - 50));
	
	// 蓝条
	m_blueBar_h = Sprite::create("exam3/AnimationScene/animation/background/1-new-b.png");
	m_blueBar_h->setTag(10002);
	m_blueBar_h->setPosition(Vec2(m_bloodBar_h->getContentSize().width / 2 + 5, visibleSize.height - 50));

	m_blueBar_e = Sprite::create("exam3/AnimationScene/animation/background/2-new-b.png");
	m_blueBar_e->setTag(10003);
	m_blueBar_e->setPosition(Vec2(visibleSize.width - m_bloodBar_e->getContentSize().width / 2 - 5, visibleSize.height - 50));
	
	//角色UI层
	auto blood_bg_h = Sprite::create("exam3/AnimationScene/animation/background/1-new-f.png");
	blood_bg_h->setPosition(Vec2(m_bloodBar_h->getContentSize().width / 2 + 5, visibleSize.height - 50));
	auto blood_bg_e = Sprite::create("exam3/AnimationScene/animation/background/2-new-f.png");
	blood_bg_e->setPosition(Vec2(visibleSize.width - m_bloodBar_e->getContentSize().width / 2 - 5, visibleSize.height - 50));
	
	this->addChild(blood_bg_h, 5);
	this->addChild(m_bloodBar_h, 5);
	this->addChild(m_blueBar_h, 5);

	this->addChild(blood_bg_e, 5);
	this->addChild(m_bloodBar_e, 5);
	this->addChild(m_blueBar_e, 5);
	
	//英雄数、敌人数初始化
	auto m_heroOne = Sprite::create("exam3/AnimationScene/animation/background/hero-1.png");
	auto m_heroTwo = Sprite::create("exam3/AnimationScene/animation/background/hero-1.png");
	auto m_heroThree = Sprite::create("exam3/AnimationScene/animation/background/hero-1.png");
	m_heroOne->setPosition(Vec2(150, visibleSize.height - 105));
	m_heroOne->setTag(10004);
	m_heroTwo->setPosition(Vec2(105, visibleSize.height - 105));
	m_heroTwo->setTag(10005);
	m_heroThree->setPosition(Vec2(60, visibleSize.height - 105));
	m_heroThree->setTag(10006);

	m_heroOne->setScale(0.8);
	m_heroTwo->setScale(0.8);
	m_heroThree->setScale(0.8);

	this->addChild(m_heroThree, 5);
	this->addChild(m_heroTwo, 5);
	this->addChild(m_heroOne, 5);

	auto m_enemyOne = Sprite::create("exam3/AnimationScene/animation/background/enemy-1.png");
	auto m_enemyTwo = Sprite::create("exam3/AnimationScene/animation/background/enemy-1.png");
	auto m_enemyThree = Sprite::create("exam3/AnimationScene/animation/background/enemy-1.png");
	m_enemyOne->setPosition(Vec2(visibleSize.width-150, visibleSize.height - 105));
	m_enemyOne->setTag(10007);
	m_enemyTwo->setPosition(Vec2(visibleSize.width -105, visibleSize.height - 105));
	m_enemyTwo->setTag(10008);
	m_enemyThree->setPosition(Vec2(visibleSize.width -60, visibleSize.height - 105));
	m_enemyThree->setTag(10009);

	m_enemyOne->setScale(0.8);
	m_enemyTwo->setScale(0.8);
	m_enemyThree->setScale(0.8);

	this->addChild(m_enemyThree, 5);
	this->addChild(m_enemyTwo, 5);
	this->addChild(m_enemyOne, 5);

	// update
	this->scheduleUpdate();

	return true;
}

Scene* AnimationScene::createScene()
{
	auto scene = Scene::create();
	auto layer = AnimationScene::create();
	scene->addChild(layer);
	return scene;
}

void AnimationScene::update(float delta)
{
	struct timeval now;
	gettimeofday(&now, NULL);
	int nowTime = now.tv_sec;
	if ((nowTime - m_manager->getOldTime()) == 1)
	{
		if (m_manager->getSceneTime() > 0)
		{
			m_manager->updateSceneTime();
		}
		m_manager->setOldTime(nowTime);
		auto tips_time = (LabelBMFont*)this->getChildByTag(115);
		tips_time->setString(StringUtils::format("%02d", m_manager->getSceneTime()));
		if (m_manager->getSceneTime() == 0 && m_manager->isTimeOver() == false)
		{
			if (!m_player->isDeath() &&! m_enemy->isDeath() && m_manager->getHeroNum() > 0 && m_manager->getEnemyNum() > 0)
			{
				if (1.0*m_player->getLife() / m_player->getMaxLife() > 1.0*m_enemy->getLife() / m_enemy->getMaxLife())
				{
					m_manager->updateEnemyNum();
					if (m_manager->getEnemyNum() == 2)
					{
						auto enemyOne = this->getChildByTag(10007);
						enemyOne->setColor(Color3B::GRAY);
					}
					else if (m_manager->getEnemyNum() == 1)
					{
						auto enemyTwo = this->getChildByTag(10008);
						enemyTwo->setColor(Color3B::GRAY);
					}
					if (m_manager->getEnemyNum() == 0)
					{
						auto enemyThree = this->getChildByTag(10009);
						enemyThree->setColor(Color3B::GRAY);
					}
					if (m_manager->getEnemyNum() >= 0)
					{
						auto _stand = CallFunc::create(CC_CALLBACK_0(AnimationScene::heroStand, this));
						auto _move = CallFunc::create(CC_CALLBACK_0(AnimationScene::heroMove, this));
						auto _round2 = CallFunc::create(CC_CALLBACK_0(AnimationScene::round2, this));
						auto _round3 = CallFunc::create(CC_CALLBACK_0(AnimationScene::round3, this));
						auto _round4 = CallFunc::create(CC_CALLBACK_0(AnimationScene::round4, this));
						auto _finalround = CallFunc::create(CC_CALLBACK_0(AnimationScene::finalround, this));
						auto _ready = CallFunc::create(CC_CALLBACK_0(AnimationScene::ready, this));
						auto _go = CallFunc::create(CC_CALLBACK_0(AnimationScene::go, this));
						auto _timeover = CallFunc::create(CC_CALLBACK_0(AnimationScene::timeover, this));
						auto _player1wins = CallFunc::create(CC_CALLBACK_0(AnimationScene::player1wins, this));
						auto _waiting = CallFunc::create(CC_CALLBACK_0(AnimationScene::waiting, this));
						//auto ko_wins = Sequence::create(_timeover, DelayTime::create(2.0f), _player1wins, DelayTime::create(2.0f), _waiting, NULL);
						auto _perfect = CallFunc::create(CC_CALLBACK_0(AnimationScene::perfect, this));
						Sequence* wins;
						if (m_player->getLife() == m_player->getMaxLife())
							wins = Sequence::create(_timeover, DelayTime::create(1.5f), _player1wins, DelayTime::create(1.5f), _perfect, DelayTime::create(1.0f), _waiting, NULL);
						else
							wins = Sequence::create(_timeover, DelayTime::create(2.0f), _player1wins, DelayTime::create(2.0f), _waiting, NULL);
						auto stand_spawn = Spawn::create(_stand, wins, NULL);
						if (m_manager->getEnemyNum() > 0)
						{
							m_manager->updateRound();
							int roundNum = m_manager->getRoundNum();
							CallFunc* _round;
							if (roundNum == 2)
								_round = _round2;
							else if (roundNum == 3)
								_round = _round3;
							else if (roundNum == 4)
								_round = _round4;
							else if (roundNum == 5)
								_round = _finalround;
							auto gametips = Sequence::create(_round, DelayTime::create(1.5f), _ready, DelayTime::create(1.0f), _go, DelayTime::create(0.7f), NULL);
							auto move_spawn = Spawn::create(_move, gametips, NULL);
							auto sss = Sequence::create(
								stand_spawn, DelayTime::create(2.5f), move_spawn, DelayTime::create(3.5f),
								NULL
							);
							//m_enemy->reLife();
							this->runAction(sss);
						}
						else
						{
							auto ss = CallFunc::create(CC_CALLBACK_0(AnimationScene::backToResult, this));
							Sequence* a;
							if (m_player->getLife() == m_player->getMaxLife())
								a = Sequence::create(_timeover, DelayTime::create(1.5f), _player1wins, DelayTime::create(1.5f), _perfect, DelayTime::create(1.0f), NULL);
							else
								a = Sequence::create(_timeover, DelayTime::create(2.0f), _player1wins, DelayTime::create(2.0f), NULL);
							auto stands_spawn = Spawn::create(_stand, a, NULL);
							auto sss = Sequence::create(
								stands_spawn, DelayTime::create(2.5f), ss,
								NULL
							);
							this->runAction(sss);

						}
						m_player->updateLife();
					}
				}
				else if (1.0*m_player->getLife() / m_player->getMaxLife() < 1.0*m_enemy->getLife() / m_enemy->getMaxLife())
				{
					m_manager->updateHeroNum();
					if (m_manager->getHeroNum() == 2)
					{
						auto heroOne = this->getChildByTag(10004);
						heroOne->setColor(Color3B::GRAY);
					}
					else if (m_manager->getHeroNum() == 1)
					{
						auto heroTwo = this->getChildByTag(10005);
						heroTwo->setColor(Color3B::GRAY);
					}
					else if (m_manager->getHeroNum() == 0)
					{
						auto heroThree = this->getChildByTag(10006);
						heroThree->setColor(Color3B::GRAY);
					}
					if (m_manager->getHeroNum() >= 0)
					{
						auto _stand = CallFunc::create(CC_CALLBACK_0(AnimationScene::enemyStand, this));
						auto _move = CallFunc::create(CC_CALLBACK_0(AnimationScene::enemyMove, this));
						auto _round2 = CallFunc::create(CC_CALLBACK_0(AnimationScene::round2, this));
						auto _round3 = CallFunc::create(CC_CALLBACK_0(AnimationScene::round3, this));
						auto _round4 = CallFunc::create(CC_CALLBACK_0(AnimationScene::round4, this));
						auto _finalround = CallFunc::create(CC_CALLBACK_0(AnimationScene::finalround, this));
						auto _ready = CallFunc::create(CC_CALLBACK_0(AnimationScene::ready, this));
						auto _go = CallFunc::create(CC_CALLBACK_0(AnimationScene::go, this));
						auto _timeover = CallFunc::create(CC_CALLBACK_0(AnimationScene::timeover, this));
						auto _player2wins = CallFunc::create(CC_CALLBACK_0(AnimationScene::player2wins, this));
						auto _waiting = CallFunc::create(CC_CALLBACK_0(AnimationScene::waiting, this));
						auto _perfect = CallFunc::create(CC_CALLBACK_0(AnimationScene::perfect, this));
						//auto ko_wins = Sequence::create(_timeover, DelayTime::create(2.0f), _player2wins, DelayTime::create(2.0f), _waiting, NULL);
						Sequence* wins;
						if (m_enemy->getLife() == m_enemy->getMaxLife())
							wins = Sequence::create(_timeover, DelayTime::create(1.5f), _player2wins, DelayTime::create(1.5f), _perfect, DelayTime::create(1.0f), _waiting, NULL);
						else
							wins = Sequence::create(_timeover, DelayTime::create(2.0f), _player2wins, DelayTime::create(2.0f), _waiting, NULL);
						auto stand_spawn = Spawn::create(_stand, wins, NULL);

						if (m_manager->getHeroNum() > 0)
						{
							m_manager->updateRound();
							int roundNum = m_manager->getRoundNum();
							CallFunc* _round;
							if (roundNum == 2)
								_round = _round2;
							else if (roundNum == 3)
								_round = _round3;
							else if (roundNum == 4)
								_round = _round4;
							else if (roundNum == 5)
								_round = _finalround;
							auto gametips = Sequence::create(_round, DelayTime::create(1.5f), _ready, DelayTime::create(1.0f), _go, DelayTime::create(0.7f), NULL);
							auto move_spawn = Spawn::create(_move, gametips, NULL);
							auto sss = Sequence::create(
								stand_spawn, DelayTime::create(2.5f), move_spawn, DelayTime::create(3.5f),
								NULL
							);
							this->runAction(sss);
							//m_player->reLife();
						}
						else
						{
							auto ss = CallFunc::create(CC_CALLBACK_0(AnimationScene::backToResult, this));
							//auto a = Sequence::create(_timeover, DelayTime::create(1.0f), _player2wins, DelayTime::create(1.5f), NULL);
							Sequence* a;
							if (m_enemy->getLife() == m_enemy->getMaxLife())
								a = Sequence::create(_timeover, DelayTime::create(1.5f), _player2wins, DelayTime::create(1.5f), _perfect, DelayTime::create(1.0f), NULL);
							else
								a = Sequence::create(_timeover, DelayTime::create(2.0f), _player2wins, DelayTime::create(2.0f), NULL);

							auto stands_spawn = Spawn::create(_stand, a, NULL);
							auto sss = Sequence::create(
								stands_spawn, DelayTime::create(2.5f), ss,
								NULL
							);
							this->runAction(sss);
						}
						m_enemy->updateLife();
					}
				}
				else
				{
					m_manager->updateHeroNum();
					m_manager->updateEnemyNum();
					if (m_manager->getHeroNum() == 2)
					{
						auto heroOne = this->getChildByTag(10004);
						heroOne->setColor(Color3B::GRAY);
					}
					else if (m_manager->getHeroNum() == 1)
					{
						auto heroTwo = this->getChildByTag(10005);
						heroTwo->setColor(Color3B::GRAY);
					}
					else if (m_manager->getHeroNum() == 0)
					{
						auto heroThree = this->getChildByTag(10006);
						heroThree->setColor(Color3B::GRAY);
					}

					if (m_manager->getEnemyNum() == 2)
					{
						auto enemyOne = this->getChildByTag(10007);
						enemyOne->setColor(Color3B::GRAY);
					}
					else if (m_manager->getEnemyNum() == 1)
					{
						auto enemyTwo = this->getChildByTag(10008);
						enemyTwo->setColor(Color3B::GRAY);
					}
					if (m_manager->getEnemyNum() == 0)
					{
						auto enemyThree = this->getChildByTag(10009);
						enemyThree->setColor(Color3B::GRAY);
					}
					if (m_manager->getHeroNum() >= 0 && m_manager->getEnemyNum() >= 0)
					{
						auto _stand = CallFunc::create(CC_CALLBACK_0(AnimationScene::enemyStand, this));
						auto _move = CallFunc::create(CC_CALLBACK_0(AnimationScene::enemyMove, this));
						auto _round2 = CallFunc::create(CC_CALLBACK_0(AnimationScene::round2, this));
						auto _round3 = CallFunc::create(CC_CALLBACK_0(AnimationScene::round3, this));
						auto _round4 = CallFunc::create(CC_CALLBACK_0(AnimationScene::round4, this));
						auto _finalround = CallFunc::create(CC_CALLBACK_0(AnimationScene::finalround, this));
						auto _ready = CallFunc::create(CC_CALLBACK_0(AnimationScene::ready, this));
						auto _go = CallFunc::create(CC_CALLBACK_0(AnimationScene::go, this));
						auto _timeover = CallFunc::create(CC_CALLBACK_0(AnimationScene::timeover, this));
						auto _drawgame = CallFunc::create(CC_CALLBACK_0(AnimationScene::drawgame, this));
						auto _waiting = CallFunc::create(CC_CALLBACK_0(AnimationScene::waiting, this));
						auto ko_wins = Sequence::create(_timeover, DelayTime::create(2.0f), _drawgame, DelayTime::create(2.0f), _waiting, NULL);
						auto stand_spawn = Spawn::create(_stand, ko_wins, NULL);

						if (m_manager->getHeroNum() > 0 && m_manager->getEnemyNum() > 0)
						{
							m_manager->updateRound();
							int roundNum = m_manager->getRoundNum();
							CallFunc* _round;
							if (roundNum == 2)
								_round = _round2;
							else if (roundNum == 3)
								_round = _round3;
							else if (roundNum == 4)
								_round = _round4;
							else if (roundNum == 5)
								_round = _finalround;
							auto gametips = Sequence::create(_round, DelayTime::create(1.5f), _ready, DelayTime::create(1.0f), _go, DelayTime::create(0.7f), NULL);
							auto move_spawn = Spawn::create(_move, gametips, NULL);
							auto sss = Sequence::create(
								stand_spawn, DelayTime::create(2.5f), move_spawn, DelayTime::create(3.5f),
								NULL
							);
							this->runAction(sss);
						}
						else
						{
							auto ss = CallFunc::create(CC_CALLBACK_0(AnimationScene::backToResult, this));
							auto a = Sequence::create(_timeover, DelayTime::create(1.0f), _drawgame, DelayTime::create(1.5f), NULL);
							auto stands_spawn = Spawn::create(_stand, a, NULL);
							auto sss = Sequence::create(
								stands_spawn, DelayTime::create(2.5f), ss,
								NULL
							);
							this->runAction(sss);
						}
						m_enemy->updateLife();
					}
				}
			}
		}
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();
	
	if (m_player->isDeath()&&m_player->getIsReLife()==false&& m_manager->getHeroNum() > 0)
	{
		m_manager->updateHeroNum();
		if (m_manager->getHeroNum() == 2)
		{
			auto heroOne = this->getChildByTag(10004);
			heroOne->setColor(Color3B::GRAY);
		}
		else if (m_manager->getHeroNum() == 1)
		{
			auto heroTwo = this->getChildByTag(10005);
			heroTwo->setColor(Color3B::GRAY);
		}
		else if (m_manager->getHeroNum() == 0)
		{
			auto heroThree = this->getChildByTag(10006);
			heroThree->setColor(Color3B::GRAY);
		}
		if (m_manager->getHeroNum() >=0)
		{
			auto _stand = CallFunc::create(CC_CALLBACK_0(AnimationScene::enemyStand, this));
			auto _move = CallFunc::create(CC_CALLBACK_0(AnimationScene::enemyMove, this));
			auto _round2 = CallFunc::create(CC_CALLBACK_0(AnimationScene::round2, this));
			auto _round3 = CallFunc::create(CC_CALLBACK_0(AnimationScene::round3, this));
			auto _round4 = CallFunc::create(CC_CALLBACK_0(AnimationScene::round4, this));
			auto _finalround = CallFunc::create(CC_CALLBACK_0(AnimationScene::finalround, this));
			auto _ready = CallFunc::create(CC_CALLBACK_0(AnimationScene::ready, this));
			auto _go = CallFunc::create(CC_CALLBACK_0(AnimationScene::go, this));
			auto _ko = CallFunc::create(CC_CALLBACK_0(AnimationScene::ko, this));
			auto _player2wins = CallFunc::create(CC_CALLBACK_0(AnimationScene::player2wins, this));
			auto _waiting= CallFunc::create(CC_CALLBACK_0(AnimationScene::waiting, this));
			auto _perfect = CallFunc::create(CC_CALLBACK_0(AnimationScene::perfect, this));
			Sequence* wins;
			if (m_enemy->getLife() == m_enemy->getMaxLife())
			{
				wins = Sequence::create(_ko, DelayTime::create(1.5f), _player2wins, DelayTime::create(1.5f), _perfect, DelayTime::create(1.0f), _waiting, NULL);
			}
			else
				wins= Sequence::create(_ko, DelayTime::create(2.0f), _player2wins, DelayTime::create(2.0f), _waiting, NULL);
			auto stand_spawn = Spawn::create(_stand, wins, NULL);
			if (m_manager->getHeroNum() > 0)
			{
				m_manager->updateRound();
				int roundNum = m_manager->getRoundNum();
				CallFunc* _round;
				if (roundNum == 2)
					_round = _round2;
				else if (roundNum == 3)
					_round = _round3;
				else if (roundNum == 4)
					_round = _round4;
				else if (roundNum == 5)
					_round = _finalround;
				auto gametips = Sequence::create(_round, DelayTime::create(1.5f), _ready, DelayTime::create(1.0f), _go, DelayTime::create(0.7f), NULL);
				auto move_spawn = Spawn::create(_move, gametips, NULL);
				auto sss = Sequence::create(
					stand_spawn, DelayTime::create(2.5f), move_spawn, DelayTime::create(3.5f),
					NULL
				);
				m_player->setreLife();
				this->runAction(sss);
			}
			else
			{
				auto ss = CallFunc::create(CC_CALLBACK_0(AnimationScene::backToResult, this));
				Sequence* a;// = Sequence::create(_ko, DelayTime::create(2.0f), _player2wins, DelayTime::create(1.5f), NULL);
				if (m_enemy->getLife() == m_enemy->getMaxLife())
					a= Sequence::create(_ko, DelayTime::create(1.5f), _player2wins, DelayTime::create(1.5f), _perfect, DelayTime::create(1.0f), NULL);
				else
					a = Sequence::create(_ko, DelayTime::create(2.0f), _player2wins, DelayTime::create(2.0f),  NULL);
				auto stands_spawn = Spawn::create(_stand, a, NULL);
				auto sss = Sequence::create(
					stands_spawn, DelayTime::create(2.5f), ss,
					NULL
				);
				this->runAction(sss);
			}
			m_enemy->updateLife();
		}
		
	}
	else if (m_enemy->isDeath() && m_enemy->getIsReLife() == false && m_manager->getEnemyNum() > 0)
	{
		m_manager->updateEnemyNum();
		if (m_manager->getEnemyNum() == 2)
		{
			auto enemyOne = this->getChildByTag(10007);
			enemyOne->setColor(Color3B::GRAY);
		}
		else if (m_manager->getEnemyNum() == 1)
		{
			auto enemyTwo = this->getChildByTag(10008);
			enemyTwo->setColor(Color3B::GRAY);
		}
		if (m_manager->getEnemyNum() == 0)
		{
			auto enemyThree = this->getChildByTag(10009);
			enemyThree->setColor(Color3B::GRAY);
		}
		if (m_manager->getEnemyNum() >= 0)
		{
			
			auto _stand = CallFunc::create(CC_CALLBACK_0(AnimationScene::heroStand, this));
			auto _move = CallFunc::create(CC_CALLBACK_0(AnimationScene::heroMove, this));
			auto _round2 = CallFunc::create(CC_CALLBACK_0(AnimationScene::round2, this));
			auto _round3 = CallFunc::create(CC_CALLBACK_0(AnimationScene::round3, this));
			auto _round4 = CallFunc::create(CC_CALLBACK_0(AnimationScene::round4, this));
			auto _finalround = CallFunc::create(CC_CALLBACK_0(AnimationScene::finalround, this));
			auto _ready = CallFunc::create(CC_CALLBACK_0(AnimationScene::ready, this));
			auto _go = CallFunc::create(CC_CALLBACK_0(AnimationScene::go, this));
			auto _ko = CallFunc::create(CC_CALLBACK_0(AnimationScene::ko, this));
			auto _player1wins = CallFunc::create(CC_CALLBACK_0(AnimationScene::player1wins, this));
			auto _waiting = CallFunc::create(CC_CALLBACK_0(AnimationScene::waiting, this));
			auto _perfect = CallFunc::create(CC_CALLBACK_0(AnimationScene::perfect, this));
			Sequence* wins;
			if (m_player->getLife() == m_player->getMaxLife())
				wins = Sequence::create(_ko, DelayTime::create(1.5f), _player1wins, DelayTime::create(1.5f), _perfect, DelayTime::create(1.0f), _waiting, NULL);
			else
				wins = Sequence::create(_ko, DelayTime::create(2.0f), _player1wins, DelayTime::create(2.0f), _waiting, NULL);
			auto stand_spawn = Spawn::create(_stand, wins,NULL);
			if (m_manager->getEnemyNum() > 0)
			{
				m_manager->updateRound();
				int roundNum = m_manager->getRoundNum();
				CallFunc* _round;
				if (roundNum == 2)
					_round = _round2;
				else if (roundNum == 3)
					_round = _round3;
				else if (roundNum == 4)
					_round = _round4;
				else if (roundNum == 5)
					_round = _finalround;
				auto gametips = Sequence::create(_round, DelayTime::create(1.5f), _ready, DelayTime::create(1.0f), _go, DelayTime::create(0.7f), NULL);
				auto move_spawn = Spawn::create(_move, gametips, NULL);
				auto sss = Sequence::create(
					stand_spawn, DelayTime::create(2.5f), move_spawn, DelayTime::create(3.5f),
					NULL
				);
				m_enemy->setreLife();
				this->runAction(sss);
			}
			else
			{
				auto ss = CallFunc::create(CC_CALLBACK_0(AnimationScene::backToResult, this));
				Sequence* a;// = Sequence::create(_ko, DelayTime::create(2.0f), _player1wins, DelayTime::create(1.5f), NULL);
				if (m_player->getLife() == m_player->getMaxLife())
					a = Sequence::create(_ko, DelayTime::create(1.5f), _player1wins, DelayTime::create(1.5f), _perfect, DelayTime::create(1.0f), NULL);
				else
					a = Sequence::create(_ko, DelayTime::create(2.0f), _player1wins, DelayTime::create(2.0f), NULL); 
				auto stands_spawn = Spawn::create(_stand, a, NULL);
				auto sss = Sequence::create(
					stands_spawn, DelayTime::create(2.5f),ss,
					NULL
				);
				this->runAction(sss);
				
			}
			
			m_player->updateLife();
		}
	}
	
	auto cloud_1 = (Sprite*)getChildByTag(112);
	auto cloud_2 = (Sprite*)getChildByTag(113);
	if (cloud_1->getPositionX() > -(3 / 2 *cloud_1->getContentSize().width - visibleSize.width))
	{
		cloud_1->setPositionX(cloud_1->getPositionX() - 1);
	} else
	{
		cloud_1->setPositionX(visibleSize.width/2);
	}
	cloud_2->setPositionX(cloud_1->getPositionX() + cloud_1->getContentSize().width);

	// 更新血条
	float temp_blood_e = float(m_enemy->getLife()) / m_enemy->getMaxLife();
	float x = 44 + (354-88-44)*(1- temp_blood_e);
	m_bloodBar_e->setTextureRect(Rect(x,0,354 - x,m_bloodBar_e->getContentSize().height));
	m_bloodBar_e->setPositionX(visibleSize.width - m_bloodBar_e->getContentSize().width/2 - 5);

	float temp_blood_h = float(m_player->getLife()) / m_player->getMaxLife();
	m_bloodBar_h->setTextureRect(Rect(0,0,88+(354-88-44)* temp_blood_h,m_bloodBar_h->getContentSize().height));
	m_bloodBar_h->setPositionX(m_bloodBar_h->getContentSize().width/2 + 5);

	// 更新蓝条
	float temp_blue_h = float(m_player->getBlue()) / m_player->getMaxBlue();
	m_blueBar_h->setTextureRect(Rect(0, 0, 88 + (354 - 88 - 44)* temp_blue_h, m_blueBar_h->getContentSize().height));
	m_blueBar_h->setPositionX(m_blueBar_h->getContentSize().width / 2 + 5);

	// 控制角色移动
	if (m_joystick->getDirection() == Joystick_dir::_RIGHT)
	{
		m_player->play(MOVERIGHT);
	} else if (m_joystick->getDirection() == Joystick_dir::_LEFT)
	{
		m_player->play(MOVELEFT);
	} 
	else if (m_player->ishurt()&&!m_player->isAttack())
	{
		m_player->play(SMITTEN);
	}
	else if (m_joystick->getDirection() == Joystick_dir::_STOP)
	{
		m_player->play(STAND);
	}
}

void AnimationScene::enemyStand()
{
	m_enemy->play(STAND);
	m_aimanager->setIsGo(false);
}
void AnimationScene::enemyMove()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	m_enemy->setPosition(Vec2(visibleSize.width - 120, 150));
}


void AnimationScene::heroStand()
{
	m_player->play(STAND);
	m_aimanager->setIsGo(false);
}
void AnimationScene::heroMove()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	m_player->setPosition(Vec2(visibleSize.width / 5, 35));
}

void AnimationScene::attackCallback(Ref* pSender)
{
	if (!m_player->isAttack())
	{
		m_player->play(ATTACK);
	}
	else
	{
		m_player->play(STAND);
	}
		
}

void AnimationScene::defendCallback(Ref* pSender)
{
	if (!m_player->isAttack()) 
	    m_player->play(DEFEND);
}

void AnimationScene::round1()
{
	SimpleAudioEngine::sharedEngine()->playBackgroundMusic("exam3/AnimationScene/bgm/gamebgm.wav", true);
	SimpleAudioEngine::sharedEngine()->playEffect("exam3/AnimationScene/bgm/round1.wav");
	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto roundOne = Sprite::create("exam3/AnimationScene/animation/tips/round1.png");
	roundOne->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	roundOne->setScale(0.7);
	ScaleBy* ScaleBy = ScaleBy::create(0.7, 0.9);
	roundOne->setTag(10010);
	this->addChild(roundOne, 5);
	roundOne->runAction(ScaleBy);
	m_manager->resetSceneTime();
	m_manager->setTimeOver(false);
}
void AnimationScene::round2()
{
	SimpleAudioEngine::sharedEngine()->playBackgroundMusic("exam3/AnimationScene/bgm/gamebgm.wav", true);
	SimpleAudioEngine::sharedEngine()->playEffect("exam3/AnimationScene/bgm/round2.wav");
	this->removeChildByTag(10010);
	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto roundTwo = Sprite::create("exam3/AnimationScene/animation/tips/round2.png");
	roundTwo->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	roundTwo->setScale(0.7);
	ScaleBy* ScaleBy = ScaleBy::create(0.7, 0.9);
	roundTwo->setTag(10010);
	this->addChild(roundTwo, 5);
	roundTwo->runAction(ScaleBy);
	m_manager->resetSceneTime();
	m_manager->setTimeOver(false);
}
void AnimationScene::round3()
{
	SimpleAudioEngine::sharedEngine()->playBackgroundMusic("exam3/AnimationScene/bgm/gamebgm.wav", true);
	SimpleAudioEngine::sharedEngine()->playEffect("exam3/AnimationScene/bgm/round3.wav");
	this->removeChildByTag(10010);
	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto roundThree = Sprite::create("exam3/AnimationScene/animation/tips/round3.png");
	roundThree->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	roundThree->setScale(0.7);
	ScaleBy* ScaleBy = ScaleBy::create(0.7, 0.9);
	roundThree->setTag(10010);
	this->addChild(roundThree, 5);
	roundThree->runAction(ScaleBy);
	m_manager->resetSceneTime();
	m_manager->setTimeOver(false);
}
void AnimationScene::round4()
{
	SimpleAudioEngine::sharedEngine()->playBackgroundMusic("exam3/AnimationScene/bgm/gamebgm.wav", true);
	SimpleAudioEngine::sharedEngine()->playEffect("exam3/AnimationScene/bgm/round4.wav");
	this->removeChildByTag(10010);
	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto roundFour = Sprite::create("exam3/AnimationScene/animation/tips/round4.png");
	roundFour->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	roundFour->setScale(0.7);
	ScaleBy* ScaleBy = ScaleBy::create(0.7, 0.9);
	roundFour->setTag(10010);
	this->addChild(roundFour, 5);
	roundFour->runAction(ScaleBy);
	m_manager->resetSceneTime();
	m_manager->setTimeOver(false);
}
void AnimationScene::finalround()
{
	SimpleAudioEngine::sharedEngine()->playBackgroundMusic("exam3/AnimationScene/bgm/gamebgm.wav", true);
	SimpleAudioEngine::sharedEngine()->playEffect("exam3/AnimationScene/bgm/finalround.wav");
	this->removeChildByTag(10010);
	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto finalround = Sprite::create("exam3/AnimationScene/animation/tips/finalround.png");
	finalround->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	finalround->setScale(0.7);
	ScaleBy* ScaleBy = ScaleBy::create(0.7, 0.9);
	finalround->setTag(10010);
	this->addChild(finalround, 5);
	finalround->runAction(ScaleBy);
	m_manager->resetSceneTime();
	m_manager->setTimeOver(false);
}
void AnimationScene::ready()
{
	SimpleAudioEngine::sharedEngine()->playEffect("exam3/AnimationScene/bgm/ready.wav");
	this->removeChildByTag(10010);
	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto ready = Sprite::create("exam3/AnimationScene/animation/tips/ready.png");
	ready->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	ready->setScale(0.8);
	ScaleBy* ScaleBy = ScaleBy::create(0.6, 0.9);
	ready->setTag(10010);
	this->addChild(ready, 5);
	ready->runAction(ScaleBy);
}
void AnimationScene::go()
{
	SimpleAudioEngine::sharedEngine()->playEffect("exam3/AnimationScene/bgm/go.wav");
	this->removeChildByTag(10010);
	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto go = Sprite::create("exam3/AnimationScene/animation/tips/go.png");
	go->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	go->setScale(0.5);
	go->setTag(10010);
	this->addChild(go, 5);
	FiniteTimeAction*  action = CCSequence::create(
		ScaleBy::create(0.7, 0.9),
		CCRemoveSelf::create(),
		NULL);
	go->runAction(action);
	m_aimanager->setIsGo(true);
}
void AnimationScene::ko()
{
	SimpleAudioEngine::sharedEngine()->playEffect("exam3/AnimationScene/bgm/KO.wav");
	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto ko = Sprite::create("exam3/AnimationScene/animation/tips/ko.png");
    ko->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	ko->setScale(0.9);
	ScaleBy* ScaleBy = ScaleBy::create(0.6, 0.9);
	ko->setTag(10010);
	this->addChild(ko, 5);
	ko->runAction(ScaleBy);
}
void AnimationScene::timeover()
{
	SimpleAudioEngine::sharedEngine()->playEffect("exam3/AnimationScene/bgm/timeover.wav");
	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto timeover = Sprite::create("exam3/AnimationScene/animation/tips/timeover.png");
	timeover->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	timeover->setScale(0.7);
	ScaleBy* ScaleBy = ScaleBy::create(0.6, 0.8);
	timeover->setTag(10010);
	this->addChild(timeover, 5);
	timeover->runAction(ScaleBy);
	m_manager->setTimeOver(true);
}


void AnimationScene::perfect()
{
	SimpleAudioEngine::sharedEngine()->playEffect("exam3/AnimationScene/bgm/perfect.wav");
	this->removeChildByTag(10010);
	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto perfect = Sprite::create("exam3/AnimationScene/animation/tips/perfect.png");
	perfect->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	perfect->setScale(0.5);
	ScaleBy* ScaleBy = ScaleBy::create(0.8, 0.9);
	perfect->setTag(10010);
	this->addChild(perfect, 5);
	perfect->runAction(ScaleBy);
}
void AnimationScene::drawgame()
{
	SimpleAudioEngine::sharedEngine()->playEffect("exam3/AnimationScene/bgm/drawgame.wav");
	m_manager->updateTime();
	this->removeChildByTag(10010);
	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto drawgame = Sprite::create("exam3/AnimationScene/animation/tips/drawgame.png");
	drawgame->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	drawgame->setScale(0.7);
	ScaleBy* ScaleBy = ScaleBy::create(0.8, 0.8);
	drawgame->setTag(10010);
	this->addChild(drawgame, 5);
	drawgame->runAction(ScaleBy);
}
void AnimationScene::player1wins()
{
	SimpleAudioEngine::sharedEngine()->playEffect("exam3/AnimationScene/bgm/player1.wav");
	m_manager->updateTime();
	this->removeChildByTag(10010);
	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto player1win = Sprite::create("exam3/AnimationScene/animation/tips/player1win.png");
	player1win->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	player1win->setScale(0.5);
	ScaleBy* ScaleBy = ScaleBy::create(0.8, 0.9);
	player1win->setTag(10010);
	this->addChild(player1win, 5);
	player1win->runAction(ScaleBy);
}
void AnimationScene::player2wins()
{
	SimpleAudioEngine::sharedEngine()->playEffect("exam3/AnimationScene/bgm/player2.wav");
	m_manager->updateTime();
	this->removeChildByTag(10010);
	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto player2win = Sprite::create("exam3/AnimationScene/animation/tips/player2win.png");
	player2win->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	player2win->setScale(0.5);
	ScaleBy* ScaleBy = ScaleBy::create(0.8, 0.9);
	player2win->setTag(10010);
	this->addChild(player2win, 5);
	player2win->runAction(ScaleBy);
}
void AnimationScene::waiting()
{

	if (m_manager->isTimeOver())
	{
		m_player->reLife();
		m_enemy->reLife();
	}

	SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
	this->removeChildByTag(10010);
	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto waiting = Sprite::create("exam3/AnimationScene/animation/background/bg1.jpg");
	waiting->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	waiting->setScale(0.5);
	waiting->setTag(10010);
	this->addChild(waiting, 5);
}
void AnimationScene::backToResult()
{
	Scene * scene_1 = Scene::create();
	ResultScene* layers = ResultScene::create();
	layers->m_critNum_e = m_enemy->getCritNum();
	layers->m_attackedNum_e = m_enemy->getAttackedNum();
	layers->m_attackNum_e = m_enemy->getAttackNum();
	layers->m_sufferedDamage_e= m_enemy->getSufferedDamage();
	layers->m_critNum_h = m_player->getCritNum();
	layers->m_attackedNum_h = m_player->getAttackedNum();
	layers->m_attackNum_h = m_player->getAttackNum();
	layers->m_sufferedDamage_h = m_player->getSufferedDamage();
	//1 is player1 win,2 is player 2wins,0 is both lose
	if (m_manager->getHeroNum() != 0)
		layers->m_gameresult = 1;
	else if (m_manager->getEnemyNum() != 0)
		layers->m_gameresult = 2;
	else
		layers->m_gameresult = 0;
	layers->timecause = m_manager->getWholeTime();
	scene_1->addChild(layers);
	Director::getInstance()->replaceScene(TransitionCrossFade::create(1.0f, scene_1));
}
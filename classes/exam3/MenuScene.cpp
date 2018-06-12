#include "MenuScene.h"
#include "AnimationScene.h"
MenuScene::MenuScene()
{
	
}

MenuScene::~MenuScene()
{

}

bool MenuScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();

	// 背景
	auto background = Sprite::create("exam3/AnimationScene/animation/background/bg.jpg");
	background->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	background->setScale(0.5);
	background->setTag(110);
	this->addChild(background);

	// 开始按钮
	auto startItem = MenuItemImage::create(
		"exam3/AnimationScene/animation/control/start.png",
		"exam3/AnimationScene/animation/control/start.png",
		CC_CALLBACK_1(MenuScene::startCallback, this));

	startItem->setPosition(Vec2(visibleSize.width /2, 280));
	startItem->setScale(0.8);
	auto menu = Menu::create(startItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 5);

	// 设置按钮
	auto setItem = MenuItemImage::create(
		"exam3/AnimationScene/animation/control/settings.png",
		"exam3/AnimationScene/animation/control/settings.png",
		CC_CALLBACK_1(MenuScene::setCallback, this));

	setItem->setPosition(Vec2(visibleSize.width /2, 220));
	setItem->setScale(0.8);
	auto menu1 = Menu::create(setItem, NULL);
	menu1->setPosition(Vec2::ZERO);
	this->addChild(menu1, 5);

	// 退出按钮
	auto quitItem = MenuItemImage::create(
		"exam3/AnimationScene/animation/control/quit.png",
		"exam3/AnimationScene/animation/control/quit.png",
		CC_CALLBACK_1(MenuScene::quitCallback, this));

	quitItem->setPosition(Vec2(visibleSize.width /2, 160));
	quitItem->setScale(0.8);
	auto menu2 = Menu::create(quitItem, NULL);
	menu2->setPosition(Vec2::ZERO);
	this->addChild(menu2, 5);

	//背景音和音效加载
	auto audioengine = SimpleAudioEngine::sharedEngine();
	//bgm预加载为空实现
	//路径不能过长
	audioengine->preloadBackgroundMusic("exam3/AnimationScene/bgm/menubgm.wav");
	audioengine->preloadBackgroundMusic("exam3/AnimationScene/bgm/gamebgm.wav");
	audioengine->preloadEffect("exam3/AnimationScene/bgm/KO.wav");
	audioengine->preloadEffect("exam3/AnimationScene/bgm/go.wav");
	audioengine->preloadEffect("exam3/AnimationScene/bgm/finalround.wav");
	audioengine->preloadEffect("exam3/AnimationScene/bgm/congraduation.wav");
	audioengine->preloadEffect("exam3/AnimationScene/bgm/playerone.wav");
	audioengine->preloadEffect("exam3/AnimationScene/bgm/playertwo.wav");
	audioengine->preloadEffect("exam3/AnimationScene/bgm/ready.wav");
	audioengine->preloadEffect("exam3/AnimationScene/bgm/roud1.wav");
	audioengine->preloadEffect("exam3/AnimationScene/bgm/roud2.wav");
	audioengine->preloadEffect("exam3/AnimationScene/bgm/roud3.wav");
	audioengine->preloadEffect("exam3/AnimationScene/bgm/roud4.wav");
	audioengine->preloadEffect("exam3/AnimationScene/bgm/timeover.wav");
	audioengine->preloadEffect("exam3/AnimationScene/bgm/defend.wav");
	audioengine->preloadEffect("exam3/AnimationScene/bgm/knife.wav");
	audioengine->preloadEffect("exam3/AnimationScene/bgm/sword.wav");
	audioengine->playBackgroundMusic("exam3/AnimationScene/bgm/menubgm.wav",true);
	//log("%f", audioengine->getBackgroundMusicVolume());
	audioengine->setBackgroundMusicVolume(0.5);
	//log("%f", audioengine->getBackgroundMusicVolume());
	audioengine->setEffectsVolume(0.5);
	// update
	this->scheduleUpdate();

	return true;
}

Scene* MenuScene::createScene()
{
	auto scene = Scene::create();
	auto layer = MenuScene::create();
	scene->addChild(layer);
	return scene;
}

void MenuScene::update(float delta)
{
	//log("%f", SimpleAudioEngine::sharedEngine()->getBackgroundMusicVolume());
}


void MenuScene::startCallback(Ref* pSender)
{
	SimpleAudioEngine::getInstance()->stopBackgroundMusic(false);
	auto scene = AnimationScene::createScene();
	Director::getInstance()->pushScene(scene);
}

void MenuScene::quitCallback(Ref* pSender)
{

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
	return;
#endif

	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}
void MenuScene::setCallback(Ref* pSender)
{
	auto scene = SettingScene::createScene();
	Director::getInstance()->pushScene(scene);
}

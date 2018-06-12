#include "SettingScene.h"
SettingScene::SettingScene()
{
	
}

SettingScene::~SettingScene()
{

}

bool SettingScene::init()
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
	
	Dictionary* strings = Dictionary::createWithContentsOfFile("fonts/my.xml");
	const char *bgm_text = ((String*)strings->objectForKey("music"))->getCString();
	const char *eft_text = ((String*)strings->objectForKey("effect"))->getCString();

	auto tips_bgm = Label::createWithBMFont("fonts/u.fnt", bgm_text);
	tips_bgm->setPosition(Vec2(visibleSize.width * 1 / 2 - 40, 280));
	tips_bgm->setScale(0.8);
	this->addChild(tips_bgm, 5);

	auto tips_eft = Label::createWithBMFont("fonts/u.fnt", eft_text);
	tips_eft->setPosition(Vec2(visibleSize.width * 1 / 2 - 40, 220));
	tips_eft->setScale(0.8);
	this->addChild(tips_eft, 5);

	Sprite* bg = Sprite::create("exam3/AnimationScene/animation/control/mask.png");
	Sprite* off = Sprite::create("exam3/AnimationScene/animation/control/off.png");
	Sprite* on = Sprite::create("exam3/AnimationScene/animation/control/on.png");
	Sprite* thumb = Sprite::create("exam3/AnimationScene/animation/control/thumb.png");
	Label* labelon = Label::create("on", "Arial", 20);
	Label* labeloff = Label::create("off", "Arial", 20);

	Sprite* bg2 = Sprite::create("exam3/AnimationScene/animation/control/mask.png");
	Sprite* off2 = Sprite::create("exam3/AnimationScene/animation/control/off.png");
	Sprite* on2 = Sprite::create("exam3/AnimationScene/animation/control/on.png");
	Sprite* thumb2 = Sprite::create("exam3/AnimationScene/animation/control/thumb.png");
	Label* labelon2 = Label::create("on", "Arial", 20);
	Label* labeloff2 = Label::create("off", "Arial", 20);

	//创建和放置control switch
	controlSwitch1 = ControlSwitch::create(bg, on, off,thumb,labelon, labeloff);
	controlSwitch1->setPosition(Vec2(visibleSize.width *1/2+50, 280));
	controlSwitch1->setScale(1.1);
	this->addChild(controlSwitch1);
	
	controlSwitch2 = ControlSwitch::create(bg2, on2, off2, thumb2, labelon2, labeloff2);
	controlSwitch2->setPosition(Vec2(visibleSize.width *1/2+50, 220));
	controlSwitch2->setScale(1.1);
	this->addChild(controlSwitch2);

	//绑定事件
	controlSwitch1->addTargetWithActionForControlEvents(this, cccontrol_selector(SettingScene::switch1Changed), Control::EventType::VALUE_CHANGED);
	controlSwitch2->addTargetWithActionForControlEvents(this, cccontrol_selector(SettingScene::switch2Changed), Control::EventType::VALUE_CHANGED);	

	// 返回按钮
	auto quitItem = MenuItemImage::create(
		"exam3/AnimationScene/animation/control/quit.png",
		"exam3/AnimationScene/animation/control/quit.png",
		CC_CALLBACK_1(SettingScene::returnCallback, this));

	quitItem->setPosition(Vec2(visibleSize.width /2, 160));
	quitItem->setScale(0.8);
	auto Setting2 = Menu::create(quitItem, NULL);
	Setting2->setPosition(Vec2::ZERO);
	this->addChild(Setting2, 5);

	// update
	this->scheduleUpdate();

	return true;
}

Scene* SettingScene::createScene()
{
	auto scene = Scene::create();
	auto layer = SettingScene::create();
	scene->addChild(layer);
	return scene;
}

void SettingScene::update(float delta)
{
}


void SettingScene::returnCallback(Ref* pSender)
{
	auto scene = MenuScene::createScene();
	Director::getInstance()->pushScene(scene);
}

void SettingScene::switch1Changed(Ref* psender , Control::EventType controlEvent)
{
	//获取事件的传递者ControlSwitch
	ControlSwitch* controlSwitch = (ControlSwitch*)psender;
	//根据开关控件的状态
	if (controlSwitch->isOn())
	{
		SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(1.0f);
	}
	else
	{
		SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.0f);
	}
}
void SettingScene::switch2Changed(Ref* psender, Control::EventType controlEvent)
{
	//获取事件的传递者ControlSwitch
	ControlSwitch* controlSwitch = (ControlSwitch*)psender;

	//根据开关控件的状态
	if (controlSwitch->isOn())
	{
		SimpleAudioEngine::sharedEngine()->setEffectsVolume(1.0f);
	}
	else
	{
		SimpleAudioEngine::sharedEngine()->setEffectsVolume(0.0f);
	}
}
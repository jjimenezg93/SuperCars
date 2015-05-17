/*
 * RankingMenuScene.cpp
 *
 *  Created on: 09/05/2015
 *      Author: jjimenezg93
 */

#include "RankingMenuScene.h"
#include "MainMenuScene.h"

USING_NS_CC;

Scene* RankingMenu::createScene() {
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = RankingMenu::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

bool RankingMenu::init() {
	// 1. super init first
	if (!Layer::init()) {
		return false;
	}

	origin = Director::getInstance()->getVisibleOrigin();
	visibleSize = Director::getInstance()->getVisibleSize();

	_background = Sprite::create("menuBackground.png");
	_background->setAnchorPoint(Vec2(0.5,0));
	_background->setPosition(origin.x, origin.y);
	this->addChild(_background);

	createMenu();

	return true;
}

void RankingMenu::createMenu() {
	auto fastestLapLabel = Label::createWithTTF("Fastest lap", "fonts/squares_bold.ttf", 36);
	fastestLapLabel->setAnchorPoint(Vec2(0.5, 0.5));
	fastestLapLabel->setPosition(
			Vec2(origin.x + visibleSize.width/2,
					origin.y + visibleSize.height/2));

	this->addChild(fastestLapLabel);

	auto fastestLapName = Label::createWithTTF("", "fonts/squares_bold.ttf", 32);
	fastestLapName->setAnchorPoint(Vec2(0, 0.5));
	fastestLapName->setPosition(
			Vec2(origin.x + visibleSize.width/4,
					origin.y + visibleSize.height/2 - fastestLapLabel->getContentSize().height));

	char fastestName [50];
	sprintf(fastestName, "%s", UserDefault::getInstance()->getStringForKey("playerName").c_str());
	std::string fastestLabelName (fastestName);

	fastestLapName->setString(fastestLabelName.c_str());
	CCLog("name ranking: %s", UserDefault::getInstance()->getStringForKey("playerName").c_str());

	this->addChild(fastestLapName);

	auto fastestLapValue = Label::createWithTTF("", "fonts/squares_bold.ttf", 32);
	fastestLapValue->setAnchorPoint(Vec2(0, 0.5));
	fastestLapValue->setPosition(
			Vec2(origin.x + visibleSize.width/2,
					origin.y + visibleSize.height/2 - fastestLapLabel->getContentSize().height));

	this->addChild(fastestLapValue);

	char fastestText [50];
	sprintf(fastestText, "%.2f s", UserDefault::getInstance()->getFloatForKey("fastestLap"));
	std::string fastestLabelText (fastestText);

	fastestLapValue->setString(fastestLabelText);


	auto resetButton = MenuItemImage::create("reset_button.png",
				"reset_button_pressed.png", CC_CALLBACK_1(RankingMenu::resetRanking, this));

	resetButton->setAnchorPoint(Vec2(1,0));
	resetButton->setPosition(Vec2(origin.x + visibleSize.width,
						origin.y));

	auto backButton = MenuItemImage::create("back_button.png",
				"back_button_pressed.png", CC_CALLBACK_1(RankingMenu::backMainMenu, this));

	backButton->setAnchorPoint(Vec2(0,0));
	backButton->setPosition(Vec2(origin.x,
						origin.y));

	auto menu = Menu::create(resetButton, backButton, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 10);
}

void RankingMenu::resetRanking(Ref* pSender) {
	UserDefault::getInstance()->setFloatForKey("fastestLap", 0);
	auto rankingMenuScene = RankingMenu::createScene();
	Director::getInstance()->replaceScene(rankingMenuScene);
}

void RankingMenu::backMainMenu(Ref* pSender) {
	auto mainMenuScene = MainMenu::createScene();
	Director::getInstance()->replaceScene(mainMenuScene);
}

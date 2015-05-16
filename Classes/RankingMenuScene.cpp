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
	auto fastestLapLabel = Label::createWithTTF("Fastest lap", "fonts/squares_bold.ttf", 32);
	fastestLapLabel->setAnchorPoint(Vec2(0.5, 0.5));
	fastestLapLabel->setPosition(
			Vec2(origin.x + visibleSize.width/2,
					origin.y + visibleSize.height/2));

	this->addChild(fastestLapLabel);

	auto fastestLapValue = Label::createWithTTF("", "fonts/squares_bold.ttf", 32);
	fastestLapValue->setAnchorPoint(Vec2(0.5, 0.5));
	fastestLapValue->setPosition(
			Vec2(origin.x + visibleSize.width/2,
					origin.y + visibleSize.height/2 - fastestLapLabel->getContentSize().height));

	this->addChild(fastestLapValue);

	char fastestText [50];
	sprintf(fastestText, "%.2f s", UserDefault::getInstance()->getFloatForKey("fastestLap"));
	std::string fastestLabelText (fastestText);

	fastestLapValue->setString(fastestLabelText);

	auto backButton = MenuItemImage::create("back_button.png",
				"back_button_pressed.png", CC_CALLBACK_1(RankingMenu::backMainMenu, this));

	backButton->setAnchorPoint(Vec2(0,0));
	backButton->setPosition(Vec2(origin.x,
						origin.y));

	auto menu = Menu::create(backButton, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 10);

	/********** THIS IS TO CHECK FASTEST LAPS WORK ***************/
	UserDefault::getInstance()->setFloatForKey("fastestLap", 10);
}

void RankingMenu::backMainMenu(Ref* pSender) {
	auto mainMenuScene = MainMenu::createScene();
	Director::getInstance()->replaceScene(mainMenuScene);
}

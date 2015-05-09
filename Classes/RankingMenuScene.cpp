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
	auto backButton = MenuItemImage::create("back_button.png",
				"back_button_pressed.png", CC_CALLBACK_1(RankingMenu::backMainMenu, this));

	backButton->setAnchorPoint(Vec2(0,0));
	backButton->setPosition(Vec2(origin.x,
						origin.y));

	auto menu = Menu::create(backButton, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 10);
}

void RankingMenu::backMainMenu(Ref* pSender) {
	auto mainMenuScene = MainMenu::createScene();
	Director::getInstance()->replaceScene(mainMenuScene);
}

/*
 * EndRaceScene.cpp
 *
 *  Created on: 30/04/2015
 *      Author: jjimenezg93
 */

#include "MainMenuScene.h"
#include "EndRaceScene.h"
#include <stdio.h>

USING_NS_CC;

Scene* EndRace::createScene() {
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = EndRace::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

bool EndRace::init() {
	// 1. super init first
	if (!Layer::init()) {
		return false;
	}

	origin = Director::getInstance()->getVisibleOrigin();
	visibleSize = Director::getInstance()->getVisibleSize();

	auto background = Sprite::create("menuBackground.png");
	background->setAnchorPoint(Vec2(0.5,0));
	background->setPosition(origin.x, origin.y);
	this->addChild(background);
	/****** DEBUG
	auto gameTitleLabel = Label::createWithTTF("EndRace", "fonts/squares_bold.ttf", 34);
		gameTitleLabel->setAnchorPoint(Vec2(1,1));
		gameTitleLabel->setPosition(Vec2(origin.x + visibleSize.width / 2,
									origin.y + visibleSize.height - gameTitleLabel->getContentSize().height));
		this->addChild(gameTitleLabel);
	******/
	auto backButton = MenuItemImage::create("back_button.png",
			"back_button_pressed.png", CC_CALLBACK_1(EndRace::backMainMenu, this));

	backButton->setAnchorPoint(Vec2(0,0));
	backButton->setPosition(Vec2(origin.x,
					origin.y));

	auto menu = Menu::create(backButton, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 10);

	return true;
}

void EndRace::backMainMenu(Ref* pSender) {
	auto mainMenuScene = MainMenu::createScene();
	Director::getInstance()->replaceScene(mainMenuScene);
}

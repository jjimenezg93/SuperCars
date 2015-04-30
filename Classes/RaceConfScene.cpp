/*
 * MainMenuScene.cpp
 *
 *  Created on: 26/04/2015
 *      Author: jjimenezg93
 */

#include "RaceScene.h"
#include "MainMenuScene.h"
#include "RaceConfScene.h"
#include <stdio.h>

USING_NS_CC;

Scene* RaceConf::createScene() {
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = RaceConf::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

bool RaceConf::init() {
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

	auto startRaceButton = MenuItemImage::create("startRace_button.png",
			"startRace_button_pressed.png", CC_CALLBACK_1(RaceConf::startRace, this));

	startRaceButton->setAnchorPoint(Vec2(0.5,0.5));
	startRaceButton->setPosition(Vec2(origin.x + visibleSize.width/2,
			origin.y + visibleSize.height/2));

	auto backMainMenuButton = MenuItemImage::create("back_button.png",
				"back_button_pressed.png", CC_CALLBACK_1(RaceConf::backMainMenu, this));

	backMainMenuButton->setAnchorPoint(Vec2(0,0));
	backMainMenuButton->setPosition(Vec2(origin.x,
						origin.y));

	auto menu = Menu::create(startRaceButton, backMainMenuButton, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 10);

	return true;
}

void RaceConf::startRace(Ref* pSender) {
	auto raceScene = Race::createScene();
	Director::getInstance()->replaceScene(raceScene);
}

void RaceConf::backMainMenu(Ref* pSender) {
	auto mainMenuScene = MainMenu::createScene();
	Director::getInstance()->replaceScene(mainMenuScene);
}

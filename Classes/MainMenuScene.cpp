/*
 * MainMenuScene.cpp
 *
 *  Created on: 26/04/2015
 *      Author: jjimenezg93
 */

#include "RaceScene.h"
#include "MainMenuScene.h"
#include <stdio.h>

USING_NS_CC;

Scene* MainMenu::createScene() {
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = MainMenu::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

bool MainMenu::init() {
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

	auto playButton = MenuItemImage::create("button_play_game.png",
			"play_pressed.png", CC_CALLBACK_1(MainMenu::startGame, this));

	playButton->setAnchorPoint(Vec2(0.5,0.5));
	playButton->setPosition(Vec2(origin.x + visibleSize.width/2,
					origin.y + visibleSize.height/2));

	auto menu = Menu::create(playButton, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 10);

	return true;
}

void MainMenu::startGame(Ref* pSender) {
	auto raceScene = Race::createScene();
	Director::getInstance()->runWithScene(raceScene);
}

void MainMenu::menuCloseCallback(Ref* pSender) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
	return;
#endif

	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

/*
 * MainMenuScene.cpp
 *
 *  Created on: 26/04/2015
 *      Author: jjimenezg93
 */

#include "RaceConfScene.h"
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

	auto title = Sprite::create("SuperCars-title.png");
	title->setAnchorPoint(Vec2(0.5,1));
	title->setPosition(Vec2(origin.x + visibleSize.width / 2,
			origin.y + visibleSize.height - (title->getContentSize().height/2)));
	this->addChild(title);

	auto playButton = MenuItemImage::create("play_button.png",
			"play_button_pressed.png", CC_CALLBACK_1(MainMenu::playGame, this));

	playButton->setAnchorPoint(Vec2(0.5,0.5));
	playButton->setPosition(Vec2(origin.x + visibleSize.width/2,
					origin.y + visibleSize.height/2));

	auto rankingButton = MenuItemImage::create("ranking_button.png",
				"ranking_button_pressed.png", CC_CALLBACK_1(MainMenu::showRanking, this));

		rankingButton->setAnchorPoint(Vec2(0.5,0.5));
		rankingButton->setPosition(Vec2(origin.x + visibleSize.width/2,
						origin.y + visibleSize.height/2 - playButton->getContentSize().height));

	auto exitButton = MenuItemImage::create("exit_button.png",
				"exit_button_pressed.png", CC_CALLBACK_1(MainMenu::exitGame, this));

		exitButton->setAnchorPoint(Vec2(0,0));
		exitButton->setPosition(Vec2(origin.x,
						origin.y));

	auto menu = Menu::create(playButton, rankingButton, exitButton, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 10);

	UserDefault::getInstance()->setFloatForKey("speed",12);

	//CCLog("Speed SET TO: %f", UserDefault::getInstance()->getFloatForKey("speed",12));

	return true;
}

void MainMenu::playGame(Ref* pSender) {
	auto raceConfScene = RaceConf::createScene();
	Director::getInstance()->replaceScene(raceConfScene);
}

void MainMenu::showRanking(Ref* pSender) {

}

void MainMenu::exitGame(Ref* pSender) {
	Director::getInstance()->end();
}

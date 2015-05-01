/*
 * RaceMenuScene.cpp
 *
 *  Created on: 30/04/2015
 *      Author: jjimenezg93
 */

#include "MainMenuScene.h"
#include "RaceScene.h"
#include "RaceMenuScene.h"
#include <stdio.h>

USING_NS_CC;

Scene* RaceMenu::createScene() {
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = RaceMenu::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

bool RaceMenu::init() {
	// 1. super init first
	if (!Layer::init()) {
		return false;
	}

	origin = Director::getInstance()->getVisibleOrigin();
	visibleSize = Director::getInstance()->getVisibleSize();

	_background = Sprite::create("menuBackground.png");
	_background->setAnchorPoint(Vec2(0.5, 0));
	_background->setPosition(origin.x, origin.y);
	this->addChild(_background);

	createMenu();

	return true;
}

void RaceMenu::createMenu() {

	auto resumeButton = MenuItemImage::create("resume_button.png",
			"resume_button_pressed.png",
			CC_CALLBACK_1(RaceMenu::resumeRace, this));

	resumeButton->setAnchorPoint(Vec2(0.5, 0.5));
	resumeButton->setPosition(
			Vec2(origin.x + visibleSize.width / 2,
					origin.y + visibleSize.height / 2));

	auto backButton = MenuItemImage::create("exit_button.png",
			"exit_button_pressed.png", CC_CALLBACK_1(RaceMenu::quitRace, this));

	backButton->setAnchorPoint(Vec2(0, 0));
	backButton->setPosition(Vec2(origin.x, origin.y));

	auto menu = Menu::create(resumeButton, backButton, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 10);
}

void RaceMenu::resumeRace(Ref* pSender) {
	Director::getInstance()->popScene();
	Director::getInstance()->resume();
}

void RaceMenu::quitRace(Ref* pSender) {
	Director::getInstance()->resume();
	Director::getInstance()->popScene();
	auto mainMenuScene = MainMenu::createScene();
	Director::getInstance()->replaceScene(mainMenuScene);
}

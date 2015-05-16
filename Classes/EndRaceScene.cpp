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

	_background = Sprite::create("menuBackground.png");
	_background->setAnchorPoint(Vec2(0.5, 0));
	_background->setPosition(origin.x, origin.y);
	this->addChild(_background);

	createMenu();

	return true;
}

void EndRace::createMenu() {

	/*** RACE FASTEST LAP ***/
	auto raceFastestLapLabel = Label::createWithTTF("Race fastest lap", "fonts/squares_bold.ttf", 32);
	raceFastestLapLabel->setAnchorPoint(Vec2(0.5, 0.5));
	raceFastestLapLabel->setPosition(
			Vec2(origin.x + visibleSize.width/2,
					origin.y + visibleSize.height/2));

	this->addChild(raceFastestLapLabel);

	auto raceFastestLapValue = Label::createWithTTF("", "fonts/squares_bold.ttf", 32);
	raceFastestLapValue->setAnchorPoint(Vec2(0.5, 0.5));
	raceFastestLapValue->setPosition(
			Vec2(origin.x + visibleSize.width/2,
					origin.y + visibleSize.height/2 - raceFastestLapLabel->getContentSize().height));

	this->addChild(raceFastestLapValue);

	char raceFastestText [50];
	sprintf(raceFastestText, "%.2f s", UserDefault::getInstance()->getFloatForKey("raceFastestLap"));
	std::string raceFastestLabelText (raceFastestText);

	raceFastestLapValue->setString(raceFastestLabelText);

	/*** TOTAL FASTEST LAP ***/

	auto fastestLapLabel = Label::createWithTTF("Fastest lap", "fonts/squares_bold.ttf", 32);
	fastestLapLabel->setAnchorPoint(Vec2(0.5, 0.5));
	fastestLapLabel->setPosition(
			Vec2(origin.x + visibleSize.width/2,
					origin.y + visibleSize.height/4));

	this->addChild(fastestLapLabel);

	auto fastestLapValue = Label::createWithTTF("", "fonts/squares_bold.ttf", 32);
	fastestLapValue->setAnchorPoint(Vec2(0.5, 0.5));
	fastestLapValue->setPosition(
			Vec2(origin.x + visibleSize.width/2,
					origin.y + visibleSize.height/4 - fastestLapLabel->getContentSize().height));

	this->addChild(fastestLapValue);

	char fastestText [50];
	sprintf(fastestText, "%.2f s", UserDefault::getInstance()->getFloatForKey("fastestLap"));
	std::string fastestLabelText (fastestText);

	fastestLapValue->setString(fastestLabelText);


	auto backButton = MenuItemImage::create("back_button.png",
			"back_button_pressed.png",
			CC_CALLBACK_1(EndRace::backMainMenu, this));

	backButton->setAnchorPoint(Vec2(0, 0));
	backButton->setPosition(Vec2(origin.x, origin.y));

	auto menu = Menu::create(backButton, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 10);
}

void EndRace::backMainMenu(Ref* pSender) {
	auto mainMenuScene = MainMenu::createScene();
	Director::getInstance()->replaceScene(mainMenuScene);
}

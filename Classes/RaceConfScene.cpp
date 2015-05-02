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

using namespace cocos2d;
using namespace cocos2d::extension;
using namespace ui;

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

	_background = Sprite::create("menuBackground.png");
	_background->setAnchorPoint(Vec2(0.5, 0));
	_background->setPosition(origin.x, origin.y);
	this->addChild(_background);

	createMenu();
	createConfMenu();

	UserDefault::getInstance()->setFloatForKey("speed", 12);

	//CCLog("Speed SET TO: %f", UserDefault::getInstance()->getFloatForKey("speed",12));

	return true;
}

void RaceConf::createConfMenu() {
	_margin = 10; // margin to use between labels and widgets

	// editBoxSize is used for the size of labels and UI widgets
	Size editBoxSize = Size(250, 60);

	/****	NAME TEXT LABEL	****/
	nameLabel = Label::createWithTTF("Name", "fonts/squares_bold.ttf", 32);
	nameLabel->setAnchorPoint(Vec2(1, 0.5));
	nameLabel->enableOutline(Color4B::BLACK, 2);
	nameLabel->setPosition(
			Vec2(origin.x + editBoxSize.width,
					origin.y + visibleSize.height - editBoxSize.height * 2));
	this->addChild(nameLabel);

	/****	NAME INPUT FIELD	****/
	Scale9Sprite* pNormal = Scale9Sprite::create("input_field.png");
	_playerName = EditBox::create(editBoxSize, pNormal);
	_playerName->setAnchorPoint(Vec2(0, 0.5));
	_playerName->setPosition(
			Vec2(origin.x + editBoxSize.width + _margin,
					origin.y + visibleSize.height - editBoxSize.height * 2));
	_playerName->setFont("fonts/squares_bold.ttf", 22);
	_playerName->setFontColor(Color3B::WHITE);
	_playerName->setMaxLength(10);
	_playerName->setPlaceHolder("Name");

	this->addChild(_playerName);

	/****	DIFFICULTY TEXT LABEL	****/
	difficultyLabel = Label::createWithTTF("Difficulty",
			"fonts/squares_bold.ttf", 28);
	difficultyLabel->setAnchorPoint(Vec2(1, 0.5));
	difficultyLabel->enableOutline(Color4B::BLACK, 2);
	difficultyLabel->setPosition(
			Vec2(origin.x + editBoxSize.width,
					origin.y + visibleSize.height - editBoxSize.height * 4));
	this->addChild(difficultyLabel);

	_difficultySlider = ControlSlider::create("loadBar.png",
			"loadBarProgress.png", "loadBall.png", "loadBallPressed.png");
	_difficultySlider->setAnchorPoint(Vec2(1, 0.5));
	_difficultySlider->setPosition(
			Vec2(origin.x + editBoxSize.width * 2,
					origin.y + visibleSize.height - editBoxSize.height * 4));

	_difficultySlider->setMinimumValue(1.f);
	_difficultySlider->setMaximumValue(3.f);
	this->schedule(schedule_selector(RaceConf::sliderUpdate), (float) 0);
	this->addChild(_difficultySlider);

	_tempLabel = Label::createWithTTF("",
			"fonts/squares_bold.ttf", 28);
	_tempLabel->setAnchorPoint(Vec2(0, 0));
	_tempLabel->enableOutline(Color4B::BLACK, 2);
	_tempLabel->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + 140));

	this->addChild(_tempLabel);



	/****	OPPONENTS TEXT LABEL	****/
	opponentsLabel = Label::createWithTTF("Rivals", "fonts/squares_bold.ttf",
			32);
	opponentsLabel->setAnchorPoint(Vec2(1, 0.5));
	opponentsLabel->enableOutline(Color4B::BLACK, 2);
	opponentsLabel->setPosition(
			Vec2(origin.x + editBoxSize.width,
					origin.y + visibleSize.height - editBoxSize.height * 6));
	this->addChild(opponentsLabel);

	/****	LAPS TEXT LABEL	****/
	lapsLabel = Label::createWithTTF("Laps", "fonts/squares_bold.ttf", 32);
	lapsLabel->setAnchorPoint(Vec2(1, 0.5));
	lapsLabel->enableOutline(Color4B::BLACK, 2);
	lapsLabel->setPosition(
			Vec2(origin.x + editBoxSize.width,
					origin.y + visibleSize.height - editBoxSize.height * 8));
	this->addChild(lapsLabel);
}

void RaceConf::sliderUpdate(float delta) {
	char diff [50];
	_difficultySlider->setValue(round(_difficultySlider->getValue()));
	sprintf(diff, "diff = %f", _difficultySlider->getValue());
	std::string diffLabel (diff);

	_tempLabel->setString(diffLabel);
}

void RaceConf::createMenu() {
	auto startRaceButton = MenuItemImage::create("startRace_button.png",
			"startRace_button_pressed.png",
			CC_CALLBACK_1(RaceConf::startRace, this));

	startRaceButton->setAnchorPoint(Vec2(1, 0));
	startRaceButton->setPosition(Vec2(origin.x + visibleSize.width, origin.y));

	auto backMainMenuButton = MenuItemImage::create("back_button.png",
			"back_button_pressed.png",
			CC_CALLBACK_1(RaceConf::backMainMenu, this));

	backMainMenuButton->setAnchorPoint(Vec2(0, 0));
	backMainMenuButton->setPosition(Vec2(origin.x, origin.y));

	auto menu = Menu::create(startRaceButton, backMainMenuButton, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 10);
}

void RaceConf::startRace(Ref* pSender) {
	UserDefault::getInstance()->setStringForKey("playerName",
			_playerName->getText());
	UserDefault::getInstance()->setFloatForKey("difficulty", _difficultySlider->getValue());
	auto raceScene = Race::createScene();
	Director::getInstance()->replaceScene(raceScene);
}

void RaceConf::backMainMenu(Ref* pSender) {
	auto mainMenuScene = MainMenu::createScene();
	Director::getInstance()->replaceScene(mainMenuScene);
}

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


	_carFiles = {"audi_r8.png", "audi_r8_black.png", "gallardo.png"};
	_carFilesPos = 0;

	origin = Director::getInstance()->getVisibleOrigin();
	visibleSize = Director::getInstance()->getVisibleSize();

	_background = Sprite::create("menuBackground.png");
	_background->setAnchorPoint(Vec2(0.5, 0));
	_background->setPosition(origin.x, origin.y);
	this->addChild(_background);

	createMenu();
	createConfMenu();

	return true;
}

void RaceConf::createConfMenu() {
	_margin = 10; // margin to use between labels and widgets

	// editBoxSize is used for the size of labels and UI widgets
	Size editBoxSize = Size(250, 60);

	/**********		NAME	**********/
	nameLabel = Label::createWithTTF("Name", "fonts/squares_bold.ttf", 32);
	nameLabel->setAnchorPoint(Vec2(1, 0.5));
	nameLabel->enableOutline(Color4B::BLACK, 2);
	nameLabel->setPosition(
			Vec2(origin.x + editBoxSize.width,
					origin.y + visibleSize.height - editBoxSize.height * 2));
	this->addChild(nameLabel);

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


	/**********		DIFFICULTY	**********/
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
	_difficultySlider->setMaximumValue(4.f);
	this->schedule(schedule_selector(RaceConf::diffUpdate), 0.f);
	this->addChild(_difficultySlider);

	_difficultyValue = Label::createWithTTF("",
			"fonts/squares_bold.ttf", 28);
	_difficultyValue->setAnchorPoint(Vec2(0, 0.5));
	_difficultyValue->enableOutline(Color4B::BLACK, 2);
	_difficultyValue->setPosition(_difficultySlider->getPosition() + Vec2(_margin, 0));

	this->addChild(_difficultyValue);


	/**********		OPPONENTS	**********/
	opponentsLabel = Label::createWithTTF("Rivals", "fonts/squares_bold.ttf",
			32);
	opponentsLabel->setAnchorPoint(Vec2(1, 0.5));
	opponentsLabel->enableOutline(Color4B::BLACK, 2);
	opponentsLabel->setPosition(
			Vec2(origin.x + editBoxSize.width,
					origin.y + visibleSize.height - editBoxSize.height * 6));
	this->addChild(opponentsLabel);

	_opponentsSlider = ControlSlider::create("loadBar.png",
			"loadBarProgress.png", "loadBall.png", "loadBallPressed.png");
	_opponentsSlider->setAnchorPoint(Vec2(1, 0.5));
	_opponentsSlider->setPosition(
			Vec2(origin.x + editBoxSize.width * 2,
					origin.y + visibleSize.height - editBoxSize.height * 6));

	_opponentsSlider->setMinimumValue(0.f);
	_opponentsSlider->setMaximumValue(5.f);
	this->schedule(schedule_selector(RaceConf::oppUpdate), 0.f);
	this->addChild(_opponentsSlider);

	_opponentsValue = Label::createWithTTF("",
			"fonts/squares_bold.ttf", 28);
	_opponentsValue->setAnchorPoint(Vec2(0, 0.5));
	_opponentsValue->enableOutline(Color4B::BLACK, 2);
	_opponentsValue->setPosition(_opponentsSlider->getPosition() + Vec2(_margin, 0));

	this->addChild(_opponentsValue);


	/**********		LAPS	**********/
	lapsLabel = Label::createWithTTF("Laps", "fonts/squares_bold.ttf", 32);
	lapsLabel->setAnchorPoint(Vec2(1, 0.5));
	lapsLabel->enableOutline(Color4B::BLACK, 2);
	lapsLabel->setPosition(
			Vec2(origin.x + editBoxSize.width,
					origin.y + visibleSize.height - editBoxSize.height * 8));
	this->addChild(lapsLabel);

	_lapsSlider = ControlSlider::create("loadBar.png",
			"loadBarProgress.png", "loadBall.png", "loadBallPressed.png");
	_lapsSlider->setAnchorPoint(Vec2(1, 0.5));
	_lapsSlider->setPosition(
			Vec2(origin.x + editBoxSize.width * 2,
					origin.y + visibleSize.height - editBoxSize.height * 8));

	_lapsSlider->setMinimumValue(1.f);
	_lapsSlider->setMaximumValue(10.f);
	this->schedule(schedule_selector(RaceConf::lapsUpdate), 0.f);
	this->addChild(_lapsSlider);

	_lapsValue = Label::createWithTTF("",
			"fonts/squares_bold.ttf", 28);
	_lapsValue->setAnchorPoint(Vec2(0, 0.5));
	_lapsValue->enableOutline(Color4B::BLACK, 2);
	_lapsValue->setPosition(_lapsSlider->getPosition() + Vec2(_margin, 0));

	this->addChild(_lapsValue);

	/***	CAR SELECTION	***/
	_carsLabel = Label::createWithTTF("Car", "fonts/squares_bold.ttf", 32);
	_carsLabel->setAnchorPoint(Vec2(0.5, 0.5));
	_carsLabel->enableOutline(Color4B::BLACK, 2);
	_carsLabel->setPosition(Vec2(origin.x + visibleSize.width/2,
					origin.y + visibleSize.height - editBoxSize.height * 10));
	this->addChild(_carsLabel);

	_car = Sprite::create(_carFiles.at(_carFilesPos));
	_car->setAnchorPoint(Vec2(0.5, 0.5));
	_car->setPosition(Vec2(origin.x + visibleSize.width/2,
			origin.y + visibleSize.height - editBoxSize.height * 12));
	_car->setRotation(-90);

	this->addChild(_car);

	/*** LEFT ARROW ***/
	leftArrow = Sprite::create("left_arrow.png");
	leftArrow->setAnchorPoint(Vec2(0.5, 0.5));

	leftArrow->setPosition(Vec2(origin.x + visibleSize.width/2 - _car->getContentSize().height,
			origin.y + visibleSize.height - editBoxSize.height * 12));

	this->addChild(leftArrow, 100);

	/*** RIGHT ARROW ***/
	rightArrow = Sprite::create("right_arrow.png");
	rightArrow->setAnchorPoint(Vec2(0.5, 0.5));

	rightArrow->setPosition(Vec2(origin.x + visibleSize.width/2 + _car->getContentSize().height,
			origin.y + visibleSize.height - editBoxSize.height * 12));

	this->addChild(rightArrow, 100);

	leftArrow->setTag(1);
	rightArrow->setTag(2);

	/*** CONTROL LISTENER ***/
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setEnabled(true);
	touchListener->setSwallowTouches(true);

	touchListener->onTouchBegan = CC_CALLBACK_2(RaceConf::onTouchBegan, this);

	// using SceneGraphPriority, when the Node is destroyed, the listener is removed automatically
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener,	leftArrow);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener->clone(), rightArrow);
}

bool RaceConf::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) {
	Vec2 touchPoint = touch->getLocation();
	Rect boundingBoxArrow = event->getCurrentTarget()->getBoundingBox();

	if (boundingBoxArrow.containsPoint(touchPoint)
			&& event->getCurrentTarget()->getTag() == 1) {
		if (_carFilesPos <= 0){
			_carFilesPos = _carFiles.size() - 1;
			CCLog("size - 1 = %d", _carFilesPos);
			_car->setTexture(_carFiles.at(_carFilesPos));
			CCLog("file = %s", _carFiles.at(_carFilesPos).c_str());
		} else {
			_carFilesPos -= 1;
			_car->setTexture(_carFiles.at(_carFilesPos));
		}
	} else if (boundingBoxArrow.containsPoint(touchPoint)
			&& event->getCurrentTarget()->getTag() == 2) {
		if (_carFilesPos >= _carFiles.size()-1){
				_carFilesPos = 0;
				_car->setTexture(_carFiles.at(_carFilesPos));
			} else {
				_carFilesPos += 1;
				_car->setTexture(_carFiles.at(_carFilesPos));
			}
	}
	return false;
}

void RaceConf::diffUpdate(float delta) {
	char diff [50];
	_difficultySlider->setValue(round(_difficultySlider->getValue()));
	sprintf(diff, "%d", int(_difficultySlider->getValue()));
	std::string diffLabel (diff);

	_difficultyValue->setString(diffLabel);
}

void RaceConf::oppUpdate(float delta) {
	char opp [50];
	_opponentsSlider->setValue(round(_opponentsSlider->getValue()));
	sprintf(opp, "%d", int(_opponentsSlider->getValue()));
	std::string oppLabel (opp);

	_opponentsValue->setString(oppLabel);
}

void RaceConf::lapsUpdate(float delta) {
	char laps [50];
	_lapsSlider->setValue(round(_lapsSlider->getValue()));
	sprintf(laps, "%d", int(_lapsSlider->getValue()));
	std::string lapsLabel (laps);

	_lapsValue->setString(lapsLabel);
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
	UserDefault::getInstance()->setStringForKey("playerName", _playerName->getText());
	UserDefault::getInstance()->setIntegerForKey("difficulty", (int) _difficultySlider->getValue());
	UserDefault::getInstance()->setIntegerForKey("opponents", _opponentsSlider->getValue() + 1);
	UserDefault::getInstance()->setIntegerForKey("laps", _lapsSlider->getValue());
	UserDefault::getInstance()->setStringForKey("playerSprite", _carFiles.at(_carFilesPos));
	auto raceScene = Race::createScene();
	Director::getInstance()->replaceScene(raceScene);
}

void RaceConf::backMainMenu(Ref* pSender) {
	auto mainMenuScene = MainMenu::createScene();
	Director::getInstance()->replaceScene(mainMenuScene);
}

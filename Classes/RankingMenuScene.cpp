/*
 * RankingMenuScene.cpp
 *
 *  Created on: 09/05/2015
 *      Author: jjimenezg93
 */

#include "RankingMenuScene.h"
#include "MainMenuScene.h"
#include "json/document.h"
#include "json/filestream.h"
#include "json/rapidjson.h"
#include "json/writer.h"
#include "json/stringbuffer.h"

using namespace cocos2d;
using namespace rapidjson;

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

	loadJSON();
	createMenu();

	return true;
}

void RankingMenu::loadJSON() {
	Document fastestLaps;

	FILE* file = fopen("/data/data/org.jjimenezg93.SuperCars/files/fastestLaps.json", "r");
	FileStream fs(file);
	fastestLaps.ParseStream<0>(fs);

	rapidjson::Value::MemberIterator itr;
	short cont = 1;
	for (itr = fastestLaps.MemberonBegin(); itr != fastestLaps.MemberonEnd(); ++itr) {
		if (cont == 1){
			_firstName = itr->name.GetString();
			CCLog("name1 = %s", _firstName.c_str());
		} else if (cont == 2){
			_secondName = itr->name.GetString();
			CCLog("name2 = %s", _secondName.c_str());
		} else if (cont == 3) {
			_thirdName = itr->name.GetString();
			CCLog("name3 = %s", _thirdName.c_str());
		}
		++cont;
	}
	fclose(file);
}

void RankingMenu::createMenu() {
	Document fastestLaps;

	FILE* file = fopen("/data/data/org.jjimenezg93.SuperCars/files/fastestLaps.json", "r");
	FileStream fs(file);
	fastestLaps.ParseStream<0>(fs);

	auto fastestLapLabel = Label::createWithTTF("Fastest lap", "fonts/squares_bold.ttf", 36);
	fastestLapLabel->setAnchorPoint(Vec2(0.5, 0.5));
	fastestLapLabel->setPosition(
			Vec2(origin.x + visibleSize.width/2,
					visibleSize.height - visibleSize.height/4));

	this->addChild(fastestLapLabel);

	/****	FIRST	****/
	auto fastestLapName = Label::createWithTTF("", "fonts/squares_bold.ttf", 32);
	fastestLapName->setAnchorPoint(Vec2(0, 0.5));
	fastestLapName->setPosition(
			Vec2(origin.x + visibleSize.width/4,
					origin.y + visibleSize.height/2 - fastestLapLabel->getContentSize().height));

	char fastestName [50];
	sprintf(fastestName, "%s", _firstName.c_str());
	std::string fastestLabelName (fastestName);

	fastestLapName->setString(fastestLabelName.c_str());

	this->addChild(fastestLapName);

	auto fastestLapValue = Label::createWithTTF("", "fonts/squares_bold.ttf", 32);
	fastestLapValue->setAnchorPoint(Vec2(0, 0.5));
	fastestLapValue->setPosition(
			Vec2(origin.x + visibleSize.width/2,
					origin.y + visibleSize.height/2 - fastestLapLabel->getContentSize().height));

	this->addChild(fastestLapValue);

	char fastestText [50];
	sprintf(fastestText, "%.2f s", fastestLaps[_firstName.c_str()].GetDouble());
	std::string fastestLabelText (fastestText);

	fastestLapValue->setString(fastestLabelText);

	/****	SECOND	****/
	auto fastestLapName2 = Label::createWithTTF("", "fonts/squares_bold.ttf",
			32);
	fastestLapName2->setAnchorPoint(Vec2(0, 0.5));
	fastestLapName2->setPosition(
			Vec2(origin.x + visibleSize.width / 4,
					origin.y + visibleSize.height/2 - fastestLapLabel->getContentSize().height * 2));

	char fastestName2[50];
	sprintf(fastestName2, "%s",	_secondName.c_str());
	std::string fastestLabelName2(fastestName2);

	fastestLapName2->setString(fastestLabelName2.c_str());

	this->addChild(fastestLapName2);

	auto fastestLapValue2 = Label::createWithTTF("", "fonts/squares_bold.ttf",
			32);
	fastestLapValue2->setAnchorPoint(Vec2(0, 0.5));
	fastestLapValue2->setPosition(
			Vec2(origin.x + visibleSize.width / 2,
					origin.y + visibleSize.height/2 - fastestLapLabel->getContentSize().height * 2));

	this->addChild(fastestLapValue2);

	char fastestText2[50];
	sprintf(fastestText2, "%.2f s", fastestLaps[_secondName.c_str()].GetDouble());
	std::string fastestLabelText2(fastestText2);

	fastestLapValue2->setString(fastestLabelText2);

	/****	THIRD	****/
	auto fastestLapName3 = Label::createWithTTF("", "fonts/squares_bold.ttf", 32);
	fastestLapName3->setAnchorPoint(Vec2(0, 0.5));
	fastestLapName3->setPosition(
			Vec2(origin.x + visibleSize.width / 4,
					origin.y + visibleSize.height/2 - fastestLapLabel->getContentSize().height * 3));

	char fastestName3[50];
	sprintf(fastestName3, "%s", _thirdName.c_str());
	std::string fastestLabelName3(fastestName3);

	fastestLapName3->setString(fastestLabelName3.c_str());

	this->addChild(fastestLapName3);

	auto fastestLapValue3 = Label::createWithTTF("", "fonts/squares_bold.ttf", 32);
	fastestLapValue3->setAnchorPoint(Vec2(0, 0.5));
	fastestLapValue3->setPosition(
			Vec2(origin.x + visibleSize.width / 2,
					origin.y + visibleSize.height/2 - fastestLapLabel->getContentSize().height * 3));

	this->addChild(fastestLapValue3);

	char fastestText3[50];
	sprintf(fastestText3, "%.2f s", fastestLaps[_thirdName.c_str()].GetDouble());
	std::string fastestLabelText3(fastestText3);

	fastestLapValue3->setString(fastestLabelText3);

	/***	BUTTONS	***/
	auto resetButton = MenuItemImage::create("reset_button.png",
				"reset_button_pressed.png", CC_CALLBACK_1(RankingMenu::resetRanking, this));

	resetButton->setAnchorPoint(Vec2(1,0));
	resetButton->setPosition(Vec2(origin.x + visibleSize.width,
						origin.y));

	auto backButton = MenuItemImage::create("back_button.png",
				"back_button_pressed.png", CC_CALLBACK_1(RankingMenu::backMainMenu, this));

	backButton->setAnchorPoint(Vec2(0,0));
	backButton->setPosition(Vec2(origin.x,
						origin.y));

	auto menu = Menu::create(resetButton, backButton, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 10);

	fclose(file);
}

void RankingMenu::resetRanking(Ref* pSender) {
	Document fastestLaps;

	FILE* fileOpen = fopen("/data/data/org.jjimenezg93.SuperCars/files/fastestLaps.json", "w");

	fastestLaps.SetObject();

	fclose(fileOpen);

	UserDefault::getInstance()->setFloatForKey("fastestLap", 0);
	auto rankingMenuScene = RankingMenu::createScene();
	Director::getInstance()->replaceScene(rankingMenuScene);
}

void RankingMenu::backMainMenu(Ref* pSender) {
	auto mainMenuScene = MainMenu::createScene();
	Director::getInstance()->replaceScene(mainMenuScene);
}

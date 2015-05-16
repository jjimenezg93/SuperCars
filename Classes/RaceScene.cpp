/*
 * RaceScene.cpp
 *
 *  Created on: 17/04/2015
 *      Author: jjimenezg93
 */

#include "RaceScene.h"
#include "RaceMenuScene.h"
#include "EndRaceScene.h"
#include <time.h>
#include "SimpleAudioEngine.h"
#include <stdio.h>

#define COCOS2D_DEBUG 1

using namespace cocos2d;

Scene* Race::createScene() {
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = Race::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

bool Race::init() {
	// 1. super init first
	if (!Layer::init()) {
		return false;
	}

	//for future objects adjustement
	origin = Director::getInstance()->getVisibleOrigin();
	visibleSize = Director::getInstance()->getVisibleSize();
	playerPos = 1;

	/*** MAP ***/
	_tileMap = TMXTiledMap::create("background.tmx");
	_tileMap->setPositionX(_tileMap->getPositionX() - 4); // need to move -4 pixels (each side) because map size is 1024x608
	_tileAuxiliarMap = TMXTiledMap::create("background.tmx");
	_tileAuxiliarMap->setPositionX(_tileMap->getPositionX());
	_tileAuxiliarMap->setPositionY(1024);
	this->addChild(_tileMap);
	this->addChild(_tileAuxiliarMap);

	/*** CONFIGURATION VARIABLES ***/
	/*****
	 * HOW TO GET USER NAME FROM RACECONF
	char diff [50];
	sprintf(diff, "diff = %f", UserDefault::getInstance()->getFloatForKey("difficulty"));
	std::string diffLabel (diff);

	auto userName = Label::createWithTTF(diffLabel, "fonts/squares_bold.ttf", 26);
	userName->enableOutline(Color4B::BLACK, 2);
	userName->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));

	this->addChild(userName);
	*****/
	//CCLog("Speed: %f", _speed);
	time = 0;
	_fastestLap = 60;

	_difficulty = UserDefault::getInstance()->getIntegerForKey("difficulty");
	_timeStopped = 0;
	_laps = UserDefault::getInstance()->getIntegerForKey("laps");
	_currentLap = 0;
	_opponents = UserDefault::getInstance()->getIntegerForKey("opponents");
	_speed = (short) 4 + _difficulty * _opponents;
	_currentPosition = _opponents;


	/*** PLAYER CREATION ***/
	player = Sprite::create("audi_r8.png");

	player->setAnchorPoint(Vec2(0.5, 0));
	player->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y));

	this->addChild(player, 100);

	spawnOpponents();

	createMenu();
	createLapLine();

	this->createControls(origin, visibleSize);

	audio = CocosDenshion::SimpleAudioEngine::getInstance();
	audio->playBackgroundMusic("background_music.wav", true);
	audio->setBackgroundMusicVolume(0.3f);

	scheduleAll();

	return true;
}

void Race::scheduleAll() {
	this->unscheduleAllSelectors();
	this->scheduleUpdate();
	this->schedule(schedule_selector(Race::timerMethod), (float) 0.01);
	this->schedule(schedule_selector(Race::moveMap), (float) 0);
	this->schedule(schedule_selector(Race::checkPosition), (float) 0);
	this->schedule(schedule_selector(Race::createObstacle),	0.5f);
	this->schedule(schedule_selector(Race::checkLap), 1.f);
}

void Race::update(float dt) {
	if (_tileMap->getPositionY() <= -1024) {
		_tileMap->setPositionY(1024);
	} else if (_tileAuxiliarMap->getPositionY() <= -1024) {
		_tileAuxiliarMap->setPositionY(1024);
	}
	moveObstacles(_obstacles);
	checkCollisions(_obstacles);
	checkDeletion(_obstacles);
	if (!audio->isBackgroundMusicPlaying()){
		audio->resumeBackgroundMusic();
	}
}

void Race::timerMethod(float dt) {
	time += dt;
	__String* timeText = __String::createWithFormat("%.2f", time);
	timerLabel->setString(timeText->getCString());
}

void Race::moveMap(float dt) {
	_tileMap->setPositionY(_tileMap->getPositionY() - _speed);
	_tileAuxiliarMap->setPositionY(_tileAuxiliarMap->getPositionY() - _speed);
}

void Race::createObstacle(float dt) {
	Sprite* obstacle = Sprite::create("rock.png");
	int positionX = round(rand() % 300 + 1) + 120; // RANDOM POSITIONS ALONG THE ROAD
	obstacle->setPosition(positionX, 1030);
	CCLog("Obstacle X position %d", positionX);
	obstacle->setTag(1);
	this->addChild(obstacle, 20);
	_obstacles.pushBack(obstacle);
}

void Race::spawnOpponents() {
	for (int i = 1; i < _opponents; ++i) {
		Sprite* opponent = Sprite::create("gallardo.png");

		opponent->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + 2000 * i));
		opponent->setTag(3);

		this->addChild(opponent, 100);
		_obstacles.pushBack(opponent);
	}
}

void Race::checkLap(float dt) {
	if (_currentLap == _laps) {
		//checkCollisions(_obstacles);
		this->unscheduleAllSelectors();
		showEndRace(this);
	}
}

void Race::carStopped(float dt) {
	if (_timeStopped < 1) {
		_timeStopped++;
	} else {
		_timeStopped = 0;
		_eventDispatcher->resumeEventListenersForTarget(leftArrow);
		_eventDispatcher->resumeEventListenersForTarget(rightArrow);
		scheduleAll();	//this method unschedules all before scheduling main methods
	}
}

void Race::createMenu() {
	/*** Close button ***/
	auto raceMenuButton = MenuItemImage::create("menu_button.png",
			"menu_button.png", CC_CALLBACK_1(Race::showRaceMenu, this));

	raceMenuButton->setPosition(
			Vec2(
					origin.x + visibleSize.width
							- raceMenuButton->getContentSize().width / 2,
					origin.y + visibleSize.height
							- raceMenuButton->getContentSize().height / 2));

	auto menu = Menu::create(raceMenuButton, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 10);

	lapLabel = Label::createWithTTF("", "fonts/squares_bold.ttf", 26);
	lapLabel->enableOutline(Color4B::BLACK, 2);
	lapLabel->setAnchorPoint(Vec2(0,1));
	lapLabel->setPosition(Vec2(origin.x + lapLabel->getContentSize().width / 2,
								origin.y + visibleSize.height - lapLabel->getContentSize().height / 2));
	this->addChild(lapLabel, 70);
	updateLapsLabel();

	posLabel = Label::createWithTTF("", "fonts/squares_bold.ttf", 26);
	posLabel->enableOutline(Color4B::BLACK, 2);
	posLabel->setAnchorPoint(Vec2(0,1));
	posLabel->setPosition(Vec2(origin.x + posLabel->getContentSize().width / 2,
								origin.y + visibleSize.height - lapLabel->getContentSize().height));
	this->addChild(posLabel, 70);
	updatePosLabel();

	timerLabel = Label::createWithTTF("0", "fonts/squares_bold.ttf", 26);
	timerLabel->enableOutline(Color4B::BLACK, 2);
	timerLabel->setAnchorPoint(Vec2(0,1));
	timerLabel->setPosition(Vec2(origin.x + timerLabel->getContentSize().width / 2,
								origin.y + visibleSize.height - lapLabel->getContentSize().height * 2));
	this->addChild(timerLabel, 70);
}

void Race::createLapLine() {
	Sprite* lapLine = Sprite::create("lap_line.png");
	lapLine->setPosition(292, 5000);
	lapLine->setTag(2); // when player gets to the lap, lapLine->Tag changes to 20
	this->addChild(lapLine, 15);
	_obstacles.pushBack(lapLine);
}

void Race::updateLapsLabel() {
	char lapsText [50];
	sprintf(lapsText, "Laps %d/%d", _currentLap, _laps);
	std::string lapLabelText (lapsText);
	lapLabel->setString(lapLabelText);
}

void Race::updatePosLabel() {
	char posText [50];
	sprintf(posText, "Pos %d/%d", _currentPosition, _opponents);
	std::string posLabelText (posText);
	posLabel->setString(posLabelText);
}

void Race::deleteObstacle(Sprite* s) {
	_obstacles.eraseObject(s, false);
	this->removeChild(s, true);
}

void Race::moveObstacles(Vector<Sprite*> v) {
	for (auto obstacle : v) {
		// TAG 1 = ROCKS; TAG 2 = LAP_LINE; TAG 3 = OPPONENTS; TAG 20 = OLD_LAP_LINE
		if (obstacle->getTag() == 1) {
			obstacle->setPositionY(obstacle->getPositionY() - _speed);
		} else if (obstacle->getTag() == 2 || obstacle->getTag() == 20) {
			obstacle->setPositionY(obstacle->getPositionY() - _speed);
		} else if (obstacle->getTag() == 3 || obstacle->getTag() == 4) {	// opponents normal movement
			obstacle->setPositionY(obstacle->getPositionY() - (_speed - (_difficulty * 2)));
			CCLog("_speed (opponent) = %d", (short) (_speed / _difficulty));
		}
	}
}

void Race::moveInvOpponents(float dt) {
	for (auto opponent : _obstacles) {
		if (opponent->getTag() == 3 || opponent->getTag() == 4) {
			opponent->setPositionY(opponent->getPositionY() + _speed);
			checkOpponentCollisions(opponent);
		}
	}
}

void Race::checkOpponentCollisions(Sprite* v) {
	for (auto opponent : _obstacles) {
		if (v->getBoundingBox().intersectsRect(opponent->getBoundingBox())) {
			CCLog("Opponent collisioned");
		}
	}
}

void Race::checkCollisions(Vector<Sprite*> v) {
	for (auto obstacle : v) {
		if (player->getBoundingBox().intersectsRect(
				obstacle->getBoundingBox())) {
			if (obstacle->getTag() == 2) { // IF THE OBSTACLE IS THE LAP LINE
				if (_currentLap <= _laps){
					_lapsTime[_currentLap] = time;
					if (time < _fastestLap){
						_fastestLap = time;
						CCLog("New fastest lap: %.2f", _fastestLap);
					}
					CCLog("Lap %i: %.2f", _currentLap, time);
					time = 0;
				}
				_currentLap++;
				updateLapsLabel();
				createLapLine();
				obstacle->setTag(20);
				audio->playEffect("lap_complete.wav", false, 1.f, 0.f, 1.f);
			} else if (obstacle->getTag() == 1) {	// IF THE OBSTACLE IS A ROCK

				// playEffect(source, loop, frequency, stereo effect, volume)
				audio->playEffect("big-crash.wav", false, 1.f, 0.f, 1.f);

				deleteObstacle(obstacle);

				_eventDispatcher->pauseEventListenersForTarget(leftArrow);
				_eventDispatcher->pauseEventListenersForTarget(rightArrow);

				this->unscheduleAllSelectors();
				this->schedule(schedule_selector(Race::timerMethod), (float) 0.01);
				this->schedule(schedule_selector(Race::carStopped), 1.f);
				this->schedule(schedule_selector(Race::moveInvOpponents), (float) 0);
				this->schedule(schedule_selector(Race::checkPosition), (float) 0);
			} else if (obstacle->getTag() == 3 || obstacle->getTag() == 4) {	// IF THE OBSTACLE IS AN OPPONENT

				// playEffect(source, loop, frequency, stereo effect, volume)
				audio->playEffect("big-crash.wav", false, 1.f, 0.f, 1.f);

				_eventDispatcher->pauseEventListenersForTarget(leftArrow);
				_eventDispatcher->pauseEventListenersForTarget(rightArrow);

				this->unscheduleAllSelectors();
				this->schedule(schedule_selector(Race::timerMethod), (float) 0.01);
				this->schedule(schedule_selector(Race::carStopped), 1.f);
				this->schedule(schedule_selector(Race::moveInvOpponents), (float) 0);
				this->schedule(schedule_selector(Race::checkPosition), (float) 0);
			}
		}
	}
}

void Race::checkPosition(float dt) {
	// TAG == 3 OPPONENT ABOVE, TAG == 4 OPPONENT BELOW
	for (auto opponent : _obstacles) {
			if (opponent->getTag() == 3 && opponent->getPositionY() < player->getPositionY()) {
				_currentPosition -= 1;
				opponent->setTag(4);
				updatePosLabel();
			} else if (opponent->getTag() == 4 && opponent->getPositionY() > player->getPositionY()) {
				_currentPosition += 1;
				opponent->setTag(3);
				updatePosLabel();
			}
		}
}

void Race::checkDeletion(Vector<Sprite*> obstacles) {
	for (auto obstacleToDelete : obstacles) {
		if (obstacleToDelete->getTag() != 3 && obstacleToDelete->getTag() != 4 && obstacleToDelete->getPositionY() < -100) {
			deleteObstacle(obstacleToDelete);
		}
	}
}

void Race::createControls(Vec2 origin, Size visibleSize) {
	/*** Directional arrows ***/

	/*** LEFT ARROW ***/
	leftArrow = Sprite::create("left_arrow.png");
	leftArrow->setAnchorPoint(Vec2(0, 0));

	leftArrow->setPosition(Vec2(origin.x, origin.y + visibleSize.height / 2));

	this->addChild(leftArrow, 100);

	/*** RIGHT ARROW ***/
	rightArrow = Sprite::create("right_arrow.png");
	rightArrow->setAnchorPoint(Vec2(0, 0));

	rightArrow->setPosition(
			Vec2(
					origin.x + visibleSize.width
							- rightArrow->getContentSize().width,
					origin.y + visibleSize.height / 2));

	this->addChild(rightArrow, 100);

	leftArrow->setTag(1);
	rightArrow->setTag(2);

	/*** CONTROL LISTENER ***/
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setEnabled(true);
	touchListener->setSwallowTouches(true);

	touchListener->onTouchBegan = CC_CALLBACK_2(Race::onTouchBegan, this);
	//touchListener->onTouchEnded= CC_CALLBACK_2(Race::onTouchEnded, this);

	// using SceneGraphPriority, when the Node is destroyed, the listener is removed automatically
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener,
			leftArrow);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(
			touchListener->clone(), rightArrow);
}

bool Race::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) {
	Vec2 touchPoint = touch->getLocation();
	Rect boundingBoxArrow = event->getCurrentTarget()->getBoundingBox();

	if (boundingBoxArrow.containsPoint(touchPoint)
			&& event->getCurrentTarget()->getTag() == 1
			&& (playerPos == 1 || playerPos == 2)) {
		auto moveLeft = MoveBy::create(0.25, Vec2(-100, 0));
		player->runAction(moveLeft);
		playerPos--;
	} else if (boundingBoxArrow.containsPoint(touchPoint)
			&& event->getCurrentTarget()->getTag() == 2
			&& (playerPos == 0 || playerPos == 1)) {
		auto moveRight = MoveBy::create(0.25, Vec2(100, 0));
		player->runAction(moveRight);
		playerPos++;
	}
	return false;
}

void Race::showEndRace(Ref* pSender) {
	audio->end();

	float fast = _lapsTime[0];
	CCLog("lapsTime[0] = %.2f", _lapsTime[0]);
	for (auto i : _lapsTime) {
		if (i < fast && i > 0) {
			fast = i;
		}
	}

	UserDefault::getInstance()->setFloatForKey("raceFastestLap", fast);
	CCLog("fast = %.2f", fast);
	UserDefault::getInstance()->setFloatForKey("fastestLap", _fastestLap);
	CCLog("fastest lap %.2f", _fastestLap);

	auto endRaceScene = EndRace::createScene();
	Director::getInstance()->replaceScene(endRaceScene);
}

void Race::showRaceMenu(Ref* pSender) {
	audio->pauseBackgroundMusic();
	Director::getInstance()->pause();
	auto raceMenuScene = RaceMenu::createScene();
	Director::getInstance()->pushScene(raceMenuScene);
}

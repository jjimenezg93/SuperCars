/*
 * RaceScene.cpp
 *
 *  Created on: 17/04/2015
 *      Author: jjimenezg93
 */

/******* SPRITES TAGS
 * 1 = ROCKS
 * 2 = LAP_LINE
 * 3 = OPPONENT ABOVE
 * 4 = OPPONENT BELOW
 * 20 = OLD_LAP_LINE
 */


#include "RaceScene.h"
#include "RaceMenuScene.h"
#include "EndRaceScene.h"
#include <time.h>
#include "SimpleAudioEngine.h"
#include <stdio.h>
#include "json/document.h"
#include "json/filestream.h"
#include "json/rapidjson.h"
#include "json/writer.h"
#include "json/stringbuffer.h"

#define COCOS2D_DEBUG 1

using namespace cocos2d;
using namespace rapidjson;

Document fastestLaps;

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
	_playerPos = 1;

	/*** MAP ***/
	_tileMap = TMXTiledMap::create("background.tmx");
	_tileMap->setPositionX(_tileMap->getPositionX() - 4); // need to move -4 pixels (each side) because map size is 1024x608
	_tileAuxiliarMap = TMXTiledMap::create("background.tmx");
	_tileAuxiliarMap->setPositionX(_tileMap->getPositionX());
	_tileAuxiliarMap->setPositionY(1024);
	this->addChild(_tileMap);
	this->addChild(_tileAuxiliarMap);


	/**********	DELETE USER'S CAR TEXTURE FROM ELIGIBLE TEXTURES	**********/
	_carFiles.push_back("audi_r8.png");
	_carFiles.push_back("audi_r8_black.png");
	_carFiles.push_back("gallardo.png");
	std::vector<std::string>::iterator begin = _carFiles.begin();
	std::vector<std::string>::iterator end = _carFiles.end();
	short cont = 0;
	for (std::vector<std::string>::iterator it = begin; it != end; it++) {
		if (UserDefault::getInstance()->getStringForKey("playerSprite") == *it) {
			_carFiles.erase(it);
		} else cont++;
	}

	_time = 0;
	_fastestLap = 60;

	_difficulty = UserDefault::getInstance()->getIntegerForKey("difficulty");
	_timeStopped = 0;
	_oppTimeStopped = 0;
	_laps = UserDefault::getInstance()->getIntegerForKey("laps");
	_currentLap = 0;
	_numOpponents = UserDefault::getInstance()->getIntegerForKey("opponents");
	_speed = (short) 4 + _difficulty * _numOpponents;
	_currentPosition = _numOpponents;


	/*** PLAYER CREATION ***/
	_player = Sprite::create(UserDefault::getInstance()->getStringForKey("playerSprite"));

	_player->setAnchorPoint(Vec2(0.5, 0));
	_player->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 8));

	this->addChild(_player, 100);

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
	this->schedule(schedule_selector(Race::moveObstacles), (float) 0);
	this->schedule(schedule_selector(Race::moveOpponents), (float) 0);
	this->schedule(schedule_selector(Race::checkPosition), (float) 0);
	this->schedule(schedule_selector(Race::createObstacle),	0.5f);
	this->schedule(schedule_selector(Race::checkLap), 1.f);
	this->schedule(schedule_selector(Race::avoidCollision), (float) 0.5f);
	this->schedule(schedule_selector(Race::checkOppIsOut), (float) 1.f);
}

void Race::update(float dt) {
	if (_tileMap->getPositionY() <= -1024) {
		_tileMap->setPositionY(1024);
	} else if (_tileAuxiliarMap->getPositionY() <= -1024) {
		_tileAuxiliarMap->setPositionY(1024);
	}
	for (auto opponent : _opponents) {
		checkOpponentCollisions(opponent);
	}
	checkPlayerCollisions(_obstacles);
	checkDeletion(_obstacles);
	if (!audio->isBackgroundMusicPlaying()){
		audio->resumeBackgroundMusic();
	}
}

void Race::timerMethod(float dt) {
	_time += dt;
	__String* timeText = __String::createWithFormat("%.2f", _time);
	_timerLabel->setString(timeText->getCString());
}

void Race::moveMap(float dt) {
	_tileMap->setPositionY(_tileMap->getPositionY() - _speed);
	_tileAuxiliarMap->setPositionY(_tileAuxiliarMap->getPositionY() - _speed);
}

void Race::createObstacle(float dt) {
	Sprite* obstacle = Sprite::create("rock.png");
	int positionX = getRandomSpawnX(215, 435); // RANDOM POSITIONS ALONG THE ROAD
	obstacle->setPosition(positionX, 1030);
	CCLog("Obstacle X position %d", positionX);
	obstacle->setTag(1);
	this->addChild(obstacle, 20);
	_obstacles.pushBack(obstacle);
}

void Race::spawnOpponents() {
	for (int i = 1; i < _numOpponents; ++i) {

		short pos = (short) round(rand() % _carFiles.size());
		Sprite* opponent = Sprite::create(_carFiles.at(pos));

		short randomPosX = getRandomSpawnX(200, 400);

		opponent->setPosition(Vec2(origin.x + randomPosX, origin.y + 2000 * i));
		opponent->setTag(3);

		this->addChild(opponent, 100);
		_opponents.pushBack(opponent);
	}
}

short Race::getRandomSpawnX(short min, short max) {
	bool loop = true;
	short randomPosX = 0;
	while(loop == true) {
		randomPosX = (short) round(arc4random() % 400);
		if (randomPosX >= min && randomPosX <= max) {
			loop = false;
		}
	}
	return randomPosX;
}

void Race::checkLap(float dt) {
	if (_currentLap == _laps) {
		this->unscheduleAllSelectors();
		showEndRace(this);
	}
}

void Race::carStopped(float dt) {
	if (_timeStopped < 1) {
		++_timeStopped;
	} else {
		_timeStopped = 0;
		_eventDispatcher->resumeEventListenersForTarget(_leftArrow);
		_eventDispatcher->resumeEventListenersForTarget(_rightArrow);
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

	_lapLabel = Label::createWithTTF("", "fonts/squares_bold.ttf", 26);
	_lapLabel->enableOutline(Color4B::BLACK, 2);
	_lapLabel->setAnchorPoint(Vec2(0,1));
	_lapLabel->setPosition(Vec2(origin.x + _lapLabel->getContentSize().width / 2,
								origin.y + visibleSize.height - _lapLabel->getContentSize().height / 2));
	this->addChild(_lapLabel, 70);
	updateLapsLabel();

	_posLabel = Label::createWithTTF("", "fonts/squares_bold.ttf", 26);
	_posLabel->enableOutline(Color4B::BLACK, 2);
	_posLabel->setAnchorPoint(Vec2(0,1));
	_posLabel->setPosition(Vec2(origin.x + _posLabel->getContentSize().width / 2,
								origin.y + visibleSize.height - _lapLabel->getContentSize().height));
	this->addChild(_posLabel, 70);
	updatePosLabel();

	_timerLabel = Label::createWithTTF("0", "fonts/squares_bold.ttf", 26);
	_timerLabel->enableOutline(Color4B::BLACK, 2);
	_timerLabel->setAnchorPoint(Vec2(0,1));
	_timerLabel->setPosition(Vec2(origin.x + _timerLabel->getContentSize().width / 2,
								origin.y + visibleSize.height - _lapLabel->getContentSize().height * 2));
	this->addChild(_timerLabel, 70);
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
	_lapLabel->setString(lapLabelText);
}

void Race::updatePosLabel() {
	char posText [50];
	sprintf(posText, "Pos %d/%d", _currentPosition, _numOpponents);
	std::string posLabelText (posText);
	_posLabel->setString(posLabelText);
}

void Race::moveObstacles(float dt) {
	for (auto obstacle : _obstacles) {
		if (obstacle->getTag() == 1) {
			obstacle->setPositionY(obstacle->getPositionY() - _speed);
		} else if (obstacle->getTag() == 2 || obstacle->getTag() == 20) {
			obstacle->setPositionY(obstacle->getPositionY() - _speed);
		}
	}
}

void Race::moveOpponents(float dt) {
	for (auto opponent : _opponents) {
		opponent->setPositionY(opponent->getPositionY() - (_speed - (_difficulty * 2)));
		checkOpponentCollisions(opponent);
	}
}

void Race::moveInvOpponents(float dt) {
	for (auto opponent : _opponents) {
		if (opponent->getTag() == 3 || opponent->getTag() == 4) {
			opponent->setPositionY(opponent->getPositionY() + _speed);
			checkOpponentCollisions(opponent);
		}
	}
}

void Race::stoppedOpponents(float dt) {
	if (_oppTimeStopped < 1) {
		++_oppTimeStopped;
	} else {
		_oppTimeStopped = 0;
		scheduleAll();	//this method unschedules all before scheduling main methods
	}
}

void Race::moveCrashedOpponents(float dt) {
	for (auto opponent : _opponents) {
		opponent->setPositionY(opponent->getPositionY() - _speed);
		checkOpponentCollisions(opponent);
	}
}

void Race::avoidCollision(float dt) {
	for (auto opponent : _opponents) {
		for (auto obstacle : _obstacles) {
			if ((abs(obstacle->getPositionY()) - opponent->getPositionY()) <= 200 // abs(x) = |x|
			&& (abs(obstacle->getPositionX() - opponent->getPositionX()) <= 50)) {
				changeXOpponent (opponent);
			}
		}
	}
}

void Race::checkOppIsOut(float dt) {
	for (auto opponent : _opponents) {
		if (opponent->getPositionX() <= 150 || opponent->getPositionX() >= 450) {
			changeXOpponent(opponent);
		}
	}
}

void Race::changeXOpponent(Sprite* s) {
	if (s->getPositionX() <= 200) {
		auto moveCenter = MoveTo::create(0.2, Vec2(300, s->getPositionY()));
		s->runAction(moveCenter);
	} else if (s->getPositionX() >= 400) {
		auto moveCenter = MoveTo::create(0.2, Vec2(300, s->getPositionY()));
		s->runAction(moveCenter);
	} else{
		auto random = CCRANDOM_0_1();
		if (random == 0) {
			auto moveRight = MoveTo::create(0.2, Vec2(400, s->getPositionY()));
			s->runAction(moveRight);
		} else {
			auto moveLeft = MoveTo::create(0.2, Vec2(200, s->getPositionY()));
			s->runAction(moveLeft);
		}
	}
}

void Race::checkOpponentCollisions(Sprite* s) {
	for (auto obstacle : _obstacles) {
		if (obstacle->getTag() == 1
				&& s->getBoundingBox().intersectsRect(obstacle->getBoundingBox())) {
			this->unschedule(schedule_selector(Race::moveOpponents));
			this->schedule(schedule_selector(Race::moveCrashedOpponents), (float) 0);
			this->schedule(schedule_selector(Race::stoppedOpponents), (float) 1.0f);
			deleteObstacle(obstacle);
		}
	}
}

void Race::checkPlayerCollisions(Vector<Sprite*> v) {
	for (auto obstacle : v) {
		if (_player->getBoundingBox().intersectsRect(
				obstacle->getBoundingBox())) {
			if (obstacle->getTag() == 2) { // IF THE OBSTACLE IS THE LAP LINE
				if (_currentLap <= _laps) {
					_lapsTime[_currentLap] = _time;
					if (_time < _fastestLap && _time != 0) {
						_fastestLap = _time;
						CCLog("New fastest lap: %.2f", _fastestLap);
						UserDefault::getInstance()->setStringForKey(
								"fastestPlayer",
								UserDefault::getInstance()->getStringForKey(
										"playerName").c_str());
						CCLog("fastest player %s",
								UserDefault::getInstance()->getStringForKey(
										"fastestPlayer").c_str());
					}
					CCLog("Lap %i: %.2f", _currentLap, _time);
					_time = 0;
				}

				_currentLap++;
				updateLapsLabel();
				createLapLine();
				obstacle->setTag(20);
				audio->playEffect("lap_complete.wav", false, 1.f, 0.f, 1.f);
			} else if (obstacle->getTag() == 1) { // IF THE OBSTACLE IS A ROCK

				// playEffect(source, loop, frequency, stereo effect, volume)
				audio->playEffect("big-crash.wav", false, 1.f, 0.f, 1.f);

				deleteObstacle(obstacle);

				_eventDispatcher->pauseEventListenersForTarget(_leftArrow);
				_eventDispatcher->pauseEventListenersForTarget(_rightArrow);

				this->unscheduleAllSelectors();
				this->schedule(schedule_selector(Race::timerMethod), (float) 0.01);
				this->schedule(schedule_selector(Race::carStopped), 1.f);
				this->schedule(schedule_selector(Race::moveInvOpponents), (float) 0);
				this->schedule(schedule_selector(Race::checkPosition), (float) 0);
				this->schedule(schedule_selector(Race::avoidCollision), (float) 0.5f);
			}
		}
	}
	for (auto opponent : _opponents) {
		if (_player->getBoundingBox().intersectsRect(
				opponent->getBoundingBox())) { // IF THE OBSTACLE IS AN OPPONENT

			// playEffect(source, loop, frequency, stereo effect, volume)
			audio->playEffect("big-crash.wav", false, 1.f, 0.f, 1.f);

			_eventDispatcher->pauseEventListenersForTarget(_leftArrow);
			_eventDispatcher->pauseEventListenersForTarget(_rightArrow);

			this->unscheduleAllSelectors();
			this->schedule(schedule_selector(Race::timerMethod), (float) 0.01);
			this->schedule(schedule_selector(Race::carStopped), 1.f);
			this->schedule(schedule_selector(Race::moveInvOpponents), (float) 0);
			this->schedule(schedule_selector(Race::checkPosition), (float) 0);
			this->schedule(schedule_selector(Race::avoidCollision), (float) 0.5f);
		}
	}
}

void Race::checkPosition(float dt) {
	// TAG == 3 OPPONENT ABOVE, TAG == 4 OPPONENT BELOW
	for (auto opponent : _opponents) {
			if (opponent->getTag() == 3 && opponent->getPositionY() < _player->getPositionY()) {
				_currentPosition -= 1;
				opponent->setTag(4);
				updatePosLabel();
			} else if (opponent->getTag() == 4 && opponent->getPositionY() > _player->getPositionY()) {
				_currentPosition += 1;
				opponent->setTag(3);
				updatePosLabel();
			}
		}
}

void Race::checkDeletion(Vector<Sprite*> obstacles) {
	for (auto obstacleToDelete : obstacles) {
		if (obstacleToDelete->getPositionY() < -100) {
			deleteObstacle(obstacleToDelete);
		}
	}
}

void Race::deleteObstacle(Sprite* s) {
	_obstacles.eraseObject(s, false);
	this->removeChild(s, true);
}

void Race::createControls(Vec2 origin, Size visibleSize) {
	/*** Directional arrows ***/

	/*** LEFT ARROW ***/
	_leftArrow = Sprite::create("left_arrow.png");
	_leftArrow->setAnchorPoint(Vec2(0, 0));

	_leftArrow->setPosition(Vec2(origin.x, origin.y + visibleSize.height / 2));

	this->addChild(_leftArrow, 100);

	/*** RIGHT ARROW ***/
	_rightArrow = Sprite::create("right_arrow.png");
	_rightArrow->setAnchorPoint(Vec2(0, 0));

	_rightArrow->setPosition(Vec2(origin.x + visibleSize.width - _rightArrow->getContentSize().width,
					origin.y + visibleSize.height / 2));

	this->addChild(_rightArrow, 100);

	_leftArrow->setTag(1);
	_rightArrow->setTag(2);

	/*** CONTROL LISTENER ***/
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setEnabled(true);
	touchListener->setSwallowTouches(true);

	touchListener->onTouchBegan = CC_CALLBACK_2(Race::onTouchBegan, this);

	// using SceneGraphPriority, when the Node is destroyed, the listener is removed automatically
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, _leftArrow);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener->clone(), _rightArrow);
}

bool Race::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) {
	Vec2 touchPoint = touch->getLocation();
	Rect boundingBoxArrow = event->getCurrentTarget()->getBoundingBox();

	if (boundingBoxArrow.containsPoint(touchPoint)
			&& event->getCurrentTarget()->getTag() == 1
			&& (_playerPos == 1 || _playerPos == 2)) {
		auto moveLeft = MoveBy::create(0.25, Vec2(-100, 0));
		_player->runAction(moveLeft);
		_playerPos--;
	} else if (boundingBoxArrow.containsPoint(touchPoint)
			&& event->getCurrentTarget()->getTag() == 2
			&& (_playerPos == 0 || _playerPos == 1)) {
		auto moveRight = MoveBy::create(0.25, Vec2(100, 0));
		_player->runAction(moveRight);
		_playerPos++;
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

	/*** JSON ***/
	FILE* fileOpen = fopen("data/data/org.jjimenezg93.SuperCars/files/fastestLaps.json", "w");
	FileStream fs(fileOpen);
	fastestLaps.ParseStream<0>(fs);

	fastestLaps.SetObject();
	Document::AllocatorType& allocator = fastestLaps.GetAllocator();
	fastestLaps.AddMember(UserDefault::getInstance()->getStringForKey("playerName").c_str(), fast, allocator);
	CCLog("%s", fastestLaps.GetString());

	StringBuffer buffer;
	Writer<StringBuffer> writer (buffer);
	fastestLaps.Accept(writer);
	FILE* file = fopen("data/data/org.jjimenezg93.SuperCars/files/fastestLaps.json", "w");
	if (file) {
		fputs(buffer.GetString(), file);
		CCLog("json Race = %s", buffer.GetString());
		fclose(file);
	}

	UserDefault::getInstance()->setFloatForKey("raceFastestLap", fast);
	CCLog("fast = %.2f", fast);
	UserDefault::getInstance()->setFloatForKey("fastestLap", _fastestLap);
	CCLog("fastest lap %.2f", _fastestLap);

	this->unscheduleAllSelectors();

	auto endRaceScene = EndRace::createScene();
	Director::getInstance()->replaceScene(endRaceScene);
}

void Race::showRaceMenu(Ref* pSender) {
	audio->pauseBackgroundMusic();
	Director::getInstance()->pause();
	auto raceMenuScene = RaceMenu::createScene();
	Director::getInstance()->pushScene(raceMenuScene);
}

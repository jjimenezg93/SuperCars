/*
 * RaceScene.cpp
 *
 *  Created on: 17/04/2015
 *      Author: jjimenezg93
 */

#include "RaceScene.h"
#include <time.h>

#define COCOS2D_DEBUG 1

USING_NS_CC;

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
	if ( !Layer::init() ) {
		return false;
	}

	//for future objects adjustement
	origin = Director::getInstance()->getVisibleOrigin();
	visibleSize = Director::getInstance()->getVisibleSize();
	playerPos = 1;


	/*** MAP ***/
	_tileMap = TMXTiledMap::create("background.tmx");
	_tileMap->setPositionX(_tileMap->getPositionX() - 4);	// need to move -4 pixels (each side) because map size is 1024x608
	_tileAuxiliarMap = TMXTiledMap::create("background.tmx");
	_tileAuxiliarMap->setPositionX(_tileMap->getPositionX());
	_tileAuxiliarMap->setPositionY(1024);
	this->addChild(_tileMap);
	this->addChild(_tileAuxiliarMap);

	speed = 5;
	difficulty = 4;
	temp = 0;

	/*** PLAYER CREATION ***/
	player = Sprite::create("gallardo.png");

	player->setAnchorPoint(Vec2(0.5,0));
	player->setPosition(Vec2(visibleSize.width/2 + origin.x, origin.y));

	this->addChild(player);

	/*Sprite* enemy = Sprite::create("opponent.png");

	opponent->setPosition(350,700);
	opponent->setTag(2);

	this->addChild(opponent);
	_obstacles.pushBack(opponent);*/

	/*** Close button ***/
	auto closeItem = MenuItemImage::create("CloseNormal.png", "CloseSelected.png", CC_CALLBACK_1(Race::menuCloseCallback, this));

	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
								origin.y + visibleSize.height - closeItem->getContentSize().height/2));


	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);

	this->createControls(origin, visibleSize);

	this->scheduleUpdate();
	this->schedule(schedule_selector(Race::createObstacle), (float) speed/difficulty);

	return true;
}

void Race::update(float dt) {
	if (_tileMap->getPositionY() <= -1024) {
		_tileMap->setPositionY(1024);
	} else if (_tileAuxiliarMap->getPositionY() <= -1024) {
		_tileAuxiliarMap->setPositionY(1024);
	}
	moveMap(speed);
	moveObstacles(_obstacles);
	checkCollisions(_obstacles);
	for (auto deletion : _obstacles){
		if (deletion->getPositionY() < -50){
			deleteObstacle(deletion);
		}
	}

	// WITH A TIMER, CALL createObstacle() EVERY X (RANDOM) SECONDS

	/*if (temp <= 120){
		temp++;
	} else {
		temp = 0;
		createObstacle();
	}*/
}

void Race::moveMap(short speed) {
	_tileMap->setPositionY(_tileMap->getPositionY() - speed);
	_tileAuxiliarMap->setPositionY(_tileAuxiliarMap->getPositionY() - speed);
}

void Race::createObstacle(float dt) {
	Sprite* obstacle = Sprite::create("rock.png");
	int positionX = round(rand() % 300 + 1) + 150;	// RANDOM POSITIONS ALONG THE ROAD
	obstacle->setPosition(positionX, 1030);
	obstacle->setTag(1);
	this->addChild(obstacle);
	_obstacles.pushBack(obstacle);
}

void Race::deleteObstacle(Sprite* s) {
	_obstacles.eraseObject(s,false);
}

void Race::moveObstacles(Vector<Sprite *> obstacles) {
	for (auto obstacle : obstacles){
		// TAG 1 = OBSTACLES; TAG 2 = OPPONENTS
		if (obstacle->getTag() == 1){
		obstacle->setPositionY(obstacle->getPositionY() - speed);
		}/* else if (obstacle->getTag() == 2) {
			obstacle->setPositionY(obstacle->getPositionY() - speed/difficulty);
		}*/
	}
}

void Race::checkCollisions(Vector<Sprite *> v) {
	for (auto obstacle : v){
		if (player->getBoundingBox().intersectsRect(obstacle->getBoundingBox())){
			auto moveUp = MoveBy::create(0.5,Vec2(0,50));
			player->runAction(moveUp);		// TEMP ACTION TO CHECK FOR COLLISIONS WORKS FINE
		}
	}
}

void Race::createControls(Vec2 origin, Size visibleSize) {
	/*** Directional arrows ***/

	/*** LEFT ARROW ***/
	leftArrow = Sprite::create("left_arrow.png");
	leftArrow->setAnchorPoint(Vec2(0,0));

	leftArrow->setPosition(Vec2(origin.x, origin.y + visibleSize.height/2));

	this->addChild(leftArrow);

	/*** RIGHT ARROW ***/
	rightArrow = Sprite::create("right_arrow.png");
	rightArrow->setAnchorPoint(Vec2(0,0));

	rightArrow->setPosition(Vec2(origin.x + visibleSize.width - rightArrow->getContentSize().width, origin.y + visibleSize.height/2));

	this->addChild(rightArrow);

	leftArrow->setTag(1);
	rightArrow->setTag(2);

	/*** CONTROL LISTENER ***/
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setEnabled(true);
	touchListener->setSwallowTouches(true);

	touchListener->onTouchBegan = CC_CALLBACK_2(Race::onTouchBegan, this);
	//touchListener->onTouchEnded= CC_CALLBACK_2(Race::onTouchEnded, this);

	// using SceneGraphPriority, when the Node is destroyed, the listener is removed automatically
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, leftArrow);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener->clone(), rightArrow);
}

bool Race::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) {
	Vec2 touchPoint = touch->getLocation();
	Rect boundingBoxArrow = event->getCurrentTarget()->getBoundingBox();

	if (boundingBoxArrow.containsPoint(touchPoint)
			&& event->getCurrentTarget()->getTag() == 1
			&& (playerPos == 1 || playerPos == 2)){
		auto moveLeft = MoveBy::create(0.5,Vec2(-100,0));
		player->runAction(moveLeft);
		playerPos--;
	} else if(boundingBoxArrow.containsPoint(touchPoint)
			&& event->getCurrentTarget()->getTag() == 2
			&& (playerPos == 0 || playerPos == 1)) {
		auto moveRight = MoveBy::create(0.5,Vec2(100,0));
		player->runAction(moveRight);
		playerPos++;
	}
	return false;
}

/*void Race::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event) {

}*/

void Race::menuCloseCallback(Ref* pSender) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
	return;
#endif

	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

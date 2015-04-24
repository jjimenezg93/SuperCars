/*
 * RaceScene.h
 *
 *  Created on: 17/04/2015
 *      Author: jjimenezg93
 */

#ifndef RACESCENE_H_
#define RACESCENE_H_

#include "cocos2d.h"

using namespace cocos2d;

class Race : public cocos2d::Layer {
public:
	static cocos2d::Scene* createScene();
	virtual bool init();

	Vec2 origin;
	Size visibleSize;	// screen 1024x600
	short speed;
	short difficulty;
	short temp;

	TMXTiledMap* _tileMap;
	TMXTiledMap* _tileAuxiliarMap;
	TMXLayer* _background;

	// playerPos = {0, 1, 2}, beeing 0 = left, 1 = center, 2 = right
	short playerPos;

	cocos2d::Sprite* player;
	cocos2d::Sprite* leftArrow;
	cocos2d::Sprite* rightArrow;

	Vector<Sprite *> _obstacles;

	void update(float delta);
	void moveMap(short speed);
	void createControls(Vec2 origin, Size visibleSize);	//creates control arrows and appropiate listeners

	void createObstacle(float dtime);
	void deleteObstacle(Sprite* s);
	void moveObstacles(Vector<Sprite *> v);

private:

	bool onTouchBegan(Touch* touch, Event* event);
	//void onTouchEnded(Touch* touch, Event* event);

	void checkCollisions(Vector<Sprite *> v);

	void menuCloseCallback(cocos2d::Ref* pSender);

	//implement the "static create()" method manually
	CREATE_FUNC(Race);

	//virtual ~RaceScene();
};

#endif /* RACESCENE_H_ */

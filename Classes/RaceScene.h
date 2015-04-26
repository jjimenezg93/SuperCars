/*
 * RaceScene.h
 *
 *  Created on: 17/04/2015
 *      Author: jjimenezg93
 */

#ifndef RACESCENE_H_
#define RACESCENE_H_

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;

class Race : public cocos2d::Layer {
public:
	static cocos2d::Scene* createScene();
	virtual bool init();

	Vec2 origin;
	Size visibleSize;	// screen 1024x600

	float _speed;
	short _difficulty;
	short _timeStopped;
	short _laps;
	short _currentLap;
	short _opponents;
	short _currentPosition;

	TMXTiledMap* _tileMap;
	TMXTiledMap* _tileAuxiliarMap;

	CocosDenshion::SimpleAudioEngine* audio;

	Label* lapLabel;
	Label* posLabel;

	// playerPos = {0, 1, 2}, beeing 0 = left, 1 = center, 2 = right
	short playerPos;

	cocos2d::Sprite* player;
	cocos2d::Sprite* leftArrow;
	cocos2d::Sprite* rightArrow;

	Vector<Sprite *> _obstacles;

	void createControls(Vec2 origin, Size visibleSize);	//creates control arrows and appropiate listeners

	void update(float delta);	// default scheduled method

	void createObstacle(float dtime);	//custom scheduled method. Adds random obstacles to the road

	void carStopped(float dtime);	//custom scheduled method. What to do when a collision is detected

	void moveMap(float dtime);

	void checkLap(float dtime);

	/** Managing obstacles **/
	void moveObstacles(Vector<Sprite *> v);
	void deleteObstacle(Sprite* s);

private:

	bool onTouchBegan(Touch* touch, Event* event);
	//void onTouchEnded(Touch* touch, Event* event);

	void scheduleAll();		//method which schedules update(), moveMap() and createObstacle()

	void createMenu();
	void createLapLine();
	void updateLapsLabel();
	void updatePosLabel();

	//Checks if there is a collision between each obstacle on the road and the player
	void checkCollisions(Vector<Sprite *> v);

	void menuCloseCallback(cocos2d::Ref* pSender);

	//implement the "static create()" method manually
	CREATE_FUNC(Race);

	//virtual ~RaceScene();
};

#endif /* RACESCENE_H_ */

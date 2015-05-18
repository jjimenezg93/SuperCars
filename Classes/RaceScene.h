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
	static Scene* createScene();
	virtual bool init();

	Vec2 origin;
	Size visibleSize;	// screen 1024x600

	CocosDenshion::SimpleAudioEngine* audio;

	Label* lapLabel;
	Label* posLabel;
	Label* timerLabel;

	// playerPos = {0, 1, 2}, beeing 0 = left, 1 = center, 2 = right
	short playerPos;

	float time;

	Sprite* player;
	Sprite* leftArrow;
	Sprite* rightArrow;

	void update(float delta);	// default scheduled method

	void createObstacle(float delta);	//custom scheduled method. Adds random obstacles to the road

	void carStopped(float delta);	//custom scheduled method. What to do when a collision is detected

	void moveMap(float delta);

	void moveInvOpponents(float delta);

	void checkPosition(float delta);

	void checkLap(float delta);

	void timerMethod(float delta);

private:
	TMXTiledMap* _tileMap;
	TMXTiledMap* _tileAuxiliarMap;

	short _speed;
	short _difficulty;
	short _timeStopped;
	short _laps;
	short _currentLap;
	short _opponents;
	short _currentPosition;
	float _fastestLap;

	Vector<Sprite*> _obstacles;
	float _lapsTime[10];

	void spawnOpponents();

	void createControls(Vec2 origin, Size visibleSize);	//creates control arrows and appropiate listeners

	/** Managing obstacles **/
	void moveObstacles(Vector<Sprite*> v);
	void deleteObstacle(Sprite* s);

	bool onTouchBegan(Touch* touch, Event* event);

	void scheduleAll();		//method which schedules update(), moveMap() and createObstacle()

	void createMenu();
	void createLapLine();
	void updateLapsLabel();
	void updatePosLabel();

	//Checks if there is a collision between each obstacle on the road and the player
	void checkCollisions(Vector<Sprite*> v);
	void checkOpponentCollisions(Sprite* v);

	void checkDeletion(Vector<Sprite*> v);

	void showEndRace(Ref* pSender);
	void showRaceMenu(Ref* pSender);

	//implement the "static create()" method manually
	CREATE_FUNC(Race);

	//virtual ~RaceScene();
};

#endif /* RACESCENE_H_ */

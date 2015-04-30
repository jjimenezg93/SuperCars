/*
 * RaceMenuScene.h
 *
 *  Created on: 30/04/2015
 *      Author: jjimenezg93
 */

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

#ifndef RACEMENUSCENE_H_
#define RACEMENUSCENE_H_

using namespace cocos2d;

class RaceMenu: public cocos2d::Layer {
public:
	static cocos2d::Scene* createScene();
	virtual bool init();

	Vec2 origin;
	Size visibleSize; // screen 1024x600

	short _difficulty;
	short _laps;
	short _opponents;

	CocosDenshion::SimpleAudioEngine* audio;

private:
	void resumeRace(cocos2d::Ref* pSender);
	void quitRace(cocos2d::Ref* pSender);

	//implement the "static create()" method manually
	CREATE_FUNC(RaceMenu);

	//virtual ~RaceMenu();
};

#endif /* RACEMENUSCENE_H_ */

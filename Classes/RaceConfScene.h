/*
 * RaceConfScene.h
 *
 *  Created on: 26/04/2015
 *      Author: jjimenezg93
 */

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

#ifndef RACECONFSCENE_H_
#define RACECONFSCENE_H_

using namespace cocos2d;

class RaceConf: public cocos2d::Layer {
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
	void startRace(cocos2d::Ref* pSender);
	void backMainMenu(cocos2d::Ref* pSender);

	//implement the "static create()" method manually
	CREATE_FUNC(RaceConf);

	//virtual ~RaceConf();
};

#endif /* RACECONFSCENE_H_ */

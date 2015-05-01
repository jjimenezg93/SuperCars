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

class RaceMenu: public Layer {
public:
	static Scene* createScene();
	virtual bool init();

	Vec2 origin;
	Size visibleSize; // screen 1024x600


private:
	Sprite* _background;

	void createMenu();

	void resumeRace(Ref* pSender);
	void quitRace(Ref* pSender);

	//implement the "static create()" method manually
	CREATE_FUNC(RaceMenu);

	//virtual ~RaceMenu();
};

#endif /* RACEMENUSCENE_H_ */

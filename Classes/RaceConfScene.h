/*
 * RaceConfScene.h
 *
 *  Created on: 26/04/2015
 *      Author: jjimenezg93
 */

#include "cocos2d.h"

#ifndef RACECONFSCENE_H_
#define RACECONFSCENE_H_

using namespace cocos2d;

class RaceConf: public Layer {
public:
	static Scene* createScene();
	virtual bool init();

	Vec2 origin;
	Size visibleSize; // screen 1024x600

private:
	Sprite* _background;

	short _difficulty;
	short _laps;
	short _opponents;

	void createMenu();

	void startRace(Ref* pSender);
	void backMainMenu(Ref* pSender);

	//implement the "static create()" method manually
	CREATE_FUNC(RaceConf);

	//virtual ~RaceConf();
};

#endif /* RACECONFSCENE_H_ */

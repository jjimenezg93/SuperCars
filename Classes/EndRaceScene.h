/*
 * EndRaceScene.h
 *
 *  Created on: 30/04/2015
 *      Author: jjimenezg93
 */

#include "cocos2d.h"

#ifndef ENDRACESCENE_H_
#define ENDRACESCENE_H_

using namespace cocos2d;

class EndRace: public Layer {
public:
	static Scene* createScene();
	virtual bool init();

	Vec2 origin;
	Size visibleSize; // screen 1024x600


private:
	Sprite* _background;

	void createMenu();

	void backMainMenu(Ref* pSender);

	//implement the "static create()" method manually
	CREATE_FUNC(EndRace);

	//virtual ~EndRace();
};

#endif /* ENDRACESCENE_H_ */

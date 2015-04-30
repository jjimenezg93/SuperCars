/*
 * MainMenuScene.h
 *
 *  Created on: 26/04/2015
 *      Author: jjimenezg93
 */

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

#ifndef MAINMENUSCENE_H_
#define MAINMENUSCENE_H_

using namespace cocos2d;

class MainMenu: public cocos2d::Layer {
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
	void createMenu();

	void playGame(cocos2d::Ref* pSender);
	void showRanking(cocos2d::Ref* pSender);
	void exitGame(cocos2d::Ref* pSender);

	//implement the "static create()" method manually
	CREATE_FUNC(MainMenu);

	//virtual ~MainMenu();
};

#endif /* MAINMENUSCENE_H_ */

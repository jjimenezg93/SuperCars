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

class MainMenu: public Layer {
public:
	static Scene* createScene();
	virtual bool init();

	Vec2 origin;
	Size visibleSize; // screen 1024x600

private:
	Sprite* _background;
	Sprite* _logo;

	void createMenu();

	void playGame(Ref* pSender);
	void showRanking(Ref* pSender);
	void exitGame(Ref* pSender);

	//implement the "static create()" method manually
	CREATE_FUNC(MainMenu);

	//virtual ~MainMenu();
};

#endif /* MAINMENUSCENE_H_ */

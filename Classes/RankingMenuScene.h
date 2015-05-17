/*
 * RankingMenuScene.h
 *
 *  Created on: 09/05/2015
 *      Author: jjimenezg93
 */

#include "cocos2d.h"

#ifndef RANKINGMENUSCENE_H_
#define RANKINGMENUSCENE_H_

using namespace cocos2d;

class RankingMenu: public Layer {
public:
	static Scene* createScene();
	virtual bool init();

	Vec2 origin;
	Size visibleSize; // screen 1024x600

private:
	Sprite* _background;

	void createMenu();

	void resetRanking(Ref* pSender);
	void backMainMenu(Ref* pSender);

	//implement the "static create()" method manually
	CREATE_FUNC(RankingMenu);

	//virtual ~RankingMenu();
};

#endif /* RANKINGMENUSCENE_H_ */

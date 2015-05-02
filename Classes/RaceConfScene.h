/*
 * RaceConfScene.h
 *
 *  Created on: 26/04/2015
 *      Author: jjimenezg93
 */

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"

#ifndef RACECONFSCENE_H_
#define RACECONFSCENE_H_

using namespace cocos2d;
using namespace cocos2d::extension;
using namespace ui;

class RaceConf: public Layer {
public:
	static Scene* createScene();
	virtual bool init();

	Vec2 origin;
	Size visibleSize; // screen 1024x600

	Label* nameLabel;
	Label* difficultyLabel;
	Label* opponentsLabel;
	Label* lapsLabel;

private:
	short _margin;

	Sprite* _background;

	EditBox* _playerName;
	ControlSlider* _difficultySlider;
	ControlSlider* _opponentsSlider;
	ControlSlider* _lapsSlider;

	Label* _difficultyValue;
	Label* _opponentsValue;
	Label* _lapsValue;

	void createMenu();
	void createConfMenu();

	void startRace(Ref* pSender);
	void backMainMenu(Ref* pSender);

	void diffUpdate(float delta);
	void oppUpdate(float delta);
	void lapsUpdate(float delta);

	//implement the "static create()" method manually
	CREATE_FUNC(RaceConf);

	//virtual ~RaceConf();
};

#endif /* RACECONFSCENE_H_ */

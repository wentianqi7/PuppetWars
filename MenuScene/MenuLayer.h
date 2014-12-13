#ifndef __MENULAYER_H__
#define __MENULAYER_H__
#include "cocos2d.h"

using namespace cocos2d;

class MenuLayer :public CCLayer{
public:
	CREATE_FUNC(MenuLayer);
	CCSprite* background;
	virtual bool init();
	static const float TRANSIT_TIME;
	static const float GAP;
	static const ccColor3B COLOR;
	static MenuLayer* thisMenu;
	static MenuLayer* getThisMenu();

	//CCLabelTTF* hscore;
	//virtual void update(float dt);

	int getHighestLevel();
	void setHighestLevel(int level);
	int m_highest_level;		//highest game level can play

	//touch event
	virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
	//virtual void ccTouchMoved(CCTouch* pTouch, CCEvent* pEvent);
	virtual void registerWithTouchDispatcher();

	//menu index "Battle", "Equip", "Ability", "Shop", "Friend"
	static const int NUM_LABEL = 6;
	CCLabelTTF* labelList[NUM_LABEL];
};


#endif

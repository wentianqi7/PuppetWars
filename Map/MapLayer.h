#ifndef __MAPLAYER_H__
#define __MAPLAYER_H__
#include "cocos2d.h"

using namespace cocos2d;

class MapLayer :public CCLayer{
public:
	CREATE_FUNC(MapLayer);
	CCSprite* background;
	CCSprite* backIcon;
	virtual bool init();
	virtual void onEnterTransitionDidFinish();
	int m_level;		//current game level
	int m_highest_level;		//highest game level can play : stored in menu layer

	void unlockNextLevel();
	void setLevelAccomp();
	static MapLayer* thisMap;
	static MapLayer* getMap();

	//touch event
	virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
	virtual void ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent);
	virtual void registerWithTouchDispatcher();

	struct LevelIcon
	{
		CCSprite* levelSprite;
		CCRect boundingRect;
		int levelState;		//0 - lock, 1 - active, 2 - complete
	};
	static const int NUM_LEVEL = 5;
	LevelIcon levelList[NUM_LEVEL];

	enum LevelState{
		Lock = 0,
		Active,
		Complete
	};
};


#endif

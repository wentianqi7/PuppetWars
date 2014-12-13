#ifndef __EQUIPLAYER_H__
#define __EQUIPLAYER_H__
#include "cocos2d.h"
#include "Paint.h"

using namespace cocos2d;

class EquipLayer :public CCLayer{
public:
	CREATE_FUNC(EquipLayer);
	CCSprite* background;
	CCSprite* focus;
	CCSprite* confirm;
	//const float TRANSIT_TIME = 1.0f;
	virtual bool init();
	void savePaintImage();
	static CCImage* sharedImage;

	//touch event
	virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
	//virtual void ccTouchMoved(CCTouch* pTouch, CCEvent* pEvent);
	virtual void ccTouchEnded(CCTouch  *pTouch, CCEvent *pEvent);
	virtual void registerWithTouchDispatcher();

	Paint* paint;
};


#endif

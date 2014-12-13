#ifndef __PAINT_H__
#define __PAINT_H__

#include "cocos2d.h"
using namespace cocos2d;

class Paint :public CCLayer{
public:
	CREATE_FUNC(Paint);
	virtual bool init();
	CCRenderTexture* getTarget();

	CCRenderTexture* target;
	CCSprite* brush;

	//touch event
	//virtual void ccTouchesBegan(CCSet* pTouches, CCEvent* pEvent);
	virtual void ccTouchesMoved(CCSet* pTouches, CCEvent* pEvent);
	//virtual void ccTouchEnded(CCTouch  *pTouch, CCEvent *pEvent);
	//virtual void registerWithTouchDispatcher();

	void onEnter();
	void onExit();
};

#endif

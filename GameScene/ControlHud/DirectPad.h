#ifndef _DIRECTPAD_H__
#define _DIRECTPAD_H__

#include "cocos2d.h"

using namespace cocos2d;

class DirectPad;
class DirectPadDelegate{
public:
	//change
	virtual void didChangeDirectionTo(DirectPad* dPad, CCPoint direction) = 0;
	//hold on
	virtual void isHoldingDirection(DirectPad* dPad, CCPoint direction) = 0;
	//end
	virtual void dPadTouchEnded(DirectPad* dPad) = 0;
};

class DirectPad :public CCSprite, public CCTargetedTouchDelegate{
public:
	DirectPad(void);
	~DirectPad(void);
	static DirectPad* dPadWithFile(CCString* filename, float radius);
	bool initWithFile(CCString* filename, float radius);

	void onEnterTransitionDidFinish();
	void onExit();
	void update(float dt);

	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);

	void updateDirectionForTouchLocation(CCPoint location);
	CC_SYNTHESIZE(DirectPadDelegate*, _delegate, Delegate);
	CC_SYNTHESIZE(bool, _isHeld, IsHeld);

protected:
	float _radius;
	CCPoint _direction;
};

#endif

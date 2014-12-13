#ifndef __GAMELAYER_H__
#define __GAMELAYER_H__
#include "cocos2d.h"
#include "HudLayer.h"

//#include "HeroLayer.h"
class HeroLayer;
class Hero;
class ArtiIntellegence;

using namespace cocos2d;

class GameLayer :public CCLayer, public DirectPadDelegate{
public:
	CREATE_FUNC(GameLayer);
	CCSprite* background;
	CCSprite* backIcon;
	virtual bool init();
	virtual void onEnterTransitionDidFinish();

	static GameLayer* thisGame;
	static GameLayer* getGame();
	HeroLayer* heroLayer;
	Hero* my_hero;
	Hero* enemy;
	CC_SYNTHESIZE(ArtiIntellegence*, _enemyAI, ArtiIntellegence);
	bool isGameEnded;
	void doGameEnd(CCNode* pSender);

	//touch event
	virtual void ccTouchesBegan(CCSet* pTouch, CCEvent* pEvent);
	virtual void ccTouchesMoved(CCSet* pTouch, CCEvent* pEvent);
	virtual void ccTouchesEnded(CCSet* pTouch, CCEvent* pEvent);
	virtual void registerWithTouchDispatcher();


	//control pad
	virtual void didChangeDirectionTo(DirectPad *dPad, CCPoint direction);
	virtual void isHoldingDirection(DirectPad *dPad, CCPoint direction);
	virtual void dPadTouchEnded(DirectPad *dPad);

	CC_SYNTHESIZE(HudLayer*, _hud, Hud);
	

	//gesture detection
	enum  GDirection{
		kGDirectionUp = 0,
		kGDirectionDown,
		kGDirectionLeft,
		kGDirectionRight,
		KGDirectionCircle,
		kGDirectionNo
	};

	bool b_click;
	bool b_debug;
	bool b_rect;
	bool b_circle;
	bool cache_deirectionshape[4];
	int count_order;
	GDirection gd_direction;
	CCPoint ccp_last, ccp_now;
};


#endif

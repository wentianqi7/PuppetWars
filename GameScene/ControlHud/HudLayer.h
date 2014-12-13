#ifndef _HUDLAYER_H__
#define _HUDLAYER_H__

#include "cocos2d.h"
#include "DirectPad.h"

using namespace cocos2d;

class HudLayer :public CCLayer{
public:
	HudLayer(void);
	~HudLayer(void);
	CREATE_FUNC(HudLayer);
	bool init();
	CC_SYNTHESIZE(DirectPad*, _dPad, Dpad);
	
};

#endif

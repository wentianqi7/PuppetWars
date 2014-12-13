#pragma once
#include "cocos2d.h"
#include "MenuLayer.h"

USING_NS_CC;

class MenuScene :public CCScene{
public:
	MenuScene(void);
	~MenuScene(void);
	CREATE_FUNC(MenuScene);
	virtual bool init();
	//MenuLayer* _menuLayer;
	CC_SYNTHESIZE(MenuLayer*, _menuLayer, MenuLayer);
};

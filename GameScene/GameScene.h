#pragma once
#include "cocos2d.h"
#include "Gamelayer.h"

USING_NS_CC;

class GameScene :public CCScene{
public:
	GameScene(void);
	~GameScene(void);
	CREATE_FUNC(GameScene);
	virtual bool init();
	//GameLayer* _gameLayer;
	CC_SYNTHESIZE(GameLayer*, _gameLayer, GameLayer);
	CC_SYNTHESIZE(HudLayer*, _hudLayer, HudLayer);
};

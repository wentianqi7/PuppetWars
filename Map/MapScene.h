#pragma once
#include "cocos2d.h"
#include "MapLayer.h"

USING_NS_CC;

class MapScene :public CCScene{
public:
	MapScene(void);
	~MapScene(void);
	CREATE_FUNC(MapScene);
	virtual bool init();
	//MenuLayer* _menuLayer;
	CC_SYNTHESIZE(MapLayer*, _mapLayer, MapLayer);
};

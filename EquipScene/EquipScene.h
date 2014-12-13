#pragma once
#include "cocos2d.h"
#include "EquipLayer.h"

USING_NS_CC;

class EquipScene :public CCScene{
public:
	EquipScene(void);
	~EquipScene(void);
	CREATE_FUNC(EquipScene);
	virtual bool init();

	CC_SYNTHESIZE(EquipLayer*, _equipLayer, EquipLayer);
	CC_SYNTHESIZE(Paint*, _paint, Paint);
};

#include "EquipScene.h"

EquipScene::EquipScene(void){
	_equipLayer = NULL;
}

EquipScene::~EquipScene(void){

}

bool EquipScene::init(){
	bool bRet = false;
	do{
		CC_BREAK_IF(!CCScene::init());

		//equipLayer
		_equipLayer = EquipLayer::create();
		CC_BREAK_IF(!_equipLayer);
		this->addChild(_equipLayer);

		//paint layer
		_paint = Paint::create();
		this->addChild(_paint);

		bRet = true;
	} while (0);

	return bRet;
}

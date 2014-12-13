#include "MapScene.h"

MapScene::MapScene(void){
	_mapLayer = NULL;
}

MapScene::~MapScene(void){

}

bool MapScene::init(){
	bool bRet = false;
	do{
		CC_BREAK_IF(!CCScene::init());

		//menuLayer
		_mapLayer = MapLayer::create();
		CC_BREAK_IF(!_mapLayer);
		this->addChild(_mapLayer);

		bRet = true;
	} while (0);

	return bRet;
}

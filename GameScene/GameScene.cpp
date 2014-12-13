#include "GameScene.h"
#include "HudLayer.h"

GameScene::GameScene(void){
	_gameLayer = NULL;
	_hudLayer = NULL;
}

GameScene::~GameScene(void){

}

bool GameScene::init(){
	bool bRet = false;
	do{
		CC_BREAK_IF(!CCScene::init());
		
		//gameLayer
		_gameLayer = GameLayer::create();
		CC_BREAK_IF(!_gameLayer);
		this->addChild(_gameLayer);

		//hudLayer
		_hudLayer = HudLayer::create();
		this->addChild(_hudLayer);
		_hudLayer->getDpad()->setDelegate(_gameLayer);
		_gameLayer->setHud(_hudLayer);

		bRet = true;
	} while (0);

	return bRet;
}

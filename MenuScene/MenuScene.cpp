#include "MenuScene.h"

MenuScene::MenuScene(void){
	_menuLayer = NULL;
}

MenuScene::~MenuScene(void){

}

bool MenuScene::init(){
	bool bRet = false;
	do{
		CC_BREAK_IF(!CCScene::init());
		
		//menuLayer
		_menuLayer = MenuLayer::create();
		CC_BREAK_IF(!_menuLayer);
		this->addChild(_menuLayer);

		bRet = true;
	} while (0);

	return bRet;
}

#include "HudLayer.h"

HudLayer::HudLayer(void){
	_dPad = NULL;
}

HudLayer::~HudLayer(void){

}

bool HudLayer::init(){
	bool bRet = true;
	do{
		CC_BREAK_IF(!CCLayer::init());
		//first argument is the sprite, second is radius
		float radius=CCDirector::sharedDirector()->getWinSize().width / 8;
		_dPad = DirectPad::dPadWithFile(CCString::create("ui/dpad.png"), radius);
		_dPad->setPosition(ccp(radius, radius));
		//_dPad->setOpacity(100);
		this->addChild(_dPad);

		bRet = true;
	} while (0);

	return bRet;
}

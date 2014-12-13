#include "EquipLayer.h"

USING_NS_CC;

CCImage* EquipLayer::sharedImage = NULL;
bool EquipLayer::init(){
	bool bRet = false;

	do{
		CC_BREAK_IF(!CCLayer::init());
		CCDirector* pDirector = CCDirector::sharedDirector();
		CCSize winSize = pDirector->getWinSize();

		//add paint layer
		this->paint = Paint::create();
		this->addChild(paint);

		//add background
		background = CCSprite::create("ui/equip_bg.png");
		background->setAnchorPoint(ccp(0, 0));
		background->setPosition(ccp(0, 0));
		this->addChild(background);

		//add focus
		focus = CCSprite::create("ui/focus.png");
		focus->setAnchorPoint(ccp(0.5, 0.5));
		focus->setPosition(ccp(440, 145));
		this->addChild(focus);

		/*获得focus point的中心位置
		CCSprite* focus2 = CCSprite::create("ui/dpad.png");
		focus2->setPosition(focus->getPosition());
		this->addChild(focus2);
		*/

		//add confirm button
		confirm = CCSprite::create("ui/confirm.png");
		confirm->setScale(0.2);
		confirm->setPosition(ccp(winSize.width-confirm->getContentSize().width/10,
			confirm->getContentSize().height/10));
		this->addChild(confirm);

		//enable touch
		this->setTouchEnabled(true);

		bRet = true;
	} while (0);

	return bRet;
}

void EquipLayer::registerWithTouchDispatcher()
{
	CCDirector *pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
	//pDirector->getTouchDispatcher()->addStandardDelegate(this, 0);
}

bool EquipLayer::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent){
	CCPoint location = pTouch->getLocation();
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	float icon_radius = confirm->getContentSize().width / 10;
	//distance from icon target to center of the sprite
	float distanceSQ = ccpDistanceSQ(location, confirm->getPosition());
	if (distanceSQ <= icon_radius * icon_radius){
		//change the icon to pressed
		CCSpriteFrame* frame = CCSpriteFrame::create
			("ui/confirm_pressed.png", CCRectMake(0, 0, 512, 512));
		confirm->setDisplayFrame(frame);
		
		return true;
	}
	return false;
}

void EquipLayer::ccTouchEnded(CCTouch  *pTouch, CCEvent *pEvent){
	//call save image
	CCSpriteFrame* frame = CCSpriteFrame::create
		("ui/confirm.png", CCRectMake(0, 0, 512, 512));
	confirm->setDisplayFrame(frame);
	savePaintImage();
}

void EquipLayer::savePaintImage(){
	//无法储存
	//this->paint->getTarget()->saveToFile
	//	("../../Resources/inventory/screenshot.png", kCCImageFormatPNG);
	CCImage *image = this->paint->getTarget()->newCCImage();
	
	//if (image->saveToFile("inventory/weapon.png", false)) {
		//CCLog("Succeed!");
		sharedImage = image;
		CCDirector* pDirector = CCDirector::sharedDirector();
		pDirector->popScene();
	//}
	//else {
		//CCLog("Fail!");
	//}
}

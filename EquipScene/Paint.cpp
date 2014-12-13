#include "Paint.h"

bool Paint::init(){
	bool bRet = false;
	CCPoint canvasPos = ccp(482.5, 235);
	CCSize canvasSize = CCSize(335, 430);

	do{
		CC_BREAK_IF(!CCLayer::init());
		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		target = CCRenderTexture::create(canvasSize.width, canvasSize.height,
			//kCCTexture2DPixelFormat_RGB888);
			kCCTexture2DPixelFormat_RGBA8888);
		target->retain();
		target->setPosition(canvasPos);
		this->addChild(target);

		brush = CCSprite::create("ui/largeBrush.png");
		brush->setScale(0.4);
		brush->retain();
		setTouchEnabled(true);
		bRet = true;
	} while (0);

	return bRet;
}
/*
void Paint::registerWithTouchDispatcher()
{
	CCDirector *pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
	//pDirector->getTouchDispatcher()->addStandardDelegate(this, 1);
}*/

void Paint::onEnter(){
	CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this, 1);
}

void Paint::onExit(){
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

void Paint::ccTouchesMoved(CCSet* pTouches, CCEvent* pEvent){
	CCTouch *touch = (CCTouch*)pTouches->anyObject();
	CCPoint start = touch->getLocationInView();
	CCDirector* pDirector = CCDirector::sharedDirector();
	start = pDirector->convertToGL(start);
	CCPoint end = touch->getPreviousLocationInView();
	end = pDirector->convertToGL(end);

	target->begin();
	
	float distance = ccpDistance(start, end);
	float x_offset = 315;
	float y_offset = 20;

	for (int i = 0; i < distance; i++){
		float difx = end.x - start.x;
		float dify = end.y - start.y;
		float delta = (float)i / distance;
		brush->setPosition(ccp(start.x + (difx * delta) - x_offset, 
			start.y + (dify * delta) - y_offset));
		brush->visit();
	}

	target->end();
}

CCRenderTexture* Paint::getTarget(){
	return target;
}

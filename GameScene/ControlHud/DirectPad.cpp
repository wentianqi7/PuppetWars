#include "DirectPad.h"
USING_NS_CC;

DirectPad::DirectPad(void){
	_delegate = NULL;
}

DirectPad::~DirectPad(void){

}

DirectPad* DirectPad::dPadWithFile(CCString* filename, float radius){
	DirectPad* pRet = new DirectPad();
	if (pRet&&pRet->initWithFile(filename, radius)){
		float scaleRatio = radius * 2 / pRet->getContentSize().width;
		pRet->setScale(scaleRatio);
		return pRet;
	}
	else{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

bool DirectPad::initWithFile(CCString *filename, float radius)
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!CCSprite::initWithFile(filename->getCString()));
		_radius = radius;
		_direction = CCPointZero; 
		_isHeld = false;
		this->scheduleUpdate();

		bRet = true;
	} while (0);

	return bRet;
}

void DirectPad::onEnterTransitionDidFinish(){
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, true);
}

void DirectPad::onExit(){
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

void DirectPad::update(float dt){
	if (_isHeld){
		_delegate->isHoldingDirection(this, _direction);
	}
}

bool DirectPad::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent){
	CCPoint location = pTouch->getLocation();
	//distance from touch pad target to center of the sprite
	float distanceSQ = ccpDistanceSQ(location, this->getPosition());
	if (distanceSQ <= _radius*_radius){
		this->updateDirectionForTouchLocation(location);
		//turn on the touch pad
		_isHeld = true;
		return true;
	}
	return false;
}

void DirectPad::ccTouchMoved(CCTouch* pTouch, CCEvent* pEvent){
	//retrieve the coord when moved
	CCPoint location = pTouch->getLocation();
	this->updateDirectionForTouchLocation(location);
}

void DirectPad::ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent){
	_direction = CCPointZero;
	_isHeld = false;
	_delegate->dPadTouchEnded(this);
}

void DirectPad::updateDirectionForTouchLocation(CCPoint location){
	float radians = ccpToAngle(ccpSub(location, this->getPosition()));
	float degrees = -1 * CC_RADIANS_TO_DEGREES(radians);
	if (degrees <= 45 && degrees >= -45){
		//right
		_direction = ccp(1.0, 0.0);
	}
	else if (degrees >= 45 && degrees <= 135)
	{
		//bottom
		_direction = ccp(0.0, -1.0);
	}
	else if (degrees >= 135 || degrees <= -135)
	{
		//left
		_direction = ccp(-1.0, 0.0);
	}
	else if (degrees <= -45 && degrees >= -135)
	{
		//top 
		_direction = ccp(0.0, 1.0);
	}
	
	_delegate->didChangeDirectionTo(this, _direction);
}


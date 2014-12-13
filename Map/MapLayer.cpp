#include "Maplayer.h"
#include "GameScene.h"
#include "MenuLayer.h"

USING_NS_CC;

MapLayer* MapLayer::thisMap = NULL;
bool MapLayer::init(){
	bool bRet = false;

	do{
		CC_BREAK_IF(!CCLayer::init());

		CCDirector* pDirector = CCDirector::sharedDirector();
		CCSize winSize = pDirector->getWinSize();
		thisMap = this;
		m_level = 0;

		//add background
		background = CCSprite::create("ui/map/chapter0.png");
		float scaleRatio = winSize.width / background->getContentSize().width;
		background->setScale(scaleRatio);
		background->setPosition(winSize / 2);
		this->addChild(background);

		//add title
		CCSprite* chapTitle = CCSprite::create("ui/map/chapter0_title.png");
		chapTitle->setPosition(ccp(winSize.width / 2, 
			winSize.height - chapTitle->getContentSize().height / 2));
		this->addChild(chapTitle);

		//add back icon
		backIcon = CCSprite::create("ui/back.png");
		backIcon->setPosition(ccp(winSize.width - backIcon->getContentSize().width / 2,
			backIcon->getContentSize().height / 2));
		this->addChild(backIcon);


		//initiate level icons
		MenuLayer* thisMenu = MenuLayer::getThisMenu();
		for (int i = 0; i < thisMenu->getHighestLevel(); i++){
			char index[50] = "";
			sprintf(index, "ui/map/level%d.png", i);
			levelList[i].levelSprite = CCSprite::create(index);
			levelList[i].levelState = Active;
		}

		for (int i = thisMenu->getHighestLevel(); i < NUM_LEVEL; i++){
			char index[50] = "";
			sprintf(index, "ui/map/level%d_lock.png", i);
			levelList[i].levelSprite = CCSprite::create(index);
			levelList[i].levelState = Lock;
		}

		for (int i = 0; i < NUM_LEVEL; i++){
			int listSize = (int)(levelList[i].levelSprite->getContentSize().height);
			int YRange = winSize.height - listSize*4;
			levelList[i].levelSprite->setPosition(ccp(winSize.width*(i + 1) / (NUM_LEVEL + 1),
				listSize*2 + rand() % YRange));
			this->addChild(levelList[i].levelSprite);
		}

		for (int i = 0; i < NUM_LEVEL; i++){
			levelList[i].boundingRect = levelList[i].levelSprite->boundingBox();
		}

		bRet = true;
	} while (0);

	return bRet;
}

void MapLayer::registerWithTouchDispatcher(){
	CCDirector *pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

bool MapLayer::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent){
	//check if the backIcon is pressed
	CCPoint location = pTouch->getLocation();
	CCDirector* pDirector = CCDirector::sharedDirector();
	CCSize winSize = pDirector->getWinSize();
	float icon_radius = backIcon->getContentSize().height / 2;
	//distance from icon target to center of the sprite
	float distanceSQ = ccpDistanceSQ(location, backIcon->getPosition());
	if (distanceSQ <= icon_radius * icon_radius){
		//change the icon to pressed
		pDirector->popScene();
		return true;
	}

	//check if level icons are pressed
	for (int i = 0; i<NUM_LEVEL; i++){
		if (levelList[i].boundingRect.CCRect::containsPoint
			(this->getParent()->convertTouchToNodeSpace(pTouch)) == true &&
			levelList[i].levelState > Lock){
			//set curretn level to i
			m_level = i;
			return true;
		}
		else;
	}

	return false;
}

void MapLayer::ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent){
	//enter gameScene
	CCDirector* pDirector = CCDirector::sharedDirector();
	CCScene* gScene = GameScene::create();
	CCTransitionScene *reScene = NULL;

	reScene = CCTransitionCrossFade::create(1.0f, gScene);

	if (reScene){
		pDirector->pushScene(reScene);
	}
}

MapLayer* MapLayer::getMap(){
	return thisMap;
}

void MapLayer::setLevelAccomp(){
	/*
	if (levelList[m_level].levelState == Active){
		CCSpriteFrame* frame = CCSpriteFrame::create("ui/map/level_complete.png",
			CCRectMake(0, 0, 100, 100));
		levelList[m_level].levelSprite->setDisplayFrame(frame);
	}
	*/
}

void MapLayer::unlockNextLevel(){
	MenuLayer* thisMenu = MenuLayer::getThisMenu();
	m_highest_level = thisMenu->getHighestLevel();

	if (m_level == (m_highest_level - 1) && m_highest_level < 5){
		levelList[m_highest_level].levelState = Active;
		
		char index[50] = "";
		sprintf(index, "ui/map/level%d.png", m_highest_level);
		CCSpriteFrame* frame = CCSpriteFrame::create(index, CCRectMake(0, 0, 100, 100));
		levelList[m_highest_level].levelSprite->setDisplayFrame(frame);
		thisMenu->setHighestLevel(++m_highest_level);
	}
}

void MapLayer::onEnterTransitionDidFinish(){
	//enable touch
	this->setTouchEnabled(true);
}

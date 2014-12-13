#include "Menulayer.h"
#include "EquipScene.h"
#include "MapScene.h"
#include "HeroLayer.h"

USING_NS_CC;
MenuLayer* MenuLayer::thisMenu = NULL;
bool MenuLayer::init(){
	bool bRet = false;

	do{
		CC_BREAK_IF(!CCLayer::init());
		thisMenu = this;
		m_highest_level = 1;
		CCSize winSize = CCDirector::sharedDirector()->getWinSize();

		//add background
		background = CCSprite::create("ui/menu.png");
		background->setAnchorPoint(ccp(0, 0));
		background->setPosition(ccp(0, 0));
		this->addChild(background);

		//add menu
		std::string menuContent[NUM_LABEL] = { "Battle", "Equip", "Ability", "Shop", "Friend" };

		for (int i = 0; i < NUM_LABEL; i++){
			CCSprite* label_bg = CCSprite::create("ui/menu_icon.png");
			labelList[i] = CCLabelTTF::create(menuContent[i].c_str(), "Arial", 24);
			labelList[i]->setColor(ccWHITE);
			labelList[i]->setPosition(ccp(winSize.width - label_bg->getContentSize().width / 2,
				winSize.height - label_bg->getContentSize().height / 2 - 
				label_bg->getContentSize().height*i));
			this->addChild(labelList[i], 1);

			label_bg->setPosition(labelList[i]->getPosition());
			this->addChild(label_bg);
		}

		/*
		//testing
		hscore = CCLabelTTF::create("Highest Level: ", "Arial", 24);
		hscore->setPosition(winSize / 2);
		hscore->setColor(ccRED);
		this->addChild(hscore);
		//testing end
		*/

		//enable touch
		this->setTouchEnabled(true);
		this->scheduleUpdate();

		bRet = true;
	} while (0);

	return bRet;
}

void MenuLayer::registerWithTouchDispatcher()
{
	CCDirector *pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
	//pDirector->getTouchDispatcher()->addStandardDelegate(this, 0);
}

bool MenuLayer::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent){
	CCScene* mScene = MapScene::create();
	CCScene* eScene = EquipScene::create();
	CCTransitionScene *reScene = NULL;
	CCDirector* pDirector = CCDirector::sharedDirector();
	CCRect battleRect = this->labelList[0]->boundingBox();
	CCRect equipRect = this->labelList[1]->boundingBox();
	
	if (battleRect.CCRect::containsPoint(this->getParent()->convertTouchToNodeSpace(pTouch)) == true){
		reScene = CCTransitionCrossFade::create(1.0f, mScene);
	}
	else if (equipRect.CCRect::containsPoint(this->getParent()->convertTouchToNodeSpace(pTouch)) == true){
		reScene = CCTransitionCrossFade::create(1.0f, eScene);
	}
	else{
		reScene = NULL;
	}

	if (reScene){
		pDirector->pushScene(reScene);
	}

	return false;
}

MenuLayer* MenuLayer::getThisMenu(){
	return thisMenu;
}

int MenuLayer::getHighestLevel(){
	return m_highest_level;
}

void MenuLayer::setHighestLevel(int level){
	m_highest_level = level;
}

/*
void MenuLayer::update(float dt){
	char ansIndex[30] = "";
	sprintf(ansIndex, "Highest Level: %d", m_highest_level);
	hscore->setString(ansIndex);
}
*/

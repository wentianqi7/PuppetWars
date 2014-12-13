#include "Gamelayer.h"
#include "HeroLayer.h"
#include "MapLayer.h"
#include "AI.h"

USING_NS_CC;
GameLayer* GameLayer::thisGame = NULL;
bool GameLayer::init(){
	bool bRet = false;

	do{
		CC_BREAK_IF(!CCLayer::init());
		CCDirector* pDirector = CCDirector::sharedDirector();
		CCSize winSize = pDirector->getWinSize();
		thisGame = this;
		isGameEnded = false;

		//add background
		background = CCSprite::create("ui/scene_clean.png");
		//float scaleRatio = winSize.height / background->getContentSize().height;
		//background->setScale(scaleRatio);
		background->setAnchorPoint(ccp(0, 0));
		background->setPosition(ccp(0, 0));
		this->addChild(background);


		//add decoration
		CCSprite* wall = CCSprite::create("ui/wall.png");
		wall->setPosition(ccp(wall->getContentSize().width / 2, 
			105 + wall->getContentSize().height / 2));
		//wall->setOpacity(50);
		this->addChild(wall);

		CCSprite* bamboo = CCSprite::create("ui/bamboo.png");
		bamboo->setPosition(ccp(winSize.width - bamboo->getContentSize().width / 2,
			105 + bamboo->getContentSize().height / 2));
		this->addChild(bamboo);
		
		
		//add back to menu icon
		backIcon = CCSprite::create("ui/back.png");
		backIcon->setPosition(ccp(winSize.width / 2,	//winSize.width - backIcon->getContentSize().width / 2,
			winSize.height - backIcon->getContentSize().height / 2));
		this->addChild(backIcon);

		//add HeroLayer (add hero and enemy on the hero layer)
		this->heroLayer = HeroLayer::create();
		this->addChild(heroLayer);
		my_hero = this->heroLayer->getPlayerHero();
		enemy = this->heroLayer->getOpponentHero();

		bRet = true;
	} while (0);

	return bRet;
}

GameLayer* GameLayer::getGame(){
	return thisGame;
}

void GameLayer::registerWithTouchDispatcher()
{
	CCDirector *pDirector = CCDirector::sharedDirector();
	//pDirector->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
	pDirector->getTouchDispatcher()->addStandardDelegate(this, 0);
}

void GameLayer::ccTouchesBegan(CCSet* pTouch, CCEvent* pEvent){
	
	//check if the backIcon is pressed
	CCTouch *touch = (CCTouch*)pTouch->anyObject();
	CCPoint location = touch->getLocation();
	CCDirector* pDirector = CCDirector::sharedDirector();
	CCSize winSize = pDirector->getWinSize();
	float icon_radius = backIcon->getContentSize().height / 2;
	//distance from icon target to center of the sprite
	float distanceSQ = ccpDistanceSQ(location, backIcon->getPosition());
	if (distanceSQ <= icon_radius * icon_radius){
		//change the icon to pressed
		//MapLayer::getMap()->unlockNextLevel();
		pDirector->popScene();
		return;
	}
	//back button checking ended
	
	
	//gesture detection initiate
	ccp_last = touch->getLocationInView();
	ccp_last = pDirector->convertToGL(ccp_last);

	b_click = true;
	b_rect = false;
	b_circle = false;
	count_order = 0;
	gd_direction = kGDirectionNo;

	for (int i = 0; i < 4; i++){
		cache_deirectionshape[i] = false;
	}
	//gesture detection ended
}


void GameLayer::ccTouchesMoved(CCSet* pTouch, CCEvent* pEvent){
	//pattern detection
	CCTouch* touch = (CCTouch*)pTouch->anyObject();
	ccp_now = touch->getLocationInView();
	ccp_now = CCDirector::sharedDirector()->convertToGL(ccp_now);
	float distanceSQ = ccpDistanceSQ(ccp_now, ccp_last);
	float adsx = ccp_now.x - ccp_last.x;
	float adsy = ccp_now.y - ccp_last.y;

	if (abs(adsx) > 10 || abs(adsy) > 10){
		b_click = false;
	}


	if (abs(adsx) > abs(adsy)){
		if (adsx < 0){
			cache_deirectionshape[0] = 1;
			gd_direction = kGDirectionLeft;
		}
		else{
			cache_deirectionshape[1] = 1;
			gd_direction = kGDirectionRight;
		}
	}
	else{
		if (adsy < 0){
			cache_deirectionshape[2] = 1;
			gd_direction = kGDirectionDown;
		}
		else{
			cache_deirectionshape[3] = 1;
			gd_direction = kGDirectionUp;
		}
	}
	
	int x = 0;
	for (int i = 0; i < 4; i++){
		if (cache_deirectionshape[i]){
			x++;
		}
	}

	if (x >= 3){
		b_circle = true;
	}
	
}


void GameLayer::ccTouchesEnded(CCSet* pTouch, CCEvent* pEvent){
	if (isGameEnded){
		return;
	}

	//click
	if (b_click){
		//attack check
		if (my_hero->heroState == Hero::IDLE || my_hero->heroState == Hero::MOVE){
			my_hero->actionAtAttack(Hero::HitLight);
		}
		//attack check ended
		return;
	}

	//circle
	if (b_circle){
		if (my_hero->heroState == Hero::IDLE){
			my_hero->actionAtDefence(Hero::Adamantine);
		}
		return;
	}

	//Gesture Left
	if (gd_direction==kGDirectionLeft){
		if (my_hero->heroState == Hero::IDLE || my_hero->heroState == Hero::MOVE){
			if (my_hero->getDirection() < 0){
				//roll
				my_hero->actionAtAttack(Hero::RollAndHit);
			}
			else{
				//back kick
				my_hero->actionAtAttack(Hero::KickLight);
			}
		}
		else if (my_hero->heroState == Hero::RECOVER && my_hero->cbState == Hero::ComboTwo &&
			my_hero->getDirection() < 0){
			//combo final (3)
			my_hero->actionAtAttack(Hero::HitHeavy);
		}
	}
	else if (gd_direction == kGDirectionRight){
		if (my_hero->heroState == Hero::IDLE || my_hero->heroState == Hero::MOVE){
			if (my_hero->getDirection() > 0){
				//roll
				my_hero->actionAtAttack(Hero::RollAndHit);
			}
			else{
				//back kick
				my_hero->actionAtAttack(Hero::KickLight);
			}
		}
		else if (my_hero->heroState == Hero::RECOVER && my_hero->cbState == Hero::ComboTwo &&
			my_hero->getDirection() > 0){
			//combo final (3)
			my_hero->actionAtAttack(Hero::HitHeavy);
		}
	}
	else if (gd_direction == kGDirectionDown){
		if (my_hero->heroState == Hero::RECOVER && my_hero->cbState == Hero::ComboOne){
			//combo 2
			my_hero->actionAtAttack(Hero::HitLightComboSecond);
		}
	}
	else if (gd_direction == kGDirectionUp){
		if (my_hero->heroState == Hero::MOVE || my_hero->heroState == Hero::IDLE){
			//parry
			my_hero->actionAtDefence(Hero::Parry);
		}

		if (my_hero->heroState == Hero::RECOVER && my_hero->cbState == Hero::ComboInint){
			//combo 1
			my_hero->actionAtAttack(Hero::HitLightComboFirst);
		}
	}

}

void GameLayer::didChangeDirectionTo(DirectPad *dPad, CCPoint direction){
	//move
	if (direction.x){
		if (my_hero->heroState == Hero::IDLE){
			my_hero->actionAtMove();
		}
	}

	//shield start
	if (direction.y < 0){
		if (my_hero->heroState == Hero::IDLE || my_hero->heroState == Hero::MOVE){
			my_hero->actionAtDefence(Hero::Shield);
		}
	}
}

void GameLayer::isHoldingDirection(DirectPad *dPad, CCPoint direction){
	//keep shield
	if (direction.y < 0){
		if (my_hero->heroState == Hero::IDLE || my_hero->heroState == Hero::DEFENCE ||
			my_hero->heroState == Hero::MOVE){
			my_hero->actionAtDefence(Hero::Shield);
		}
		return;
	}

	//my_hero->actionAtMove();

	if (my_hero->heroState == Hero::IDLE){
		my_hero->actionAtMove();
	}
	float walkingSpeed = 1.5f;
	if (my_hero->heroState == Hero::MOVE){
		my_hero->walkWithDirection(direction, walkingSpeed);
	}
}

void GameLayer::dPadTouchEnded(DirectPad *dPad){
	if (my_hero->heroState == Hero::MOVE){
		my_hero->actionAtIdle();
	}
}


void GameLayer::onEnterTransitionDidFinish(){
	//enable touch
	this->setTouchEnabled(true);
	
	//add enemy AI
	do{
		_enemyAI = ArtiIntellegence::create();
		CC_BREAK_IF(!_enemyAI);
		this->addChild(_enemyAI);
	} while (0);
}

void GameLayer::doGameEnd(CCNode* pSender){
	CCDirector* pDirector = CCDirector::sharedDirector();
	CCSize winSize = pDirector->getWinSize();

	//stop all acitons
	isGameEnded = true;
	heroLayer->unscheduleUpdate();
	heroLayer->unscheduleAllSelectors();
	_enemyAI->unscheduleUpdate();
	_enemyAI->unscheduleAllSelectors();

	//check game win or not
	CCSprite* resultPad = NULL;
	if (my_hero->getLife() <= 0){
		//game lose
		resultPad = CCSprite::create("ui/lose.png");
	}
	else{
		//game win
		resultPad = CCSprite::create("ui/win.png");
		MapLayer* thisMap = MapLayer::getMap();
		thisMap->unlockNextLevel();
		thisMap->setLevelAccomp();
	}
	this->addChild(resultPad, 101);
	resultPad->setPosition(ccp(winSize.width / 2, 
		winSize.height / 2 + resultPad->getContentSize().height / 3));

	//disable touches except back to menu
	CCLayerColor* maskLayer = CCLayerColor::create(ccc4(255, 255, 255, 160), winSize.width, winSize.height);
	this->setZOrder(1);
	this->addChild(maskLayer, 100);
	backIcon->setZOrder(101);
	backIcon->setPosition(ccp(winSize.width / 2, winSize.height / 2 - backIcon->getContentSize().height));
}

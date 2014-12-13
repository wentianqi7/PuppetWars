#include "HeroLayer.h"
#include "EquipLayer.h"
#include <iostream>
#include <fstream>

HeroLayer::HeroLayer(void){

}

HeroLayer::~HeroLayer(void){

}

HeroLayer* HeroLayer::thisHeroLayer = NULL;
HeroLayer* HeroLayer::sharedHero = NULL;
HeroLayer* HeroLayer::getHeroLayer(){
	return thisHeroLayer;
}

HeroLayer* HeroLayer::create(){
	HeroLayer* hRet = new HeroLayer();
	if (hRet && hRet->init()){
		hRet->autorelease();
		sharedHero = hRet;
		return hRet;
	}
	else{
		CC_SAFE_DELETE(hRet);
		return NULL;
	}
}

bool HeroLayer::init(){
	bool bRet = false;
	do{
		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		const float STAGE_HEIGHT = 105 * winSize.height / 480;
		CC_BREAK_IF(!CCLayer::init());
		thisHeroLayer = this;

		//add enemy
		Hero* enemy = Hero::create();
		char wpIndex[30] = "";
		//random weapon for enemy hero
		int randWp = rand() % 4;
		sprintf(wpIndex, "inventory/weapon%d.png", randWp);
		CCString* weaponPath = CCString::create(wpIndex);
		this->addNewHero(enemy, weaponPath, ENEMY, E_WEAPON);
		enemy->setPosition(ccp(winSize.width * 3 / 4,
			STAGE_HEIGHT + enemy->getSprite()->getContentSize().height / 2));
		enemy->setDirection(-1);
		enemy->setScaleX(-1);
		setOpponentHero(enemy);

		//add hero
		Hero* hero = Hero::create();
		weaponPath = CCString::create("inventory/weapon.png");
		this->addNewHero(hero, weaponPath, HERO, H_WEAPON);

		hero->setPosition(ccp(winSize.width / 4, 
			STAGE_HEIGHT + hero->getSprite()->getContentSize().height / 2));
		hero->setDirection(1);
		setPlayerHero(hero);
		//hero->getSprite()->stopAllActions();


		//add health point bar
		//for hero
		hBar = CCSprite::create("ui/health_point_bar.png");
		float length = winSize.width / 2 - 60;
		float scaleX = length / hBar->getContentSize().width;
		hBar->setAnchorPoint(ccp(1, 0));
		hBar->setPosition(ccp(winSize.width / 2 - 50, winSize.height - hBar->getContentSize().height));
		hBar->setScaleX(scaleX);
		this->addChild(hBar);

		//for enenmy
		eBar=CCSprite::create("ui/health_point_bar.png");
		eBar->setAnchorPoint(ccp(1, 0));
		eBar->setPosition(ccp(winSize.width / 2 + 50, hBar->getPositionY()));
		eBar->setScaleX(-scaleX);
		this->addChild(eBar);

		this->scheduleUpdate();
	
		bRet = true;
	} while (0);

	return bRet;
}

void HeroLayer::addNewHero(Hero* hero, CCString* weaponPath, int hTag, int wTag){
	CCSprite* heroSprite = NULL;
	CCSprite* weaponSprite = NULL;
	
	//weaponSprite = CCSprite::create(weaponPath->getCString());
	/*
	if (isWeaponExist(weaponPath, Hero::SLASH)){
		hero->weaponType = Hero::SLASH;
	}*/
	float scaleRatio = 6;

	switch (hero->weaponType){
	case Hero::FIST:
		heroSprite = CCSprite::create("hero/ready/ready1.png");
		break;
	case Hero::SLASH:
		heroSprite = CCSprite::create("hero/ready_combat_weapon/ready1.png");
		//if (isWeaponExist(weaponPath, Hero::SLASH)){
			//设置武器位置------要改

			if (EquipLayer::sharedImage == NULL || hTag == ENEMY){
				weaponSprite = CCSprite::create(weaponPath->getCString());
			}
			else{
				CCTexture2D* wTexture = new CCTexture2D();
				wTexture->initWithImage(EquipLayer::sharedImage);
				weaponSprite = CCSprite::createWithTexture(wTexture);
			}
			weaponSprite->setScale(1 / scaleRatio);
			weaponSprite->setPosition(ccp(20 / scaleRatio, -40 / scaleRatio));
			weaponSprite->setRotation(90);
		//}
		break;
	}

	//bindSprite(hSprite, wSprite, life, hTag, wTag)
	hero->bindSprite(heroSprite, weaponSprite, Hero::LIFE, hTag, wTag);


	hero->actionAtIdle();
	this->addChild(hero);
}

Hero* HeroLayer::getPlayerHero(){
	return hero_player;
}

void HeroLayer::setPlayerHero(Hero* pHero){
	hero_player = pHero;
}

Hero* HeroLayer::getOpponentHero(){
	return hero_opponent;
}

void HeroLayer::setOpponentHero(Hero* pHero){
	hero_opponent = pHero;
}

bool HeroLayer::isWeaponExist(CCString* filename, int wType){
	bool bRet = false;
	/*
	if ((_access(filename->getCString(), 0)) != -1){
		bRet = true;
	}*/
	std::fstream _file;
	_file.open(filename->getCString(), std::ios::in);
	if(_file){
		bRet = true;
	}

	return bRet;
}


void HeroLayer::update(float dt){
	//update the position of weapon
	if (this->hero_player != NULL){
		updateHeroWeaponPosition(this->hero_player);
		updateHpbar(true);
		preventHeroOutOfScreen(hero_player);
	}

	if (this->hero_opponent != NULL){
		updateHeroWeaponPosition(this->hero_opponent);
		updateHpbar(false);
		preventHeroOutOfScreen(hero_opponent);
	}

}

void HeroLayer::updateHeroWeaponPosition(Hero* tempHero){
	CCSprite* wSprite;
	float scaleRatio = 6;
	int fNum = tempHero->getStateFrameNum();
	int currentFrameIndex = -1;

	tempHero->setScaleX(tempHero->getDirection());

	if (tempHero->weaponType == Hero::FIST){
		return;
	}
	else{
		wSprite = tempHero->getWeaponSprite();
	}
	/*---------
	CCSprite* sprite = CCSprite::create("inventory/weapon.png");
	if (wSprite->getTexture() != sprite->getTexture()){
	wSprite->setTexture(sprite->getTexture());
	}-------*/

	for (int i = 0; i < fNum; i++){
		//retrieve ith frame of hero's animation
		CCAnimationFrame* pAnimationFrame =
			dynamic_cast<CCAnimationFrame*>(tempHero->heroAnimation->getFrames()->objectAtIndex(i));
		CCSpriteFrame* tempFrame = pAnimationFrame->getSpriteFrame();

		//find out the index of current sprite frame
		if (tempHero->getSprite()->getTexture()->getName() ==
			tempFrame->getTexture()->getName()){
			currentFrameIndex = i;
			break;
		}
	}

	//if(prite->displayedFrame() == anim->getFrames()->getObjectAtIndex(i)) 
	//determine which state the hero's in
	switch (tempHero->heroState){
	case Hero::IDLE:
		//determine frame number
		switch (currentFrameIndex){
		case 0:
			wSprite->setPosition(ccp(20 / scaleRatio, -40 / scaleRatio));
			wSprite->setRotation(70);
			break;
		case 1:
			wSprite->setPosition(ccp(23 / scaleRatio, -37 / scaleRatio));
			wSprite->setRotation(72);
			break;
		case 2:
			wSprite->setPosition(ccp(23 / scaleRatio, -37 / scaleRatio));
			wSprite->setRotation(74);
			break;
		case 3:
			wSprite->setPosition(ccp(23 / scaleRatio, -37 / scaleRatio));
			wSprite->setRotation(72);
			break;
		default:
			wSprite->setPosition(ccp(20 / scaleRatio, -40 / scaleRatio));
			wSprite->setRotation(180);
			break;
		}
		break;
	case Hero::ATTACK:
		switch (tempHero->weaponType){
		case Hero::SLASH:
			switch (tempHero->aType){
			case Hero::HitLight:
				switch (currentFrameIndex){
				case 0:
					wSprite->setPosition(ccp(-240 / scaleRatio, 140 / scaleRatio));
					wSprite->setRotation(-100);
					break;
				case 1:
					wSprite->setPosition(ccp(-70 / scaleRatio, 260 / scaleRatio));
					wSprite->setRotation(-60);
					break;
				case 2:
					wSprite->setPosition(ccp(470 / scaleRatio, 80 / scaleRatio));
					wSprite->setRotation(55);
					break;
				default:
					break;
				}
				break;
			case Hero::HitLightComboFirst:
				switch (currentFrameIndex){
				case 0:
					wSprite->setPosition(ccp(-280 / scaleRatio, -140 / scaleRatio));
					wSprite->setRotation(-140);
					break;
				case 1:
					wSprite->setPosition(ccp(-200 / scaleRatio, -40 / scaleRatio));
					wSprite->setRotation(-120);
					break;
				case 2:
					wSprite->setPosition(ccp(620 / scaleRatio, -20 / scaleRatio));
					wSprite->setRotation(55);
					break;
				default:
					break;
				}
				break;
			case Hero::HitLightComboSecond:
				switch (currentFrameIndex){
				case 0:
					wSprite->setPosition(ccp(260 / scaleRatio, 40 / scaleRatio));
					wSprite->setRotation(180);
					break;
				default:
					break;
				}
				break;
			case Hero::HitHeavy:
				switch (currentFrameIndex){
				case 0:
					wSprite->setPosition(ccp(-240 / scaleRatio, 220 / scaleRatio));
					wSprite->setRotation(-90);
					break;
				case 1:
					wSprite->setPosition(ccp(-160 / scaleRatio, 100 / scaleRatio));
					wSprite->setRotation(-60);
					break;
				case 2:
					wSprite->setPosition(ccp(410 / scaleRatio, 0 / scaleRatio));
					wSprite->setRotation(55);
					break;
				default:
					break;
				}
				break;
			case Hero::RollAndHit:
				switch (currentFrameIndex){
				case 0:
					wSprite->setPosition(ccp(-240 / scaleRatio, 140 / scaleRatio));
					wSprite->setRotation(-100);
					break;
				case 1:
					wSprite->setPosition(ccp(-70 / scaleRatio, 260 / scaleRatio));
					wSprite->setRotation(-60);
					break;
				case 2:
					wSprite->setPosition(ccp(470 / scaleRatio, 80 / scaleRatio));
					wSprite->setRotation(55);
					break;
				case 3:
					wSprite->setPosition(ccp(470 / scaleRatio, 80 / scaleRatio));
					wSprite->setRotation(55);
					break;
				default:
					break;
				}
				break;
				//roll end
			case Hero::KickLight:
				switch (currentFrameIndex){
				case 0:
					wSprite->setPosition(ccp(-240 / scaleRatio, 140 / scaleRatio));
					wSprite->setRotation(-100);
					break;
				case 1:
					wSprite->setPosition(ccp(-70 / scaleRatio, 260 / scaleRatio));
					wSprite->setRotation(-60);
					break;
				case 2:
					wSprite->setPosition(ccp(100 / scaleRatio, 50 / scaleRatio));
					wSprite->setRotation(100);
					break;
				default:
					break;
				}
				break;
				//kick light end
			default:
				break;
			//default for slash end
			}
			break;
		//end case slash
		case Hero::MISSILE:

			break;
		default:
			break;
		}
		break;
	//end case attack
	case Hero::DEFENCE:
		switch (tempHero->dType){
		case Hero::Shield:
			switch (currentFrameIndex){
			case 0:
				wSprite->setPosition(ccp(90 / scaleRatio, 100 / scaleRatio));
				wSprite->setRotation(-30);
				break;
			default:
				break;
			}
			break;
			//case shield end
		case Hero::Parry:
			switch (currentFrameIndex){
			case 0:
				wSprite->setPosition(ccp(-120 / scaleRatio, 340 / scaleRatio));
				wSprite->setRotation(-70);
				break;
			default:
				break;
			}
			break;
			//case parry end
		case Hero::Adamantine:
			switch (currentFrameIndex){
			case 0:
				wSprite->setPosition(ccp(-240 / scaleRatio, 140 / scaleRatio));
				wSprite->setRotation(-100);
				break;
			default:
				break;
			}
			break;
			//case adam end
		default:
			break;
		}
		//end switch dType
		break;
	//end case defence
	case Hero::MOVE:
		switch (currentFrameIndex){
		case 0:
			wSprite->setPosition(ccp(40 / scaleRatio, -50 / scaleRatio));
			wSprite->setRotation(90);
			break;
		case 1:
			wSprite->setPosition(ccp(20 / scaleRatio, -40 / scaleRatio));
			wSprite->setRotation(90);
			break;
		case 2:
			wSprite->setPosition(ccp(60 / scaleRatio, -50 / scaleRatio));
			wSprite->setRotation(70);
			break;
		case 3:
			wSprite->setPosition(ccp(80 / scaleRatio, -30 / scaleRatio));
			wSprite->setRotation(60);
			break;
		case 4:
			wSprite->setPosition(ccp(120 / scaleRatio, -30 / scaleRatio));
			wSprite->setRotation(60);
			break;
		case 5:
			wSprite->setPosition(ccp(80 / scaleRatio, -20 / scaleRatio));
			wSprite->setRotation(60);
			break;
		default:

			break;
		}
		break;
	default:
		break;
	}
}

//update length of hp bar
void HeroLayer::updateHpbar(bool isHero){
	CCSprite* tempBar = NULL;
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	float length = winSize.width / 2 - 60;
	float scaleX = length / eBar->getContentSize().width;
	float ratioX = 0;

	if (isHero){
		tempBar = hBar;
		ratioX = hero_player->getLife() / Hero::LIFE * scaleX;
	}
	else{
		tempBar = eBar;
		ratioX = -hero_opponent->getLife() / Hero::LIFE * scaleX;
	}
	
	
	tempBar->setScaleX(ratioX);
}

void HeroLayer::preventHeroOutOfScreen(Hero* pHero){
	float heroPosX = pHero->getPosition().x;
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	if (heroPosX > winSize.width){
		pHero->setPositionX(winSize.width);
	}
	else if (heroPosX < 0){
		pHero->setPositionX(0);
	}
}






#include "Hero.h"
#include "HeroLayer.h"
#include "Gamelayer.h"

Hero::Hero(void){
	m_sprite = NULL;
	w_sprite = NULL;
	m_life = 0;
	m_speed = 0;
	m_direction = 0;
	heroState = IDLE;
	aType = AttackNo;
	dType = DefenceNo;
	cbState = ComboForbid;
	weaponType = SLASH;
	heroAnimation = NULL;
	EquipWeaponImage = NULL;
}

Hero::~Hero(void){

}

Hero* Hero::create(){
	Hero* hRet = new Hero;
	hRet->autorelease();
	return hRet;
}

void  Hero::bindSprite(CCSprite* sprite, CCSprite* weapon, int life, int hTag, int wTag){
	m_sprite = sprite;
	m_life = life;
	w_sprite = weapon;
	m_baseATK = 10;
	m_baseDNF = 4;

	if(m_sprite != NULL){
		this->addChild(m_sprite, hTag);
	}

	if (w_sprite != NULL){
		this->addChild(w_sprite, wTag);
	}
}

CCSprite* Hero::getWeaponSprite(){
	return w_sprite;
}

void Hero::setWeaponSprite(CCSprite* wSprite){
	this->w_sprite = wSprite;
}

CCSprite* Hero::getSprite(){
	return m_sprite;
}

void Hero::setSprite(CCSprite* pSprite){
	this->m_sprite = pSprite;
}

float Hero::getLife(){
	return m_life;
}

void Hero::loseLife(float damage){
	m_life -= damage;
}

int Hero::getSpeed(){
	return m_speed;
}

void Hero::setSpeed(int speed){
	m_speed = speed;
}

int Hero::getDirection(){
	return m_direction;
}

void Hero::setDirection(int direction){
	m_direction = direction;
}


CCRect Hero::getBoundingBox(){
	CCRect rect = m_sprite->boundingBox();
	CCPoint pos = this->convertToWorldSpace(rect.origin);
	if (this->getDirection() < 0){
		pos.x -= rect.size.width;
	}
	CCRect heroRect(pos.x, pos.y, rect.size.width, rect.size.height);
	return heroRect;
}

//hero action
void Hero::actionAtIdle(){
	const int numOfAction = 4;
	CCArray* framelist = CCArray::createWithCapacity(numOfAction);
	this->getSprite()->stopAllActions();
	this->heroState = IDLE;

	switch (this->weaponType){
	case FIST:
		framelist->addObject(CCSpriteFrame::create
			("hero/ready/ready1.png", CCRectMake(0, 0, 35, 72)));
		framelist->addObject(CCSpriteFrame::create
			("hero/ready/ready2.png", CCRectMake(0, 0, 37, 73))); //37 56
		framelist->addObject(CCSpriteFrame::create
			("hero/ready/ready3.png", CCRectMake(0, 0, 37, 74)));	//37 56
		framelist->addObject(CCSpriteFrame::create
			("hero/ready/ready2.png", CCRectMake(0, 0, 37, 73)));
		break;
	case SLASH:
		//add action to hero
		framelist->addObject(CCSpriteFrame::create
			("hero/ready_combat_weapon/ready1.png", CCRectMake(0, 0, 29, 78)));
		framelist->addObject(CCSpriteFrame::create
			("hero/ready_combat_weapon/ready2.png", CCRectMake(0, 0, 29, 78))); //37 56
		framelist->addObject(CCSpriteFrame::create
			("hero/ready_combat_weapon/ready3.png", CCRectMake(0, 0, 29, 77)));	//37 56
		framelist->addObject(CCSpriteFrame::create
			("hero/ready_combat_weapon/ready2.png", CCRectMake(0, 0, 29, 78)));
		break;
	default:
		framelist->addObject(CCSpriteFrame::create
			("hero/ready/ready1.png", CCRectMake(0, 0, 35, 72)));
		framelist->addObject(CCSpriteFrame::create
			("hero/ready/ready2.png", CCRectMake(0, 0, 37, 73))); //37 56
		framelist->addObject(CCSpriteFrame::create
			("hero/ready/ready3.png", CCRectMake(0, 0, 37, 74)));	//37 56
		framelist->addObject(CCSpriteFrame::create
			("hero/ready/ready2.png", CCRectMake(0, 0, 37, 73)));
		break;
	}

	this->heroAnimation = CCAnimation::
		createWithSpriteFrames(framelist, 0.3f);
	CCAnimate* animate = CCAnimate::create(heroAnimation);
	this->getSprite()->runAction(CCRepeatForever::create(animate));
}

void Hero::actionAtMove(){
	CCArray* framelist = CCArray::createWithCapacity(6);
	this->getSprite()->stopAllActions();
	this->heroState = MOVE;

	framelist->addObject(CCSpriteFrame::create
		("hero/walk/walk1.png", CCRectMake(0, 0, 47, 78)));
	framelist->addObject(CCSpriteFrame::create
		("hero/walk/walk2.png", CCRectMake(0, 0, 48, 78)));
	framelist->addObject(CCSpriteFrame::create
		("hero/walk/walk3.png", CCRectMake(0, 0, 33, 76)));
	framelist->addObject(CCSpriteFrame::create
		("hero/walk/walk4.png", CCRectMake(0, 0, 29, 76)));
	framelist->addObject(CCSpriteFrame::create
		("hero/walk/walk5.png", CCRectMake(0, 0, 33, 78)));
	framelist->addObject(CCSpriteFrame::create
		("hero/walk/walk6.png", CCRectMake(0, 0, 40, 78)));


	this->heroAnimation = CCAnimation::
		createWithSpriteFrames(framelist, 0.15f);
	CCAnimate* animate = CCAnimate::create(heroAnimation);
	this->getSprite()->runAction(CCRepeatForever::create(animate));
}

void Hero::actionAtHurt(int isBack, float duration){
	CCArray* framelist = CCArray::create();
	this->getSprite()->stopAllActions();
	this->aType = AttackNo;
	this->dType = DefenceNo;
	this->cbState = ComboForbid;
	this->heroState = HURT;

	if (isBack < 0){
		//hurt at front
		framelist->addObject(CCSpriteFrame::create
			("hero/hurt/front.png", CCRectMake(0, 0, 49, 81)));
	}
	else{
		framelist->addObject(CCSpriteFrame::create
			("hero/hurt/back.png", CCRectMake(0, 0, 36, 78)));
	}

	CCFiniteTimeAction* actionMove = CCMoveTo::create(duration,
		ccp(this->getPositionX() + 50 * duration * this->getDirection() * isBack, this->getPositionY()));
	this->runAction(actionMove);
	
	this->heroAnimation = CCAnimation::createWithSpriteFrames(framelist, duration);
	CCFiniteTimeAction* animate = CCAnimate::create(heroAnimation);
	CCFiniteTimeAction* actionDone = CCCallFuncN::create(this,
		callfuncN_selector(Hero::actionToIdle));
	CCSequence* sequence = CCSequence::create(animate, actionDone, NULL);
	this->getSprite()->runAction(sequence);
}

void Hero::actionAtAttack(Hero::AttackType aType){
	CCArray* framelist = CCArray::create();
	float realMoveDuration;
	CCFiniteTimeAction* actionMove;
	this->getSprite()->stopAllActions();
	this->heroState = ATTACK;
	this->aType = aType;

	switch (this->weaponType){
	case FIST:
		framelist->addObject(CCSpriteFrame::create
			("hero/attack/punch/punch3.png", CCRectMake(0, 0, 42, 81)));
		framelist->addObject(CCSpriteFrame::create
			("hero/attack/punch/punch4.png", CCRectMake(0, 0, 103, 80)));
		break;
	case SLASH:
		switch (aType){
		case HitLight:
			framelist->addObject(CCSpriteFrame::create
				("hero/attack/slash/hit_light/slash1.png", CCRectMake(0, 0, 53, 78)));
			framelist->addObject(CCSpriteFrame::create
				("hero/attack/slash/hit_light/slash2.png", CCRectMake(0, 0, 53, 78)));
			framelist->addObject(CCSpriteFrame::create
				("hero/attack/slash/hit_light/slash3.png", CCRectMake(0, 0, 140, 78)));
			break;
		case HitLightComboFirst:
			framelist->addObject(CCSpriteFrame::create
				("hero/attack/slash/combo1/combo0.png", CCRectMake(0, 0, 86, 76)));
			framelist->addObject(CCSpriteFrame::create
				("hero/attack/slash/combo1/combo1.png", CCRectMake(0, 0, 70, 76)));
			framelist->addObject(CCSpriteFrame::create
				("hero/attack/slash/combo1/combo2.png", CCRectMake(0, 0, 196, 76)));
			realMoveDuration = 0.2f;
			actionMove = CCMoveTo::create(realMoveDuration,
				ccp(this->getPositionX() + 40 * this->getDirection(), this->getPositionY()));
			this->runAction(actionMove);

			//ability effect
			addAbilitySpecialEffect(HitLightComboFirst, ATTACK);

			break;
		case HitLightComboSecond:
			framelist->addObject(CCSpriteFrame::create
				("hero/attack/slash/combo2/combo0.png", CCRectMake(0, 0, 57, 80)));
			this->setPositionX(this->getPositionX() + 80 * this->getDirection());
			
			//ability effect
			addAbilitySpecialEffect(HitLightComboSecond, ATTACK);
			break;
		case HitHeavy:
			framelist->addObject(CCSpriteFrame::create
				("hero/attack/slash/hit_heavy/heavy0.png", CCRectMake(0, 0, 53, 80)));
			framelist->addObject(CCSpriteFrame::create
				("hero/attack/slash/hit_heavy/heavy1.png", CCRectMake(0, 0, 37, 76)));
			framelist->addObject(CCSpriteFrame::create
				("hero/attack/slash/hit_heavy/heavy2.png", CCRectMake(0, 0, 120, 76)));
			this->setPositionX(this->getPositionX() - 50 * this->getDirection());
			realMoveDuration = 0.45f;
			actionMove = CCMoveTo::create(realMoveDuration,
				ccp(this->getPositionX() + 40 * this->getDirection(), this->getPositionY()));
			this->runAction(actionMove);

			//ability effect
			addAbilitySpecialEffect(HitHeavy, ATTACK);

			break;
		case RollAndHit:
			framelist->addObject(CCSpriteFrame::create
				("hero/attack/general/roll/roll0.png", CCRectMake(0, 0, 50, 76)));
			framelist->addObject(CCSpriteFrame::create
				("hero/attack/general/roll/roll1.png", CCRectMake(0, 0, 40, 76)));
			framelist->addObject(CCSpriteFrame::create
				("hero/attack/general/roll/roll2.png", CCRectMake(0, 0, 54, 78)));
			framelist->addObject(CCSpriteFrame::create
				("hero/attack/slash/hit_light/slash3.png", CCRectMake(0, 0, 140, 78)));

			realMoveDuration = 0.45f;
			actionMove = CCMoveTo::create(realMoveDuration, 
				ccp(this->getPositionX()+100*this->getDirection(), this->getPositionY()));
			this->runAction(actionMove);

			break;
		case KickLight:
			framelist->addObject(CCSpriteFrame::create
				("hero/attack/general/kick_light/kick0.png", CCRectMake(0, 0, 31, 77)));
			framelist->addObject(CCSpriteFrame::create
				("hero/attack/general/kick_light/kick1.png", CCRectMake(0, 0, 38, 80)));
			framelist->addObject(CCSpriteFrame::create
				("hero/attack/general/kick_light/kick2.png", CCRectMake(0, 0, 99, 76)));
			break;
		default:
			//hit light
			framelist->addObject(CCSpriteFrame::create
				("hero/attack/slash/hit_light/slash1.png", CCRectMake(0, 0, 53, 78)));
			framelist->addObject(CCSpriteFrame::create
				("hero/attack/slash/hit_light/slash2.png", CCRectMake(0, 0, 53, 78)));
			framelist->addObject(CCSpriteFrame::create
				("hero/attack/slash/hit_light/slash3.png", CCRectMake(0, 0, 140, 78)));
			break;
		}
		break;
	default:
		framelist->addObject(CCSpriteFrame::create
			("hero/attack/punch/punch3.png", CCRectMake(0, 0, 42, 81)));
		framelist->addObject(CCSpriteFrame::create
			("hero/attack/punch/punch4.png", CCRectMake(0, 0, 103, 80)));
		break;
	}

	this->heroAnimation = CCAnimation::createWithSpriteFrames(framelist, 0.15f);
	CCFiniteTimeAction* animate = CCAnimate::create(heroAnimation);
	CCFiniteTimeAction* actionDone = CCCallFuncN::create(this, 
		callfuncN_selector(Hero::attackFinished));
	CCSequence* sequence = CCSequence::create(animate, actionDone, NULL);
	this->getSprite()->runAction(sequence);
}

void Hero::actionAtDefence(DefenceType dType){
	CCArray* framelist = CCArray::create();
	this->getSprite()->stopAllActions();
	this->heroState = DEFENCE;
	Defence dnfInfo = detResistance(dType);
	this->dType = dType;

	switch (dType)
	{
	case Hero::DefenceNo:
		framelist->addObject((this->getSprite()->displayFrame()));
		break;
	case Hero::Parry:
		framelist->addObject(CCSpriteFrame::create
			("hero/defence/parry.png", CCRectMake(0, 0, 97, 81)));
		break;
	case Hero::Shield:
		framelist->addObject(CCSpriteFrame::create
			("hero/defence/shield.png", CCRectMake(0, 0, 38, 76)));
		break;
	case Hero::SuperArmor:
		break;
	case Hero::Adamantine:
		framelist->addObject(CCSpriteFrame::create
			("hero/defence/adam/adam1.png", CCRectMake(0, 0, 120, 80)));

		//ability effect
		addAbilitySpecialEffect(Adamantine, DEFENCE);
		break;
	default:
		framelist->addObject(CCSpriteFrame::create
			("hero/hurt/back.png", CCRectMake(0, 0, 36, 78)));
		break;
	}
	
	this->heroAnimation = CCAnimation::createWithSpriteFrames(framelist, dnfInfo.duration);
	CCFiniteTimeAction* animate = CCAnimate::create(heroAnimation);
	CCFiniteTimeAction* actionDone = CCCallFuncN::create(this,
		callfuncN_selector(Hero::actionToIdle));
	CCSequence* sequence = CCSequence::create(animate, actionDone, NULL);
	this->getSprite()->runAction(sequence);
}

void Hero::actionAtRecover(float duration){
	CCArray* framelist = CCArray::create();
	this->getSprite()->stopAllActions();
	this->heroState = RECOVER;
	//this->aType = AttackNo;
	this->dType = DefenceNo;
	
	framelist->addObject(this->getSprite()->displayFrame());
	this->heroAnimation = CCAnimation::createWithSpriteFrames(framelist, duration);
	CCFiniteTimeAction* animate = CCAnimate::create(heroAnimation);
	CCFiniteTimeAction* actionDone = CCCallFuncN::create(this,
		callfuncN_selector(Hero::actionToIdle));
	CCSequence* sequence = CCSequence::create(animate, actionDone, NULL);
	this->getSprite()->runAction(sequence);

}

void Hero::actionAtDead(bool isFront, bool isCrit){
	CCArray* framelist = CCArray::create();
	this->getSprite()->stopAllActions();
	this->heroState = DEAD;

	framelist->addObject(CCSpriteFrame::create
		("hero/hurt/back.png", CCRectMake(0, 0, 36, 78)));
	framelist->addObject(CCSpriteFrame::create
		("hero/hurt/back.png", CCRectMake(0, 0, 36, 78)));

	this->heroAnimation = CCAnimation::createWithSpriteFrames(framelist, 0.3f);
	CCFiniteTimeAction* animate = CCAnimate::create(heroAnimation);
	CCFiniteTimeAction* actionDone = CCCallFuncN::create(GameLayer::getGame(),
		callfuncN_selector(GameLayer::doGameEnd));
	CCSequence* sequence = CCSequence::create(animate, actionDone, NULL);
	this->getSprite()->runAction(sequence);
}

Hero::Attack Hero::detDamage(AttackType aType){
	Attack tempInfo;	//hd, dmg, fc, rd

	switch (aType){
	case Hero::AttackNo:
		tempInfo.initAttack(0, 0, 0, 0);
		break;
	case Hero::HitLight:
		tempInfo.initAttack(0.1f, 0.8f, 0.3f, 0.2f);
		break;
	case Hero::HitLightComboFirst:
		tempInfo.initAttack(0.5f, 1.5f, 2.0f, 0.5f);
		break;
	case Hero::HitLightComboSecond:
		tempInfo.initAttack(0.5f, 1.7f, 1.0f, 0.7f);
		break;
	case Hero::KickLight:
		tempInfo.initAttack(0.5f, 1.0f, 2.5f, 0.3f);
		break;
	case Hero::HitHeavy:
		tempInfo.initAttack(1.5f, 2.5f, 4.0f, 1.5f);
		break;
	case Hero::KickHeavy:
		break;
	case Hero::RollAndHit:
		tempInfo.initAttack(1.0f, 2.0f, 4.0f, 0.5f);
		break;
	default:
		tempInfo.initAttack(0.1f, 1.0f, 0.3f, 0.2f);
		break;
	}

	return tempInfo;
}

Hero::Defence Hero::detResistance(DefenceType dType){
	Defence tempInfo;	//sr, ddmg, fc, or, duration

	switch (dType){
	case Hero::DefenceNo:
		tempInfo.initDefence(0, 1.0f, 1.0f, 0, 0);
		break;
	case Hero::Parry:
		tempInfo.initDefence(0.3f, 4.0f, 3.0f, 1.2f, 0.3f);
		break;
	case Hero::Shield:
		tempInfo.initDefence(0, 1.5f, 2.0f, 0.1f, 0.1f);
		break;
	case Hero::SuperArmor:
		break;
	case Hero::Adamantine:
		tempInfo.initDefence(1.5f, 6.5f, 8.0f, 1.0f, 0.5f);
		break;
	default:
		tempInfo.initDefence(0, 1.0f, 1.0f, 0, 0);
		break;
	}
	
	return tempInfo;
}

void Hero::attackFinished(CCNode* pSender){
	HeroLayer* thisHeroLayer = HeroLayer::getHeroLayer();
	Hero* aHero = this;		// (Hero*)pSender;
	Hero* hHero = NULL;	
	this->heroState = DEFAULT;
	//aHero->stopAllActions();
	Attack atkInfo = detDamage(this->aType);
	float additionalRecover = 0;
	//this->aType = AttackNo;

	addAttackEffect(aHero, aType);

	//determine attachee id
	if (aHero == thisHeroLayer->hero_opponent){
		hHero = thisHeroLayer->hero_player;
	}else if (aHero == thisHeroLayer->hero_player){
		hHero = thisHeroLayer->hero_opponent;
	}
	
	//check collision
	if (hHero != NULL){
		if (checkCollision(aHero, hHero)){
			//hit yes
			//is hurtHero defence
			Defence dnfinfo = detResistance(hHero->dType);
			float resistIndex = 1.0f;
			float dnfforce = 1.0f;

			int isBack = -1;
			if (hHero->getDirection() == aHero->getDirection()){
				isBack = 1;
				resistIndex = 0.5f;
				dnfforce = 0;
			}
			else if (hHero->heroState == RECOVER || hHero->heroState == HURT){
				resistIndex = 0.7f;
			}
			else if (hHero->heroState == DEFENCE){
				//hurtHero defence and attack from front
				resistIndex = dnfinfo.declineDamage;
				dnfforce = dnfinfo.force;
				additionalRecover = dnfinfo.opprecover;
			}

			//check damage and atk force
			float damage = atkInfo.damage*aHero->m_baseATK - resistIndex*hHero->m_baseDNF;
			float atkForce = atkInfo.force*aHero->m_baseATK - resistIndex*hHero->m_baseDNF;
			float antiForce = atkInfo.force*aHero->m_baseATK / 2 + dnfforce*hHero->m_baseDNF * 2;
			float antithreshold = 32.0f;
		
			if (damage<1){
				damage = 1;
			}
			
			//cause damage
			if (hHero->getLife() - damage > 0){
				hHero->loseLife(damage);
			}
			else{
				//hHero dead
				hHero->loseLife(hHero->getLife());
				hHero->actionAtDead(true, true);
				return;
			}

			if (atkForce > 0){
				//defence break
				hHero->actionAtHurt(isBack, atkInfo.hurtDuration);
			}
			else{
				//defence sucess
				hHero->actionAtRecover(dnfinfo.selfrecover);
			}

			if (antiForce > antithreshold){
				//aHero get hurt
				aHero->actionAtHurt(-1, additionalRecover);
				return;
			}
		}

		//hHero defence break / success / not hit, aHero doesn't get hurt
		//enable combo
		if (aHero->cbState < ComboFinal){
			aHero->cbState++;	
		}
		else{
			aHero->cbState = -1;
		}
		aHero->actionAtRecover(atkInfo.recoverDuration + additionalRecover / 2);
	}
	//hHero == NULL (normally will not happen)
	else{
		thisHeroLayer->unscheduleUpdate();
	}

}

void Hero::actionToIdle(CCNode* pSender){
	//this->stopAllActions();
	this->aType = AttackNo;
	this->dType = DefenceNo;
	this->cbState = ComboForbid;
	this->actionAtIdle();
}

int Hero::getStateFrameNum(){
	int fNum = 0;
	switch (this->heroState){
	case IDLE:
		fNum = 4;
		break;
	case ATTACK:
		switch (this->weaponType){
		case FIST:
			fNum = 2;
			break;
		case SLASH:
			switch (this->aType){
			case HitLight:
				fNum = 3;
				break;
			case HitLightComboFirst:
				fNum = 3;
				break;
			case HitLightComboSecond:
				fNum = 1;
				break;
			case HitHeavy:
				fNum = 3;
				break;
			case RollAndHit:
				fNum = 4;
				break;
			case KickLight:
				fNum = 3;
				break;
			default:
				fNum = -1;
				break;
			}
			break;
		default:
			break;
		}
		break;
	case DEFENCE:
		switch (this->dType){
		case Hero::DefenceNo:
		case Hero::Parry:
		case Hero::Shield:
			fNum = 1;
			break;
		case Hero::SuperArmor:
			break;
		case Hero::Adamantine:
			fNum = 1;
			break;
		default:
			break;
		}
		break;
	case MOVE:
		fNum = 6;
		break;
	case HURT:
		fNum = 1;
		break;
	case RECOVER:
		fNum = 1;
		break;
	default:
		fNum = -1;
		break;
	}

	return fNum;
}


void Hero::walkWithDirection(CCPoint direction, float speed){
	if (direction.x){
		m_direction = direction.x;
	}
	this->setPositionX(this->getPositionX() + direction.x * speed);
}

bool Hero::checkCollision(Hero* tHero, Hero* opHero){
	HeroLayer* thisHeroLayer = HeroLayer::getHeroLayer();
	bool bRet = false;
	CCRect attackerRect;
	CCPoint origin;
	CCSize wpSize = ccp(60, 50);

	if (tHero->getWeaponSprite()){
		//attack with weapon
		origin = tHero->getWeaponSprite()->getPosition();
		origin.x -= 30;
		origin = this->convertToWorldSpace(origin);
		if (tHero->getDirection() < 0){
			origin.x -= wpSize.width;
		}
		attackerRect = CCRect(origin.x, origin.y - 20, wpSize.width, wpSize.height);
	}
	else{
		//attack without weapon
		origin = tHero->getBoundingBox().origin;
		attackerRect = tHero->getBoundingBox();
	}


	if (attackerRect.intersectsRect(opHero->getBoundingBox())){
		bRet = true;
	}

	return bRet;
}

void Hero::addAbilitySpecialEffect(int abID, int heroState){
	CCArray* abilityList = CCArray::create();
	CCSprite* abSprite = NULL;

	switch (heroState){
	case DEFENCE:
		abSprite = CCSprite::create("hero/defence/adam/adam4.png");
		this->addChild(abSprite);
		abilityList->addObject(CCSpriteFrame::create
			("hero/defence/adam/adam4.png", CCRectMake(0, 0, 135, 150)));
		abilityList->addObject(CCSpriteFrame::create
			("hero/defence/adam/adam3.png", CCRectMake(0, 0, 135, 150)));
		abilityList->addObject(CCSpriteFrame::create
			("hero/defence/adam/adam2.png", CCRectMake(0, 0, 135, 150)));
		abilityList->addObject(CCSpriteFrame::create
			("hero/defence/adam/adam3.png", CCRectMake(0, 0, 135, 150)));
		abilityList->addObject(CCSpriteFrame::create
			("hero/defence/adam/adam4.png", CCRectMake(0, 0, 135, 150)));
		break;
	case ATTACK:
		switch (abID){
		case HitLightComboFirst:
			abSprite = CCSprite::create("effect/sign/combo.png");
			this->addChild(abSprite);
			abSprite->setPositionY(abSprite->getPositionY() + 160);
			abilityList->addObject(CCSpriteFrame::create
				("effect/sign/combo.png", CCRectMake(0, 0, 300, 180)));
			abilityList->addObject(CCSpriteFrame::create
				("effect/sign/combo0.png", CCRectMake(0, 0, 300, 180)));
			abilityList->addObject(CCSpriteFrame::create
				("effect/sign/combo1.png", CCRectMake(0, 0, 300, 180)));
			abilityList->addObject(CCSpriteFrame::create
				("effect/sign/combo0.png", CCRectMake(0, 0, 300, 180)));
			abilityList->addObject(CCSpriteFrame::create
				("effect/sign/combo.png", CCRectMake(0, 0, 300, 180)));
			break;
		case HitLightComboSecond:
			abSprite = CCSprite::create("effect/sign/combo3.png");
			this->addChild(abSprite);
			abSprite->setPositionY(abSprite->getPositionY() + 160);
			abilityList->addObject(CCSpriteFrame::create
				("effect/sign/combo.png", CCRectMake(0, 0, 300, 180)));
			abilityList->addObject(CCSpriteFrame::create
				("effect/sign/combo2.png", CCRectMake(0, 0, 350, 210)));
			abilityList->addObject(CCSpriteFrame::create
				("effect/sign/combo3.png", CCRectMake(0, 0, 350, 210)));
			abilityList->addObject(CCSpriteFrame::create
				("effect/sign/combo2.png", CCRectMake(0, 0, 350, 210)));
			abilityList->addObject(CCSpriteFrame::create
				("effect/sign/combo.png", CCRectMake(0, 0, 300, 180)));
			break;
		case HitHeavy:
			abSprite = CCSprite::create("effect/sign/combo5.png");
			this->addChild(abSprite);
			abSprite->setPositionY(abSprite->getPositionY() + 160);
			abilityList->addObject(CCSpriteFrame::create
				("effect/sign/combo.png", CCRectMake(0, 0, 300, 180)));
			abilityList->addObject(CCSpriteFrame::create
				("effect/sign/combo4.png", CCRectMake(0, 0, 400, 240)));
			abilityList->addObject(CCSpriteFrame::create
				("effect/sign/combo5.png", CCRectMake(0, 0, 400, 240)));
			abilityList->addObject(CCSpriteFrame::create
				("effect/sign/combo4.png", CCRectMake(0, 0, 400, 240)));
			abilityList->addObject(CCSpriteFrame::create
				("effect/sign/combo.png", CCRectMake(0, 0, 300, 180)));
			break;
		}
		break;
	default:
		return;
	}

	if (abilityList->count() > 0){
		if (this->getDirection() < 0){
			abSprite->setScaleX(-1);
		}
		CCAnimate* animate = CCAnimate::create(CCAnimation::createWithSpriteFrames(abilityList, 0.1f));
		abSprite->runAction(animate);
	}
}

void Hero::addAttackEffect(Hero* pHero, Hero::AttackType aType){
	CCArray* abilityList = CCArray::create();
	CCSprite* abSprite = NULL;

	switch (aType){
	case Hero::HitLightComboSecond:
		abSprite = CCSprite::create("effect/combo2/effect2.png");
		this->addChild(abSprite);
		abilityList->addObject(CCSpriteFrame::create
			("effect/combo2/effect2.png", CCRectMake(0, 0, 63, 172)));
		abilityList->addObject(CCSpriteFrame::create
			("effect/combo2/effect1.png", CCRectMake(0, 0, 63, 172)));
		abilityList->addObject(CCSpriteFrame::create
			("effect/combo2/effect0.png", CCRectMake(0, 0, 63, 172)));
		abilityList->addObject(CCSpriteFrame::create
			("effect/combo2/effect1.png", CCRectMake(0, 0, 63, 172)));
		abilityList->addObject(CCSpriteFrame::create
			("effect/combo2/effect2.png", CCRectMake(0, 0, 63, 172)));
		abSprite->setPosition(pHero->getWeaponSprite()->getPosition());
		break;
	case Hero::HitHeavy:
		abSprite = CCSprite::create("effect/combo3/effect4.png");
		this->addChild(abSprite);
		abilityList->addObject(CCSpriteFrame::create
			("effect/combo3/effect4.png", CCRectMake(0, 0, 200, 158)));
		abilityList->addObject(CCSpriteFrame::create
			("effect/combo3/effect2.png", CCRectMake(0, 0, 200, 158)));
		abilityList->addObject(CCSpriteFrame::create
			("effect/combo3/effect1.png", CCRectMake(0, 0, 200, 158)));
		abilityList->addObject(CCSpriteFrame::create
			("effect/combo3/effect2.png", CCRectMake(0, 0, 200, 158)));
		abilityList->addObject(CCSpriteFrame::create
			("effect/combo3/effect3.png", CCRectMake(0, 0, 200, 158)));
		abilityList->addObject(CCSpriteFrame::create
			("effect/combo3/effect4.png", CCRectMake(0, 0, 200, 158)));
		abSprite->setPosition(pHero->getWeaponSprite()->getPosition());
		break;
	case Hero::HitLightComboFirst:
	case Hero::RollAndHit:
		abSprite = CCSprite::create("effect/combo1/effect2.png");
		this->addChild(abSprite);
		abilityList->addObject(CCSpriteFrame::create
			("effect/combo1/effect2.png", CCRectMake(0, 0, 200, 50)));
		abilityList->addObject(CCSpriteFrame::create
			("effect/combo1/effect1.png", CCRectMake(0, 0, 200, 50)));
		abilityList->addObject(CCSpriteFrame::create
			("effect/combo1/effect0.png", CCRectMake(0, 0, 200, 50)));
		abilityList->addObject(CCSpriteFrame::create
			("effect/combo1/effect1.png", CCRectMake(0, 0, 200, 50)));
		abilityList->addObject(CCSpriteFrame::create
			("effect/combo1/effect2.png", CCRectMake(0, 0, 200, 50)));
		abSprite->setPosition(pHero->getWeaponSprite()->getPosition());
		break;
	}

	if (abilityList->count() > 0){
		CCAnimate* animate = CCAnimate::create(CCAnimation::createWithSpriteFrames(abilityList, 0.06f));
		abSprite->runAction(animate);
	}
}

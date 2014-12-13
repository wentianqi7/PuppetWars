#include "AI.h"
#include "Hero.h"
#include "HeroLayer.h"
#include "Gamelayer.h"

#define MAX_INT ((unsigned)(-1)>>1)

inline float GetCurTime(){
	timeval time;
	gettimeofday(&time, NULL);
	unsigned long millisecs = (time.tv_sec * 1000) + (time.tv_usec / 1000);
	return (float)millisecs;
};


ArtiIntellegence::ArtiIntellegence(void){
	
}

ArtiIntellegence::~ArtiIntellegence(void){

}

bool ArtiIntellegence::init(){
	bool bRet = false;

	do{
		CC_BREAK_IF(!CCLayer::init());

		//HeroLayer* thisHeroLayer = HeroLayer::getHeroLayer();
		GameLayer* thisGame = GameLayer::getGame();
		enemy = thisGame->enemy;
		hero = thisGame->my_hero;
		//enemy->getSprite()->setVisible(false);
		nextDecisionTime = 0;
		decisionInterval = (float)MAX_INT;
		isHoldingShield = false;
		isFaceToHero = true;

		//enable update
		//this->scheduleUpdate();

		bRet = true;
	} while (0);

	return bRet;
}

void ArtiIntellegence::update(float dt){

	//assert enemy exists
	if (!HeroLayer::getHeroLayer()->hero_opponent){
		this->unscheduleUpdate();
	}

	/*testing
	if (enemy->heroState == Hero::IDLE || enemy->heroState==Hero::DEFENCE){
		enemy->actionAtDefence(Hero::Shield);
		//enemy->actionAtDefence(Hero::Parry);
	}
	
	if (hero->heroState == Hero::ATTACK){
		enemy->actionAtDefence(Hero::Parry);
	}
	*/
	
	
	//continuously update
	if (enemy->heroState == Hero::MOVE){
		float walkingSpeed = 1.5f;
		CCPoint walkDirection;
		if (isFaceToHero){
			walkDirection = getFaceToHeroDirection();
		}
		else{
			walkDirection = -getFaceToHeroDirection();
		}
		enemy->walkWithDirection(walkDirection, walkingSpeed);
	}

	if (enemy->heroState == Hero::DEFENCE && enemy->dType == Hero::Shield){
		enemy->actionAtDefence(Hero::Shield);
	}

	//make new decision after random time
	if (GetCurTime() > nextDecisionTime){
		//make new decision
		this->updateDecision(dt);
	}
	
}

void ArtiIntellegence::updateDecision(float dt){
	DecisionRatio aiDecision;
	decisionInterval = (float)(rand() % 400 + 100);		//default decisionInterval

	if (enemy->heroState<0 || enemy->heroState>2){
		//can do nothing when dead/attack/hurt/recover/default
		//update decision quicker
		decisionInterval = (float)(rand() % 150 + 50);
		nextDecisionTime = GetCurTime() + decisionInterval;
		return;
	}

	//idle, move, defence
	if (isInMeleeAttackRange(120)){
		//currently using parry / adamtine
		if (enemy->heroState == Hero::DEFENCE && enemy->dType != Hero::Shield){
			decisionInterval = (float)(rand() % 150 + 100);
			nextDecisionTime = GetCurTime() + decisionInterval;
			return;
		}

		//attack/defence/move out of range
		aiDecision.initDecision(2, 3, 5, 30, 40, 45, 80, 90, 95);
	}
	else if (isInMeleeAttackRange(130)){
		//can be attacked by roll attack, may be attacked by normal attack
		//roll/normal attack/move/defence/move out of range
		aiDecision.initDecision(3, 10, 15, 30, 33, 35, 55, 65, 95);
	}
	else if (isInMeleeAttackRange(220)){
		//can be attacked by roll attack, cannot be attacked by normal attack
		//roll/move/defence/normal attack/move out of range
		aiDecision.initDecision(5, 20, 23, 35, 38, 40, 42, 45, 98);
	}
	else if (isInMeleeAttackRange(230)){
		//may be attacked by roll attack
		//move/shield/roll/idle
		aiDecision.initDecision(10, 50, 52, 72, 75, 77, 78, 80, 98);
	}
	else{
		//out of attack range
		//move/idle/shield
		aiDecision.initDecision(30, 90, 91, 95, 95, 95, 95, 95, 100);
	}

	carryOutAction(aiDecision);

	//update decision normally
	nextDecisionTime = GetCurTime() + decisionInterval;
}

CCPoint ArtiIntellegence::getFaceToHeroDirection(){
	float distance = enemy->getPositionX() - hero->getPositionX();
	if (distance < 0){
		return ccp(1, 0);
	}
	else{
		return ccp(-1, 0);
	}
}

void ArtiIntellegence::checkMoveToHero(){
	if (enemy->heroState == Hero::IDLE){
		enemy->actionAtMove();
	}
}

bool ArtiIntellegence::isInMeleeAttackRange(float range){
	bool bRet = false;

	if (ccpDistance(enemy->getPosition(), hero->getPosition()) < range){
		bRet = true;
	}

	return bRet;
}

void ArtiIntellegence::carryOutAction(ArtiIntellegence::DecisionRatio dRate){
	int randRate = rand() % 100;
	if (randRate < dRate.dIdle){
		//idle
		enemy->actionAtIdle();
		decisionInterval = (float)(rand() % 150 + 100);
	}
	else if (randRate < dRate.dMoveTo){
		//move closer
		isFaceToHero = true;
		checkMoveToHero();
		decisionInterval = (float)(rand() % 200 + 300);
	}
	else if (randRate < dRate.dMoveAway){
		//move away
		isFaceToHero = false;
		checkMoveToHero();
		decisionInterval = (float)(rand() % 500 + 1000);
	}
	else if (randRate < dRate.dShield){
		//shielding
		enemy->setDirection(getFaceToHeroDirection().x);
		enemy->actionAtDefence(Hero::Shield);
		decisionInterval = (float)(rand() % 1000 + 500);
	}
	else if (randRate < dRate.dParry){
		//parry
		enemy->setDirection(getFaceToHeroDirection().x);
		enemy->actionAtDefence(Hero::Parry);
		decisionInterval = (float)(rand() % 100 + 300);
	}
	else if (randRate < dRate.dAdam){
		//adamtine
		enemy->setDirection(getFaceToHeroDirection().x);
		enemy->actionAtDefence(Hero::Adamantine);
		decisionInterval = (float)(rand() % 100 + 500);
	}
	else if (randRate < dRate.dLHit){
		//light hit
		enemy->setDirection(getFaceToHeroDirection().x);
		enemy->actionAtAttack(Hero::HitLight);
	}
	else if (randRate < dRate.dLKick){
		//light kick
		enemy->setDirection(getFaceToHeroDirection().x);
		enemy->actionAtAttack(Hero::KickLight);
	}
	else if (randRate < dRate.dRollnA){
		//roll to hero
		enemy->setDirection(getFaceToHeroDirection().x);
		enemy->actionAtAttack(Hero::RollAndHit);
	}
	else{
		//roll away
		enemy->setDirection(-getFaceToHeroDirection().x);
		enemy->actionAtAttack(Hero::RollAndHit);
	}
}


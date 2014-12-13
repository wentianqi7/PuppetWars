#ifndef _AI_H__
#define _AI_H__

#include "cocos2d.h"
class Hero;
using namespace cocos2d;

class ArtiIntellegence : public CCLayer{
public:
	ArtiIntellegence(void);
	~ArtiIntellegence(void);
	CREATE_FUNC(ArtiIntellegence);
	Hero* enemy;
	Hero* hero;
	float nextDecisionTime;
	float decisionInterval;

	CCPoint getFaceToHeroDirection();

	//B-Level realtime judgement (update)
	bool isInMeleeAttackRange(float range);
	void checkAttack();

	//C-Level serial judgement (idle)
	void checkMoveToHero();
	void checkStayInPlace();

	//D-Level longterm judgement (change decision parameter)


	virtual bool init();
	virtual void update(float dt);
	void updateDecision(float dt);


	struct DecisionRatio{
		int dIdle;
		int dMoveTo;
		int dMoveAway;
		int dShield;
		int dParry;
		int dAdam;
		int dLHit;
		int dLKick;
		int dRollnA;
		int dRollnM;

		void initDecision(int di, int dmt, int dma, int ds, 
			int dp, int da, int dlh, int dlk, int dra){
			dIdle = di;
			dMoveTo = dmt;
			dMoveAway = dma;
			dShield = ds;
			dParry = dp;
			dAdam = da;
			dLHit = dlh;
			dLKick = dlk;
			dRollnA = dra;
			dRollnM = 100;
		}
	};

	void carryOutAction(DecisionRatio dRate);
	bool isHoldingShield;
	bool isFaceToHero;
};

#endif

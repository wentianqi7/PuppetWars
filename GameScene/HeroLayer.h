#ifndef _HEROLAYER_H__
#define _HEROLAYER_H__

#include "Hero.h"
#include "cocos2d.h"

using namespace cocos2d;

class HeroLayer :public CCLayer{
public:
	HeroLayer(void);
	~HeroLayer(void);
	static HeroLayer* create();
	virtual bool init();
	//void MoveTo(CCPoint location);

	static HeroLayer* thisHeroLayer;
	static HeroLayer* getHeroLayer();

	//tag
	static const int HERO = 0;		
	static const int H_WEAPON = 10;
	static const int ENEMY = 1;
	static const int E_WEAPON = 11;

	//static const int STAGE_HEIGHT = 105;
	bool isWeaponExist(CCString* filename, int weaponType);
	virtual void update(float dt);

	Hero* hero_player;
	Hero* hero_opponent;
	Hero* getPlayerHero();
	void setPlayerHero(Hero* pHero);
	Hero* getOpponentHero();
	void setOpponentHero(Hero* pHero);
	void addNewHero(Hero* hero, CCString* weaponPath, int hTag, int wTag);
	void updateHeroWeaponPosition(Hero* hero);
	CCSprite* hBar;
	CCSprite* eBar;
	void updateHpbar(bool isHero);
	void preventHeroOutOfScreen(Hero* pHero);

public:
	static HeroLayer* sharedHero;
};


#endif

#ifndef _HERO_H__
#define _HERO_H__

#include "cocos2d.h"

using namespace cocos2d;


class Hero :public CCNode{
public:
	Hero(void);
	~Hero(void);
	void bindSprite(CCSprite* sprite, CCSprite* weapon, int life, int hTag, int wTag);
	static Hero* create();
	CCAnimation* heroAnimation;


	//hero state
	int heroState;
	static const int DEAD = -1;
	static const int IDLE = 0;
	static const int MOVE = 1;
	static const int DEFENCE = 2;
	static const int ATTACK = 3;
	static const int HURT = 4;
	static const int RECOVER = 5;
	static const int DEFAULT = 100;


	//attack types
	enum AttackType{
		AttackNo = -1,
		HitLight,
		HitLightComboFirst,
		HitLightComboSecond,
		KickLight,
		HitHeavy,
		KickHeavy,
		RollAndHit
	};
	AttackType aType;

	//combo state
	static const int ComboForbid = -1;
	static const int ComboInint = 0;
	static const int ComboOne = 1;
	static const int ComboTwo = 2;
	static const int ComboFinal = 3;

	int cbState;

	//defence types
	enum  DefenceType{
		DefenceNo = -1,
		Parry,
		Shield,
		SuperArmor,
		Adamantine
	};
	DefenceType dType;

	//weapon type -> action
	int weaponType;
	CCImage* EquipWeaponImage;
	static const int FIST = 0;
	static const int SLASH = 1;
	static const int MISSILE = 2;

	//charactor info
	static const int LIFE = 100;
	CCSprite* m_sprite;
	int m_speed;
	float m_life;
	int m_direction;
	float m_baseDNF;
	float m_baseATK;

	//weapon info
	CCSprite* w_sprite;

	//hero action
	void actionAtIdle();
	void actionAtMove();
	void actionAtHurt(int isBack, float duration);
	void actionAtAttack(AttackType aType);
	void actionAtDefence(DefenceType dType);
	void actionAtRecover(float duration);
	void actionAtDead(bool isFront, bool isCrit);
	void actionToIdle(CCNode* pSender);
	void attackFinished(CCNode* pSender);
	void walkWithDirection(CCPoint direction, float speed);
	void addAbilitySpecialEffect(int abID, int heroState);
	void addAttackEffect(Hero* pHero, AttackType aType);
	//void deadDone(CCNode* pSender);

	//access
	CCSprite* getSprite();
	void setSprite(CCSprite* pSprite);
	CCSprite* getWeaponSprite();
	void setWeaponSprite(CCSprite* wSprite);
	float getLife();
	void loseLife(float damage);
	CCRect getBoundingBox();
	int getSpeed();
	void setSpeed(int speed);
	int getDirection();
	void setDirection(int direction);
	int getStateFrameNum();		//return num of frames for each 

	//collision check
	bool checkCollision(Hero* thisHero, Hero* opponentHero);

	struct Attack{
		float hurtDuration;
		float damage;
		float force;
		float recoverDuration;

		void initAttack(float hd, float dmg, float fc, float rd){
			hurtDuration = hd;
			damage = dmg;
			force = fc;
			recoverDuration = rd;
		}
	};
	Attack detDamage(AttackType aType);	

	struct Defence{
		float selfrecover;
		float declineDamage;
		float force;
		float opprecover;
		float duration;

		void initDefence(float sr, float ddmg, float fc, float opr, float dtime){
			selfrecover = sr;
			declineDamage = ddmg;
			force = fc;
			opprecover = opr;
			duration = dtime;
		}
	};
	Defence detResistance(DefenceType dType);
};

#endif

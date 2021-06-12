#ifndef __KAMEK_KOOPA_H
#define __KAMEK_KOOPA_H




class daEnShell_c : public dEn_c {
public:
	u32 field_524;
	mHeapAllocator_c m_alloc_subclass;
	nw4r::g3d::ResFile *resfile;
	m3d::mdl_c bodyModel;
	m3d::fanm_c animation;
	u32 *texpat1;
	m3d::anmTexPat_c texpat2;
	ActivePhysics::Info someCollision;
	u32 aboveInfo;
	u32 field_618;
	u32 field_61C;
	u32 belowInfo2;
	u32 field_624;
	u32 field_628;
	u32 field_62C;
	u32 adjacentInfo;
	u32 field_634;
	u32 field_638;
	u32 field_63C;
	u32 belowInfo;
	u32 field_644;
	u32 field_648;
	u32 field_64C;
	mEf::es2 effect;
	u32 field_778;
	u32 shellTypeMaybe;
	u32 field_780;
	float field_784;
	u32 field_788;
	float field_78C;
	float field_790;
	float field_794;
	u32 isInTheAir;
	u32 field_79C;
	u32 field_7A0;
	u32 field_7A4;
	u32 field_7A8;
	u32 field_7AC;
	u32 playerIDHoldingShell;
	u32 field_7B4;
	u16 field_7B8;
	u16 field_7BA;
	u16 field_7BC;
	u16 field_7BE;
	u16 field_7C0;
	u16 field_7C2;
	u8 field_7C4;
	u8 field_7C5;
	u8 field_7C6;
	u8 field_7C7;
	u32 carriedByPlayerID;
	u32 field_7CC;
	float field_7D0;
	float field_7D4;
	u32 field_7D8;

};


class daEnNoko_c : public daEnShell_c {
public:
	mHeapAllocator_c allocator2;
	nw4r::g3d::ResFile *resfile2;
	m3d::mdl_c bodyModel2;
	m3d::fanm_c moveAnimation;
	u32 *resanmtexpat;
	m3d::anmTexPat_c texpat3;
	u32 isFrozen;
	u16 field_8A8;
	u8 walksOffLedges;
	u8 field_8AB;
	VEC3 somePosition;
	float XspeedBeforeFrozen;
	u32 field_8BC;
	u32 field_8C0;
	float field_8C4;
	u32 field_8C8;
	u32 dancesRemaining;
	u16 YRotIncrease;
	u16 field_8D2;
	u32 danceMove;
	mEf::es2 quickSandEffect;
	u32 field_A00;
	u32 field_A04;
	u32 field_A08;
	u32 field_A0C;
	u32 field_A10;
	u32 *self;


	void addScoreWhenHit_orig(void *other);
	void newAddScoreWhenHit(void *other);

	int onCreate_orig();
	int newOnCreate();

    void playerCollision_orig(ActivePhysics *apThis, ActivePhysics *apOther);
    void newPlayerCollision(ActivePhysics *apThis, ActivePhysics *apOther);
};


class daEnPata_c : public dEn_c { //Too lazy to do this one entirely
public:
	int onExecute_orig();
	int newOnExecute();

	void addScoreWhenHit_orig(void *other);
	void newAddScoreWhenHit(void *other);

    void playerCollision_orig(ActivePhysics *apThis, ActivePhysics *apOther);
    void newPlayerCollision(ActivePhysics *apThis, ActivePhysics *apOther);
};


class BulletBill : public dEn_c {
public:
	int onCreate_orig();
	int newOnCreate();

	void addScoreWhenHit_orig(void *other);
	void newAddScoreWhenHit(void *other);

    void playerCollision_orig(ActivePhysics *apThis, ActivePhysics *apOther);
    void newPlayerCollision(ActivePhysics *apThis, ActivePhysics *apOther);
};


#endif
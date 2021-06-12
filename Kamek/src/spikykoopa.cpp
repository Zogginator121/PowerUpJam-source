#include <game.h>
#include "spikykoopa.h"


extern int getNybbleValue(u32 settings, int fromNybble, int toNybble);
extern "C" char usedForDeterminingStatePress_or_playerCollision(dEn_c* t, ActivePhysics *apThis, ActivePhysics *apOther, int unk1);


void daEnNoko_c::newAddScoreWhenHit(void *other) {
	if(getNybbleValue(this->settings, 9, 9) == 0) {
		this->addScoreWhenHit_orig(other);
	}
}


void daEnNoko_c::newPlayerCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
	if(getNybbleValue(this->settings, 9, 9) == 0) {
		this->playerCollision_orig(apThis, apOther);
		OSReport("i'm not spiky, go ahead\n");
	}
	else {
		char hitType = usedForDeterminingStatePress_or_playerCollision(this, apThis, apOther, 0);
		if(hitType != 3) {
			this->_vf220(apOther->owner);
		}
		OSReport("i'm spiky, go away\n");
	}
}

int daEnNoko_c::newOnCreate() {
	int orig_val = this->onCreate_orig();
	OSReport("onCreate\n");

	bool isFlying = (this->name == EN_PATAPATA);
	bool isSpiky = ((isFlying) ? (getNybbleValue(this->settings, 7, 7)) : (getNybbleValue(this->settings, 9, 9)));
	if(isSpiky) {
		ActivePhysics::Info HitMeBaby;
		HitMeBaby.xDistToCenter = 0.0;
		HitMeBaby.yDistToCenter = 12.0;
		HitMeBaby.xDistToEdge = 8.5;
		HitMeBaby.yDistToEdge = 12.0;
		HitMeBaby.category1 = 0x3;
		HitMeBaby.category2 = 0x0;
		HitMeBaby.bitfield1 = 0x6F;
		HitMeBaby.bitfield2 = 0xffbafffe;
		HitMeBaby.unkShort1C = 0;
		HitMeBaby.callback = &dEn_c::collisionCallback;
	
		this->aPhysics.initWithStruct(this, &HitMeBaby);
		this->aPhysics.addToList();
	}

	return orig_val;
}



void daEnPata_c::newAddScoreWhenHit(void *other) {
	if(getNybbleValue(this->settings, 7, 7) == 0) {
		this->addScoreWhenHit_orig(other);
	}
}


void daEnPata_c::newPlayerCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
	if(getNybbleValue(this->settings, 7, 7) == 0) {
		this->playerCollision_orig(apThis, apOther);
		OSReport("i'm not spiky, go ahead\n");
	}
	else {
		char hitType = usedForDeterminingStatePress_or_playerCollision(this, apThis, apOther, 0);
		if(hitType != 3) {
			this->_vf220(apOther->owner);
		}
		OSReport("i'm spiky, go away\n");
	}
}

int daEnPata_c::newOnExecute() {
	int orig_val = this->onExecute_orig();
	// OSReport("onExecute\n");

	// bool isFlying = (this->name == EN_PATAPATA);
	// bool isSpiky = ((isFlying) ? (getNybbleValue(this->settings, 7, 7)) : (getNybbleValue(this->settings, 9, 9)));
	// if(isSpiky) {
	// 	ActivePhysics::Info HitMeBaby;
	// 	HitMeBaby.xDistToCenter = 0.0;
	// 	HitMeBaby.yDistToCenter = 12.0;
	// 	HitMeBaby.xDistToEdge = 8.5;
	// 	HitMeBaby.yDistToEdge = 12.0;
	// 	HitMeBaby.category1 = 0x3;
	// 	HitMeBaby.category2 = 0x0;
	// 	HitMeBaby.bitfield1 = 0x6F;
	// 	HitMeBaby.bitfield2 = 0xffbafffe;
	// 	HitMeBaby.unkShort1C = 0;
	// 	HitMeBaby.callback = &dEn_c::collisionCallback;
	
	// 	this->aPhysics.initWithStruct(this, &HitMeBaby);
	// 	this->aPhysics.addToList();
	// }

	// if(getNybbleValue(this->settings, 7, 7)) {
	// 	this->frzMgr.ice_timer1 = 1000;
	// 	this->frzMgr.ice_timer2 = 1000;
	// 	// this->frzMgr.perm_freeze = 1;
	// 	OSReport("-------\nKoopa Paratroopa - Freeze Manager\n");
	// 	OSReport("some_count = %d\n", this->frzMgr.some_count);
	// 	OSReport("ice_timer1 = %d\n", this->frzMgr.ice_timer1);
	// 	OSReport("ice_timer2 = %d\n", this->frzMgr.ice_timer2);
	// 	OSReport("_mstate = %d\n", this->frzMgr._mstate);
	// 	OSReport("_10 = %d\n", this->frzMgr._10);
	// 	OSReport("_nstate = %d\n", this->frzMgr._nstate);
	// 	OSReport("spawns_coin = %d\n", this->frzMgr.spawns_coin);
	// 	OSReport("_1C_timerLenType = %d\n", this->frzMgr._1C_timerLenType);
	// 	OSReport("_20_defaultTimerLenType = %d\n", this->frzMgr._20_defaultTimerLenType);
	// 	OSReport("_24 = %d\n", this->frzMgr._24);
	// 	OSReport("_28 = %d\n", this->frzMgr._28);
	// 	OSReport("perm_freeze = %d\n", this->frzMgr.perm_freeze);
	// 	OSReport("_30; = %d\n", this->frzMgr._30);
	// 	OSReport("-------\n");
	// }

	return orig_val;
}


int BulletBill::newOnCreate() {
	int orig_val = this->onCreate_orig();
	OSReport("-------\nSpawning EN_SEARCH_MAGNUM_KILLER\n");
	OSReport("settings = %d\n", this->settings);
	OSReport("settings -> nybble 5 = %d\n", getNybbleValue(this->settings, 5, 5));
	OSReport("settings -> nybble 6 = %d\n", getNybbleValue(this->settings, 6, 6));
	OSReport("settings -> nybble 7 = %d\n", getNybbleValue(this->settings, 7, 7));
	OSReport("settings -> nybble 8 = %d\n", getNybbleValue(this->settings, 8, 8));
	OSReport("settings -> nybble 9 = %d\n", getNybbleValue(this->settings, 9, 9));
	OSReport("settings -> nybble 10 = %d\n", getNybbleValue(this->settings, 10, 10));
	OSReport("settings -> nybble 11 = %d\n", getNybbleValue(this->settings, 11, 11));
	OSReport("settings -> nybble 12 = %d\n", getNybbleValue(this->settings, 12, 12));
	OSReport("-------\n");
	return orig_val;
}


void BulletBill::newAddScoreWhenHit(void *other) {
	if(getNybbleValue(this->settings, 5, 5) == 0) {
		this->addScoreWhenHit_orig(other);
	}
}


void BulletBill::newPlayerCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
	if(getNybbleValue(this->settings, 5, 5) == 0) {
		this->playerCollision_orig(apThis, apOther);
		OSReport("i'm not spiky, go ahead\n");
	}
	else {
		char hitType = usedForDeterminingStatePress_or_playerCollision(this, apThis, apOther, 0);
		if(hitType != 3) {
			this->_vf220(apOther->owner);
		}
		OSReport("i'm spiky, go away\n");
	}
}
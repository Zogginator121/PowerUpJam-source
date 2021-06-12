#include <common.h>
#include <game.h>
#include <g3dhax.h>
#include <sfx.h>
#include "boss.h"



const char* GLarcNameList [] = {
	"galaxy",
	"galaxy2",
	"galaxy3",
	"galaxy9",
	"galaxy5",
	"galaxy6",
	"galaxy7",
	"galaxy8",
	NULL	
};


class daWindmill_c : public daEnBlockMain_c {
	int onCreate();
	int onExecute();
	int onDelete();
	int onDraw();

	mHeapAllocator_c allocator;
	nw4r::g3d::ResFile resFile;
	m3d::mdl_c bodyModel;
	m3d::anmChr_c animationChr;
	Physics::Info physicsInfo;
	StandOnTopCollider sotColliderRight;
	StandOnTopCollider sotColliderLeft;

	S16Vec modelRot;

	Physics physicsL1;
	Physics::Info physicsInfoL1;
	Physics physicsR1;
	Physics::Info physicsInfoR1;
	Physics physicsL2;
	Physics::Info physicsInfoL2;
	Physics physicsR2;
	Physics::Info physicsInfoR2;
	Physics physicsB1;
	Physics::Info physicsInfoB1;
	Physics physicsB2;
	Physics::Info physicsInfoB2;
	Physics physicsO1;
	Physics::Info physicsInfoO1;
	Physics physicsO2;
	Physics::Info physicsInfoO2;
	Physics physicsO3;
	Physics::Info physicsInfoO3;
	Physics physicsO4;
	Physics::Info physicsInfoO4;
	Physics physicsO5;
	Physics::Info physicsInfoO5;
	Physics physicsO6;
	Physics::Info physicsInfoO6;

	ActivePhysics aPhysics2;

	u64 eventFlag;
	s32 timer;
	u32 delay;

	u32 effect;
	u8 type;

	int zinc;

	VEC3 type3initPos;
	int type3block;
	int type3distance;
	float type3speed;
	int type3delay;
	int type3timer;
	bool type3goingback;

	int type6wait;
	bool type6reappearing;

	int type7timer;
	bool type7doRespawn;

	bool type8fastJump;
	int type8timer;
	int type8timer2;
	bool type8p1OnGround;

	static daWindmill_c *build();

	void bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate);
	void updateModelMatrices();
	void playerCollision(ActivePhysics *apThis, ActivePhysics *apOther);
	void yoshiCollision(ActivePhysics *apThis, ActivePhysics *apOther);

	bool collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat7_GroundPoundYoshi(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCatD_Drill(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCatA_PenguinMario(ActivePhysics *apThis, ActivePhysics *apOther);

	bool collisionCat1_Fireball_E_Explosion(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat2_IceBall_15_YoshiIce(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat9_RollingObject(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat13_Hammer(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat14_YoshiFire(ActivePhysics *apThis, ActivePhysics *apOther);
	bool collisionCat3_StarPower(ActivePhysics *apThis, ActivePhysics *apOther);

};

extern int getNybbleValue(u32 settings, int fromNybble, int toNybble);
extern "C" void *dAcPy_c__ChangePowerupWithAnimation(void * Player, int powerup);
extern "C" int CanExitThisLevel();
u8 hijackMusicWithSongName(const char *songName, int themeID, bool hasFast, int channelCount, int trackCount, int *wantRealStreamID);

void daWindmill_c::playerCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
	if(this->type == 4) {
		this->_vf220(apOther->owner);
	}
	else if(this->type == 6) {
		OSReport("spawning item\n");
		this->scale = {0.0, 0.0, 0.0};
		int itemsets = 0 | (3 << 0) | (1 << 7);//| (0x1 << 22);
		VEC3 itempos = {this->pos.x + 8, this->pos.y - 28, this->pos.z};
		dStageActor_c *item = dStageActor_c::create(EN_ITEM, itemsets, &apOther->owner->pos, 0, 0);
		item->addMyActivePhysics();
		item->speed.y = 0;
		item->scale = {0.0, 0.0, 0.0};
		int doRespawn = getNybbleValue(this->settings, 6, 6);
		if(doRespawn == 1) {
			this->removeMyActivePhysics();
			this->type6wait = 180;
		}
		else {
			this->Delete(1);
		}
	}
	else if(this->type == 8) {
		this->type8fastJump = true;
	}
	else {
		return;
	}
}
void daWindmill_c::yoshiCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
	return;
}
bool daWindmill_c::collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther) {
	if(this->type != 7) {
		return collisionCat9_RollingObject(apThis, apOther);
	}
	else {
		if(getNybbleValue(this->settings, 6, 6) == 1) {
			this->scale = {0.0, 0.0, 0.0};
			this->type7doRespawn = true;
		}
		else {
			this->Delete(1);
		}
		return false;
	}
}
bool daWindmill_c::collisionCat7_GroundPoundYoshi(ActivePhysics *apThis, ActivePhysics *apOther) {
	return collisionCat7_GroundPound(apThis, apOther);
}
bool daWindmill_c::collisionCatD_Drill(ActivePhysics *apThis, ActivePhysics *apOther) {
	return collisionCat9_RollingObject(apThis, apOther);
}
bool daWindmill_c::collisionCatA_PenguinMario(ActivePhysics *apThis, ActivePhysics *apOther) {
	return collisionCat9_RollingObject(apThis, apOther);
}

bool daWindmill_c::collisionCat1_Fireball_E_Explosion(ActivePhysics *apThis, ActivePhysics *apOther) {
	return false;
}
bool daWindmill_c::collisionCat2_IceBall_15_YoshiIce(ActivePhysics *apThis, ActivePhysics *apOther) {

	return false;
}
bool daWindmill_c::collisionCat9_RollingObject(ActivePhysics *apThis, ActivePhysics *apOther) {

	return false;
}
bool daWindmill_c::collisionCat13_Hammer(ActivePhysics *apThis, ActivePhysics *apOther) {
	return collisionCat9_RollingObject(apThis, apOther);
}
bool daWindmill_c::collisionCat14_YoshiFire(ActivePhysics *apThis, ActivePhysics *apOther) {
	return collisionCat1_Fireball_E_Explosion(apThis, apOther);
}

bool daWindmill_c::collisionCat3_StarPower(ActivePhysics *apThis, ActivePhysics *apOther) {
	return collisionCat9_RollingObject(apThis, apOther);
}



daWindmill_c *daWindmill_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(daWindmill_c));
	return new(buffer) daWindmill_c;
}

//Squashing physics from magicplatform.cpp
extern "C" void HurtMarioBecauseOfBeingSquashed(void *mario, dStageActor_c *squasher, int type);

static void SquashPhysCB1(daWindmill_c *one, dStageActor_c *two) {
	if (two->stageActorType != 1)
		return;

	if (one->pos_delta.y > 0.0f)
		HurtMarioBecauseOfBeingSquashed(two, one, 1);
	else
		HurtMarioBecauseOfBeingSquashed(two, one, 9);
}

static void SquashPhysCB2(daWindmill_c *one, dStageActor_c *two) {
	if (two->stageActorType != 1)
		return;

	if (one->pos_delta.y < 0.0f)
		HurtMarioBecauseOfBeingSquashed(two, one, 2);
	else
		HurtMarioBecauseOfBeingSquashed(two, one, 10);
}

static void SquashPhysCB3(daWindmill_c *one, dStageActor_c *two, bool unkMaybeNotBool) {
	if (two->stageActorType != 1)
		return;

	if (unkMaybeNotBool) {
		if (one->pos_delta.x > 0.0f)
			HurtMarioBecauseOfBeingSquashed(two, one, 6);
		else
			HurtMarioBecauseOfBeingSquashed(two, one, 12);
	} else {
		if (one->pos_delta.x < 0.0f)
			HurtMarioBecauseOfBeingSquashed(two, one, 5);
		else
			HurtMarioBecauseOfBeingSquashed(two, one, 11);
	}
}

static bool SquashPhysCB4(daWindmill_c *one, dStageActor_c *two) {
	return (one->pos_delta.y > 0.0f);
}

static bool SquashPhysCB5(daWindmill_c *one, dStageActor_c *two) {
	return (one->pos_delta.y < 0.0f);
}

static bool SquashPhysCB6(daWindmill_c *one, dStageActor_c *two, bool unkMaybeNotBool) {
	if (unkMaybeNotBool) {
		if (one->pos_delta.x > 0.0f)
			return true;
	} else {
		if (one->pos_delta.x < 0.0f)
			return true;
	}
	return false;
}

void daWindmill_c::bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate) {
	nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr(name);
	this->animationChr.bind(&this->bodyModel, anmChr, unk);
	this->bodyModel.bindAnim(&this->animationChr, unk2);
	this->animationChr.setUpdateRate(rate);
}


int daWindmill_c::onCreate() {
	this->type = getNybbleValue(this->settings, 5, 5);
	if(this->type == 0) {
		allocator.link(-1, GameHeaps[0], 0, 0x20);
	
		this->resFile.data = getResource("galaxy", "g3d/windmill.brres");
		nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("CloudWindmill");
		bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
		SetupTextures_Enemy(&bodyModel, 0);
		SetupTextures_Player(&bodyModel, 0);
	
		bool ret;
		nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr("spinright");
		ret = this->animationChr.setup(mdl, anmChr, &this->allocator, 0);
	
		allocator.unlink();
	
		int tilenum = getNybbleValue(this->settings, 6, 7) * 8;
	
		// Stuff I do understand
		this->scale = (Vec){0.1, 0.1, 0.1};
	
		this->direction = getNybbleValue(this->settings, 8, 8);
		this->rot.x = 0;
		this->rot.y = (this->direction == 0) ? 0x2800 : 0xD800;
		this->rot.z = 0;
	
		float xDistC;
		int yDistC;
		int xDistE;
		int yDistE;
	
		int physicx1;
		int physicy1;
		int physicx2;
		int physicy2;
	
		int leftxoff;
		int leftyoff;
		int lefttopy;
		int leftrightsize;
		int leftleftsize;
		int leftrot;
		int left45;
		
		int rightxoff;
		int rightyoff;
		int righttopy;
		int rightrightsize;
		int rightleftsize;
		int rightrot;
		int right45;
	
		if(this->direction == 0) {
			float fWidth = 3 * 16.0f;
	
			xDistC = (tilenum / 2) + 68 + (((tilenum / 8) - 5) * 4);
			yDistC = 112.0;
			xDistE = tilenum;
			yDistE = 40;
	
			physicx1 = -19;
			physicy1 = 112;
			physicx2 = 56;
			physicy2 = 0;
	
			leftxoff = -28.0f;
			leftyoff = 106.0f;
			lefttopy = 0;
			leftrightsize = fWidth + 14;
			leftleftsize = 0;
			leftrot = 7200;
			left45 = 1;
			
			rightxoff = 28.0f;
			rightyoff = 136.0f;
			righttopy = 0;
			rightrightsize = fWidth;
			rightleftsize = -14;
			rightrot = -7200;
			right45 = 1;
		}
	
		if(this->direction == 1) {
			float fWidth = 3 * 16.0f;
	
			xDistC = -((tilenum / 2) + 68 + (((tilenum / 8) - 5) * 4));
			yDistC = 112.0;
			xDistE = tilenum;
			yDistE = 40;
	
			physicx1 = -56;
			physicy1 = 112;
			physicx2 = 19;
			physicy2 = 0;
	
			leftxoff = -65.0f;
			leftyoff = 106.0f;
			lefttopy = 0;
			leftrightsize = fWidth + 14;
			leftleftsize = 0;
			leftrot = 7200;
			left45 = 1;
			
			rightxoff = -9.0f;
			rightyoff = 136.0f;
			righttopy = 0;
			rightrightsize = fWidth;
			rightleftsize = -14;
			rightrot = -7200;
			right45 = 1;
		}
	
	
		ActivePhysics::Info HitMeBaby;
		HitMeBaby.xDistToCenter = xDistC;
		HitMeBaby.yDistToCenter = yDistC;
		HitMeBaby.xDistToEdge = xDistE;
		HitMeBaby.yDistToEdge = yDistE;
		HitMeBaby.category1 = 0x3;
		HitMeBaby.category2 = 0x0;
		HitMeBaby.bitfield1 = 0x6F;
		HitMeBaby.bitfield2 = 0xffbafffe;
		HitMeBaby.unkShort1C = 0;
		HitMeBaby.callback = &dEn_c::collisionCallback;
	
		this->aPhysics.initWithStruct(this, &HitMeBaby);
		this->aPhysics.addToList();
	
	
		physicsInfo.x1 = physicx1;
		physicsInfo.y1 = physicy1;
		physicsInfo.x2 = physicx2;
		physicsInfo.y2 = physicy2;
	
		physicsInfo.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
		physicsInfo.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
		physicsInfo.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;
	
		physics.setup(this, &physicsInfo, 3, currentLayerID);
		physics.flagsMaybe = 0x260;
		physics.callback1 = &daEnBlockMain_c::PhysicsCallback1;
		physics.callback2 = &daEnBlockMain_c::PhysicsCallback2;
		physics.callback3 = &daEnBlockMain_c::PhysicsCallback3;
		physics.addToList();
	
		sotColliderLeft.init(this,
				/*xOffset=*/leftxoff, /*yOffset=*/leftyoff,
				/*topYOffset=*/lefttopy,
				/*rightSize=*/leftrightsize, /*leftSize=*/leftleftsize,
				/*rotation=*/leftrot, /*_45=*/left45
				);
	
		// What is this for. I dunno
		sotColliderLeft._47 = 0xA;
		sotColliderLeft.flags = 0x80180 | 0xC00;
	
		sotColliderLeft.addToList();
	
		sotColliderRight.init(this,
				/*xOffset=*/rightxoff, /*yOffset=*/rightyoff,
				/*topYOffset=*/righttopy,
				/*rightSize=*/rightrightsize, /*leftSize=*/rightleftsize,
				/*rotation=*/rightrot, /*_45=*/right45
				);
	
		// What is this for. I dunno
		sotColliderRight._47 = 0xA;
		sotColliderRight.flags = 0x80180 | 0xC00;
	
		sotColliderRight.addToList();
	
		this->pos.z = 3300.0f;

		
		float newRate = getNybbleValue(this->settings, 9, 10) / 3;

		bindAnimChr_and_setUpdateRate(((this->direction == 0) ? "spinright" : "spinleft"), 1, 0.0, newRate);
	
		this->onExecute();
		return true;
	}
	if(this->type == 1) {
		int decoType = getNybbleValue(this->settings, 6, 6);
		if(decoType == 0) {
			allocator.link(-1, GameHeaps[0], 0, 0x20);

			this->resFile.data = getResource("galaxy", "g3d/castlebg.brres");
			nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("CloudWind2DPlanet");
			bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
			SetupTextures_Enemy(&bodyModel, 0);
			SetupTextures_Player(&bodyModel, 0);

			allocator.unlink();

			this->scale = (Vec){0.05, 0.05, 0.05};
			this->rot.x = 0;
			this->pos.z = -4000.0f;
	
			// Stuff I do understand
			this->rot.y = 0;
			this->rot.z = 0;

		
			this->onExecute();
			return true;
		}
		if(decoType == 1) {
			allocator.link(-1, GameHeaps[0], 0, 0x20);

			this->resFile.data = getResource("galaxy", "g3d/grassplatform.brres");
			nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("CloudWindGrassPlatform");
			bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
			SetupTextures_Enemy(&bodyModel, 0);
			SetupTextures_Player(&bodyModel, 0);

			allocator.unlink();

			int alternate = getNybbleValue(this->settings, 7, 7);

			physicsInfo.x1 = -42;
			physicsInfo.y1 = 20;
			physicsInfo.x2 = 40;
			physicsInfo.y2 = (alternate == 0) ? -20 : -28;
		
			physicsInfo.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
			physicsInfo.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
			physicsInfo.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;
		
			physics.setup(this, &physicsInfo, 3, currentLayerID);
			physics.flagsMaybe = 0x260;
			physics.callback1 = &daEnBlockMain_c::PhysicsCallback1;
			physics.callback2 = &daEnBlockMain_c::PhysicsCallback2;
			physics.callback3 = &daEnBlockMain_c::PhysicsCallback3;
			physics.addToList();


			this->scale = (Vec){0.1, 0.1, 0.1};
			this->pos.z = 200.0f;
			this->rot.x = (alternate == 0) ? 0 : 2000;
	
			// Stuff I do understand
			this->rot.y = 0;
			this->rot.z = 0;

		
			this->onExecute();
			return true;
		}
		if(decoType == 2) {
			int shape = getNybbleValue(this->settings, 8, 8);
			const char *modelName [] = {
				"SkyIslandStepPartsA",
				"SkyIslandStepPartsB",
				"SkyIslandStepPartsC",
				NULL
			};

			allocator.link(-1, GameHeaps[0], 0, 0x20);


			this->resFile.data = getResource("galaxy", "g3d/skysteppart.brres");
			nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl(modelName[shape]);
			bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
			SetupTextures_Enemy(&bodyModel, 0);
			SetupTextures_Player(&bodyModel, 0);

			allocator.unlink();

			int alternate = getNybbleValue(this->settings, 7, 7);

			physicsInfo.x1 = -32;
			physicsInfo.y1 = (alternate == 0) ? 31 : 29;
			physicsInfo.x2 = 32;
			physicsInfo.y2 = (alternate == 0) ? -8 : -14;
		
			physicsInfo.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
			physicsInfo.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
			physicsInfo.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;
		
			physics.setup(this, &physicsInfo, 3, currentLayerID);
			physics.flagsMaybe = 0x260;
			physics.callback1 = &daEnBlockMain_c::PhysicsCallback1;
			physics.callback2 = &daEnBlockMain_c::PhysicsCallback2;
			physics.callback3 = &daEnBlockMain_c::PhysicsCallback3;
			physics.addToList();


			this->scale = (Vec){0.1, 0.1, 0.1};
			this->pos.z = 200.0f;
			this->rot.x = (alternate == 0) ? 0 : 2000;
	
			// Stuff I do understand
			this->rot.y = 0;
			this->rot.z = 0;

		
			this->onExecute();
			return true;
		}
	}
	if(this->type == 2) {
		int platformType = getNybbleValue(this->settings, 6, 6);
		if(platformType == 0) {
			allocator.link(-1, GameHeaps[0], 0, 0x20);

			this->resFile.data = getResource("galaxy2", "g3d/skypart.brres");
			nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("SkyIslandPartsA");
			bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
			SetupTextures_Enemy(&bodyModel, 0);

			allocator.unlink();

			int newRot = getNybbleValue(this->settings, 7, 7);
			int alternate = getNybbleValue(this->settings, 8, 8);

			if(newRot == 0) {
				/**************/
				/*Main Physics*/
				/**************/
				physicsInfo.x1 = -130;
				physicsInfo.y1 = 0;
				physicsInfo.x2 = 130;
				physicsInfo.y2 = -60;
			
				physicsInfo.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
				physicsInfo.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
				physicsInfo.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

			
				physics.setup(this, &physicsInfo, 3, currentLayerID);
				physics.flagsMaybe = 0x260;
				physics.callback1 = &daEnBlockMain_c::PhysicsCallback1;
				physics.callback2 = &daEnBlockMain_c::PhysicsCallback2;
				physics.callback3 = &daEnBlockMain_c::PhysicsCallback3;

				physics.addToList();
		
				/******************/
				/*Left SOT Physics*/
				/******************/
				float fWidth = 3 * 16.0f;

				sotColliderLeft.init(this,
						/*xOffset=*/-155.0f, /*yOffset=*/-7.0f,
						/*topYOffset=*/0,
						/*rightSize=*/fWidth + 50, /*leftSize=*/0,
						/*rotation=*/3000, /*_45=*/1
						);
			
				// What is this for. I dunno
				sotColliderLeft._47 = 0xA;
				sotColliderLeft.flags = 0x80180 | 0xC00;
			
				sotColliderLeft.addToList();
			
				/*******************/
				/*Right SOT Physics*/
				/*******************/
				sotColliderRight.init(this,
						/*xOffset=*/109.0f, /*yOffset=*/6.0f,
						/*topYOffset=*/0,
						/*rightSize=*/fWidth, /*leftSize=*/-50.0f,
						/*rotation=*/-3000, /*_45=*/1
						);
			
				// What is this for. I dunno
				sotColliderRight._47 = 0xA;
				sotColliderRight.flags = 0x80180 | 0xC00;
			
				sotColliderRight.addToList();

				/***************/
				/*Left1 Physics*/
				/***************/
				physicsInfoL1.x1 = -61;
				physicsInfoL1.y1 = 20;
				physicsInfoL1.x2 = -58;
				physicsInfoL1.y2 = -10;
			
				physicsInfoL1.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
				physicsInfoL1.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
				physicsInfoL1.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

			
				physicsL1.setup(this, &physicsInfoL1, 3, currentLayerID);
				physicsL1.flagsMaybe = 0x260;
				physicsL1.callback1 = &daEnBlockMain_c::PhysicsCallback1;
				physicsL1.callback2 = &daEnBlockMain_c::PhysicsCallback2;
				physicsL1.callback3 = &daEnBlockMain_c::PhysicsCallback3;

				physicsL1.addToList();

				/****************/
				/*Right1 Physics*/
				/****************/
				physicsInfoR1.x1 = 58;
				physicsInfoR1.y1 = 20;
				physicsInfoR1.x2 = 61;
				physicsInfoR1.y2 = -10;
			
				physicsInfoR1.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
				physicsInfoR1.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
				physicsInfoR1.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

			
				physicsR1.setup(this, &physicsInfoR1, 3, currentLayerID);
				physicsR1.flagsMaybe = 0x260;
				physicsR1.callback1 = &daEnBlockMain_c::PhysicsCallback1;
				physicsR1.callback2 = &daEnBlockMain_c::PhysicsCallback2;
				physicsR1.callback3 = &daEnBlockMain_c::PhysicsCallback3;

				physicsR1.addToList();

				/***************/
				/*Left2 Physics*/
				/***************/
				physicsInfoL2.x1 = -160;
				physicsInfoL2.y1 = -5;
				physicsInfoL2.x2 = -157;
				physicsInfoL2.y2 = -120;
			
				physicsInfoL2.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
				physicsInfoL2.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
				physicsInfoL2.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

			
				physicsL2.setup(this, &physicsInfoL2, 3, currentLayerID);
				physicsL2.flagsMaybe = 0x260;
				physicsL2.callback1 = &daEnBlockMain_c::PhysicsCallback1;
				physicsL2.callback2 = &daEnBlockMain_c::PhysicsCallback2;
				physicsL2.callback3 = &daEnBlockMain_c::PhysicsCallback3;

				physicsL2.addToList();

				/****************/
				/*Right2 Physics*/
				/****************/
				physicsInfoR2.x1 = 157;
				physicsInfoR2.y1 = -5;
				physicsInfoR2.x2 = 160;
				physicsInfoR2.y2 = -120;
			
				physicsInfoR2.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
				physicsInfoR2.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
				physicsInfoR2.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

			
				physicsR2.setup(this, &physicsInfoR2, 3, currentLayerID);
				physicsR2.flagsMaybe = 0x260;
				physicsR2.callback1 = &daEnBlockMain_c::PhysicsCallback1;
				physicsR2.callback2 = &daEnBlockMain_c::PhysicsCallback2;
				physicsR2.callback3 = &daEnBlockMain_c::PhysicsCallback3;

				physicsR2.addToList();

				/****************/
				/*Bottom Physics*/
				/****************/
				physicsInfoB1.x1 = -100;
				physicsInfoB1.y1 = -62;
				physicsInfoB1.x2 = 100;
				physicsInfoB1.y2 = -132;
			
				physicsInfoB1.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
				physicsInfoB1.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
				physicsInfoB1.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

			
				physicsB1.setup(this, &physicsInfoB1, 3, currentLayerID);
				physicsB1.flagsMaybe = 0x260;
				physicsB1.callback1 = &daEnBlockMain_c::PhysicsCallback1;
				physicsB1.callback2 = &daEnBlockMain_c::PhysicsCallback2;
				physicsB1.callback3 = &daEnBlockMain_c::PhysicsCallback3;

				physicsB1.addToList();

				/******/
				/*Misc*/
				/******/
				this->pos.z = 2920.0f;
			}
			if(newRot == 1) {
				/**************/
				/*Main Physics*/
				/**************/
				physicsInfo.x1 = -220;
				physicsInfo.y1 = 10;
				physicsInfo.x2 = -111;
				physicsInfo.y2 = -30;
			
				physicsInfo.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
				physicsInfo.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
				physicsInfo.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

			
				physics.setup(this, &physicsInfo, 3, currentLayerID);
				physics.flagsMaybe = 0x260;
				physics.callback1 = &daEnBlockMain_c::PhysicsCallback1;
				physics.callback2 = &daEnBlockMain_c::PhysicsCallback2;
				physics.callback3 = &daEnBlockMain_c::PhysicsCallback3;

				physics.addToList();

				/***************/
				/*Left1 Physics*/
				/***************/
				physicsInfoL1.x1 = -111;
				physicsInfoL1.y1 = 7;
				physicsInfoL1.x2 = -95;
				physicsInfoL1.y2 = -30;
			
				physicsInfoL1.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
				physicsInfoL1.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
				physicsInfoL1.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

			
				physicsL1.setup(this, &physicsInfoL1, 3, currentLayerID);
				physicsL1.flagsMaybe = 0x260;
				physicsL1.callback1 = &daEnBlockMain_c::PhysicsCallback1;
				physicsL1.callback2 = &daEnBlockMain_c::PhysicsCallback2;
				physicsL1.callback3 = &daEnBlockMain_c::PhysicsCallback3;

				physicsL1.addToList();

				/****************/
				/*Right1 Physics*/
				/****************/
				physicsInfoR1.x1 = -95;
				physicsInfoR1.y1 = 4;
				physicsInfoR1.x2 = 0;
				physicsInfoR1.y2 = -30;
			
				physicsInfoR1.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
				physicsInfoR1.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
				physicsInfoR1.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

			
				physicsR1.setup(this, &physicsInfoR1, 3, currentLayerID);
				physicsR1.flagsMaybe = 0x260;
				physicsR1.callback1 = &daEnBlockMain_c::PhysicsCallback1;
				physicsR1.callback2 = &daEnBlockMain_c::PhysicsCallback2;
				physicsR1.callback3 = &daEnBlockMain_c::PhysicsCallback3;

				physicsR1.addToList();
		
				/******************/
				/*Left SOT Physics*/
				/******************/
				float fWidth = 2 * 16.0f;

				sotColliderLeft.init(this,
						/*xOffset=*/-1.0f, /*yOffset=*/3.0f,
						/*topYOffset=*/0,
						/*rightSize=*/fWidth, /*leftSize=*/0,
						/*rotation=*/1500, /*_45=*/1
						);
			
				// What is this for. I dunno
				sotColliderLeft._47 = 0xA;
				sotColliderLeft.flags = 0x80180 | 0xC00;
			
				sotColliderLeft.addToList();

				/****************/
				/*Right2 Physics*/
				/****************/
				physicsInfoR2.x1 = 31;
				physicsInfoR2.y1 = 7;
				physicsInfoR2.x2 = 64;
				physicsInfoR2.y2 = -30;
			
				physicsInfoR2.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
				physicsInfoR2.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
				physicsInfoR2.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

			
				physicsR2.setup(this, &physicsInfoR2, 3, currentLayerID);
				physicsR2.flagsMaybe = 0x260;
				physicsR2.callback1 = &daEnBlockMain_c::PhysicsCallback1;
				physicsR2.callback2 = &daEnBlockMain_c::PhysicsCallback2;
				physicsR2.callback3 = &daEnBlockMain_c::PhysicsCallback3;

				physicsR2.addToList();

				/*****************/
				/*Bottom1 Physics*/
				/*****************/
				physicsInfoB1.x1 = 64;
				physicsInfoB1.y1 = 4;
				physicsInfoB1.x2 = 80;
				physicsInfoB1.y2 = -30;
			
				physicsInfoB1.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
				physicsInfoB1.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
				physicsInfoB1.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

			
				physicsB1.setup(this, &physicsInfoB1, 3, currentLayerID);
				physicsB1.flagsMaybe = 0x260;
				physicsB1.callback1 = &daEnBlockMain_c::PhysicsCallback1;
				physicsB1.callback2 = &daEnBlockMain_c::PhysicsCallback2;
				physicsB1.callback3 = &daEnBlockMain_c::PhysicsCallback3;

				physicsB1.addToList();

				/*****************/
				/*Bottom2 Physics*/
				/*****************/
				physicsInfoB2.x1 = 80;
				physicsInfoB2.y1 = 1;
				physicsInfoB2.x2 = 96;
				physicsInfoB2.y2 = -30;
			
				physicsInfoB2.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
				physicsInfoB2.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
				physicsInfoB2.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

			
				physicsB2.setup(this, &physicsInfoB2, 3, currentLayerID);
				physicsB2.flagsMaybe = 0x260;
				physicsB2.callback1 = &daEnBlockMain_c::PhysicsCallback1;
				physicsB2.callback2 = &daEnBlockMain_c::PhysicsCallback2;
				physicsB2.callback3 = &daEnBlockMain_c::PhysicsCallback3;

				physicsB2.addToList();

				/****************/
				/*Other1 Physics*/
				/****************/
				physicsInfoO1.x1 = 96;
				physicsInfoO1.y1 = -2;
				physicsInfoO1.x2 = 200;
				physicsInfoO1.y2 = -30;
			
				physicsInfoO1.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
				physicsInfoO1.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
				physicsInfoO1.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

			
				physicsO1.setup(this, &physicsInfoO1, 3, currentLayerID);
				physicsO1.flagsMaybe = 0x260;
				physicsO1.callback1 = &daEnBlockMain_c::PhysicsCallback1;
				physicsO1.callback2 = &daEnBlockMain_c::PhysicsCallback2;
				physicsO1.callback3 = &daEnBlockMain_c::PhysicsCallback3;

				physicsO1.addToList();
			
				/*******************/
				/*Right SOT Physics*/
				/*******************/
				sotColliderRight.init(this,
						/*xOffset=*/215.0f, /*yOffset=*/-4.0f,
						/*topYOffset=*/0,
						/*rightSize=*/fWidth, /*leftSize=*/-15.0f,
						/*rotation=*/-1500, /*_45=*/1
						);
			
				// What is this for. I dunno
				sotColliderRight._47 = 0xA;
				sotColliderRight.flags = 0x80180 | 0xC00;
			
				sotColliderRight.addToList();

				/****************/
				/*Other2 Physics*/
				/****************/
				physicsInfoO2.x1 = 244;
				physicsInfoO2.y1 = -8;
				physicsInfoO2.x2 = 247;
				physicsInfoO2.y2 = -30;
			
				physicsInfoO2.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
				physicsInfoO2.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
				physicsInfoO2.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

			
				physicsO2.setup(this, &physicsInfoO2, 3, currentLayerID);
				physicsO2.flagsMaybe = 0x260;
				physicsO2.callback1 = &daEnBlockMain_c::PhysicsCallback1;
				physicsO2.callback2 = &daEnBlockMain_c::PhysicsCallback2;
				physicsO2.callback3 = &daEnBlockMain_c::PhysicsCallback3;

				physicsO2.addToList();

				/****************/
				/*Other3 Physics*/
				/****************/
				physicsInfoO3.x1 = -212;
				physicsInfoO3.y1 = -30;
				physicsInfoO3.x2 = 239;
				physicsInfoO3.y2 = -60;
			
				physicsInfoO3.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
				physicsInfoO3.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
				physicsInfoO3.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

			
				physicsO3.setup(this, &physicsInfoO3, 3, currentLayerID);
				physicsO3.flagsMaybe = 0x260;
				physicsO3.callback1 = &daEnBlockMain_c::PhysicsCallback1;
				physicsO3.callback2 = &daEnBlockMain_c::PhysicsCallback2;
				physicsO3.callback3 = &daEnBlockMain_c::PhysicsCallback3;

				physicsO3.addToList();

				/****************/
				/*Other4 Physics*/
				/****************/
				physicsInfoO4.x1 = -188;
				physicsInfoO4.y1 = -60;
				physicsInfoO4.x2 = 207;
				physicsInfoO4.y2 = -90;
			
				physicsInfoO4.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
				physicsInfoO4.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
				physicsInfoO4.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

			
				physicsO4.setup(this, &physicsInfoO4, 3, currentLayerID);
				physicsO4.flagsMaybe = 0x260;
				physicsO4.callback1 = &daEnBlockMain_c::PhysicsCallback1;
				physicsO4.callback2 = &daEnBlockMain_c::PhysicsCallback2;
				physicsO4.callback3 = &daEnBlockMain_c::PhysicsCallback3;

				physicsO4.addToList();

				/****************/
				/*Other5 Physics*/
				/****************/
				physicsInfoO5.x1 = -156;
				physicsInfoO5.y1 = -90;
				physicsInfoO5.x2 = 175;
				physicsInfoO5.y2 = -120;
			
				physicsInfoO5.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
				physicsInfoO5.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
				physicsInfoO5.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

			
				physicsO5.setup(this, &physicsInfoO5, 3, currentLayerID);
				physicsO5.flagsMaybe = 0x260;
				physicsO5.callback1 = &daEnBlockMain_c::PhysicsCallback1;
				physicsO5.callback2 = &daEnBlockMain_c::PhysicsCallback2;
				physicsO5.callback3 = &daEnBlockMain_c::PhysicsCallback3;

				physicsO5.addToList();

				/****************/
				/*Other6 Physics*/
				/****************/
				physicsInfoO6.x1 = -60;
				physicsInfoO6.y1 = -120;
				physicsInfoO6.x2 = 79;
				physicsInfoO6.y2 = -130;
			
				physicsInfoO6.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
				physicsInfoO6.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
				physicsInfoO6.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

			
				physicsO6.setup(this, &physicsInfoO6, 3, currentLayerID);
				physicsO6.flagsMaybe = 0x260;
				physicsO6.callback1 = &daEnBlockMain_c::PhysicsCallback1;
				physicsO6.callback2 = &daEnBlockMain_c::PhysicsCallback2;
				physicsO6.callback3 = &daEnBlockMain_c::PhysicsCallback3;

				physicsO6.addToList();

				/******/
				/*Misc*/
				/******/
				this->pos.z = 2975.0f;
			}
			if(newRot == 3) {
				/**************/
				/*Main Physics*/
				/**************/
				physicsInfo.x1 = 111;
				physicsInfo.y1 = 10;
				physicsInfo.x2 = 220;
				physicsInfo.y2 = -30;
			
				physicsInfo.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
				physicsInfo.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
				physicsInfo.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

			
				physics.setup(this, &physicsInfo, 3, currentLayerID);
				physics.flagsMaybe = 0x260;
				physics.callback1 = &daEnBlockMain_c::PhysicsCallback1;
				physics.callback2 = &daEnBlockMain_c::PhysicsCallback2;
				physics.callback3 = &daEnBlockMain_c::PhysicsCallback3;

				physics.addToList();

				/***************/
				/*Left1 Physics*/
				/***************/
				physicsInfoL1.x1 = 95;
				physicsInfoL1.y1 = 7;
				physicsInfoL1.x2 = 111;
				physicsInfoL1.y2 = -30;
			
				physicsInfoL1.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
				physicsInfoL1.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
				physicsInfoL1.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

			
				physicsL1.setup(this, &physicsInfoL1, 3, currentLayerID);
				physicsL1.flagsMaybe = 0x260;
				physicsL1.callback1 = &daEnBlockMain_c::PhysicsCallback1;
				physicsL1.callback2 = &daEnBlockMain_c::PhysicsCallback2;
				physicsL1.callback3 = &daEnBlockMain_c::PhysicsCallback3;

				physicsL1.addToList();

				/****************/
				/*Right1 Physics*/
				/****************/
				physicsInfoR1.x1 = 0;
				physicsInfoR1.y1 = 4;
				physicsInfoR1.x2 = 95;
				physicsInfoR1.y2 = -30;
			
				physicsInfoR1.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
				physicsInfoR1.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
				physicsInfoR1.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

			
				physicsR1.setup(this, &physicsInfoR1, 3, currentLayerID);
				physicsR1.flagsMaybe = 0x260;
				physicsR1.callback1 = &daEnBlockMain_c::PhysicsCallback1;
				physicsR1.callback2 = &daEnBlockMain_c::PhysicsCallback2;
				physicsR1.callback3 = &daEnBlockMain_c::PhysicsCallback3;

				physicsR1.addToList();
		
				/******************/
				/*Left SOT Physics*/
				/******************/
				float fWidth = 2 * 16.0f;

				sotColliderLeft.init(this,
						/*xOffset=*/-32.0f, /*yOffset=*/8.0f,
						/*topYOffset=*/0,
						/*rightSize=*/fWidth, /*leftSize=*/0,
						/*rotation=*/-1500, /*_45=*/1
						);
			
				// What is this for. I dunno
				sotColliderLeft._47 = 0xA;
				sotColliderLeft.flags = 0x80180 | 0xC00;
			
				sotColliderLeft.addToList();

				/****************/
				/*Right2 Physics*/
				/****************/
				physicsInfoR2.x1 = -64;
				physicsInfoR2.y1 = 7;
				physicsInfoR2.x2 = -31;
				physicsInfoR2.y2 = -30;
			
				physicsInfoR2.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
				physicsInfoR2.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
				physicsInfoR2.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

			
				physicsR2.setup(this, &physicsInfoR2, 3, currentLayerID);
				physicsR2.flagsMaybe = 0x260;
				physicsR2.callback1 = &daEnBlockMain_c::PhysicsCallback1;
				physicsR2.callback2 = &daEnBlockMain_c::PhysicsCallback2;
				physicsR2.callback3 = &daEnBlockMain_c::PhysicsCallback3;

				physicsR2.addToList();

				/*****************/
				/*Bottom1 Physics*/
				/*****************/
				physicsInfoB1.x1 = -80;
				physicsInfoB1.y1 = 4;
				physicsInfoB1.x2 = -64;
				physicsInfoB1.y2 = -30;
			
				physicsInfoB1.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
				physicsInfoB1.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
				physicsInfoB1.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

			
				physicsB1.setup(this, &physicsInfoB1, 3, currentLayerID);
				physicsB1.flagsMaybe = 0x260;
				physicsB1.callback1 = &daEnBlockMain_c::PhysicsCallback1;
				physicsB1.callback2 = &daEnBlockMain_c::PhysicsCallback2;
				physicsB1.callback3 = &daEnBlockMain_c::PhysicsCallback3;

				physicsB1.addToList();

				/*****************/
				/*Bottom2 Physics*/
				/*****************/
				physicsInfoB2.x1 = -96;
				physicsInfoB2.y1 = 1;
				physicsInfoB2.x2 = -80;
				physicsInfoB2.y2 = -30;
			
				physicsInfoB2.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
				physicsInfoB2.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
				physicsInfoB2.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

			
				physicsB2.setup(this, &physicsInfoB2, 3, currentLayerID);
				physicsB2.flagsMaybe = 0x260;
				physicsB2.callback1 = &daEnBlockMain_c::PhysicsCallback1;
				physicsB2.callback2 = &daEnBlockMain_c::PhysicsCallback2;
				physicsB2.callback3 = &daEnBlockMain_c::PhysicsCallback3;

				physicsB2.addToList();

				/****************/
				/*Other1 Physics*/
				/****************/
				physicsInfoO1.x1 = -200;
				physicsInfoO1.y1 = -2;
				physicsInfoO1.x2 = -96;
				physicsInfoO1.y2 = -30;
			
				physicsInfoO1.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
				physicsInfoO1.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
				physicsInfoO1.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

			
				physicsO1.setup(this, &physicsInfoO1, 3, currentLayerID);
				physicsO1.flagsMaybe = 0x260;
				physicsO1.callback1 = &daEnBlockMain_c::PhysicsCallback1;
				physicsO1.callback2 = &daEnBlockMain_c::PhysicsCallback2;
				physicsO1.callback3 = &daEnBlockMain_c::PhysicsCallback3;

				physicsO1.addToList();
			
				/*******************/
				/*Right SOT Physics*/
				/*******************/
				sotColliderRight.init(this,
						/*xOffset=*/-232.0f, /*yOffset=*/-6.5f,
						/*topYOffset=*/0,
						/*rightSize=*/fWidth, /*leftSize=*/-15.0f,
						/*rotation=*/1500, /*_45=*/1
						);
			
				// What is this for. I dunno
				sotColliderRight._47 = 0xA;
				sotColliderRight.flags = 0x80180 | 0xC00;
			
				sotColliderRight.addToList();

				/****************/
				/*Other2 Physics*/
				/****************/
				physicsInfoO2.x1 = -247;
				physicsInfoO2.y1 = -8;
				physicsInfoO2.x2 = -244;
				physicsInfoO2.y2 = -30;
			
				physicsInfoO2.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
				physicsInfoO2.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
				physicsInfoO2.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

			
				physicsO2.setup(this, &physicsInfoO2, 3, currentLayerID);
				physicsO2.flagsMaybe = 0x260;
				physicsO2.callback1 = &daEnBlockMain_c::PhysicsCallback1;
				physicsO2.callback2 = &daEnBlockMain_c::PhysicsCallback2;
				physicsO2.callback3 = &daEnBlockMain_c::PhysicsCallback3;

				physicsO2.addToList();

				/****************/
				/*Other3 Physics*/
				/****************/
				physicsInfoO3.x1 = -239;
				physicsInfoO3.y1 = -30;
				physicsInfoO3.x2 = 212;
				physicsInfoO3.y2 = -60;
			
				physicsInfoO3.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
				physicsInfoO3.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
				physicsInfoO3.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

			
				physicsO3.setup(this, &physicsInfoO3, 3, currentLayerID);
				physicsO3.flagsMaybe = 0x260;
				physicsO3.callback1 = &daEnBlockMain_c::PhysicsCallback1;
				physicsO3.callback2 = &daEnBlockMain_c::PhysicsCallback2;
				physicsO3.callback3 = &daEnBlockMain_c::PhysicsCallback3;

				physicsO3.addToList();

				/****************/
				/*Other4 Physics*/
				/****************/
				physicsInfoO4.x1 = -207;
				physicsInfoO4.y1 = -60;
				physicsInfoO4.x2 = 188;
				physicsInfoO4.y2 = -90;
			
				physicsInfoO4.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
				physicsInfoO4.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
				physicsInfoO4.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

			
				physicsO4.setup(this, &physicsInfoO4, 3, currentLayerID);
				physicsO4.flagsMaybe = 0x260;
				physicsO4.callback1 = &daEnBlockMain_c::PhysicsCallback1;
				physicsO4.callback2 = &daEnBlockMain_c::PhysicsCallback2;
				physicsO4.callback3 = &daEnBlockMain_c::PhysicsCallback3;

				physicsO4.addToList();

				/****************/
				/*Other5 Physics*/
				/****************/
				physicsInfoO5.x1 = -175;
				physicsInfoO5.y1 = -90;
				physicsInfoO5.x2 = 156;
				physicsInfoO5.y2 = -120;
			
				physicsInfoO5.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
				physicsInfoO5.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
				physicsInfoO5.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

			
				physicsO5.setup(this, &physicsInfoO5, 3, currentLayerID);
				physicsO5.flagsMaybe = 0x260;
				physicsO5.callback1 = &daEnBlockMain_c::PhysicsCallback1;
				physicsO5.callback2 = &daEnBlockMain_c::PhysicsCallback2;
				physicsO5.callback3 = &daEnBlockMain_c::PhysicsCallback3;

				physicsO5.addToList();

				/****************/
				/*Other6 Physics*/
				/****************/
				physicsInfoO6.x1 = -79;
				physicsInfoO6.y1 = -120;
				physicsInfoO6.x2 = 60;
				physicsInfoO6.y2 = -130;
			
				physicsInfoO6.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
				physicsInfoO6.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
				physicsInfoO6.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

			
				physicsO6.setup(this, &physicsInfoO6, 3, currentLayerID);
				physicsO6.flagsMaybe = 0x260;
				physicsO6.callback1 = &daEnBlockMain_c::PhysicsCallback1;
				physicsO6.callback2 = &daEnBlockMain_c::PhysicsCallback2;
				physicsO6.callback3 = &daEnBlockMain_c::PhysicsCallback3;

				physicsO6.addToList();

				/******/
				/*Misc*/
				/******/
				this->pos.z = 2975.0f;
			}

			/************/
			/*Other Shit*/
			/************/
			int rotations [] = {
				0,
				16000,
				32000,
				-16000,
				NULL
			};

			// Stuff I do understand
			this->scale = (Vec){0.1, 0.1, 0.1};
			if(newRot == 0) {
				this->modelRot.x = (alternate == 0) ? 0 : 2000;
				this->modelRot.z = 0;
			}
			if(newRot == 1) {
				this->modelRot.x = 0;
				this->modelRot.z = (alternate == 0) ? 0 : 2000;
			}
			if(newRot == 2) {
				this->modelRot.x = (alternate == 0) ? 0 : -2000;
				this->modelRot.z = 0;
			}
			if(newRot == 3) {
				this->modelRot.x = 0;
				this->modelRot.z = (alternate == 0) ? 0 : -2000;
			}
			this->rot.x = 0;
			this->rot.y = 0;
			this->rot.z = 0;
	
			this->modelRot.y = rotations[newRot];

		
			this->onExecute();
			return true;
		}
		if(platformType == 1) {
			allocator.link(-1, GameHeaps[0], 0, 0x20);

			this->resFile.data = getResource("galaxy3", "g3d/skypart.brres");
			nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("SkyIslandPartsB");
			bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
			SetupTextures_Enemy(&bodyModel, 0);

			allocator.unlink();

			int newRot = getNybbleValue(this->settings, 7, 7);
			int alternate = getNybbleValue(this->settings, 8, 8);

			if(newRot == 0) {
				/**************/
				/*Main Physics*/
				/**************/
				physicsInfo.x1 = -74;
				physicsInfo.y1 = -10;
				physicsInfo.x2 = 98;
				physicsInfo.y2 = -55;
			
				physicsInfo.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
				physicsInfo.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
				physicsInfo.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

			
				physics.setup(this, &physicsInfo, 3, currentLayerID);
				physics.flagsMaybe = 0x260;
				physics.callback1 = &daEnBlockMain_c::PhysicsCallback1;
				physics.callback2 = &daEnBlockMain_c::PhysicsCallback2;
				physics.callback3 = &daEnBlockMain_c::PhysicsCallback3;

				physics.addToList();

				/******/
				/*Misc*/
				/******/
				this->pos.z = 2951.0f;
			}
			if(newRot == 1) {
				/**************/
				/*Main Physics*/
				/**************/
				physicsInfo.x1 = -72;
				physicsInfo.y1 = 0;
				physicsInfo.x2 = 106;
				physicsInfo.y2 = -55;
			
				physicsInfo.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
				physicsInfo.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
				physicsInfo.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

			
				physics.setup(this, &physicsInfo, 3, currentLayerID);
				physics.flagsMaybe = 0x260;
				physics.callback1 = &daEnBlockMain_c::PhysicsCallback1;
				physics.callback2 = &daEnBlockMain_c::PhysicsCallback2;
				physics.callback3 = &daEnBlockMain_c::PhysicsCallback3;

				physics.addToList();

				/***************/
				/*Left1 Physics*/
				/***************/
				physicsInfoL1.x1 = -72;
				physicsInfoL1.y1 = 28;
				physicsInfoL1.x2 = -2;
				physicsInfoL1.y2 = 0;
			
				physicsInfoL1.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
				physicsInfoL1.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
				physicsInfoL1.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

			
				physicsL1.setup(this, &physicsInfoL1, 3, currentLayerID);
				physicsL1.flagsMaybe = 0x260;
				physicsL1.callback1 = &daEnBlockMain_c::PhysicsCallback1;
				physicsL1.callback2 = &daEnBlockMain_c::PhysicsCallback2;
				physicsL1.callback3 = &daEnBlockMain_c::PhysicsCallback3;

				physicsL1.addToList();

				/***************/
				/*Left2 Physics*/
				/***************/
				physicsInfoL2.x1 = -2;
				physicsInfoL2.y1 = 4;
				physicsInfoL2.x2 = 8;
				physicsInfoL2.y2 = 0;
			
				physicsInfoL2.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
				physicsInfoL2.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
				physicsInfoL2.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

			
				physicsL2.setup(this, &physicsInfoL2, 3, currentLayerID);
				physicsL2.flagsMaybe = 0x260;
				physicsL2.callback1 = &daEnBlockMain_c::PhysicsCallback1;
				physicsL2.callback2 = &daEnBlockMain_c::PhysicsCallback2;
				physicsL2.callback3 = &daEnBlockMain_c::PhysicsCallback3;

				physicsL2.addToList();

				/******/
				/*Misc*/
				/******/
				this->pos.z = 2992.0f;
			}
			if(newRot == 3) {
				/**************/
				/*Main Physics*/
				/**************/
				physicsInfo.x1 = -106;
				physicsInfo.y1 = 0;
				physicsInfo.x2 = 72;
				physicsInfo.y2 = -55;
			
				physicsInfo.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
				physicsInfo.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
				physicsInfo.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

			
				physics.setup(this, &physicsInfo, 3, currentLayerID);
				physics.flagsMaybe = 0x260;
				physics.callback1 = &daEnBlockMain_c::PhysicsCallback1;
				physics.callback2 = &daEnBlockMain_c::PhysicsCallback2;
				physics.callback3 = &daEnBlockMain_c::PhysicsCallback3;

				physics.addToList();

				/***************/
				/*Left1 Physics*/
				/***************/
				physicsInfoL1.x1 = 2;
				physicsInfoL1.y1 = 28;
				physicsInfoL1.x2 = 72;
				physicsInfoL1.y2 = 0;
			
				physicsInfoL1.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
				physicsInfoL1.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
				physicsInfoL1.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

			
				physicsL1.setup(this, &physicsInfoL1, 3, currentLayerID);
				physicsL1.flagsMaybe = 0x260;
				physicsL1.callback1 = &daEnBlockMain_c::PhysicsCallback1;
				physicsL1.callback2 = &daEnBlockMain_c::PhysicsCallback2;
				physicsL1.callback3 = &daEnBlockMain_c::PhysicsCallback3;

				physicsL1.addToList();

				/***************/
				/*Left2 Physics*/
				/***************/
				physicsInfoL2.x1 = -8;
				physicsInfoL2.y1 = 3;
				physicsInfoL2.x2 = 2;
				physicsInfoL2.y2 = 0;
			
				physicsInfoL2.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
				physicsInfoL2.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
				physicsInfoL2.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

			
				physicsL2.setup(this, &physicsInfoL2, 3, currentLayerID);
				physicsL2.flagsMaybe = 0x260;
				physicsL2.callback1 = &daEnBlockMain_c::PhysicsCallback1;
				physicsL2.callback2 = &daEnBlockMain_c::PhysicsCallback2;
				physicsL2.callback3 = &daEnBlockMain_c::PhysicsCallback3;

				physicsL2.addToList();

				/******/
				/*Misc*/
				/******/
				this->pos.z = 2992.0f;
			}

			/************/
			/*Other Shit*/
			/************/
			int rotations [] = {
				0,
				16000,
				32000,
				-16000,
				NULL
			};

			// Stuff I do understand
			this->scale = (Vec){0.1, 0.1, 0.1};
			if(newRot == 0) {
				this->modelRot.x = (alternate == 0) ? 0 : 2000;
				this->modelRot.z = 0;
			}
			if(newRot == 1) {
				this->modelRot.x = 0;
				this->modelRot.z = (alternate == 0) ? 0 : 2000;
			}
			if(newRot == 2) {
				this->modelRot.x = (alternate == 0) ? 0 : -2000;
				this->modelRot.z = 0;
			}
			if(newRot == 3) {
				this->modelRot.x = 0;
				this->modelRot.z = (alternate == 0) ? 0 : -2000;
			}
			this->rot.x = 0;
			this->rot.y = 0;
			this->rot.z = 0;
	
			this->modelRot.y = rotations[newRot];

		
			this->onExecute();
			return true;
		}
		if(platformType == 2) {
			allocator.link(-1, GameHeaps[0], 0, 0x20);

			this->resFile.data = getResource("galaxy4", "g3d/skypart.brres");
			nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("SkyIslandPartsC");
			bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
			SetupTextures_Enemy(&bodyModel, 0);

			allocator.unlink();

			int newRot = getNybbleValue(this->settings, 7, 7);
			int alternate = getNybbleValue(this->settings, 8, 8);

			if(newRot == 0) {
				/**************/
				/*Main Physics*/
				/**************/
				physicsInfo.x1 = -66;
				physicsInfo.y1 = 125;
				physicsInfo.x2 = 66;
				physicsInfo.y2 = -100;
			
				physicsInfo.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
				physicsInfo.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
				physicsInfo.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

			
				physics.setup(this, &physicsInfo, 3, currentLayerID);
				physics.flagsMaybe = 0x260;
				physics.callback1 = &daEnBlockMain_c::PhysicsCallback1;
				physics.callback2 = &daEnBlockMain_c::PhysicsCallback2;
				physics.callback3 = &daEnBlockMain_c::PhysicsCallback3;

				physics.addToList();

				/******/
				/*Misc*/
				/******/
				this->pos.z = 2967.0f;
			}
			if(newRot == 1) {
				/**************/
				/*Main Physics*/
				/**************/
				physicsInfo.x1 = -66;
				physicsInfo.y1 = 125;
				physicsInfo.x2 = 66;
				physicsInfo.y2 = -100;
			
				physicsInfo.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
				physicsInfo.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
				physicsInfo.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

			
				physics.setup(this, &physicsInfo, 3, currentLayerID);
				physics.flagsMaybe = 0x260;
				physics.callback1 = &daEnBlockMain_c::PhysicsCallback1;
				physics.callback2 = &daEnBlockMain_c::PhysicsCallback2;
				physics.callback3 = &daEnBlockMain_c::PhysicsCallback3;

				physics.addToList();

				/******/
				/*Misc*/
				/******/
				this->pos.z = 2967.0f;
			}
			if(newRot == 2) {
				/**************/
				/*Main Physics*/
				/**************/
				physicsInfo.x1 = -66;
				physicsInfo.y1 = 125;
				physicsInfo.x2 = 66;
				physicsInfo.y2 = -100;
			
				physicsInfo.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
				physicsInfo.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
				physicsInfo.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

			
				physics.setup(this, &physicsInfo, 3, currentLayerID);
				physics.flagsMaybe = 0x260;
				physics.callback1 = &daEnBlockMain_c::PhysicsCallback1;
				physics.callback2 = &daEnBlockMain_c::PhysicsCallback2;
				physics.callback3 = &daEnBlockMain_c::PhysicsCallback3;

				physics.addToList();

				/******/
				/*Misc*/
				/******/
				this->pos.z = 2967.0f;
			}
			if(newRot == 3) {
				/**************/
				/*Main Physics*/
				/**************/
				physicsInfo.x1 = -66;
				physicsInfo.y1 = 125;
				physicsInfo.x2 = 66;
				physicsInfo.y2 = -100;
			
				physicsInfo.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
				physicsInfo.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
				physicsInfo.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

			
				physics.setup(this, &physicsInfo, 3, currentLayerID);
				physics.flagsMaybe = 0x260;
				physics.callback1 = &daEnBlockMain_c::PhysicsCallback1;
				physics.callback2 = &daEnBlockMain_c::PhysicsCallback2;
				physics.callback3 = &daEnBlockMain_c::PhysicsCallback3;

				physics.addToList();

				/******/
				/*Misc*/
				/******/
				this->pos.z = 2967.0f;
			}

			/************/
			/*Other Shit*/
			/************/
			int rotations [] = {
				0,
				16000,
				32000,
				-16000,
				NULL
			};

			// Stuff I do understand
			this->scale = (Vec){0.1, 0.1, 0.1};
			if(newRot == 0) {
				this->modelRot.x = (alternate == 0) ? 0 : 2000;
				this->modelRot.z = 0;
			}
			if(newRot == 1) {
				this->modelRot.x = 0;
				this->modelRot.z = (alternate == 0) ? 0 : 2000;
			}
			if(newRot == 2) {
				this->modelRot.x = (alternate == 0) ? 0 : -2000;
				this->modelRot.z = 0;
			}
			if(newRot == 3) {
				this->modelRot.x = 0;
				this->modelRot.z = (alternate == 0) ? 0 : -2000;
			}
			this->rot.x = 0;
			this->rot.y = 0;
			this->rot.z = 0;
	
			this->modelRot.y = rotations[newRot];

		
			this->onExecute();
			return true;
		}
		if(platformType == 3) {
			allocator.link(-1, GameHeaps[0], 0, 0x20);

			this->resFile.data = getResource("galaxy5", "g3d/skypart.brres");
			nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("SkyIslandPartsD");
			bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
			SetupTextures_Enemy(&bodyModel, 0);

			allocator.unlink();

			int newRot = getNybbleValue(this->settings, 7, 7);
			int alternate = getNybbleValue(this->settings, 8, 8);

			if(newRot == 0) {
				/**************/
				/*Main Physics*/
				/**************/
				physicsInfo.x1 = -204;
				physicsInfo.y1 = 188;
				physicsInfo.x2 = 204;
				physicsInfo.y2 = -300;
			
				physicsInfo.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
				physicsInfo.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
				physicsInfo.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

			
				physics.setup(this, &physicsInfo, 3, currentLayerID);
				physics.flagsMaybe = 0x260;
				physics.callback1 = &daEnBlockMain_c::PhysicsCallback1;
				physics.callback2 = &daEnBlockMain_c::PhysicsCallback2;
				physics.callback3 = &daEnBlockMain_c::PhysicsCallback3;

				physics.addToList();

				/******/
				/*Misc*/
				/******/
				this->pos.z = 2927.0f;
			}
			if(newRot == 1) {
				/**************/
				/*Main Physics*/
				/**************/
				physicsInfo.x1 = -172;
				physicsInfo.y1 = 193;
				physicsInfo.x2 = 172;
				physicsInfo.y2 = -100;
			
				physicsInfo.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
				physicsInfo.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
				physicsInfo.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

			
				physics.setup(this, &physicsInfo, 3, currentLayerID);
				physics.flagsMaybe = 0x260;
				physics.callback1 = &daEnBlockMain_c::PhysicsCallback1;
				physics.callback2 = &daEnBlockMain_c::PhysicsCallback2;
				physics.callback3 = &daEnBlockMain_c::PhysicsCallback3;

				physics.addToList();

				/***************/
				/*Left1 Physics*/
				/***************/
				physicsInfoL1.x1 = -113;
				physicsInfoL1.y1 = 210;
				physicsInfoL1.x2 = -36;
				physicsInfoL1.y2 = 193;
			
				physicsInfoL1.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
				physicsInfoL1.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
				physicsInfoL1.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

			
				physicsL1.setup(this, &physicsInfoL1, 3, currentLayerID);
				physicsL1.flagsMaybe = 0x260;
				physicsL1.callback1 = &daEnBlockMain_c::PhysicsCallback1;
				physicsL1.callback2 = &daEnBlockMain_c::PhysicsCallback2;
				physicsL1.callback3 = &daEnBlockMain_c::PhysicsCallback3;

				physicsL1.addToList();

				/***************/
				/*Left2 Physics*/
				/***************/
				physicsInfoL2.x1 = -36;
				physicsInfoL2.y1 = 206;
				physicsInfoL2.x2 = -20;
				physicsInfoL2.y2 = 193;
			
				physicsInfoL2.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
				physicsInfoL2.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
				physicsInfoL2.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

			
				physicsL2.setup(this, &physicsInfoL2, 3, currentLayerID);
				physicsL2.flagsMaybe = 0x260;
				physicsL2.callback1 = &daEnBlockMain_c::PhysicsCallback1;
				physicsL2.callback2 = &daEnBlockMain_c::PhysicsCallback2;
				physicsL2.callback3 = &daEnBlockMain_c::PhysicsCallback3;

				physicsL2.addToList();

				/****************/
				/*Right1 Physics*/
				/****************/
				physicsInfoR1.x1 = -20;
				physicsInfoR1.y1 = 202;
				physicsInfoR1.x2 = -4;
				physicsInfoR1.y2 = 193;
			
				physicsInfoR1.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
				physicsInfoR1.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
				physicsInfoR1.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

			
				physicsR1.setup(this, &physicsInfoR1, 3, currentLayerID);
				physicsR1.flagsMaybe = 0x260;
				physicsR1.callback1 = &daEnBlockMain_c::PhysicsCallback1;
				physicsR1.callback2 = &daEnBlockMain_c::PhysicsCallback2;
				physicsR1.callback3 = &daEnBlockMain_c::PhysicsCallback3;

				physicsR1.addToList();

				/****************/
				/*Right2 Physics*/
				/****************/
				physicsInfoR2.x1 = -4;
				physicsInfoR2.y1 = 198;
				physicsInfoR2.x2 = 12;
				physicsInfoR2.y2 = 193;
			
				physicsInfoR2.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
				physicsInfoR2.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
				physicsInfoR2.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

			
				physicsR2.setup(this, &physicsInfoR2, 3, currentLayerID);
				physicsR2.flagsMaybe = 0x260;
				physicsR2.callback1 = &daEnBlockMain_c::PhysicsCallback1;
				physicsR2.callback2 = &daEnBlockMain_c::PhysicsCallback2;
				physicsR2.callback3 = &daEnBlockMain_c::PhysicsCallback3;

				physicsR2.addToList();

				/******/
				/*Misc*/
				/******/
				this->pos.z = 2948.0f;
			}
			if(newRot == 3) {
				/**************/
				/*Main Physics*/
				/**************/
				physicsInfo.x1 = -172;
				physicsInfo.y1 = 193;
				physicsInfo.x2 = 172;
				physicsInfo.y2 = -100;
			
				physicsInfo.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
				physicsInfo.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
				physicsInfo.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

			
				physics.setup(this, &physicsInfo, 3, currentLayerID);
				physics.flagsMaybe = 0x260;
				physics.callback1 = &daEnBlockMain_c::PhysicsCallback1;
				physics.callback2 = &daEnBlockMain_c::PhysicsCallback2;
				physics.callback3 = &daEnBlockMain_c::PhysicsCallback3;

				physics.addToList();

				/***************/
				/*Left1 Physics*/
				/***************/
				physicsInfoL1.x1 = 36;
				physicsInfoL1.y1 = 210;
				physicsInfoL1.x2 = 113;
				physicsInfoL1.y2 = 193;
			
				physicsInfoL1.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
				physicsInfoL1.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
				physicsInfoL1.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

			
				physicsL1.setup(this, &physicsInfoL1, 3, currentLayerID);
				physicsL1.flagsMaybe = 0x260;
				physicsL1.callback1 = &daEnBlockMain_c::PhysicsCallback1;
				physicsL1.callback2 = &daEnBlockMain_c::PhysicsCallback2;
				physicsL1.callback3 = &daEnBlockMain_c::PhysicsCallback3;

				physicsL1.addToList();

				/***************/
				/*Left2 Physics*/
				/***************/
				physicsInfoL2.x1 = 20;
				physicsInfoL2.y1 = 206;
				physicsInfoL2.x2 = 36;
				physicsInfoL2.y2 = 193;
			
				physicsInfoL2.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
				physicsInfoL2.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
				physicsInfoL2.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

			
				physicsL2.setup(this, &physicsInfoL2, 3, currentLayerID);
				physicsL2.flagsMaybe = 0x260;
				physicsL2.callback1 = &daEnBlockMain_c::PhysicsCallback1;
				physicsL2.callback2 = &daEnBlockMain_c::PhysicsCallback2;
				physicsL2.callback3 = &daEnBlockMain_c::PhysicsCallback3;

				physicsL2.addToList();

				/****************/
				/*Right1 Physics*/
				/****************/
				physicsInfoR1.x1 = 4;
				physicsInfoR1.y1 = 202;
				physicsInfoR1.x2 = 20;
				physicsInfoR1.y2 = 193;
			
				physicsInfoR1.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
				physicsInfoR1.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
				physicsInfoR1.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

			
				physicsR1.setup(this, &physicsInfoR1, 3, currentLayerID);
				physicsR1.flagsMaybe = 0x260;
				physicsR1.callback1 = &daEnBlockMain_c::PhysicsCallback1;
				physicsR1.callback2 = &daEnBlockMain_c::PhysicsCallback2;
				physicsR1.callback3 = &daEnBlockMain_c::PhysicsCallback3;

				physicsR1.addToList();

				/****************/
				/*Right2 Physics*/
				/****************/
				physicsInfoR2.x1 = -12;
				physicsInfoR2.y1 = 198;
				physicsInfoR2.x2 = 4;
				physicsInfoR2.y2 = 193;
			
				physicsInfoR2.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
				physicsInfoR2.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
				physicsInfoR2.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

			
				physicsR2.setup(this, &physicsInfoR2, 3, currentLayerID);
				physicsR2.flagsMaybe = 0x260;
				physicsR2.callback1 = &daEnBlockMain_c::PhysicsCallback1;
				physicsR2.callback2 = &daEnBlockMain_c::PhysicsCallback2;
				physicsR2.callback3 = &daEnBlockMain_c::PhysicsCallback3;

				physicsR2.addToList();

				/******/
				/*Misc*/
				/******/
				this->pos.z = 2953.0f;
			}

			/************/
			/*Other Shit*/
			/************/
			int rotations [] = {
				0,
				16000,
				32000,
				-16000,
				NULL
			};

			// Stuff I do understand
			this->scale = (Vec){0.1, 0.1, 0.1};
			if(newRot == 0) {
				this->modelRot.x = (alternate == 0) ? 0 : 2000;
				this->modelRot.z = 0;
			}
			if(newRot == 1) {
				this->modelRot.x = 0;
				this->modelRot.z = (alternate == 0) ? 0 : 2000;
			}
			if(newRot == 2) {
				this->modelRot.x = (alternate == 0) ? 0 : -2000;
				this->modelRot.z = 0;
			}
			if(newRot == 3) {
				this->modelRot.x = 0;
				this->modelRot.z = (alternate == 0) ? 0 : -2000;
			}
			this->rot.x = 0;
			this->rot.y = 0;
			this->rot.z = 0;
	
			this->modelRot.y = rotations[newRot];

		
			this->onExecute();
			return true;
		}
		if(platformType == 4) {
			allocator.link(-1, GameHeaps[0], 0, 0x20);

			this->resFile.data = getResource("galaxy6", "g3d/skypart.brres");
			nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("SkyIslandPartsE");
			bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
			SetupTextures_Enemy(&bodyModel, 0);

			allocator.unlink();

			int newRot = getNybbleValue(this->settings, 7, 7);
			int alternate = getNybbleValue(this->settings, 8, 8);

			if(newRot == 0) {
				/**************/
				/*Main Physics*/
				/**************/
				physicsInfo.x1 = -102;
				physicsInfo.y1 = 110;
				physicsInfo.x2 = 102;
				physicsInfo.y2 = -130;
			
				physicsInfo.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
				physicsInfo.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
				physicsInfo.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

			
				physics.setup(this, &physicsInfo, 3, currentLayerID);
				physics.flagsMaybe = 0x260;
				physics.callback1 = &daEnBlockMain_c::PhysicsCallback1;
				physics.callback2 = &daEnBlockMain_c::PhysicsCallback2;
				physics.callback3 = &daEnBlockMain_c::PhysicsCallback3;

				physics.addToList();

				/***************/
				/*Left1 Physics*/
				/***************/
				physicsInfoL1.x1 = -102;
				physicsInfoL1.y1 = 160;
				physicsInfoL1.x2 = 0;
				physicsInfoL1.y2 = 110;
			
				physicsInfoL1.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
				physicsInfoL1.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
				physicsInfoL1.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

			
				physicsL1.setup(this, &physicsInfoL1, 3, currentLayerID);
				physicsL1.flagsMaybe = 0x260;
				physicsL1.callback1 = &daEnBlockMain_c::PhysicsCallback1;
				physicsL1.callback2 = &daEnBlockMain_c::PhysicsCallback2;
				physicsL1.callback3 = &daEnBlockMain_c::PhysicsCallback3;

				physicsL1.addToList();

				/***************/
				/*Left2 Physics*/
				/***************/
				physicsInfoL2.x1 = 96;
				physicsInfoL2.y1 = 127;
				physicsInfoL2.x2 = 100;
				physicsInfoL2.y2 = 110;
			
				physicsInfoL2.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
				physicsInfoL2.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
				physicsInfoL2.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

			
				physicsL2.setup(this, &physicsInfoL2, 3, currentLayerID);
				physicsL2.flagsMaybe = 0x260;
				physicsL2.callback1 = &daEnBlockMain_c::PhysicsCallback1;
				physicsL2.callback2 = &daEnBlockMain_c::PhysicsCallback2;
				physicsL2.callback3 = &daEnBlockMain_c::PhysicsCallback3;

				physicsL2.addToList();

				/******/
				/*Misc*/
				/******/
				this->pos.z = 2967.0f;
			}
			if(newRot == 1) {
				/**************/
				/*Main Physics*/
				/**************/
				physicsInfo.x1 = -102;
				physicsInfo.y1 = 110;
				physicsInfo.x2 = 102;
				physicsInfo.y2 = -130;
			
				physicsInfo.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
				physicsInfo.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
				physicsInfo.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

			
				physics.setup(this, &physicsInfo, 3, currentLayerID);
				physics.flagsMaybe = 0x260;
				physics.callback1 = &daEnBlockMain_c::PhysicsCallback1;
				physics.callback2 = &daEnBlockMain_c::PhysicsCallback2;
				physics.callback3 = &daEnBlockMain_c::PhysicsCallback3;

				physics.addToList();

				/***************/
				/*Left1 Physics*/
				/***************/
				physicsInfoL1.x1 = -102;
				physicsInfoL1.y1 = 160;
				physicsInfoL1.x2 = 0;
				physicsInfoL1.y2 = 110;
			
				physicsInfoL1.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
				physicsInfoL1.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
				physicsInfoL1.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

			
				physicsL1.setup(this, &physicsInfoL1, 3, currentLayerID);
				physicsL1.flagsMaybe = 0x260;
				physicsL1.callback1 = &daEnBlockMain_c::PhysicsCallback1;
				physicsL1.callback2 = &daEnBlockMain_c::PhysicsCallback2;
				physicsL1.callback3 = &daEnBlockMain_c::PhysicsCallback3;

				physicsL1.addToList();
			
				/*******************/
				/*Right SOT Physics*/
				/*******************/
				float fWidth = 3 * 17.0f;

				sotColliderRight.init(this,
						/*xOffset=*/125.0f, /*yOffset=*/105.0f,
						/*topYOffset=*/0,
						/*rightSize=*/fWidth / 2, /*leftSize=*/-(fWidth / 2),
						/*rotation=*/0, /*_45=*/1
						);
			
				// What is this for. I dunno
				sotColliderRight._47 = 0xA;
				sotColliderRight.flags = 0x80180 | 0xC00;
			
				sotColliderRight.addToList();		

				/******/
				/*Misc*/
				/******/
				this->pos.z = 2967.0f;
			}
			if(newRot == 3) {
				/**************/
				/*Main Physics*/
				/**************/
				physicsInfo.x1 = -102;
				physicsInfo.y1 = 110;
				physicsInfo.x2 = 102;
				physicsInfo.y2 = -130;
			
				physicsInfo.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
				physicsInfo.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
				physicsInfo.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

			
				physics.setup(this, &physicsInfo, 3, currentLayerID);
				physics.flagsMaybe = 0x260;
				physics.callback1 = &daEnBlockMain_c::PhysicsCallback1;
				physics.callback2 = &daEnBlockMain_c::PhysicsCallback2;
				physics.callback3 = &daEnBlockMain_c::PhysicsCallback3;

				physics.addToList();

				/***************/
				/*Left1 Physics*/
				/***************/
				physicsInfoL1.x1 = 0;
				physicsInfoL1.y1 = 160;
				physicsInfoL1.x2 = 102;
				physicsInfoL1.y2 = 110;
			
				physicsInfoL1.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
				physicsInfoL1.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
				physicsInfoL1.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

			
				physicsL1.setup(this, &physicsInfoL1, 3, currentLayerID);
				physicsL1.flagsMaybe = 0x260;
				physicsL1.callback1 = &daEnBlockMain_c::PhysicsCallback1;
				physicsL1.callback2 = &daEnBlockMain_c::PhysicsCallback2;
				physicsL1.callback3 = &daEnBlockMain_c::PhysicsCallback3;

				physicsL1.addToList();
			
				/*******************/
				/*Right SOT Physics*/
				/*******************/
				float fWidth = 3 * 17.0f;

				sotColliderRight.init(this,
						/*xOffset=*/-125.0f, /*yOffset=*/105.0f,
						/*topYOffset=*/0,
						/*rightSize=*/fWidth / 2, /*leftSize=*/-(fWidth / 2),
						/*rotation=*/0, /*_45=*/1
						);
			
				// What is this for. I dunno
				sotColliderRight._47 = 0xA;
				sotColliderRight.flags = 0x80180 | 0xC00;
			
				sotColliderRight.addToList();	

				/******/
				/*Misc*/
				/******/
				this->pos.z = 2967.0f;
			}

			/************/
			/*Other Shit*/
			/************/
			int rotations [] = {
				0,
				16000,
				32000,
				-16000,
				NULL
			};

			// Stuff I do understand
			this->scale = (Vec){0.1, 0.1, 0.1};
			if(newRot == 0) {
				this->modelRot.x = (alternate == 0) ? 0 : 2000;
				this->modelRot.z = 0;
			}
			if(newRot == 1) {
				this->modelRot.x = 0;
				this->modelRot.z = (alternate == 0) ? 0 : 2000;
			}
			if(newRot == 2) {
				this->modelRot.x = (alternate == 0) ? 0 : -2000;
				this->modelRot.z = 0;
			}
			if(newRot == 3) {
				this->modelRot.x = 0;
				this->modelRot.z = (alternate == 0) ? 0 : -2000;
			}
			this->rot.x = 0;
			this->rot.y = 0;
			this->rot.z = 0;
	
			this->modelRot.y = rotations[newRot];

		
			this->onExecute();
			return true;
		}
		if(platformType == 5) {
			allocator.link(-1, GameHeaps[0], 0, 0x20);

			this->resFile.data = getResource("galaxy7", "g3d/skypart.brres");
			nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("SkyIslandPartsF");
			bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
			SetupTextures_Enemy(&bodyModel, 0);

			allocator.unlink();

			int newRot = getNybbleValue(this->settings, 7, 7);
			int alternate = getNybbleValue(this->settings, 8, 8);

			if(newRot == 0) {
				/**************/
				/*Main Physics*/
				/**************/
				physicsInfo.x1 = -116;
				physicsInfo.y1 = 180;
				physicsInfo.x2 = 116;
				physicsInfo.y2 = 105;
			
				physicsInfo.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
				physicsInfo.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
				physicsInfo.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

			
				physics.setup(this, &physicsInfo, 3, currentLayerID);
				physics.flagsMaybe = 0x260;
				physics.callback1 = &daEnBlockMain_c::PhysicsCallback1;
				physics.callback2 = &daEnBlockMain_c::PhysicsCallback2;
				physics.callback3 = &daEnBlockMain_c::PhysicsCallback3;

				physics.addToList();

				/***************/
				/*Left1 Physics*/
				/***************/
				physicsInfoL1.x1 = -116;
				physicsInfoL1.y1 = 197;
				physicsInfoL1.x2 = -36;
				physicsInfoL1.y2 = 180;
			
				physicsInfoL1.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
				physicsInfoL1.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
				physicsInfoL1.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

			
				physicsL1.setup(this, &physicsInfoL1, 3, currentLayerID);
				physicsL1.flagsMaybe = 0x260;
				physicsL1.callback1 = &daEnBlockMain_c::PhysicsCallback1;
				physicsL1.callback2 = &daEnBlockMain_c::PhysicsCallback2;
				physicsL1.callback3 = &daEnBlockMain_c::PhysicsCallback3;

				physicsL1.addToList();
			
				/*******************/
				/*Right SOT Physics*/
				/*******************/
				float fWidth = 3 * 16.0f;

				sotColliderRight.init(this,
						/*xOffset=*/-14.0f, /*yOffset=*/189.0f,
						/*topYOffset=*/0,
						/*rightSize=*/fWidth / 2, /*leftSize=*/-(fWidth / 2),
						/*rotation=*/-4000, /*_45=*/1
						);
			
				// What is this for. I dunno
				sotColliderRight._47 = 0xA;
				sotColliderRight.flags = 0x80180 | 0xC00;
			
				sotColliderRight.addToList();

				/***************/
				/*Left2 Physics*/
				/***************/
				physicsInfoL2.x1 = -116;
				physicsInfoL2.y1 = 30;
				physicsInfoL2.x2 = 116;
				physicsInfoL2.y2 = -180;
			
				physicsInfoL2.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
				physicsInfoL2.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
				physicsInfoL2.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

			
				physicsL2.setup(this, &physicsInfoL2, 3, currentLayerID);
				physicsL2.flagsMaybe = 0x260;
				physicsL2.callback1 = &daEnBlockMain_c::PhysicsCallback1;
				physicsL2.callback2 = &daEnBlockMain_c::PhysicsCallback2;
				physicsL2.callback3 = &daEnBlockMain_c::PhysicsCallback3;

				physicsL2.addToList();

				/****************/
				/*Right1 Physics*/
				/****************/
				physicsInfoR1.x1 = -36;
				physicsInfoR1.y1 = 40;
				physicsInfoR1.x2 = 36;
				physicsInfoR1.y2 = 30;
			
				physicsInfoR1.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
				physicsInfoR1.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
				physicsInfoR1.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

			
				physicsR1.setup(this, &physicsInfoR1, 3, currentLayerID);
				physicsR1.flagsMaybe = 0x260;
				physicsR1.callback1 = &daEnBlockMain_c::PhysicsCallback1;
				physicsR1.callback2 = &daEnBlockMain_c::PhysicsCallback2;
				physicsR1.callback3 = &daEnBlockMain_c::PhysicsCallback3;

				physicsR1.addToList();

				/****************/
				/*Right2 Physics*/
				/****************/
				physicsInfoR2.x1 = -104;
				physicsInfoR2.y1 = 223;
				physicsInfoR2.x2 = -100;
				physicsInfoR2.y2 = 197;
			
				physicsInfoR2.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
				physicsInfoR2.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
				physicsInfoR2.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

			
				physicsR2.setup(this, &physicsInfoR2, 3, currentLayerID);
				physicsR2.flagsMaybe = 0x260;
				physicsR2.callback1 = &daEnBlockMain_c::PhysicsCallback1;
				physicsR2.callback2 = &daEnBlockMain_c::PhysicsCallback2;
				physicsR2.callback3 = &daEnBlockMain_c::PhysicsCallback3;

				physicsR2.addToList();

				/******/
				/*Misc*/
				/******/
				this->pos.z = 2925.0f;
			}

			/************/
			/*Other Shit*/
			/************/
			int rotations [] = {
				0,
				16000,
				32000,
				-16000,
				NULL
			};

			// Stuff I do understand
			this->scale = (Vec){0.1, 0.1, 0.1};
			if(newRot == 0) {
				this->modelRot.x = (alternate == 0) ? 0 : 2000;
				this->modelRot.z = 0;
			}
			if(newRot == 1) {
				this->modelRot.x = 0;
				this->modelRot.z = (alternate == 0) ? 0 : 2000;
			}
			if(newRot == 2) {
				this->modelRot.x = (alternate == 0) ? 0 : -2000;
				this->modelRot.z = 0;
			}
			if(newRot == 3) {
				this->modelRot.x = 0;
				this->modelRot.z = (alternate == 0) ? 0 : -2000;
			}
			this->rot.x = 0;
			this->rot.y = 0;
			this->rot.z = 0;
	
			this->modelRot.y = rotations[newRot];

		
			this->onExecute();
			return true;
		}
	}
	if(this->type == 3) {
		int blockType = getNybbleValue(this->settings, 6, 6);
		this->type3timer = 0;
		this->type3block = blockType;
		this->type3distance = getNybbleValue(this->settings, 7, 7) * 16;
		this->type3speed = getNybbleValue(this->settings, 8, 8) * 0.1;
		this->type3delay = getNybbleValue(this->settings, 10, 10) * 60;
		if(blockType == 0) {
			allocator.link(-1, GameHeaps[0], 0, 0x20);

			this->resFile.data = getResource("galaxy8", "g3d/zigzagcrush.brres");
			nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("ZigZagCrushBlockA");
			bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
			SetupTextures_Enemy(&bodyModel, 0);
			SetupTextures_Player(&bodyModel, 0);

			allocator.unlink();

			int alternate = getNybbleValue(this->settings, 9, 9);

			/**************/
			/*Main Physics*/
			/**************/
			physicsInfo.x1 = -200;
			physicsInfo.y1 = 272;
			physicsInfo.x2 = -20;
			physicsInfo.y2 = 28;
		
			physicsInfo.otherCallback1 = (void*)&SquashPhysCB1;
			physicsInfo.otherCallback2 = (void*)&SquashPhysCB2;
			physicsInfo.otherCallback3 = (void*)&SquashPhysCB3;
		
			physics.setup(this, &physicsInfo, 3, currentLayerID);
			physics.flagsMaybe = 0x260;
			physics.callback1 = (void*)&SquashPhysCB4;
			physics.callback2 = (void*)&SquashPhysCB5;
			physics.callback3 = (void*)&SquashPhysCB6;
			physics.addToList();

			/***************/
			/*Left1 Physics*/
			/***************/
			physicsInfoL1.x1 = -200;
			physicsInfoL1.y1 = -44;
			physicsInfoL1.x2 = -20;
			physicsInfoL1.y2 = -288;
		
			physicsInfoL1.otherCallback1 = (void*)&SquashPhysCB1;
			physicsInfoL1.otherCallback2 = (void*)&SquashPhysCB2;
			physicsInfoL1.otherCallback3 = (void*)&SquashPhysCB3;

		
			physicsL1.setup(this, &physicsInfoL1, 3, currentLayerID);
			physicsL1.flagsMaybe = 0x260;
			physicsL1.callback1 = (void*)&SquashPhysCB4;
			physicsL1.callback2 = (void*)&SquashPhysCB5;
			physicsL1.callback3 = (void*)&SquashPhysCB6;

			physicsL1.addToList();

			/***************/
			/*Left2 Physics*/
			/***************/
			physicsInfoL2.x1 = -200;
			physicsInfoL2.y1 = 28;
			physicsInfoL2.x2 = -120;
			physicsInfoL2.y2 = -44;
		
			physicsInfoL2.otherCallback1 = (void*)&SquashPhysCB1;
			physicsInfoL2.otherCallback2 = (void*)&SquashPhysCB2;
			physicsInfoL2.otherCallback3 = (void*)&SquashPhysCB3;

		
			physicsL2.setup(this, &physicsInfoL2, 3, currentLayerID);
			physicsL2.flagsMaybe = 0x260;
			physicsL2.callback1 = (void*)&SquashPhysCB4;
			physicsL2.callback2 = (void*)&SquashPhysCB5;
			physicsL2.callback3 = (void*)&SquashPhysCB6;

			physicsL2.addToList();

			/****************/
			/*Right1 Physics*/
			/****************/
			physicsInfoR1.x1 = -20;
			physicsInfoR1.y1 = 196;
			physicsInfoR1.x2 = 20;
			physicsInfoR1.y2 = 108;
		
			physicsInfoR1.otherCallback1 = (void*)&SquashPhysCB1;
			physicsInfoR1.otherCallback2 = (void*)&SquashPhysCB2;
			physicsInfoR1.otherCallback3 = (void*)&SquashPhysCB3;

		
			physicsR1.setup(this, &physicsInfoR1, 3, currentLayerID);
			physicsR1.flagsMaybe = 0x260;
			physicsR1.callback1 = (void*)&SquashPhysCB4;
			physicsR1.callback2 = (void*)&SquashPhysCB5;
			physicsR1.callback3 = (void*)&SquashPhysCB6;

			physicsR1.addToList();

			/****************/
			/*Right2 Physics*/
			/****************/
			physicsInfoR2.x1 = -20;
			physicsInfoR2.y1 = -120;
			physicsInfoR2.x2 = 20;
			physicsInfoR2.y2 = -208;
		
			physicsInfoR2.otherCallback1 = (void*)&SquashPhysCB1;
			physicsInfoR2.otherCallback2 = (void*)&SquashPhysCB2;
			physicsInfoR2.otherCallback3 = (void*)&SquashPhysCB3;

		
			physicsR2.setup(this, &physicsInfoR2, 3, currentLayerID);
			physicsR2.flagsMaybe = 0x260;
			physicsR2.callback1 = (void*)&SquashPhysCB4;
			physicsR2.callback2 = (void*)&SquashPhysCB5;
			physicsR2.callback3 = (void*)&SquashPhysCB6;

			physicsR2.addToList();


			this->scale = (Vec){0.2, 0.2, 0.2};
			this->pos.z = 535.0f;
			this->rot.x = (alternate == 0) ? 0 : 2000;
	
			// Stuff I do understand
			this->rot.y = 0;
			this->rot.z = 0;

			this->type3initPos = this->pos;
			this->type3goingback = false;
		
			this->onExecute();
			return true;
		}
		if(blockType == 1) {
			allocator.link(-1, GameHeaps[0], 0, 0x20);

			this->resFile.data = getResource("galaxy8", "g3d/zigzagcrush.brres");
			nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("ZigZagCrushBlockB");
			bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
			SetupTextures_Enemy(&bodyModel, 0);
			SetupTextures_Player(&bodyModel, 0);

			allocator.unlink();

			int alternate = getNybbleValue(this->settings, 9, 9);

			/**************/
			/*Main Physics*/
			/**************/
			physicsInfo.x1 = 20;
			physicsInfo.y1 = 272;
			physicsInfo.x2 = 200;
			physicsInfo.y2 = 28;
		
			physicsInfo.otherCallback1 = (void*)&SquashPhysCB1;
			physicsInfo.otherCallback2 = (void*)&SquashPhysCB2;
			physicsInfo.otherCallback3 = (void*)&SquashPhysCB3;
		
			physics.setup(this, &physicsInfo, 3, currentLayerID);
			physics.flagsMaybe = 0x260;
			physics.callback1 = (void*)&SquashPhysCB4;
			physics.callback2 = (void*)&SquashPhysCB5;
			physics.callback3 = (void*)&SquashPhysCB6;
			physics.addToList();

			/***************/
			/*Left1 Physics*/
			/***************/
			physicsInfoL1.x1 = 20;
			physicsInfoL1.y1 = -44;
			physicsInfoL1.x2 = 200;
			physicsInfoL1.y2 = -288;
		
			physicsInfoL1.otherCallback1 = (void*)&SquashPhysCB1;
			physicsInfoL1.otherCallback2 = (void*)&SquashPhysCB2;
			physicsInfoL1.otherCallback3 = (void*)&SquashPhysCB3;

		
			physicsL1.setup(this, &physicsInfoL1, 3, currentLayerID);
			physicsL1.flagsMaybe = 0x260;
			physicsL1.callback1 = (void*)&SquashPhysCB4;
			physicsL1.callback2 = (void*)&SquashPhysCB5;
			physicsL1.callback3 = (void*)&SquashPhysCB6;

			physicsL1.addToList();

			/***************/
			/*Left2 Physics*/
			/***************/
			physicsInfoL2.x1 = 120;
			physicsInfoL2.y1 = 28;
			physicsInfoL2.x2 = 200;
			physicsInfoL2.y2 = -44;
		
			physicsInfoL2.otherCallback1 = (void*)&SquashPhysCB1;
			physicsInfoL2.otherCallback2 = (void*)&SquashPhysCB2;
			physicsInfoL2.otherCallback3 = (void*)&SquashPhysCB3;

		
			physicsL2.setup(this, &physicsInfoL2, 3, currentLayerID);
			physicsL2.flagsMaybe = 0x260;
			physicsL2.callback1 = (void*)&SquashPhysCB4;
			physicsL2.callback2 = (void*)&SquashPhysCB5;
			physicsL2.callback3 = (void*)&SquashPhysCB6;

			physicsL2.addToList();

			/****************/
			/*Right1 Physics*/
			/****************/
			physicsInfoR1.x1 = -20;
			physicsInfoR1.y1 = 272;
			physicsInfoR1.x2 = 20;
			physicsInfoR1.y2 = 184;
		
			physicsInfoR1.otherCallback1 = (void*)&SquashPhysCB1;
			physicsInfoR1.otherCallback2 = (void*)&SquashPhysCB2;
			physicsInfoR1.otherCallback3 = (void*)&SquashPhysCB3;

		
			physicsR1.setup(this, &physicsInfoR1, 3, currentLayerID);
			physicsR1.flagsMaybe = 0x260;
			physicsR1.callback1 = (void*)&SquashPhysCB4;
			physicsR1.callback2 = (void*)&SquashPhysCB5;
			physicsR1.callback3 = (void*)&SquashPhysCB6;

			physicsR1.addToList();

			/****************/
			/*Right2 Physics*/
			/****************/
			physicsInfoR2.x1 = -20;
			physicsInfoR2.y1 = 116;
			physicsInfoR2.x2 = 20;
			physicsInfoR2.y2 = 28;
		
			physicsInfoR2.otherCallback1 = (void*)&SquashPhysCB1;
			physicsInfoR2.otherCallback2 = (void*)&SquashPhysCB2;
			physicsInfoR2.otherCallback3 = (void*)&SquashPhysCB3;

		
			physicsR2.setup(this, &physicsInfoR2, 3, currentLayerID);
			physicsR2.flagsMaybe = 0x260;
			physicsR2.callback1 = (void*)&SquashPhysCB4;
			physicsR2.callback2 = (void*)&SquashPhysCB5;
			physicsR2.callback3 = (void*)&SquashPhysCB6;

			physicsR2.addToList();

			/****************/
			/*Other1 Physics*/
			/****************/
			physicsInfoO1.x1 = -20;
			physicsInfoO1.y1 = -44;
			physicsInfoO1.x2 = 20;
			physicsInfoO1.y2 = -132;
		
			physicsInfoO1.otherCallback1 = (void*)&SquashPhysCB1;
			physicsInfoO1.otherCallback2 = (void*)&SquashPhysCB2;
			physicsInfoO1.otherCallback3 = (void*)&SquashPhysCB3;

		
			physicsO1.setup(this, &physicsInfoO1, 3, currentLayerID);
			physicsO1.flagsMaybe = 0x260;
			physicsO1.callback1 = (void*)&SquashPhysCB4;
			physicsO1.callback2 = (void*)&SquashPhysCB5;
			physicsO1.callback3 = (void*)&SquashPhysCB6;

			physicsO1.addToList();

			/****************/
			/*Other2 Physics*/
			/****************/
			physicsInfoO2.x1 = -20;
			physicsInfoO2.y1 = -200;
			physicsInfoO2.x2 = 20;
			physicsInfoO2.y2 = -288;
		
			physicsInfoO2.otherCallback1 = (void*)&SquashPhysCB1;
			physicsInfoO2.otherCallback2 = (void*)&SquashPhysCB2;
			physicsInfoO2.otherCallback3 = (void*)&SquashPhysCB3;

		
			physicsO2.setup(this, &physicsInfoO2, 3, currentLayerID);
			physicsO2.flagsMaybe = 0x260;
			physicsO2.callback1 = (void*)&SquashPhysCB4;
			physicsO2.callback2 = (void*)&SquashPhysCB5;
			physicsO2.callback3 = (void*)&SquashPhysCB6;

			physicsO2.addToList();


			this->scale = (Vec){0.2, 0.2, 0.2};
			this->pos.z = 535.0f;
			this->rot.x = (alternate == 0) ? 0 : 2000;
	
			// Stuff I do understand
			this->rot.y = 0;
			this->rot.z = 0;

			this->type3initPos = this->pos;
			this->type3goingback = false;

			this->onExecute();
			return true;
		}
		if(blockType == 2) {
			this->pos.y += 208.0f;
			int Asets = 0 | (3 << 28) | (0 << 24) | (getNybbleValue(this->settings, 7, 7) << 20) | (getNybbleValue(this->settings, 8, 8) << 16) | (getNybbleValue(this->settings, 9, 9) << 12) | (getNybbleValue(this->settings, 10, 10) << 8);
			int Bsets = 0 | (3 << 28) | (1 << 24) | (getNybbleValue(this->settings, 7, 7) << 20) | (getNybbleValue(this->settings, 8, 8) << 16) | (getNybbleValue(this->settings, 9, 9) << 12) | (getNybbleValue(this->settings, 10, 10) << 8);
			CreateActor(172, Asets, this->pos, 0, 0);
			CreateActor(172, Bsets, this->pos, 0, 0);

			this->Delete(1);

			return true;
		}
	}
	if(this->type == 4) {
		int needleType = getNybbleValue(this->settings, 6, 6);
		int needleScale = getNybbleValue(this->settings, 7, 7);

		allocator.link(-1, GameHeaps[0], 0, 0x20);
	
		this->resFile.data = getResource("galaxy", "g3d/needle.brres");
		nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl(((needleType == 0) ? "NeedlePlant" : "NeedlePlantFly"));
		bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
		SetupTextures_Player(&bodyModel, 0);
	
		allocator.unlink();
	
	
		// Stuff I do understand
		this->scale = ((needleScale == 0) ? (Vec){0.15, 0.15, 0.15} : (Vec){0.3, 0.3, 0.3});
	
		this->rot.x = 0;
		this->rot.y = 0;
		this->rot.z = 0;

	
		ActivePhysics::Info HitMeBaby;
		HitMeBaby.xDistToCenter = 0.0;
		HitMeBaby.yDistToCenter = (needleScale == 0) ? 0.0 : 4.5;
		HitMeBaby.xDistToEdge = (needleScale == 0) ? 2.0 : 4.0;
		HitMeBaby.yDistToEdge = (needleScale == 0) ? 8.5 : 17.0;
		HitMeBaby.category1 = 0x3;
		HitMeBaby.category2 = 0x0;
		HitMeBaby.bitfield1 = 0x6F;
		HitMeBaby.bitfield2 = 0xffbafffe;
		HitMeBaby.unkShort1C = 0;
		HitMeBaby.callback = &dEn_c::collisionCallback;
	
		this->aPhysics.initWithStruct(this, &HitMeBaby);
		this->aPhysics.addToList();

	
		ActivePhysics::Info HitMyNeedleBaby;
		HitMyNeedleBaby.xDistToCenter = 0.0;
		HitMyNeedleBaby.yDistToCenter = (needleScale == 0) ? 20.0 : 40.0;
		HitMyNeedleBaby.xDistToEdge = (needleScale == 0) ? 12.0 : 24.0;
		HitMyNeedleBaby.yDistToEdge = (needleScale == 0) ? 12.0 : 24.0;
		HitMyNeedleBaby.category1 = 0x3;
		HitMyNeedleBaby.category2 = 0x0;
		HitMyNeedleBaby.bitfield1 = 0x6F;
		HitMyNeedleBaby.bitfield2 = 0xffbafffe;
		HitMyNeedleBaby.unkShort1C = 0;
		HitMyNeedleBaby.callback = &dEn_c::collisionCallback;
	
		this->aPhysics2.initWithStruct(this, &HitMyNeedleBaby);
		this->aPhysics2.addToList();
	

		this->onExecute();
		return true;
	}
	if(this->type == 5) {
		int compType = getNybbleValue(this->settings, 6, 6);
		int newRot = getNybbleValue(this->settings, 7, 7);
		const char *compModelName [] = {
			"CloudWind2DPlanetA",
			"CloudWind2DPlanetB",
			"CloudWind2DPlanetC",
			"CloudWind2DPlanetD",
			NULL
		};
		allocator.link(-1, GameHeaps[0], 0, 0x20);
	
		this->resFile.data = getResource("galaxy9", "g3d/windcloudparts.brres");
		nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl(compModelName[compType]);
		bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
		SetupTextures_Enemy(&bodyModel, 0);
		SetupTextures_Player(&bodyModel, 0);
	
		allocator.unlink();
	
		int alternate = getNybbleValue(this->settings, 8, 8);
	
		if(compType == 0) {
			/**************/
			/*Main Physics*/
			/**************/
			physicsInfo.x1 = -410;
			physicsInfo.y1 = 180;
			physicsInfo.x2 = 118;
			physicsInfo.y2 = 102;
		
			physicsInfo.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
			physicsInfo.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
			physicsInfo.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;
		
			physics.setup(this, &physicsInfo, 3, currentLayerID);
			physics.flagsMaybe = 0x260;
			physics.callback1 = &daEnBlockMain_c::PhysicsCallback1;
			physics.callback2 = &daEnBlockMain_c::PhysicsCallback2;
			physics.callback3 = &daEnBlockMain_c::PhysicsCallback3;
			physics.addToList();

			/***************/
			/*Left1 Physics*/
			/***************/
			physicsInfoL1.x1 = -118;
			physicsInfoL1.y1 = 102;
			physicsInfoL1.x2 = 118;
			physicsInfoL1.y2 = -300;
		
			physicsInfoL1.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
			physicsInfoL1.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
			physicsInfoL1.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

		
			physicsL1.setup(this, &physicsInfoL1, 3, currentLayerID);
			physicsL1.flagsMaybe = 0x260;
			physicsL1.callback1 = &daEnBlockMain_c::PhysicsCallback1;
			physicsL1.callback2 = &daEnBlockMain_c::PhysicsCallback2;
			physicsL1.callback3 = &daEnBlockMain_c::PhysicsCallback3;

			physicsL1.addToList();

			/***************/
			/*Left2 Physics*/
			/***************/
			physicsInfoL2.x1 = 90;
			physicsInfoL2.y1 = 790;
			physicsInfoL2.x2 = 240;
			physicsInfoL2.y2 = 150;
		
			physicsInfoL2.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
			physicsInfoL2.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
			physicsInfoL2.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

		
			physicsL2.setup(this, &physicsInfoL2, 3, currentLayerID);
			physicsL2.flagsMaybe = 0x260;
			physicsL2.callback1 = &daEnBlockMain_c::PhysicsCallback1;
			physicsL2.callback2 = &daEnBlockMain_c::PhysicsCallback2;
			physicsL2.callback3 = &daEnBlockMain_c::PhysicsCallback3;

			physicsL2.addToList();

			/****************/
			/*Right1 Physics*/
			/****************/
			physicsInfoR1.x1 = 240;
			physicsInfoR1.y1 = 790;
			physicsInfoR1.x2 = 290;
			physicsInfoR1.y2 = 762;
		
			physicsInfoR1.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
			physicsInfoR1.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
			physicsInfoR1.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

		
			physicsR1.setup(this, &physicsInfoR1, 3, currentLayerID);
			physicsR1.flagsMaybe = 0x260;
			physicsR1.callback1 = &daEnBlockMain_c::PhysicsCallback1;
			physicsR1.callback2 = &daEnBlockMain_c::PhysicsCallback2;
			physicsR1.callback3 = &daEnBlockMain_c::PhysicsCallback3;

			physicsR1.addToList();

			/****************/
			/*Right2 Physics*/
			/****************/
			physicsInfoR2.x1 = 0;
			physicsInfoR2.y1 = 196;
			physicsInfoR2.x2 = 80;
			physicsInfoR2.y2 = 180;
		
			physicsInfoR2.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
			physicsInfoR2.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
			physicsInfoR2.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

		
			physicsR2.setup(this, &physicsInfoR2, 3, currentLayerID);
			physicsR2.flagsMaybe = 0x260;
			physicsR2.callback1 = &daEnBlockMain_c::PhysicsCallback1;
			physicsR2.callback2 = &daEnBlockMain_c::PhysicsCallback2;
			physicsR2.callback3 = &daEnBlockMain_c::PhysicsCallback3;

			physicsR2.addToList();

			/*****************/
			/*Bottom1 Physics*/
			/*****************/
			physicsInfoB1.x1 = -16;
			physicsInfoB1.y1 = 192;
			physicsInfoB1.x2 = 0;
			physicsInfoB1.y2 = 180;
		
			physicsInfoB1.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
			physicsInfoB1.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
			physicsInfoB1.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

		
			physicsB1.setup(this, &physicsInfoB1, 3, currentLayerID);
			physicsB1.flagsMaybe = 0x260;
			physicsB1.callback1 = &daEnBlockMain_c::PhysicsCallback1;
			physicsB1.callback2 = &daEnBlockMain_c::PhysicsCallback2;
			physicsB1.callback3 = &daEnBlockMain_c::PhysicsCallback3;

			physicsB1.addToList();

			/*****************/
			/*Bottom2 Physics*/
			/*****************/
			physicsInfoB2.x1 = -32;
			physicsInfoB2.y1 = 187;
			physicsInfoB2.x2 = -16;
			physicsInfoB2.y2 = 180;
		
			physicsInfoB2.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
			physicsInfoB2.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
			physicsInfoB2.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

		
			physicsB2.setup(this, &physicsInfoB2, 3, currentLayerID);
			physicsB2.flagsMaybe = 0x260;
			physicsB2.callback1 = &daEnBlockMain_c::PhysicsCallback1;
			physicsB2.callback2 = &daEnBlockMain_c::PhysicsCallback2;
			physicsB2.callback3 = &daEnBlockMain_c::PhysicsCallback3;

			physicsB2.addToList();

			/****************/
			/*Other1 Physics*/
			/****************/
			physicsInfoO1.x1 = -48;
			physicsInfoO1.y1 = 184;
			physicsInfoO1.x2 = -32;
			physicsInfoO1.y2 = 180;
		
			physicsInfoO1.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
			physicsInfoO1.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
			physicsInfoO1.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

		
			physicsO1.setup(this, &physicsInfoO1, 3, currentLayerID);
			physicsO1.flagsMaybe = 0x260;
			physicsO1.callback1 = &daEnBlockMain_c::PhysicsCallback1;
			physicsO1.callback2 = &daEnBlockMain_c::PhysicsCallback2;
			physicsO1.callback3 = &daEnBlockMain_c::PhysicsCallback3;

			physicsO1.addToList();
		}
		if(compType == 1) {
			/**************/
			/*Main Physics*/
			/**************/
			physicsInfo.x1 = -22;
			physicsInfo.y1 = 74;
			physicsInfo.x2 = 64;
			physicsInfo.y2 = -268;
		
			physicsInfo.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
			physicsInfo.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
			physicsInfo.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;
		
			physics.setup(this, &physicsInfo, 3, currentLayerID);
			physics.flagsMaybe = 0x260;
			physics.callback1 = &daEnBlockMain_c::PhysicsCallback1;
			physics.callback2 = &daEnBlockMain_c::PhysicsCallback2;
			physics.callback3 = &daEnBlockMain_c::PhysicsCallback3;
			physics.addToList();
			
			/*******************/
			/*Right SOT Physics*/
			/*******************/
			float fWidth = 13 * 16.0f;

			sotColliderRight.init(this,
					/*xOffset=*/10.0f, /*yOffset=*/160.0f,
					/*topYOffset=*/0,
					/*rightSize=*/fWidth / 2, /*leftSize=*/-(fWidth / 2) - 8,
					/*rotation=*/-10500, /*_45=*/1
					);
		
			// What is this for. I dunno
			sotColliderRight._47 = 0xA;
			sotColliderRight.flags = 0x80180 | 0xC00;
		
			sotColliderRight.addToList();
	
			sotColliderLeft.init(this,
					/*xOffset=*/-72.0f, /*yOffset=*/254.0f,
					/*topYOffset=*/0,
					/*rightSize=*/22.0f, /*leftSize=*/0.0f,
					/*rotation=*/0, /*_45=*/1
					);
		
			// What is this for. I dunno
			sotColliderLeft._47 = 0xA;
			sotColliderLeft.flags = 0x80180 | 0xC00;
		
			sotColliderLeft.addToList();

			/***************/
			/*Left1 Physics*/
			/***************/
			physicsInfoL1.x1 = -60;
			physicsInfoL1.y1 = 216;
			physicsInfoL1.x2 = -40;
			physicsInfoL1.y2 = 35;
		
			physicsInfoL1.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
			physicsInfoL1.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
			physicsInfoL1.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

		
			physicsL1.setup(this, &physicsInfoL1, 3, currentLayerID);
			physicsL1.flagsMaybe = 0x260;
			physicsL1.callback1 = &daEnBlockMain_c::PhysicsCallback1;
			physicsL1.callback2 = &daEnBlockMain_c::PhysicsCallback2;
			physicsL1.callback3 = &daEnBlockMain_c::PhysicsCallback3;

			physicsL1.addToList();

			/***************/
			/*Left2 Physics*/
			/***************/
			physicsInfoL2.x1 = -72;
			physicsInfoL2.y1 = 254;
			physicsInfoL2.x2 = -68;
			physicsInfoL2.y2 = 216;
		
			physicsInfoL2.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
			physicsInfoL2.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
			physicsInfoL2.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

		
			physicsL2.setup(this, &physicsInfoL2, 3, currentLayerID);
			physicsL2.flagsMaybe = 0x260;
			physicsL2.callback1 = &daEnBlockMain_c::PhysicsCallback1;
			physicsL2.callback2 = &daEnBlockMain_c::PhysicsCallback2;
			physicsL2.callback3 = &daEnBlockMain_c::PhysicsCallback3;

			physicsL2.addToList();

			/****************/
			/*Right1 Physics*/
			/****************/
			physicsInfoR1.x1 = -40;
			physicsInfoR1.y1 = 74;
			physicsInfoR1.x2 = -22;
			physicsInfoR1.y2 = 35;
		
			physicsInfoR1.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
			physicsInfoR1.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
			physicsInfoR1.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

		
			physicsR1.setup(this, &physicsInfoR1, 3, currentLayerID);
			physicsR1.flagsMaybe = 0x260;
			physicsR1.callback1 = &daEnBlockMain_c::PhysicsCallback1;
			physicsR1.callback2 = &daEnBlockMain_c::PhysicsCallback2;
			physicsR1.callback3 = &daEnBlockMain_c::PhysicsCallback3;

			physicsR1.addToList();
		}
		if(compType == 2) {
			/**************/
			/*Main Physics*/
			/**************/
			physicsInfo.x1 = -138;
			physicsInfo.y1 = 26;
			physicsInfo.x2 = -50;
			physicsInfo.y2 = -318;
		
			physicsInfo.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
			physicsInfo.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
			physicsInfo.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;
		
			physics.setup(this, &physicsInfo, 3, currentLayerID);
			physics.flagsMaybe = 0x260;
			physics.callback1 = &daEnBlockMain_c::PhysicsCallback1;
			physics.callback2 = &daEnBlockMain_c::PhysicsCallback2;
			physics.callback3 = &daEnBlockMain_c::PhysicsCallback3;
			physics.addToList();
			
			/*******************/
			/*Right SOT Physics*/
			/*******************/
			float fWidth = 13 * 16.0f;

			sotColliderRight.init(this,
					/*xOffset=*/-80.0f, /*yOffset=*/114.0f,
					/*topYOffset=*/0,
					/*rightSize=*/(fWidth / 2) + 22, /*leftSize=*/-(fWidth / 2),
					/*rotation=*/10500, /*_45=*/1
					);
		
			// What is this for. I dunno
			sotColliderRight._47 = 0xA;
			sotColliderRight.flags = 0x80180 | 0xC00;
		
			sotColliderRight.addToList();
	
			sotColliderLeft.init(this,
					/*xOffset=*/-12.0f, /*yOffset=*/220.0f,
					/*topYOffset=*/0,
					/*rightSize=*/158.0f, /*leftSize=*/0.0f,
					/*rotation=*/0, /*_45=*/1
					);
		
			// What is this for. I dunno
			sotColliderLeft._47 = 0xA;
			sotColliderLeft.flags = 0x80180 | 0xC00;
		
			sotColliderLeft.addToList();

			/***************/
			/*Left1 Physics*/
			/***************/
			physicsInfoL1.x1 = 12;
			physicsInfoL1.y1 = 220;
			physicsInfoL1.x2 = 146;
			physicsInfoL1.y2 = 128;
		
			physicsInfoL1.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
			physicsInfoL1.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
			physicsInfoL1.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

		
			physicsL1.setup(this, &physicsInfoL1, 3, currentLayerID);
			physicsL1.flagsMaybe = 0x260;
			physicsL1.callback1 = &daEnBlockMain_c::PhysicsCallback1;
			physicsL1.callback2 = &daEnBlockMain_c::PhysicsCallback2;
			physicsL1.callback3 = &daEnBlockMain_c::PhysicsCallback3;

			physicsL1.addToList();

			/***************/
			/*Left2 Physics*/
			/***************/
			physicsInfoL2.x1 = -12;
			physicsInfoL2.y1 = 180;
			physicsInfoL2.x2 = 12;
			physicsInfoL2.y2 = 128;
		
			physicsInfoL2.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
			physicsInfoL2.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
			physicsInfoL2.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

		
			physicsL2.setup(this, &physicsInfoL2, 3, currentLayerID);
			physicsL2.flagsMaybe = 0x260;
			physicsL2.callback1 = &daEnBlockMain_c::PhysicsCallback1;
			physicsL2.callback2 = &daEnBlockMain_c::PhysicsCallback2;
			physicsL2.callback3 = &daEnBlockMain_c::PhysicsCallback3;

			physicsL2.addToList();

			/****************/
			/*Right1 Physics*/
			/****************/
			physicsInfoR1.x1 = -50;
			physicsInfoR1.y1 = 135;
			physicsInfoR1.x2 = -12;
			physicsInfoR1.y2 = -14;
		
			physicsInfoR1.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
			physicsInfoR1.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
			physicsInfoR1.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

		
			physicsR1.setup(this, &physicsInfoR1, 3, currentLayerID);
			physicsR1.flagsMaybe = 0x260;
			physicsR1.callback1 = &daEnBlockMain_c::PhysicsCallback1;
			physicsR1.callback2 = &daEnBlockMain_c::PhysicsCallback2;
			physicsR1.callback3 = &daEnBlockMain_c::PhysicsCallback3;

			physicsR1.addToList();
		}
		if(compType == 3) {
			/**************/
			/*Main Physics*/
			/**************/
			physicsInfo.x1 = -105;
			physicsInfo.y1 = 94;
			physicsInfo.x2 = 110;
			physicsInfo.y2 = -240;
		
			physicsInfo.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
			physicsInfo.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
			physicsInfo.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;
		
			physics.setup(this, &physicsInfo, 3, currentLayerID);
			physics.flagsMaybe = 0x260;
			physics.callback1 = &daEnBlockMain_c::PhysicsCallback1;
			physics.callback2 = &daEnBlockMain_c::PhysicsCallback2;
			physics.callback3 = &daEnBlockMain_c::PhysicsCallback3;
			physics.addToList();

			/***************/
			/*Left1 Physics*/
			/***************/
			physicsInfoL1.x1 = -40;
			physicsInfoL1.y1 = 108;
			physicsInfoL1.x2 = 45;
			physicsInfoL1.y2 = 94;
		
			physicsInfoL1.otherCallback1 = &daEnBlockMain_c::OPhysicsCallback1;
			physicsInfoL1.otherCallback2 = &daEnBlockMain_c::OPhysicsCallback2;
			physicsInfoL1.otherCallback3 = &daEnBlockMain_c::OPhysicsCallback3;

		
			physicsL1.setup(this, &physicsInfoL1, 3, currentLayerID);
			physicsL1.flagsMaybe = 0x260;
			physicsL1.callback1 = &daEnBlockMain_c::PhysicsCallback1;
			physicsL1.callback2 = &daEnBlockMain_c::PhysicsCallback2;
			physicsL1.callback3 = &daEnBlockMain_c::PhysicsCallback3;

			physicsL1.addToList();
		}

		/************/
		/*Other Shit*/
		/************/
		int rotations [] = {
			0,
			16000,
			32000,
			-16000,
			NULL
		};

		// Stuff I do understand
		this->scale = (Vec){0.2, 0.2, 0.2};
		if(newRot == 0) {
			this->modelRot.x = (alternate == 0) ? 0 : 2000;
			this->modelRot.z = 0;
		}
		if(newRot == 1) {
			this->modelRot.x = 0;
			this->modelRot.z = (alternate == 0) ? 0 : 2000;
		}
		if(newRot == 2) {
			this->modelRot.x = (alternate == 0) ? 0 : -2000;
			this->modelRot.z = 0;
		}
		if(newRot == 3) {
			this->modelRot.x = 0;
			this->modelRot.z = (alternate == 0) ? 0 : -2000;
		}
		this->rot.x = 0;
		this->rot.y = 0;
		this->rot.z = 0;

		this->modelRot.y = rotations[newRot];

		this->pos.z = 2840.0f;

		this->onExecute();
		return true;
	}
	if(this->type == 6) {
		allocator.link(-1, GameHeaps[0], 0, 0x20);
	
		this->resFile.data = getResource("galaxy", "g3d/misc.brres");
		nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("flower");
		bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
		SetupTextures_Item(&bodyModel, 0);
	
		// bool ret;
		// nw4r::g3d::ResAnmChr anmChr = this->resFile.GetResAnmChr("wait2");
		// ret = this->animationChr.setup(mdl, anmChr, &this->allocator, 0);
	
		allocator.unlink();
	
	
		// Stuff I do understand
		this->scale = (Vec){1.4, 1.4, 1.4};
	
		this->rot.x = -3000;
		this->rot.y = 0;
		this->rot.z = 0;
	
	
		ActivePhysics::Info HitMeBaby;
		HitMeBaby.xDistToCenter = 8.0;
		HitMeBaby.yDistToCenter = -15.0;
		HitMeBaby.xDistToEdge = 7.0;
		HitMeBaby.yDistToEdge = 7.0;
		HitMeBaby.category1 = 0x3;
		HitMeBaby.category2 = 0x0;
		HitMeBaby.bitfield1 = 0x6F;
		HitMeBaby.bitfield2 = 0xffbafffe;
		HitMeBaby.unkShort1C = 0;
		HitMeBaby.callback = &dEn_c::collisionCallback;
	
		this->aPhysics.initWithStruct(this, &HitMeBaby);
		this->aPhysics.addToList();
	
		this->pos.z = 3300.0f;
		this->pos.y -= 2;


		// bindAnimChr_and_setUpdateRate("wait2", 1, 0.0, 1.0);
	
		this->onExecute();
		return true;
	}
	if(this->type == 7) {
		this->deleteForever = true;

		// Model creation	
		allocator.link(-1, GameHeaps[0], 0, 0x20);

		this->resFile.data = getResource("galaxy", "g3d/misc.brres");
		nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl("cloud");
		bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
		SetupTextures_MapObj(&bodyModel, 0);

		allocator.unlink();

		// Stuff I do understand
		this->scale = (Vec){1.2, 1.2, 1.2};

		this->rot.x = 0;
		this->rot.y = 0;
		this->rot.z = 0;
		this->type7doRespawn = false;
		this->type7timer = 0;


		float fWidth = 3 * 16.0f;
		sotColliderRight.init(this,
				/*xOffset=*/-23.0f, /*yOffset=*/7.0f,
				/*topYOffset=*/0,
				/*rightSize=*/fWidth, /*leftSize=*/0,
				/*rotation=*/0, /*_45=*/1
				);

		// What is this for. I dunno
		sotColliderRight._47 = 0xA;
		sotColliderRight.flags = 0x80180 | 0xC00;

		sotColliderRight.addToList();


		ActivePhysics::Info HitMeBaby;

		HitMeBaby.xDistToCenter = 0.0;
		HitMeBaby.yDistToCenter = 8.0;

		HitMeBaby.xDistToEdge = 24;
		HitMeBaby.yDistToEdge = 1;

		HitMeBaby.category1 = 0x3;
		HitMeBaby.category2 = 0x0;
		HitMeBaby.bitfield1 = 0x6F;
		HitMeBaby.bitfield2 = 0xffbafffe;
		HitMeBaby.unkShort1C = 0;
		HitMeBaby.callback = &dEn_c::collisionCallback;

		this->aPhysics.initWithStruct(this, &HitMeBaby);
		this->aPhysics.addToList();

		this->pos.y += 16;
		this->pos.z = 4000;



		this->onExecute();
		return true;
	}
	if(this->type == 8) {
		this->deleteForever = true;

		// Model creation	
		allocator.link(-1, GameHeaps[0], 0, 0x20);

		this->resFile.data = getResource("star_coin", "g3d/t02.brres");
		nw4r::g3d::ResMdl mdl = this->resFile.GetResMdl(((CanExitThisLevel() == 0) ? "star_coinA" : "star_coinB"));
		bodyModel.setup(mdl, &allocator, 0x224, 1, 0);
		SetupTextures_Player(&bodyModel, 0);

		allocator.unlink();

		ActivePhysics::Info HitMeBaby;

		HitMeBaby.xDistToCenter = 0.0;
		HitMeBaby.yDistToCenter = 0.0;

		HitMeBaby.xDistToEdge = 7.5;
		HitMeBaby.yDistToEdge = 7.5;

		HitMeBaby.category1 = 0x3;
		HitMeBaby.category2 = 0x0;
		HitMeBaby.bitfield1 = 0x6F;
		HitMeBaby.bitfield2 = 0xffbafffe;
		HitMeBaby.unkShort1C = 0;
		HitMeBaby.callback = &dEn_c::collisionCallback;

		this->aPhysics.initWithStruct(this, &HitMeBaby);
		this->aPhysics.addToList();

		// Stuff I do understand
		this->scale = (Vec){0.5, 0.5, 0.5};

		this->rot.x = 0;
		this->rot.y = 0;
		this->rot.z = 0;
		this->type8fastJump = false;
		this->type8timer = false;

		this->pos.z = 4000;


		this->onExecute();
		return true;
	}
}


int daWindmill_c::onDelete() {
	if(this->type == 0) {
		physics.removeFromList();
	}
	if(this->type == 1) {
		int decoType = getNybbleValue(this->settings, 6, 6);
		if(decoType == 1) {
			physics.removeFromList();
		}
	}
	return true;
}

int daWindmill_c::onDraw() {
	bodyModel.scheduleForDrawing();
	return true;
}


void daWindmill_c::updateModelMatrices() {
	float posytrans = 0.0f;
	float posxtrans = 0.0f;
	if(this->type == 1) {
		int decoType = getNybbleValue(this->settings, 6, 6);
		if(decoType == 0) {
			posytrans = -500.0f;
		}
	}
	if(this->type == 6) {
		posxtrans = 8.0f;
		posytrans = -16.0f;
	}
	matrix.translation(pos.x + posxtrans, pos.y + posytrans, pos.z);
	if(this->type == 2 || this->type == 5) {
		matrix.applyRotationYXZ(&modelRot.x, &modelRot.y, &modelRot.z);
	}
	else {
		matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);
	}

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
}

int daWindmill_c::onExecute() {
	bodyModel._vf1C();
	if(this->type == 0 || (this->type == 2 && getNybbleValue(this->settings, 6, 6) == 0)) {
		physics.update();
		sotColliderRight.update();
		sotColliderLeft.update();
	}
	if(this->type == 1) {
		int decoType = getNybbleValue(this->settings, 6, 6);
		if(decoType == 1) {
			physics.update();
		}
	}
	if(this->type == 2 || this->type == 3 || this->type == 5) {
		physics.update();
		physicsL1.update();
		physicsR1.update();
		physicsL2.update();
		physicsR2.update();
		physicsB1.update();
		physicsB2.update();
		physicsO1.update();
		physicsO2.update();
		physicsO3.update();
		physicsO4.update();
		physicsO5.update();
		physicsO6.update();
		sotColliderRight.update();
		sotColliderLeft.update();
	}
	updateModelMatrices();
	if(this->type == 0) {
		if(this->animationChr.isAnimationDone()) {
			this->animationChr.setCurrentFrame(0.0);
		}
	}
	dAcPy_c *player = dAcPy_c::findByID(0);
	if(this->type == 3) {
		// OSReport("type3\n");
		if(this->type3goingback == false) {
			if(this->type3block == 0) {
				if(this->pos.x > (this->type3initPos.x - this->type3distance)) {
					this->pos.x -= this->type3speed;
					// OSReport("nogback, b0, moving %f\n", this->type3speed);
				}
				else {
					this->pos.x = (this->type3initPos.x - this->type3distance);
					this->type3timer++;
					if(this->type3timer >= this->type3delay) {
						this->type3goingback = true;
						this->type3timer = 0;
					}
					// OSReport("nogback, b0, waiting\n");
				}
			}
			if(this->type3block == 1) {
				if(this->pos.x < (this->type3initPos.x + this->type3distance)) {
					this->pos.x += this->type3speed;
					// OSReport("nogback, b1, moving %f\n", this->type3speed);
				}
				else {
					this->pos.x = (this->type3initPos.x + this->type3distance);
					this->type3timer++;
					if(this->type3timer >= this->type3delay) {
						this->type3goingback = true;
						this->type3timer = 0;
					}
					// OSReport("nogback, b1, waiting\n");
				}
			}
		}
		if(this->type3goingback == true) {
			if(this->type3block == 0) {
				if(this->pos.x < (this->type3initPos.x)) {
					this->pos.x += this->type3speed;
					// OSReport("yegback, b0, moving %f\n", this->type3speed);
				}
				else {
					this->pos.x = (this->type3initPos.x);
					this->type3timer++;
					if(this->type3timer >= this->type3delay) {
						this->type3goingback = false;
						this->type3timer = 0;
					}
					// OSReport("yegback, b0, waiting\n");
				}
			}
			if(this->type3block == 1) {
				if(this->pos.x > (this->type3initPos.x)) {
					this->pos.x -= this->type3speed;
					// OSReport("yegback, b1, moving %f\n", this->type3speed);
				}
				else {
					this->pos.x = (this->type3initPos.x);
					this->type3timer++;
					if(this->type3timer >= this->type3delay) {
						this->type3goingback = false;
						this->type3timer = 0;
					}
					// OSReport("yegback, b1, waiting\n");
				}
			}
		}
	}
	if(this->type == 4) {
		this->rot.y += (getNybbleValue(this->settings, 8, 8) == 0) ? 0x200 : -0x200;
	}
	if(this->type == 6) {
		this->rot.y += 0x200;

		if(this->type6wait != 0) {
			this->scale = {0.0, 0.0, 0.0};
			this->type6wait--;
		}
		else {
			this->type6reappearing = true;
			if(this->type6reappearing && this->scale.x < 1.4) {
				this->scale.x += 0.01;
				this->scale.y += 0.01;
				this->scale.z += 0.01;
			}
			if(this->scale.x > 1.4) {
				this->scale = {1.4, 1.4, 1.4};
			}
			if(this->scale.x > 1.0) {
				this->addMyActivePhysics();
			}
		}
	}
	if(this->type == 7) {
		sotColliderRight.update();
		if(this->type7doRespawn) {
			this->type7timer++;
			if(this->type7timer == 1) {
				this->removeMyActivePhysics();
				this->sotColliderRight.removeFromList();
			}
			if(this->type7timer > 240 && this->type7timer < 253) {
				this->scale.x += 0.1;
				this->scale.y += 0.1;
				this->scale.z += 0.1;
				this->rot.y += 68264;
			}
			if(this->type7timer == 253) {
				this->scale = {1.2, 1.2, 1.2};
				this->rot.y = 0;
				this->type7timer = 0;
				this->type7doRespawn = false;
				this->addMyActivePhysics();
				this->sotColliderRight.addToList();
			}
		}
	}
	if(this->type == 8) {
		if(!this->type8fastJump) {
			this->rot.y += 0x200;
		}
		else {
			this->rot.y += 0x2000;
			this->type8timer++;
			if(this->type8timer == 1) {
				this->removeMyActivePhysics();
				StopBGMMusic();
				nw4r::snd::SoundHandle keyHandle;
				PlaySoundWithFunctionB4(SoundRelatedClass, &keyHandle, SE_OBJ_BROOM_KEY_GET, 1);
				WLClass::instance->_4 = 5;
				WLClass::instance->_8 = 0;
				dStage32C_c::instance->freezeMarioBossFlag = 1;
			}
			if(this->type8timer < 16) {
				this->pos.y += 3;
			}
			if(this->type8timer > 15 && this->type8timer < 32) {
				this->pos.y -= 3;
			}
			if(this->type8timer == 32) {
				this->scale = {0.0, 0.0, 0.0};
			}
			if(this->type8p1OnGround == 0) {
				dStageActor_c *player = GetSpecificPlayerActor(0);
				if (player->collMgr.isOnTopOfTile() && player->currentZoneID == currentZoneID) {
					this->type8p1OnGround = 1;
				}
			}
			else {
				this->type8timer2++;
				if(this->type8timer2 == 60) {
					nw4r::snd::StrmSoundHandle starHandle;
					int sID;
					hijackMusicWithSongName("sfx/STAR", -1, false, 2, 1, &sID);			//"Super Mario Galaxy 2" - "You Got A Star !"
					PlaySoundWithFunctionB4(SoundRelatedClass, &starHandle, sID, 1);
					BossGoalForAllPlayers();
					UpdateGameMgr();
				}
				if(this->type8timer2 == 300) {
					CreateActor(191, 12, this->pos, 0, 0);
					this->Delete(1);
				}
			}
		}
	}

	return true;
}






class EN_STAR_COIN : public dEn_c {
	int onCreate_orig();
	int newZorder();
};

int EN_STAR_COIN::newZorder() {
	int orig_val = this->onCreate_orig();
	if(getNybbleValue(this->settings, 6, 6) == 1) {
		this->scale = {0.5, 0.5, 0.5};
		this->pos.z = 3000.0f;
	}
	return orig_val;
}
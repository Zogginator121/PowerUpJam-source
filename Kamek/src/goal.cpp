#include <common.h>
#include <game.h>
#include <g3dhax.h>
#include <sfx.h>
#include "boss.h"



const char* GOarcNameList [] = {
	"goal",
	NULL	
};



class daNewGoal_c : public daEnBlockMain_c {
	int onCreate();
	int onExecute();
	int onDelete();
	int onDraw();

	mHeapAllocator_c allocator;
	nw4r::g3d::ResFile resFile;
	m3d::mdl_c bodyModel;

	bool type8fastJump;
	int type8timer;
	int type8timer2;
	bool type8p1OnGround;

	static daNewGoal_c *build();

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

extern "C" int CanExitThisLevel();
u8 hijackMusicWithSongName(const char *songName, int themeID, bool hasFast, int channelCount, int trackCount, int *wantRealStreamID);

void daNewGoal_c::playerCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
	this->type8fastJump = true;
}
void daNewGoal_c::yoshiCollision(ActivePhysics *apThis, ActivePhysics *apOther) {
	this->type8fastJump = true;
}
bool daNewGoal_c::collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther) {
	this->type8fastJump = true;
	return false;
}
bool daNewGoal_c::collisionCat7_GroundPoundYoshi(ActivePhysics *apThis, ActivePhysics *apOther) {
	this->type8fastJump = true;
	return false;
}
bool daNewGoal_c::collisionCatD_Drill(ActivePhysics *apThis, ActivePhysics *apOther) {
	return false;
}
bool daNewGoal_c::collisionCatA_PenguinMario(ActivePhysics *apThis, ActivePhysics *apOther) {
	return false;
}

bool daNewGoal_c::collisionCat1_Fireball_E_Explosion(ActivePhysics *apThis, ActivePhysics *apOther) {
	return false;
}
bool daNewGoal_c::collisionCat2_IceBall_15_YoshiIce(ActivePhysics *apThis, ActivePhysics *apOther) {

	return false;
}
bool daNewGoal_c::collisionCat9_RollingObject(ActivePhysics *apThis, ActivePhysics *apOther) {

	return false;
}
bool daNewGoal_c::collisionCat13_Hammer(ActivePhysics *apThis, ActivePhysics *apOther) {
	return false;
}
bool daNewGoal_c::collisionCat14_YoshiFire(ActivePhysics *apThis, ActivePhysics *apOther) {
	return false;
}

bool daNewGoal_c::collisionCat3_StarPower(ActivePhysics *apThis, ActivePhysics *apOther) {
	return false;
}



daNewGoal_c *daNewGoal_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(daNewGoal_c));
	return new(buffer) daNewGoal_c;
}

extern int getNybbleValue(u32 settings, int fromNybble, int toNybble);

int daNewGoal_c::onCreate() {
	this->deleteForever = true;

	const char *filename [] = {
 		"g3d/t00.brres",//don´t use it will crash idk why
		"g3d/t01.brres",//Bommerang
		"g3d/t02.brres",//Propeller
		"g3d/t03.brres",//Ice 
		"g3d/t04.brres",//Frog
		"g3d/t05.brres",//Penguin
		"g3d/t06.brres",//Mushroom
		"g3d/t07.brres",//Propeller2 - Not used 
		"g3d/t08.brres",//Star
		"g3d/t09.brres",//Hammer                                                                                                                                                                    
		"g3d/t10.brres",//Spike
		"g3d/t11.brres",//Fire - No Colors in Model, I´ll Fix this later
		"g3d/t12.brres",//Cloud
		"g3d/t13.brres",//Bob-Omb
		"g3d/t14.brres",//Thunder
		"g3d/t15.brres",//Ice2 - Not used 

		NULL
	};
	
	Vec scales [] = {


		//thx to RSM
		//the raw data is displayed in hex 8 is 8, 9 is 9, 10 is A, 11 is B, 12 is C, 13 is D, 14 is E, 15 is F...
		(Vec){1.0, 1.0, 1.0},//1
		(Vec){1.0, 1.0, 1.0},//2
		(Vec){1.0, 1.0, 1.0},//3
		(Vec){0.2, 0.2, 0.2},//4
		(Vec){1.0, 1.0, 1.0},//5
		(Vec){1.0, 1.0, 1.0},//6
		(Vec){1.0, 1.0, 1.0},//7
		(Vec){1.0, 1.0, 1.0},//8
		(Vec){2.0, 2.0, 2.0},//9
		(Vec){1.0, 1.0, 1.0},//11 == B
		(Vec){0.3, 0.3, 0.3},//10 == A
		(Vec){1.0, 1.0, 1.0},//12 == C
		(Vec){1.0, 1.0, 1.0},//13 == D
		(Vec){1.0, 1.0, 1.0},//14 == E
		(Vec){1.0, 1.0, 1.0},//15 == F
		(Vec){1.0, 1.0, 1.0},//00 == XX
		NULL
	};

	// Model creation	
	allocator.link(-1, GameHeaps[0], 0, 0x20);

	this->resFile.data = getResource("goal", filename[getNybbleValue(this->settings, 12, 12)]);
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
	this->scale = scales[getNybbleValue(this->settings, 12, 12)];

	this->rot.x = 90;
	this->rot.y = 0;
	this->rot.z = 0;
	this->type8fastJump = false;
	this->type8timer = false;

	this->pos.z = 4000;


	this->onExecute();
	return true;
}


int daNewGoal_c::onDelete() {
	return true;
}

int daNewGoal_c::onDraw() {
	bodyModel.scheduleForDrawing();
	return true;
}


void daNewGoal_c::updateModelMatrices() {
	matrix.translation(pos.x, pos.y, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
}

int daNewGoal_c::onExecute() {
	bodyModel._vf1C();
	updateModelMatrices();
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
				const char *songname [] = {
					"sfx/MUSHROOM",
					"sfx/FIRE",
					"sfx/PROPELLER",	//"Super Mario 3D World" - "World Clear!"
					"sfx/ICE",			//"Mario vs. Donkey Kong 2 March of the Minis" - "Stage Clear VI"
					NULL
				};

				nw4r::snd::StrmSoundHandle starHandle;
				int sID;
				hijackMusicWithSongName(songname[getNybbleValue(this->settings, 12, 12)], -1, false, 2, 1, &sID);
				PlaySoundWithFunctionB4(SoundRelatedClass, &starHandle, sID, 1);
				BossGoalForAllPlayers();
				UpdateGameMgr();
			}
			if(this->type8timer2 == 300) {
				CreateActor(191, getNybbleValue(this->settings, 12, 12), this->pos, 0, 0);
				this->Delete(1);
			}
		}
	}

	return true;
}
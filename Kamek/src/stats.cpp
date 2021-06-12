#include <common.h>
#include <game.h>
#include <stage.h>
#include "sfx.h"
#include "stats.h"
#include "boomeranghax.h"


extern int globalCoins;
extern int globalIceShoot;
extern int globalPropeller;
extern u8 LastLevelPlayed[2];

CREATE_STATE(dStatsSpawner_c, Hidden);
CREATE_STATE(dStatsSpawner_c, ShowWait);
CREATE_STATE(dStatsSpawner_c, ButtonActivateWait);
CREATE_STATE(dStatsSpawner_c, Wait);
CREATE_STATE(dStatsSpawner_c, HideWait);

dStatsSpawner_c *dStatsSpawner_c::instance = 0;
dStatsSpawner_c *dStatsSpawner_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(dStatsSpawner_c));
	dStatsSpawner_c *c = new(buffer) dStatsSpawner_c;

	instance = c;
	return c;
}

#define ANIM_WINDOW_APPEAR 0
#define ANIM_WINDOW_LOOP 1
#define ANIM_BG_IN 2
#define ANIM_BG_OUT 3
#define ANIM_WINDOW_DISAPPEAR 4

int numberToSelect;
int finalNumber;

/*****************************************************************************/
// Events

dStatsSpawner_c::dStatsSpawner_c() : state(this, &StateID_ShowWait) {
	layoutLoaded = false;
	visible = false;
}

int dStatsSpawner_c::onCreate() {
	if (!layoutLoaded) {
		OSReport("1\n");
		bool gotFile;
		if(this->settings == 2) {
			gotFile = layout.loadArc("PropellerStats.arc", false);
		}
		if(this->settings == 3) {
			gotFile = layout.loadArc("IcyStats.arc", false);
		}
		if(this->settings == 12) {
			gotFile = layout.loadArc("CloudyStats.arc", false);
		}
		if(this->settings == 8) {
			gotFile = layout.loadArc("StarStats.arc", false);
		}
		if(this->settings == 6) {
			gotFile = layout.loadArc("MushroomStats.arc", false);
		}
		if (this->settings == 5) {
			gotFile = layout.loadArc("PenuinStats.arc", false);
		}
		if (!gotFile)
			return false;

		selected = 1;
		lastTopRowChoice = 1;

		layout.build("Stats.brlyt");

		OSReport("2\n");
		if (IsWideScreen()) {
			layout.layout.rootPane->scale.x = 1.0f;
		} else {
			layout.clippingEnabled = true;
			layout.clipX = 0;
			layout.clipY = 52;
			layout.clipWidth = 640;
			layout.clipHeight = 352;
			layout.layout.rootPane->scale.x = 0.731f;
			layout.layout.rootPane->scale.y = 0.7711f;
		}

		OSReport("3\n");
		static const char *brlanNames[] = {
			"Stats_appearWindow.brlan",
			"Stats_loopWindow.brlan",
			"Stats_inBG.brlan",
			"Stats_outBG.brlan",
			"Stats_disappearWindow.brlan",
		};

		OSReport("4\n");
		static const char *groupNames[] = {
			"A00_inWindow",
			"A00_inWindow",
			"C00_BG_00", 
			"C00_BG_00",
			"A00_inWindow",
		};
		static const int brlanIDs[] = {
			0,
			1,
			2,
			3,
			4,
		};

		OSReport("5\n");

		layout.loadAnimations(brlanNames, 5);
		OSReport("loadAnimations\n");
		layout.loadGroups(groupNames, brlanIDs, 5);
		OSReport("loadGroups\n");
		layout.disableAllAnimations();
		OSReport("disableAllAnimations\n");

		layout.drawOrder = 140;
		OSReport("6\n");

		static const char *tbNames[] = {
			"T_Title_00",
			"T_SC_00", "T_Score_00",
			"T_Coin_00", "T_Coin_01", "T_Score_01",
			"T_Timer_00", "T_Timer_01", "T_Score_02",
			"T_Total_00", "T_Total_01",
			"T_Rank_00",
		};
		layout.getTextBoxes(tbNames, &T_Title_00, 12);

		OSReport("7\n");
		P_BG_00 = layout.findPictureByName("P_BG_00");
		WindowLeft = layout.findPictureByName("WindowLeft");
		WindowCentre = layout.findPictureByName("WindowCentre");
		WindowRight = layout.findPictureByName("WindowRight");
		P_SCoff_00 = layout.findPictureByName("P_SCoff_00");
		P_SCoff_01 = layout.findPictureByName("P_SCoff_01");
		P_SCoff_02 = layout.findPictureByName("P_SCoff_02");
		P_SCon_00 = layout.findPictureByName("P_SCon_00");
		P_SCon_01 = layout.findPictureByName("P_SCon_01");
		P_SCon_02 = layout.findPictureByName("P_SCon_02");
		P_Coin_00 = layout.findPictureByName("P_Coin_00");
		P_Timer_00 = layout.findPictureByName("P_Timer_00");
		OSReport("8\n");




		/*****************/
		/*Penuin Stats*/		//^done
		/*****************/
		if (this->settings == 5) {
			/**********************/
			/*Star Coin Visibility*/
			/**********************/
			bool isStarCoin1 = (dGameDisplay_c::instance->layout.findPictureByName("P_collection_00")->flag == 1);
			bool isStarCoin2 = (dGameDisplay_c::instance->layout.findPictureByName("P_collection_01")->flag == 1);
			bool isStarCoin3 = (dGameDisplay_c::instance->layout.findPictureByName("P_collection_02")->flag == 1);

			P_SCoff_00->SetVisible(!isStarCoin1);
			P_SCon_00->SetVisible(isStarCoin1);

			P_SCoff_01->SetVisible(!isStarCoin2);
			P_SCon_01->SetVisible(isStarCoin2);

			P_SCoff_02->SetVisible(!isStarCoin3);
			P_SCon_02->SetVisible(isStarCoin3);


			/*************/
			/*Coins Count*/
			/*************/
			char coinCount[32];
			if (globalCoins > 102) {
				globalCoins = 102;
			}
			sprintf(coinCount, "%03d/102", globalCoins);//dGameDisplay_c::instance->coins);
			wchar_t coinCountW[32];
			coinCountW[0] = coinCount[0];
			coinCountW[1] = coinCount[1];
			coinCountW[2] = coinCount[2];
			coinCountW[3] = coinCount[3];
			coinCountW[4] = coinCount[4];
			coinCountW[5] = coinCount[5];
			coinCountW[6] = coinCount[6];
			T_Coin_01->SetString(coinCountW);


			/************/
			/*Ice Shoots*/
			/************/
			char timeCount[32];
			wchar_t timeCountW[32];
			if (globalIceShoot < 100) {
				sprintf(timeCount, "%02d", globalIceShoot);
				timeCountW[0] = timeCount[0];
				timeCountW[1] = timeCount[1];
				timeCountW[2] = timeCount[2];
			}
			else {
				sprintf(timeCount, ">99");
				timeCountW[0] = timeCount[0];
				timeCountW[1] = timeCount[1];
				timeCountW[2] = timeCount[2];
			}
			T_Timer_01->SetString(timeCountW);


			/*****************/
			/*Star Coin Score*/ //Max: 150
			/*****************/
			if (isStarCoin1) {
				score1 += 50;
			}
			if (isStarCoin2) {
				score1 += 50;
			}
			if (isStarCoin3) {
				score1 += 50;
			}
			char firstScore[32];
			sprintf(firstScore, "+%d", score1);
			wchar_t firstScoreW[32];
			firstScoreW[0] = firstScore[0];
			firstScoreW[1] = firstScore[1];
			firstScoreW[2] = firstScore[2];
			firstScoreW[3] = firstScore[3];
			T_Score_00->SetString(firstScoreW);

			/*************/
			/*Coins Score*/ //Max: 102
			/*************/
			score2 = globalCoins;
			char secondScore[32];
			sprintf(secondScore, "+%d", score2);
			wchar_t secondScoreW[32];
			secondScoreW[0] = secondScore[0];
			secondScoreW[1] = secondScore[1];
			secondScoreW[2] = secondScore[2];
			secondScoreW[3] = secondScore[3];
			T_Score_01->SetString(secondScoreW);

			/***************/
			/*Iceball Score*/ //Minimum: 20 shoots -> 50 points
			/***************/
			score3 = (globalIceShoot <= 20) ? 50 : (50 - ((globalIceShoot - 20) * 5));
			if (score3 < 0) {
				score3 = 0;
			}
			char thirdScore[32];
			sprintf(thirdScore, "+%d", score3);
			wchar_t thirdScoreW[32];
			thirdScoreW[0] = thirdScore[0];
			thirdScoreW[1] = thirdScore[1];
			thirdScoreW[2] = thirdScore[2];
			T_Score_02->SetString(thirdScoreW);

			/*************/
			/*Total Score*/
			/*************/
			score4 = score1 + score2 + score3;
			char lastScore[32];
			sprintf(lastScore, "%d", score4);
			wchar_t lastScoreW[32];
			lastScoreW[0] = lastScore[0];
			lastScoreW[1] = lastScore[1];
			lastScoreW[2] = lastScore[2];
			lastScoreW[3] = lastScore[3];
			T_Total_01->SetString(lastScoreW);

			/************/ //Mushroom: 0
			/*Final Rank*/ //Flower: 151
			/************/ //Star: 302
			char rank[32];
			if (score4 >= 0 && score4 < 151) {
				sprintf(rank, "Rank ;"); //Mushroom
				this->rank = 1;
			}
			if (score4 >= 151 && score4 < 302) {
				sprintf(rank, "Rank <"); //Flower
				this->rank = 2;
			}
			if (score4 >= 302) {
				sprintf(rank, "Rank ="); //Star
				this->rank = 3;
			}
			wchar_t rankW[32];
			rankW[0] = rank[0];
			rankW[1] = rank[1];
			rankW[2] = rank[2];
			rankW[3] = rank[3];
			rankW[4] = rank[4];
			rankW[5] = rank[5];
			T_Rank_00->SetString(rankW);
		}
		/*****************/
		/*Star Stats*/			//^done
		/*****************/
		if (this->settings == 8) {
			/**********************/
			/*Star Coin Visibility*/
			/**********************/
			bool isStarCoin1 = (dGameDisplay_c::instance->layout.findPictureByName("P_collection_00")->flag == 1);
			bool isStarCoin2 = (dGameDisplay_c::instance->layout.findPictureByName("P_collection_01")->flag == 1);
			bool isStarCoin3 = (dGameDisplay_c::instance->layout.findPictureByName("P_collection_02")->flag == 1);

			P_SCoff_00->SetVisible(!isStarCoin1);
			P_SCon_00->SetVisible(isStarCoin1);

			P_SCoff_01->SetVisible(!isStarCoin2);
			P_SCon_01->SetVisible(isStarCoin2);

			P_SCoff_02->SetVisible(!isStarCoin3);
			P_SCon_02->SetVisible(isStarCoin3);


			/*************/
			/*Coins Count*/
			/*************/
			char coinCount[32];
			if (globalCoins > 264) {
				globalCoins = 264;
			}
			sprintf(coinCount, "%03d/265", (globalCoins + 1));//dGameDisplay_c::instance->coins);
			wchar_t coinCountW[32];
			coinCountW[0] = coinCount[0];
			coinCountW[1] = coinCount[1];
			coinCountW[2] = coinCount[2];
			coinCountW[3] = coinCount[3];
			coinCountW[4] = coinCount[4];
			coinCountW[5] = coinCount[5];
			coinCountW[6] = coinCount[6];
			T_Coin_01->SetString(coinCountW);


			/***********/
			/*Time Left*/
			/***********/
			char timeCount[32];
			sprintf(timeCount, "%03d", dGameDisplay_c::instance->timer);
			wchar_t timeCountW[32];
			timeCountW[0] = timeCount[0];
			timeCountW[1] = timeCount[1];
			timeCountW[2] = timeCount[2];
			T_Timer_01->SetString(timeCountW);


			/*****************/
			/*Star Coin Score*/ //Max: 150
			/*****************/
			if (isStarCoin1) {
				score1 += 50;
			}
			if (isStarCoin2) {
				score1 += 50;
			}
			if (isStarCoin3) {
				score1 += 50;
			}
			char firstScore[32];
			sprintf(firstScore, "+%d", score1);
			wchar_t firstScoreW[32];
			firstScoreW[0] = firstScore[0];
			firstScoreW[1] = firstScore[1];
			firstScoreW[2] = firstScore[2];
			firstScoreW[3] = firstScore[3];
			T_Score_00->SetString(firstScoreW);

			/*************/
			/*Coins Score*/ //Max: 265
			/*************/
			score2 += globalCoins + 1;
			char secondScore[32];
			sprintf(secondScore, "+%d", score2);
			wchar_t secondScoreW[32];
			secondScoreW[0] = secondScore[0];
			secondScoreW[1] = secondScore[1];
			secondScoreW[2] = secondScore[2];
			secondScoreW[3] = secondScore[3];
			T_Score_01->SetString(secondScoreW);

			/************/
			/*Time Score*/ //Minimum Time: 200 //Max: 50
			/************/
			score3 = dGameDisplay_c::instance->timer / 4;
			if (score3 > 50) {
				score3 = 50;
			}
			char thirdScore[32];
			sprintf(thirdScore, "+%d", score3);
			wchar_t thirdScoreW[32];
			thirdScoreW[0] = thirdScore[0];
			thirdScoreW[1] = thirdScore[1];
			thirdScoreW[2] = thirdScore[2];
			thirdScoreW[3] = thirdScore[3];
			T_Score_02->SetString(thirdScoreW);

			/*************/
			/*Total Score*/
			/*************/
			score4 = score1 + score2 + score3;
			char lastScore[32];
			sprintf(lastScore, "%d", score4);
			wchar_t lastScoreW[32];
			lastScoreW[0] = lastScore[0];
			lastScoreW[1] = lastScore[1];
			lastScoreW[2] = lastScore[2];
			lastScoreW[3] = lastScore[3];
			T_Total_01->SetString(lastScoreW);

			/************/ //Mushroom: 0
			/*Final Rank*/ //Flower: 230
			/************/ //Star: 465
			char rank[32];
			if (score4 >= 0 && score4 < 230) {
				sprintf(rank, "Rank ;"); //Mushroom
				this->rank = 1;
			}
			if (score4 >= 230 && score4 < 465) {
				sprintf(rank, "Rank <"); //Flower
				this->rank = 2;
			}
			if (score4 >= 465) {
				sprintf(rank, "Rank ="); //Star
				this->rank = 3;
			}
			wchar_t rankW[32];
			rankW[0] = rank[0];
			rankW[1] = rank[1];
			rankW[2] = rank[2];
			rankW[3] = rank[3];
			rankW[4] = rank[4];
			rankW[5] = rank[5];
			T_Rank_00->SetString(rankW);
		}
		/*****************/
		/*Mushroom Stats*/		//^done
		/*****************/
		if (this->settings == 6) {
			/**********************/
			/*Star Coin Visibility*/
			/**********************/
			bool isStarCoin1 = (dGameDisplay_c::instance->layout.findPictureByName("P_collection_00")->flag == 1);
			bool isStarCoin2 = (dGameDisplay_c::instance->layout.findPictureByName("P_collection_01")->flag == 1);
			bool isStarCoin3 = (dGameDisplay_c::instance->layout.findPictureByName("P_collection_02")->flag == 1);

			P_SCoff_00->SetVisible(!isStarCoin1);
			P_SCon_00->SetVisible(isStarCoin1);

			P_SCoff_01->SetVisible(!isStarCoin2);
			P_SCon_01->SetVisible(isStarCoin2);

			P_SCoff_02->SetVisible(!isStarCoin3);
			P_SCon_02->SetVisible(isStarCoin3);


			/*************/
			/*Coins Count*/
			/*************/
			char coinCount[32];
			if (globalCoins > 264) {
				globalCoins = 264;
			}
			sprintf(coinCount, "%03d/265", (globalCoins + 1));//dGameDisplay_c::instance->coins);
			wchar_t coinCountW[32];
			coinCountW[0] = coinCount[0];
			coinCountW[1] = coinCount[1];
			coinCountW[2] = coinCount[2];
			coinCountW[3] = coinCount[3];
			coinCountW[4] = coinCount[4];
			coinCountW[5] = coinCount[5];
			coinCountW[6] = coinCount[6];
			T_Coin_01->SetString(coinCountW);


			/***********/
			/*Time Left*/
			/***********/
			char timeCount[32];
			sprintf(timeCount, "%03d", dGameDisplay_c::instance->timer);
			wchar_t timeCountW[32];
			timeCountW[0] = timeCount[0];
			timeCountW[1] = timeCount[1];
			timeCountW[2] = timeCount[2];
			T_Timer_01->SetString(timeCountW);


			/*****************/
			/*Star Coin Score*/ //Max: 150
			/*****************/
			if (isStarCoin1) {
				score1 += 50;
			}
			if (isStarCoin2) {
				score1 += 50;
			}
			if (isStarCoin3) {
				score1 += 50;
			}
			char firstScore[32];
			sprintf(firstScore, "+%d", score1);
			wchar_t firstScoreW[32];
			firstScoreW[0] = firstScore[0];
			firstScoreW[1] = firstScore[1];
			firstScoreW[2] = firstScore[2];
			firstScoreW[3] = firstScore[3];
			T_Score_00->SetString(firstScoreW);

			/*************/
			/*Coins Score*/ //Max: 265
			/*************/
			score2 += globalCoins + 1;
			char secondScore[32];
			sprintf(secondScore, "+%d", score2);
			wchar_t secondScoreW[32];
			secondScoreW[0] = secondScore[0];
			secondScoreW[1] = secondScore[1];
			secondScoreW[2] = secondScore[2];
			secondScoreW[3] = secondScore[3];
			T_Score_01->SetString(secondScoreW);

			/************/
			/*Time Score*/ //Minimum Time: 200 //Max: 50
			/************/
			score3 = dGameDisplay_c::instance->timer / 4;
			if (score3 > 50) {
				score3 = 50;
			}
			char thirdScore[32];
			sprintf(thirdScore, "+%d", score3);
			wchar_t thirdScoreW[32];
			thirdScoreW[0] = thirdScore[0];
			thirdScoreW[1] = thirdScore[1];
			thirdScoreW[2] = thirdScore[2];
			thirdScoreW[3] = thirdScore[3];
			T_Score_02->SetString(thirdScoreW);

			/*************/
			/*Total Score*/
			/*************/
			score4 = score1 + score2 + score3;
			char lastScore[32];
			sprintf(lastScore, "%d", score4);
			wchar_t lastScoreW[32];
			lastScoreW[0] = lastScore[0];
			lastScoreW[1] = lastScore[1];
			lastScoreW[2] = lastScore[2];
			lastScoreW[3] = lastScore[3];
			T_Total_01->SetString(lastScoreW);

			/************/ //Mushroom: 0
			/*Final Rank*/ //Flower: 230
			/************/ //Star: 465
			char rank[32];
			if (score4 >= 0 && score4 < 230) {
				sprintf(rank, "Rank ;"); //Mushroom
				this->rank = 1;
			}
			if (score4 >= 230 && score4 < 465) {
				sprintf(rank, "Rank <"); //Flower
				this->rank = 2;
			}
			if (score4 >= 465) {
				sprintf(rank, "Rank ="); //Star
				this->rank = 3;
			}
			wchar_t rankW[32];
			rankW[0] = rank[0];
			rankW[1] = rank[1];
			rankW[2] = rank[2];
			rankW[3] = rank[3];
			rankW[4] = rank[4];
			rankW[5] = rank[5];
			T_Rank_00->SetString(rankW);
		}
		/*****************/
		/*Propeller Stats*/		//^done
		/*****************/
		if(this->settings == 2) {
			/**********************/
			/*Star Coin Visibility*/
			/**********************/
			bool isStarCoin1 = (dGameDisplay_c::instance->layout.findPictureByName("P_collection_00")->flag == 1);
			bool isStarCoin2 = (dGameDisplay_c::instance->layout.findPictureByName("P_collection_01")->flag == 1);
			bool isStarCoin3 = (dGameDisplay_c::instance->layout.findPictureByName("P_collection_02")->flag == 1);

			P_SCoff_00->SetVisible(!isStarCoin1);
			P_SCon_00->SetVisible(isStarCoin1);

			P_SCoff_01->SetVisible(!isStarCoin2);
			P_SCon_01->SetVisible(isStarCoin2);

			P_SCoff_02->SetVisible(!isStarCoin3);
			P_SCon_02->SetVisible(isStarCoin3);


			/*************/
			/*Coins Count*/
			/*************/
			char coinCount[32];
			if(globalCoins > 69) {
				globalCoins = 69;
			}
			sprintf(coinCount, "%03d/IDK", globalCoins);//dGameDisplay_c::instance->coins);
			wchar_t coinCountW[32];
			coinCountW[0] = coinCount[0];
			coinCountW[1] = coinCount[1];
			coinCountW[2] = coinCount[2];
			coinCountW[3] = coinCount[3];
			coinCountW[4] = coinCount[4];
			coinCountW[5] = coinCount[5];
			coinCountW[6] = coinCount[6];
			T_Coin_01->SetString(coinCountW);


			/*****************/
			/*Propeller Jumps*/
			/*****************/
			char timeCount[32];
			wchar_t timeCountW[32];
			if(globalPropeller < 100) {
				sprintf(timeCount, "%02d", globalPropeller);
				timeCountW[0] = timeCount[0];
				timeCountW[1] = timeCount[1];
				timeCountW[2] = timeCount[2];
			}
			else {
				sprintf(timeCount, ">99");
				timeCountW[0] = timeCount[0];
				timeCountW[1] = timeCount[1];
				timeCountW[2] = timeCount[2];
			}
			T_Timer_01->SetString(timeCountW);


			/*****************/
			/*Star Coin Score*/ //Max: 150
			/*****************/
			if(isStarCoin1) {
				score1 += 50;
			}
			if(isStarCoin2) {
				score1 += 50;
			}
			if(isStarCoin3) {
				score1 += 50;
			}
			char firstScore[32];
			sprintf(firstScore, "+%d", score1);
			wchar_t firstScoreW[32];
			firstScoreW[0] = firstScore[0];
			firstScoreW[1] = firstScore[1];
			firstScoreW[2] = firstScore[2];
			firstScoreW[3] = firstScore[3];
			T_Score_00->SetString(firstScoreW);

			/*************/
			/*Coins Score*/ //Max: IDK
			/*************/
			score2 = globalCoins;
			char secondScore[32];
			sprintf(secondScore, "+%d", score2);
			wchar_t secondScoreW[32];
			secondScoreW[0] = secondScore[0];
			secondScoreW[1] = secondScore[1];
			secondScoreW[2] = secondScore[2];
			secondScoreW[3] = secondScore[3];
			T_Score_01->SetString(secondScoreW);

			/*****************/
			/*Propeller Score*/ //Minimum: IDK propels -> 50 points
			/*****************/
			score3 = (globalPropeller <= 20) ? 50 : (50 - ((globalPropeller - 20) * 5));
			if(score3 < 0) {
				score3 = 0;
			}
			char thirdScore[32];
			sprintf(thirdScore, "+%d", score3);
			wchar_t thirdScoreW[32];
			thirdScoreW[0] = thirdScore[0];
			thirdScoreW[1] = thirdScore[1];
			thirdScoreW[2] = thirdScore[2];
			T_Score_02->SetString(thirdScoreW);

			/*************/
			/*Total Score*/
			/*************/
			score4 = score1 + score2 + score3;
			char lastScore[32];
			sprintf(lastScore, "%d", score4);
			wchar_t lastScoreW[32];
			lastScoreW[0] = lastScore[0];
			lastScoreW[1] = lastScore[1];
			lastScoreW[2] = lastScore[2];
			lastScoreW[3] = lastScore[3];
			T_Total_01->SetString(lastScoreW);

			/************/ //Mushroom: 0
			/*Final Rank*/ //Flower: IDK
			/************/ //Star: IDK
			char rank[32];
			if(score4 >= 0 && score4 < 135) {
				sprintf(rank, "Rank ;"); //Mushroom
				this->rank = 1;
			}
			if(score4 >= 135 && score4 < 270) {
				sprintf(rank, "Rank <"); //Flower
				this->rank = 2;
			}
			if(score4 >= 270) {
				sprintf(rank, "Rank ="); //Star
				this->rank = 3;
			}
			wchar_t rankW[32];
			rankW[0] = rank[0];
			rankW[1] = rank[1];
			rankW[2] = rank[2];
			rankW[3] = rank[3];
			rankW[4] = rank[4];
			rankW[5] = rank[5];
			T_Rank_00->SetString(rankW);
		}
		/***********/
		/*Ice Stats*/			//^done
		/***********/
		if(this->settings == 3) {
			/**********************/
			/*Star Coin Visibility*/
			/**********************/
			bool isStarCoin1 = (dGameDisplay_c::instance->layout.findPictureByName("P_collection_00")->flag == 1);
			bool isStarCoin2 = (dGameDisplay_c::instance->layout.findPictureByName("P_collection_01")->flag == 1);
			bool isStarCoin3 = (dGameDisplay_c::instance->layout.findPictureByName("P_collection_02")->flag == 1);

			P_SCoff_00->SetVisible(!isStarCoin1);
			P_SCon_00->SetVisible(isStarCoin1);

			P_SCoff_01->SetVisible(!isStarCoin2);
			P_SCon_01->SetVisible(isStarCoin2);

			P_SCoff_02->SetVisible(!isStarCoin3);
			P_SCon_02->SetVisible(isStarCoin3);


			/*************/
			/*Coins Count*/
			/*************/
			char coinCount[32];
			if(globalCoins > 102) {
				globalCoins = 102;
			}
			sprintf(coinCount, "%03d/102", globalCoins);//dGameDisplay_c::instance->coins);
			wchar_t coinCountW[32];
			coinCountW[0] = coinCount[0];
			coinCountW[1] = coinCount[1];
			coinCountW[2] = coinCount[2];
			coinCountW[3] = coinCount[3];
			coinCountW[4] = coinCount[4];
			coinCountW[5] = coinCount[5];
			coinCountW[6] = coinCount[6];
			T_Coin_01->SetString(coinCountW);


			/************/
			/*Ice Shoots*/
			/************/
			char timeCount[32];
			wchar_t timeCountW[32];
			if(globalIceShoot < 100) {
				sprintf(timeCount, "%02d", globalIceShoot);
				timeCountW[0] = timeCount[0];
				timeCountW[1] = timeCount[1];
				timeCountW[2] = timeCount[2];
			}
			else {
				sprintf(timeCount, ">99");
				timeCountW[0] = timeCount[0];
				timeCountW[1] = timeCount[1];
				timeCountW[2] = timeCount[2];
			}
			T_Timer_01->SetString(timeCountW);


			/*****************/
			/*Star Coin Score*/ //Max: 150
			/*****************/
			if(isStarCoin1) {
				score1 += 50;
			}
			if(isStarCoin2) {
				score1 += 50;
			}
			if(isStarCoin3) {
				score1 += 50;
			}
			char firstScore[32];
			sprintf(firstScore, "+%d", score1);
			wchar_t firstScoreW[32];
			firstScoreW[0] = firstScore[0];
			firstScoreW[1] = firstScore[1];
			firstScoreW[2] = firstScore[2];
			firstScoreW[3] = firstScore[3];
			T_Score_00->SetString(firstScoreW);

			/*************/
			/*Coins Score*/ //Max: 102
			/*************/
			score2 = globalCoins;
			char secondScore[32];
			sprintf(secondScore, "+%d", score2);
			wchar_t secondScoreW[32];
			secondScoreW[0] = secondScore[0];
			secondScoreW[1] = secondScore[1];
			secondScoreW[2] = secondScore[2];
			secondScoreW[3] = secondScore[3];
			T_Score_01->SetString(secondScoreW);

			/***************/
			/*Iceball Score*/ //Minimum: 20 shoots -> 50 points
			/***************/
			score3 = (globalIceShoot <= 20) ? 50 : (50 - ((globalIceShoot - 20) * 5));
			if(score3 < 0) {
				score3 = 0;
			}
			char thirdScore[32];
			sprintf(thirdScore, "+%d", score3);
			wchar_t thirdScoreW[32];
			thirdScoreW[0] = thirdScore[0];
			thirdScoreW[1] = thirdScore[1];
			thirdScoreW[2] = thirdScore[2];
			T_Score_02->SetString(thirdScoreW);

			/*************/
			/*Total Score*/
			/*************/
			score4 = score1 + score2 + score3;
			char lastScore[32];
			sprintf(lastScore, "%d", score4);
			wchar_t lastScoreW[32];
			lastScoreW[0] = lastScore[0];
			lastScoreW[1] = lastScore[1];
			lastScoreW[2] = lastScore[2];
			lastScoreW[3] = lastScore[3];
			T_Total_01->SetString(lastScoreW);

			/************/ //Mushroom: 0
			/*Final Rank*/ //Flower: 151
			/************/ //Star: 302
			char rank[32];
			if(score4 >= 0 && score4 < 151) {
				sprintf(rank, "Rank ;"); //Mushroom
				this->rank = 1;
			}
			if(score4 >= 151 && score4 < 302) {
				sprintf(rank, "Rank <"); //Flower
				this->rank = 2;
			}
			if(score4 >= 302) {
				sprintf(rank, "Rank ="); //Star
				this->rank = 3;
			}
			wchar_t rankW[32];
			rankW[0] = rank[0];
			rankW[1] = rank[1];
			rankW[2] = rank[2];
			rankW[3] = rank[3];
			rankW[4] = rank[4];
			rankW[5] = rank[5];
			T_Rank_00->SetString(rankW);
		}
		/*************/
		/*Cloud Stats*/			//^done
		/*************/
		if(this->settings == 12) {
			/**********************/
			/*Star Coin Visibility*/
			/**********************/
			bool isStarCoin1 = (dGameDisplay_c::instance->layout.findPictureByName("P_collection_00")->flag == 1);
			bool isStarCoin2 = (dGameDisplay_c::instance->layout.findPictureByName("P_collection_01")->flag == 1);
			bool isStarCoin3 = (dGameDisplay_c::instance->layout.findPictureByName("P_collection_02")->flag == 1);

			P_SCoff_00->SetVisible(!isStarCoin1);
			P_SCon_00->SetVisible(isStarCoin1);

			P_SCoff_01->SetVisible(!isStarCoin2);
			P_SCon_01->SetVisible(isStarCoin2);

			P_SCoff_02->SetVisible(!isStarCoin3);
			P_SCon_02->SetVisible(isStarCoin3);


			/*************/
			/*Coins Count*/
			/*************/
			char coinCount[32];
			if(globalCoins > 264) {
				globalCoins = 264;
			}
			sprintf(coinCount, "%03d/265", (globalCoins + 1));//dGameDisplay_c::instance->coins);
			wchar_t coinCountW[32];
			coinCountW[0] = coinCount[0];
			coinCountW[1] = coinCount[1];
			coinCountW[2] = coinCount[2];
			coinCountW[3] = coinCount[3];
			coinCountW[4] = coinCount[4];
			coinCountW[5] = coinCount[5];
			coinCountW[6] = coinCount[6];
			T_Coin_01->SetString(coinCountW);


			/***********/
			/*Time Left*/
			/***********/
			char timeCount[32];
			sprintf(timeCount, "%03d", dGameDisplay_c::instance->timer);
			wchar_t timeCountW[32];
			timeCountW[0] = timeCount[0];
			timeCountW[1] = timeCount[1];
			timeCountW[2] = timeCount[2];
			T_Timer_01->SetString(timeCountW);


			/*****************/
			/*Star Coin Score*/ //Max: 150
			/*****************/
			if(isStarCoin1) {
				score1 += 50;
			}
			if(isStarCoin2) {
				score1 += 50;
			}
			if(isStarCoin3) {
				score1 += 50;
			}
			char firstScore[32];
			sprintf(firstScore, "+%d", score1);
			wchar_t firstScoreW[32];
			firstScoreW[0] = firstScore[0];
			firstScoreW[1] = firstScore[1];
			firstScoreW[2] = firstScore[2];
			firstScoreW[3] = firstScore[3];
			T_Score_00->SetString(firstScoreW);

			/*************/
			/*Coins Score*/ //Max: 265
			/*************/
			score2 += globalCoins + 1;
			char secondScore[32];
			sprintf(secondScore, "+%d", score2);
			wchar_t secondScoreW[32];
			secondScoreW[0] = secondScore[0];
			secondScoreW[1] = secondScore[1];
			secondScoreW[2] = secondScore[2];
			secondScoreW[3] = secondScore[3];
			T_Score_01->SetString(secondScoreW);

			/************/
			/*Time Score*/ //Minimum Time: 200 //Max: 50
			/************/
			score3 = dGameDisplay_c::instance->timer / 4;
			if(score3 > 50) {
				score3 = 50;
			}
			char thirdScore[32];
			sprintf(thirdScore, "+%d", score3);
			wchar_t thirdScoreW[32];
			thirdScoreW[0] = thirdScore[0];
			thirdScoreW[1] = thirdScore[1];
			thirdScoreW[2] = thirdScore[2];
			thirdScoreW[3] = thirdScore[3];
			T_Score_02->SetString(thirdScoreW);

			/*************/
			/*Total Score*/
			/*************/
			score4 = score1 + score2 + score3;
			char lastScore[32];
			sprintf(lastScore, "%d", score4);
			wchar_t lastScoreW[32];
			lastScoreW[0] = lastScore[0];
			lastScoreW[1] = lastScore[1];
			lastScoreW[2] = lastScore[2];
			lastScoreW[3] = lastScore[3];
			T_Total_01->SetString(lastScoreW);

			/************/ //Mushroom: 0
			/*Final Rank*/ //Flower: 230
			/************/ //Star: 465
			char rank[32];
			if(score4 >= 0 && score4 < 230) {
				sprintf(rank, "Rank ;"); //Mushroom
				this->rank = 1;
			}
			if(score4 >= 230 && score4 < 465) {
				sprintf(rank, "Rank <"); //Flower
				this->rank = 2;
			}
			if(score4 >= 465) {
				sprintf(rank, "Rank ="); //Star
				this->rank = 3;
			}
			wchar_t rankW[32];
			rankW[0] = rank[0];
			rankW[1] = rank[1];
			rankW[2] = rank[2];
			rankW[3] = rank[3];
			rankW[4] = rank[4];
			rankW[5] = rank[5];
			T_Rank_00->SetString(rankW);
		}

		SaveBlock *save = GetSaveFile()->GetBlock(-1);
		int oldRank = save->completions[(LastLevelPlayed[0] + 8)][LastLevelPlayed[1]];
		OSReport("Previous Rank: %d\n", oldRank);
		OSReport("New Rank: %d\n", this->rank);
		if(this->rank > oldRank) {
			save->completions[(LastLevelPlayed[0] + 8)][LastLevelPlayed[1]] = this->rank;
			OSReport("Setting new rank\n");
		}
		else {
			OSReport("Old rank was better\n");
		}

		layoutLoaded = true;
	}

	visible = false;

	return true;
}

int dStatsSpawner_c::onExecute() {
	state.execute();

	layout.execAnimations();
	layout.update();

	return true;
}

int dStatsSpawner_c::onDraw() {
	if (visible) {
		layout.scheduleForDrawing();
	}
	
	return true;
}

int dStatsSpawner_c::onDelete() {
	instance = 0;
	if (StageC4::instance)
		StageC4::instance->_1D = 0; // disable no-pause

	return layout.free();
}



// Hidden
void dStatsSpawner_c::beginState_Hidden() { }
void dStatsSpawner_c::executeState_Hidden() { }
void dStatsSpawner_c::endState_Hidden() { }

// ShowWait
void dStatsSpawner_c::beginState_ShowWait() {
	OSReport("9\n");
	nw4r::snd::SoundHandle handle;
	PlaySoundWithFunctionB4(SoundRelatedClass, &handle, SE_SYS_KO_DIALOGUE_IN, 1);
	OSReport("10\n");

	layout.disableAllAnimations();
	layout.enableNonLoopAnim(ANIM_WINDOW_APPEAR);
	OSReport("11\n");
	visible = true;
	scaleEase = 0.0;
	StageC4::instance->_1D = 1; // enable no-pause
}
void dStatsSpawner_c::executeState_ShowWait() {
	OSReport("12\n");
	if (!layout.isAnimOn(ANIM_WINDOW_APPEAR)) {
		// selected = 1;
		OSReport("13\n");
		state.setState(&StateID_ButtonActivateWait);
	}
}
void dStatsSpawner_c::endState_ShowWait() {
	OSReport("15\n");
	nw4r::snd::SoundHandle handle;
	// PlaySoundWithFunctionB4(SoundRelatedClass, &handle, SE_OBJ_CLOUD_BLOCK_TO_JUGEM, 1);
	timer = 1;
}

// ButtonActivateWait
void dStatsSpawner_c::beginState_ButtonActivateWait() { }
void dStatsSpawner_c::executeState_ButtonActivateWait() {
	if (!layout.isAnyAnimOn())
		state.setState(&StateID_Wait);
}
void dStatsSpawner_c::endState_ButtonActivateWait() { }

// Wait
void dStatsSpawner_c::beginState_Wait() {
	if(this->rank == 3) {
		nw4r::snd::SoundHandle clapHandle;
		PlaySoundWithFunctionB4(SoundRelatedClass, &clapHandle, SE_SYS_NICE_S, 1);
	}
}
void dStatsSpawner_c::executeState_Wait() {
	if (timer < 90) {
		scaleEase = -((cos(timer * 3.14 /20)-0.9)/timer*10)+1;
		timer++;
		return;
	}

	int nowPressed = Remocon_GetPressed(GetActiveRemocon());
	int newSelection = -1;

	if ((nowPressed & WPAD_ONE) || (nowPressed & WPAD_TWO)) {
		// Hide the thing
		state.setState(&StateID_HideWait);

	}
}
void dStatsSpawner_c::endState_Wait() { }

// HideWait
void dStatsSpawner_c::beginState_HideWait() {
	nw4r::snd::SoundHandle handle;
	PlaySoundWithFunctionB4(SoundRelatedClass, &handle, SE_SYS_DIALOGUE_OUT_AUTO, 1);
	layout.enableNonLoopAnim(ANIM_WINDOW_DISAPPEAR);

	timer = 26;
	// PlaySoundWithFunctionB4(SoundRelatedClass, &handle, SE_OBJ_CS_KINOHOUSE_DISAPP, 1);

	HideSelectCursor(SelectCursorPointer, 0);
}
void dStatsSpawner_c::executeState_HideWait() {
	if (timer > 0) {
		timer--;
		scaleEase = -((cos(timer * 3.14 /13.5)-0.9)/timer*10)+1;
		if (scaleEase < 0.0f)
			scaleEase = 0.0f;
	}

	if (!layout.isAnimOn(ANIM_WINDOW_DISAPPEAR)) {
		ExitStage(WORLD_MAP, 0, BEAT_LEVEL, MARIO_WIPE);
		this->Delete(1);
	}
}
void dStatsSpawner_c::endState_HideWait() {
	visible = false;
}
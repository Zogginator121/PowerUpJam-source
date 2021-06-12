#ifndef __KAMEK_STATS_H
#define __KAMEK_STATS_H
#include <game.h>


class dStatsSpawner_c : public dStageActor_c {
	public:
		static dStatsSpawner_c *build();
		static dStatsSpawner_c *instance;

		dStatsSpawner_c();

		int onCreate();
		int onDelete();
		int onExecute();
		int onDraw();

		int beforeExecute() { return true; }
		int afterExecute(int) { return true; }
		int beforeDraw() { return true; }
		int afterDraw(int) { return true; }

		m2d::EmbedLayout_c layout;
		bool layoutLoaded;

		bool visible;
		float scaleEase;
		int timer;

		int selected, lastTopRowChoice;

		nw4r::lyt::TextBox
			*T_Title_00,
			*T_SC_00, *T_Score_00,
			*T_Coin_00, *T_Coin_01, *T_Score_01,
			*T_Timer_00, *T_Timer_01, *T_Score_02,
			*T_Total_00, *T_Total_01,
			*T_Rank_00;

		nw4r::lyt::Picture
			*P_BG_00,
			*WindowLeft, *WindowCentre, *WindowRight,
			*P_SCoff_00, *P_SCoff_01, *P_SCoff_02,
			*P_SCon_00, *P_SCon_01, *P_SCon_02,
			*P_Coin_00,
			*P_Timer_00;

		nw4r::lyt::Pane
			*N_Stats_00,
			*N_Main_00,
			*N_Title_00,
			*N_SC_00,
			*N_Coin_00,
			*N_Timer_00,
			*N_Total_00,
			*N_Rank_00;


		int score1 = 0;
		int score2 = 0;
		int score3 = 0;
		int score4 = 0;
		int rank;

		// void selectNumber(int num);
		// void doSummon();

		dStateWrapper_c<dStatsSpawner_c> state;

		USING_STATES(dStatsSpawner_c);
		DECLARE_STATE(Hidden);
		DECLARE_STATE(ShowWait);
		DECLARE_STATE(ButtonActivateWait);
		DECLARE_STATE(Wait);
		DECLARE_STATE(HideWait);
};

#endif


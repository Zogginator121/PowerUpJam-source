#include <game.h>
#include "levelinfo.h"
#include <newer.h>

class PregameLytHandler {
	public:
		m2d::EmbedLayout_c layout;

		nw4r::lyt::Pane *rootPane;

		nw4r::lyt::TextBox
			*T_minus_00, *T_world_00, *T_worldNum_00,
			*T_pictureFont_00, *T_corseNum_00,
			*T_remainder_00, *T_remainder_01, *T_remainder_02, *T_remainder_03,
			*T_remainder_10, *T_remainder_11, *T_remainder_12, *T_remainder_13,
			*T_x_00, *T_x_01, *T_x_02, *T_x_03, *T_x_10, *T_x_11, *T_x_12, *T_x_13,
			*T_x_00_o, *T_x_10_o,
			*T_otasukePlay_00, *T_otasukePlay_01,
			*T_recommend_00, *T_remainder_00_o, *T_remainder_10_o;

		nw4r::lyt::Picture
			*P_Wx_00[9], *P_coin_00, *P_free_00,
			*P_batB_0x[4], *P_bat_00,
			*P_batB_1x[4], *P_bat_01,
			*P_batB_2x[4], *P_bat_02,
			*P_batB_3x[4], *P_bat_03,
			*P_luijiIcon_00_o, *P_luijiIcon_10_o, *P_coinStage_00;

		nw4r::lyt::Pane
			*N_mario_00, *N_luiji_00, *N_kinoB_01, *N_kinoY_00,
			*N_zankiPos_x[4], *N_zanki_00,
			*Null_battPosxP[4], *N_batt_x[4],
			*N_batt, *N_otasukePlay_00;

		u8 layoutLoaded, somethingHasBeenDone, isVisible, hasShownLuigiThing_orSomething;

		u32 currentStateID;

		u32 _2E8;

		u32 countdownToEndabilityCopy, activePlayerCountMultBy4_maybe;
		u32 batteryLevels[4];
		u32 pgCountdown;

		void hijack_loadLevelNumber(); // replaces 80B6BDD0
};

// Notes:
// Deleted; P_coinStage_00, T_recommend_00, T_worldNum_00,
// T_-_00, T_pictureFont_00, T_corseNum_00, T_world_00
// P_Wx_00, P_coin_00, P_free_00

extern char CurrentLevel;
extern char CurrentWorld;
u8 hijackMusicWithSongName(const char *songName, int themeID, bool hasFast, int channelCount, int trackCount, int *wantRealStreamID);

void LoadPregameStyleNameAndNumber(m2d::EmbedLayout_c *layout) {
	nw4r::lyt::TextBox
		*LevelNumShadow, *LevelNum,
		*LevelNameShadow, *LevelName;

	LevelNumShadow = layout->findTextBoxByName("LevelNumShadow");
	LevelNum = layout->findTextBoxByName("LevelNum");
	LevelNameShadow = layout->findTextBoxByName("LevelNameShadow");
	LevelName = layout->findTextBoxByName("LevelName");

	// work out the thing now
	dLevelInfo_c::entry_s *level = dLevelInfo_c::s_info.searchBySlot(CurrentWorld, CurrentLevel);
	if (level) {
		wchar_t convLevelName[160];
		const char *srcLevelName = dLevelInfo_c::s_info.getNameForLevel(level);
		int i = 0;
		while (i < 159 && srcLevelName[i]) {
			convLevelName[i] = srcLevelName[i];
			i++;
		}
		convLevelName[i] = 0;
		LevelNameShadow->SetString(convLevelName);
		LevelName->SetString(convLevelName);

		wchar_t levelNumber[32];
		wcscpy(levelNumber, L"World ");
		getNewerLevelNumberString(level->displayWorld, level->displayLevel, &levelNumber[6]);

		LevelNum->SetString(levelNumber);

		// make the picture shadowy
		int sidx = 0;
		while (levelNumber[sidx]) {
			if (levelNumber[sidx] == 11) {
				levelNumber[sidx+1] = 0x200 | (levelNumber[sidx+1]&0xFF);
				sidx += 2;
			}
			sidx++;
		}
		LevelNumShadow->SetString(levelNumber);

	} else {
		LevelNameShadow->SetString(L"Not found in LevelInfo!");
		LevelName->SetString(L"Not found in LevelInfo!");
	}
}

#include "fileload.h"
void PregameLytHandler::hijack_loadLevelNumber() {
	OSReport("l = %d and w = %d\n", CurrentLevel, CurrentWorld);

	if(CurrentLevel < 9 && CurrentWorld < 2)  {
		nw4r::snd::StrmSoundHandle owoHandle;
		int sID;
		hijackMusicWithSongName("sfx/OwO", -1, false, 2, 1, &sID);
		PlaySoundWithFunctionB4(SoundRelatedClass, &owoHandle, sID, 1);
	}
	
	LoadPregameStyleNameAndNumber(&layout);

	nw4r::lyt::Picture *LevelSample;
	nw4r::lyt::Picture *Icon;
	nw4r::lyt::Picture *BG;
	nw4r::lyt::Picture *Belt;
	LevelSample = layout.findPictureByName("LevelSample");
	Icon = layout.findPictureByName("Icon");
	BG = layout.findPictureByName("BG");
	Belt = layout.findPictureByName("Belt");

	// this is not the greatest way to read a file but I suppose it works in a pinch
	char sampleTplName[64];
	char iconTplName[64];
	char beltTplName[64];
	char bgTplName[64];
	sprintf(sampleTplName, "/LevelSamples/%02d-%02d.tpl", CurrentWorld+1, CurrentLevel+1);
	sprintf(iconTplName, "/LevelSamples/%02d-%02d_I.tpl", CurrentWorld+1, CurrentLevel+1);
	sprintf(beltTplName, "/LevelSamples/%02d-%02d_B.tpl", CurrentWorld+1, CurrentLevel+1);
	sprintf(bgTplName, "/LevelSamples/%02d-%02d_BG.tpl", CurrentWorld+1, CurrentLevel+1);
	static File sampleTpl;
	static File iconTpl;
	static File beltTpl;
	static File bgTpl;
	if (sampleTpl.open(sampleTplName)) {
		LevelSample->material->texMaps[0].ReplaceImage((TPLPalette*)sampleTpl.ptr(), 0);
	}
	if (iconTpl.open(iconTplName)) {
		Icon->material->texMaps[0].ReplaceImage((TPLPalette*)iconTpl.ptr(), 0);
	}
	if (beltTpl.open(beltTplName)) {
		Belt->material->texMaps[0].ReplaceImage((TPLPalette*)beltTpl.ptr(), 0);
	}
	if (bgTpl.open(bgTplName)) {
		BG->material->texMaps[0].ReplaceImage((TPLPalette*)bgTpl.ptr(), 0);
	}
}





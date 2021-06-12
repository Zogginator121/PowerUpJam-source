#include <game.h>
#include <stage.h>


void NoConsoleAllowed() {
	StartLevelInfo sl;
	sl.replayTypeMaybe = 0;
	sl.entrance = 0xFF;
	sl.areaMaybe = 0;
	sl.unk4 = 0;
	sl.purpose = 0;

	sl.world1 = 0;
	sl.world2 = 0;
	sl.level1 = 1;
	sl.level2 = 1;

	DoStartLevel(GetGameMgr(), &sl);
}
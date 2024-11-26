#pragma once
#include "Character.h"
#include "Battle.h"
//First(ˆê”ÔÅ‰‚ÉŒÄ‚Ño‚·‚Æ‚«true),WaveSwitch(Wave‚ªØ‚è‘Ö‚í‚é‚Æ‚«‚Étrue),
//pAllWave(‘SWave”),pWaveNum(Œ»İ‚ÌWave”),pEnemyNum(“G‚Ì”)
bool InitLoadData(bool First, bool WaveSwitch, int* pWaveNum = 0, int* pEnemyNum = 0, int* pAllWave = 0);
//pEnemy(\‘¢‘Ì)
bool EnemyLoadData(int* InCornerCount);

void SetFileAddress(CBattle* InAddress);
void InitSave();
void UnInitEnemyLoadData();
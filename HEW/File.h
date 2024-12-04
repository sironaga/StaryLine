#pragma once
#include "Character.h"
#include "Battle.h"
//First(ˆê”ÔÅ‰‚ÉŒÄ‚Ño‚·‚Æ‚«true),WaveSwitch(Wave‚ªØ‚è‘Ö‚í‚é‚Æ‚«‚Étrue),
//pAllWave(‘SWave”),pWaveNum(Œ»İ‚ÌWave”),pEnemyNum(“G‚Ì”)
bool InitLoadData (bool WaveSwitch, int *pPattern , int* pEnemyNum );
//pEnemy(\‘¢‘Ì)
bool EnemyLoadData(int* InCornerCount, int* pSize);

void SetFileAddress(CBattle* InAddress);
void InitSave();
void UnInitEnemyLoadData();
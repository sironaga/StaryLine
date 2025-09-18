#pragma once
#include "CharacterInclude.h"
#include "Battle.h"
#include "SceneResult.h"
//First(ˆê”ÔÅ‰‚ÉŒÄ‚Ño‚·‚Æ‚«true),WaveSwitch(Wave‚ªØ‚è‘Ö‚í‚é‚Æ‚«‚Étrue),
//pAllWave(‘SWave”),pWaveNum(Œ»İ‚ÌWave”),pEnemyNum(“G‚Ì”)
bool InitLoadData (bool WaveSwitch, int *pPattern , int* pEnemyNum );
//pEnemy(\‘¢‘Ì)
bool EnemyLoadData(int* InCornerCount, int* pSize);

void InitSave();
void UnInitEnemyLoadData();
void InStageData ();
bool OutStageData();
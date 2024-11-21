#pragma once
#include<Windows.h>
#include<Xinput.h>
#pragma comment(lib, "xinput.lib")
#include"_StructList.h"

void Controller_Update();
FLOAT2 CGetLStick();
FLOAT2 CGetRStick();
FLOAT2 CGetTrigger();
bool CGetButtons(WORD InButton);
void CSetLeftVibration(int Frame, int Strength);
void CSetRigthVibration(int Frame, int Strength);
void CSetStopVibration();
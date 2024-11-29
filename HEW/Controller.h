#pragma once
#include<Windows.h>
#include<Xinput.h>
#pragma comment(lib, "xinput.lib")
#include"_StructList.h"
#include "VectorOperator.h"

#define DEADZONE (13000.0f)
void Controller_Update();
CVector2<float> CGetLStick();
CVector2<float> CGetRStick();
CVector2<float> CGetTrigger();
bool CGetButtons(WORD InButton);
void CSetLeftVibration(int Frame, int Strength);
void CSetRigthVibration(int Frame, int Strength);
void CSetStopVibration();
bool CGetButtonsTriger(WORD InButton);
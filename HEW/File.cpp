#include <stdio.h>
#include <iostream>
#include <fstream>
//=====インクルード部=====
#include <string>
#include "File.h"
#include"SpriteDrawer.h"
//=====デファイン=====
#define FILENAME_ENEMY ("Asset/Data/EnemyData.txt")
//=====グローバル変数=====
using namespace std;
streamoff g_NowLine = 0;//現在のファイルの場所
CBattle *g_pFileBattle;
//初期化
bool InitLoadData(bool First, bool WaveSwitch,  int* pWaveNum, int* pEnemyNum, int* pAllWave)
{
	int num2;//int型へ変更用
	string num;//ファイルデータを格納用
	//ファイルオープン
	ifstream ifs(FILENAME_ENEMY, ios::in);
	//行数を指定
	ifs.seekg(g_NowLine, ios::cur);
	//正常にファイルオープン出来たか
	if (ifs)
	{
		if (First)
		{
			getline(ifs, num);//1行読み込み
			num2 = atoi(num.c_str());//int型へ変換
			*pAllWave = num2;//ポインタに格納

			getline(ifs, num);
			num2 = atoi(num.c_str());
			*pWaveNum = num2;

			getline(ifs, num);
			num2 = atoi(num.c_str());
			*pEnemyNum = num2;

			g_NowLine = ifs.tellg();////現在のファイルの場所を格納
		}
		else if (WaveSwitch)
		{
			getline(ifs, num);
			num2 = atoi(num.c_str());
			*pWaveNum = num2;

			getline(ifs, num);
			num2 = atoi(num.c_str());
			*pEnemyNum = num2;

			g_NowLine = ifs.tellg();
		}
		//ファイルクローズ
		ifs.close();
	}
	else
	{
		return false;
	}
	return true;
}
//敵データを読み込み
bool EnemyLoadData(TPolygon* pEnemy)
{
	int i=0;//ループ用
	char word;
	int num2;//int型へ変更用
	string num;//ファイルデータを格納用
	string str;//ファイルデータを格納用
	//ファイルオープン
	ifstream ifs(FILENAME_ENEMY, ios::in);
	//行数を指定
	ifs.seekg(g_NowLine, ios::cur);
	//正常にファイルオープン出来たか
	if (ifs)
	{
		getline(ifs, num);
		num2 = atoi(num.c_str());
		pEnemy->nCornerCount = num2;
		num.clear();
		//配列一個一個に数字を代入
		while (ifs.get(word))
		{
			if (word == ' ')
			{
				i++;
				num.clear();
				continue;
			}
			if (word == '\n')break;
			num += word;
			num2 = atoi(num.c_str());
			pEnemy->nVertexNumber[i] = num2;
		}
		g_NowLine = ifs.tellg();//現在の場所を記憶
		//g_NowLine += 4;
		//ファイルクローズ
		ifs.close();
	}
	else
	{
		return false;
	}
	return true;
}
void InitSave()
{
	int AllWave = 0;
	int Wave = 0;
	int MaxEnemy=0;
	InitLoadData(true, false, &Wave, &MaxEnemy, &AllWave);
	TPolygon tPolygon[MAX_WAVE] [MAX_ENEMY] ;
	g_pFileBattle->SetMaxWave(AllWave);
	for (int i = 0; i < AllWave; i++)
	{
		for (int l = 0; l < MaxEnemy; l++)
		{
			EnemyLoadData(&tPolygon[i][l]);
			g_pFileBattle->SaveEnemyData(tPolygon[i][l], i, 1);//最後は敵のサイズ(float型)
		}
		InitLoadData(false, true, &Wave, &MaxEnemy, &AllWave);
	}
	
}
//終了処理
void UnInitEnemyLoadData()
{
	g_NowLine = 0;
}
void SetFileAddress(CBattle* InAddress)
{
	g_pFileBattle = InAddress;
}
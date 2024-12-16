//=====インクルード部=====
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

#include "File.h"

//=====デファイン=====
#define FILENAME_ENEMY ("Asset/Data/EnemyData.csv")

std::vector<std::string> split(const std::string text, const char delimiter);

//=====グローバル変数=====
using namespace std;
streamoff g_NowLine = 0;//現在のファイルの場所
CBattle *g_pFileBattle;
//初期化
bool InitLoadData(bool WaveSwitch, int* pPattern, int* pEnemyNum)
{
	ifstream ifs(FILENAME_ENEMY, ios::in);
	string word;
	vector<string> vst;
	if (!ifs)
	{
		return false;
	}
	//行数を指定
	ifs.seekg(g_NowLine, ios::cur);
	
	 if (WaveSwitch)
	{
		getline(ifs, word);
		vst = split(word, ',');
		*pPattern = atoi(vst[0].c_str());
		g_NowLine = ifs.tellg();
	}
	else
	{
		getline(ifs, word);
		word.clear();
		getline(ifs, word);
		vst = split(word, ',');
		*pEnemyNum = atoi(vst[0].c_str());
		g_NowLine = ifs.tellg();
	}
	ifs.close();


	return true;
}
//敵データを読み込み
bool EnemyLoadData(int * InCornerCount,int* pSize)
{
	string word;
	vector<string> vst;
	//ファイルオープン
	ifstream ifs(FILENAME_ENEMY, ios::in);
	//行数を指定
	ifs.seekg(g_NowLine, ios::cur);
	//正常にファイルオープン出来たか
	if (ifs)
	{
		getline(ifs, word);
		vst = split(word, ',');
		*InCornerCount = atoi(vst[0].c_str());
		word.clear();
		getline(ifs, word);
		vst = split(word, ',');
		*pSize = atoi(vst[0].c_str());

		g_NowLine = ifs.tellg();//現在の場所を記憶
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
	g_NowLine = 0;
	int AllWave = 0;
	int Wave = 0;
	int MaxEnemy = 0;
	int MaxPattern = 0;
	int size = 0;
	int CornerCount = 0;

	InitLoadData(true, &MaxPattern,  &MaxEnemy );
 	g_pFileBattle->SetMaxPattern(MaxPattern);
	for (int j = 0; j < MaxPattern; j++)
	{
		InitLoadData(false, &MaxPattern,  &MaxEnemy);
		for (int l = 0; l < MaxEnemy; l++)
		{
			EnemyLoadData(&CornerCount, &size);
			g_pFileBattle->SaveEnemyData(CornerCount, j);//最後は敵のサイズ(float型)
		}
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
std::vector<std::string> split(const std::string text, const char delimiter) {
	vector<string> columns;
	if (text.empty()) {
		return columns;
	}
	stringstream stream{ text };
	string buff;
	int i = 0;
	while (getline(stream, buff, delimiter)) {
		if (i == 0)
		{
			i++;
			continue;
		}
		columns.push_back(buff);
	}
	return columns;
}
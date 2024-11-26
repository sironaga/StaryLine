#include "Battle.h"
#include "SceneGame.h"
#include "Main.h"
#include "DirectXTex/TextureLoad.h"
#include "Controller.h"
#include"SpriteDrawer.h"
#include "Input.h"

//防衛最前線ライン
#define SENTER_POSX (0)
#define SENTER_POSY (0)

//味方コアの位置
#define ALLYCORE_POSX (-500)
#define ALLYCORE_POSY (0)

//味方の生成位置
#define ALLYCREATE_POSX (-1450)
#define ALLYCREATE_POSY_UP (-800)
#define ALLYCREATE_POSY_CENTER (0)
#define ALLYCREATE_POSY_DOWN (800)
//敵の生成位置
#define ENEMYCREATE_POSX (1450)
#define ENEMYCREATE_POSY_CENTER (0)

//戦闘フィールドのサイズ
#define BATTLEFIELDWIDTH (1920)
#define BATTLEFIELDHEIGHT (1080)

ID3D11ShaderResourceView* m_pLogTex[10];
int Quick = 1;

CBattle::CBattle()
	: m_nAllyCount(0)
	, m_nAllyDateCount(0)
	, m_nCreateAllyNum(0)
	, m_nEnemyCount(0)
	, m_nEnemyDateCount{0,0,0,0,0}
	, m_nCreateEnemyNum(0)
	, m_nNowWave(0)
	, m_nMaxWave(0)
	, m_pAlly{}
	, m_pEnemy{}
	, m_nAllyTypes{ 0,0,0,0,0,0 }
	, m_nEnemyTypes{ 0,0,0}
	, m_nBattleTime(0)
	, m_bFirstFight(false)
	, m_nFirstPosSetting(0)
{
	HRESULT hr;

	hr = LoadTextureFromFile(GetDevice(), "Asset/Numbers/number_0.png", &m_pLogTex[0]);
	hr = LoadTextureFromFile(GetDevice(), "Asset/Numbers/number_1.png", &m_pLogTex[1]);
	hr = LoadTextureFromFile(GetDevice(), "Asset/Numbers/number_2.png", &m_pLogTex[2]);
	hr = LoadTextureFromFile(GetDevice(), "Asset/Numbers/number_3.png", &m_pLogTex[3]);
	hr = LoadTextureFromFile(GetDevice(), "Asset/Numbers/number_4.png", &m_pLogTex[4]);
	hr = LoadTextureFromFile(GetDevice(), "Asset/Numbers/number_5.png", &m_pLogTex[5]);
	hr = LoadTextureFromFile(GetDevice(), "Asset/Numbers/number_6.png", &m_pLogTex[6]);
	hr = LoadTextureFromFile(GetDevice(), "Asset/Numbers/number_7.png", &m_pLogTex[7]);
	hr = LoadTextureFromFile(GetDevice(), "Asset/Numbers/number_8.png", &m_pLogTex[8]);
	hr = LoadTextureFromFile(GetDevice(), "Asset/Numbers/number_9.png", &m_pLogTex[9]);
	if (FAILED(hr)) {
		MessageBox(NULL, "Numbers 画像", "Error", MB_OK);
	}

	Vertex vtx[] = {
		//背景表示の座標
		{{-100, -100, 0.0f}, {0.0f, 0.0f}},
		{{-100,  100, 0.0f}, {0.0f, 1.0f}},
		{{ 100, -100, 0.0f}, {1.0f, 0.0f}},
		{{ 100,  100, 0.0f}, {1.0f, 1.0f}},
	};
	m_pLogVtx = CreateVertexBuffer(vtx, 4);

	for (int i = 0; i < MAX_ALLY; i++)
	{
		m_tAllyData[i].nCornerCount = -1;

		//switch (m_nFirstPosSetting)
		//{
		//case 0:
		//	m_tAllyData[i].m_tCreatePos.X = ALLYCREATE_POSX;
		//	m_tAllyData[i].m_tCreatePos.Y = ALLYCREATE_POSY_UP;
		//	m_tAllyData[i].m_tCreatePos.Z = 0.0f;
		//	m_nFirstPosSetting = 1;
		//	break;
		//case 1:
		//	m_tAllyData[i].m_tCreatePos.X = ALLYCREATE_POSX;
		//	m_tAllyData[i].m_tCreatePos.Y = ALLYCREATE_POSY_CENTER;
		//	m_tAllyData[i].m_tCreatePos.Z = 0.0f;
		//	m_nFirstPosSetting = 2;
		//	break;
		//case 2:
		//	m_tAllyData[i].m_tCreatePos.X = ALLYCREATE_POSX;
		//	m_tAllyData[i].m_tCreatePos.Y = ALLYCREATE_POSY_DOWN;
		//	m_tAllyData[i].m_tCreatePos.Z = 0.0f;
		//	m_nFirstPosSetting = 0;
		//	break;
		//}
		m_tAllyData[i].m_tCreatePos.X = ALLYCREATE_POSX;
		m_tAllyData[i].m_tCreatePos.Y = ALLYCREATE_POSY_CENTER;
		m_tAllyData[i].m_tCreatePos.Z = 0.0f;

		m_tAllyData[i].Size = 1.0f;
	}
	for (int j = 0; j < MAX_WAVE; j++)
	{
		for (int i = 0; i < MAX_ENEMY; i++)
		{
			m_tEnemyData[j][i].nCornerCount = -1;

			m_tEnemyData[j][i].m_tCreatePos.X = ENEMYCREATE_POSX;
			m_tEnemyData[j][i].m_tCreatePos.Y = ENEMYCREATE_POSY_CENTER;
			m_tEnemyData[j][i].m_tCreatePos.Z = 0.0f;
			m_tEnemyData[j][i].Size = 1.0f;
		}
	}

}

CBattle::~CBattle()
{
	if (m_pLogVtx)m_pLogVtx->Release();
	for (int i = 0; i < 10; i++)
	{
		if (m_pLogTex[i])m_pLogTex[i]->Release();
	}
	for (int i = 0; i < MAX_ALLY; i++)
	{
		if (!m_pAlly[i])continue;
		delete m_pAlly[i];
		m_pAlly[i] = nullptr;
	}
	for (int l = 0; l < MAX_ENEMY; l++)
	{
		if (!m_tEnemyData[l])continue;
		delete m_pEnemy[l];
		m_pEnemy[l] = nullptr;

	}
}

void CBattle::Update(void)
{
	Quick = 1;
	if (IsKeyPress(VK_CONTROL) || CGetButtons(XINPUT_GAMEPAD_X))Quick = 10;

	//削除処理
	Delete();

	//生成命令判定
	TimeAxis();

	//生成処理
	CreateEntity();

	for (int i = 0; i < m_nAllyCount; i++)
	{
		if (m_pAlly[i]->GetStatus() == CFighter::Battle)//ステータスがバトルだったら
		{
			//索敵処理
			Search(i, Ally);

			//攻撃判定
			for (int l = 0; l < m_nEnemyCount; l++)
			{
				m_pAlly[i]->m_bIsAttack = false;
				//攻撃当たり判定に入ってるかどうか
				if (m_pEnemy[l]->GetStatus() == CFighter::Battle)
				{
					if (m_pAlly[i]->AtkCollisionCheck(m_pEnemy[l]->GetSize(), m_pEnemy[l]->GetPos()))
					{
						m_pAlly[i]->m_bIsAttack = true;
						//攻撃処理
						Battle(i, l, Ally);
						break;
					}
				}
			}
			if (!m_pAlly[i]->m_bIsAttack)
			{
				//移動処理
				Move(i, Ally);
			}
		}
	}
	for (int i = 0; i < m_nEnemyCount; i++)
	{
		if (m_pEnemy[i]->GetStatus() == CFighter::Battle)//ステータスがバトルだったら
		{
			//索敵処理
			Search(i, Enemy);

			//攻撃判定
			for (int l = 0; l < m_nAllyCount; l++)
			{
				m_pEnemy[i]->m_bIsAttack = false;

				//攻撃当たり判定に入ってるかどうか
				if (m_pAlly[l]->GetStatus() == CFighter::Battle)//ステータスがバトルだったら
				{
					if (m_pEnemy[i]->AtkCollisionCheck(m_pAlly[l]->GetSize(), m_pAlly[l]->GetPos()))
					{
						m_pEnemy[i]->m_bIsAttack = true;
						//攻撃処理
						Battle(i, l, Enemy);

						break;
					}
				}
			}
			if (!m_pEnemy[i]->m_bIsAttack)
			{
				//移動処理
				Move(i, Enemy);
			}
		}
	}

	Alive();

	//味方の更新(アニメーション)
	for (int i = 0; i < m_nAllyCount; i++)m_pAlly[i]->Update();

	//敵の更新(アニメーション)
	for (int i = 0; i < m_nEnemyCount; i++)m_pEnemy[i]->Update();

	m_nBattleTime++;//戦闘時間の更新

	//勝敗判定
	if (m_nNowWave < m_nMaxWave)
	{
		if (GetNowPhase() == BATTLE)
		{
			if (m_nEnemyCount <= 0)
			{
				m_nNowWave++;
				MessageBox(NULL, "現在Waveの敵が全滅したため次のWaveへ", "勝敗", MB_OK);
				NextWaveInit();
				ChangePhase(DRAWING);
			}
		}
	}
	else
	{
		if (GetNowPhase() == BATTLE)
		{
			if (m_nEnemyCount <= 0)
			{
				MessageBox(NULL, "敵が全滅したため勝利", "勝敗", MB_OK);
				NextWaveInit();
				ChangeScene(SCENE_TITLE);
			}
		}
	}
	if (GetNowPhase() == BATTLE)
	{
		if (m_nAllyCount <= 0)
		{
			m_nNowWave = 1;
			MessageBox(NULL,"味方が全滅したため敗北", "勝敗", MB_OK);
			NextWaveInit();
			ChangeScene(SCENE_TITLE);
		}
	}

}

void CBattle::Draw(void)
{
	//SaveAllyLogDraw();
	CreateAllyLogDraw();
	CreateEnemyLogDraw();
/*当たり判定のデバック*/
//0：全て
//Aを押しながら：味方だけ
//Eを押しながら：敵だけ
//3：3角形だけ
//4：4角形だけ
//5：5角形だけ
//6：6角形だけ
//7：7角形だけ
//8：8角形だけ

	//味方の描画
	for (int i = 0; i < m_nAllyCount; i++)
	{
		m_pAlly[i]->Draw();
		if (IsKeyPress('0') || IsKeyPress('A') && IsKeyPress('3'))
		{
			if (m_pAlly[i]->GetCornerCount() == 3)
				m_pAlly[i]->CollisionDraw();
		}
		if (IsKeyPress('0') || IsKeyPress('A') && IsKeyPress('4'))
		{
			if (m_pAlly[i]->GetCornerCount() == 4)
				m_pAlly[i]->CollisionDraw();
		}
		if (IsKeyPress('0') || IsKeyPress('A') && IsKeyPress('5'))
		{
			if (m_pAlly[i]->GetCornerCount() == 5)
				m_pAlly[i]->CollisionDraw();
		}
		if (IsKeyPress('0') || IsKeyPress('A') && IsKeyPress('6'))
		{
			if (m_pAlly[i]->GetCornerCount() == 6)
				m_pAlly[i]->CollisionDraw();
		}
		if (IsKeyPress('0') || IsKeyPress('A') && IsKeyPress('7'))
		{
			if (m_pAlly[i]->GetCornerCount() == 7)
				m_pAlly[i]->CollisionDraw();
		}
		if (IsKeyPress('0') || IsKeyPress('A') && IsKeyPress('8'))
		{
			if (m_pAlly[i]->GetCornerCount() == 8)
				m_pAlly[i]->CollisionDraw();
		}
	}

	//敵の描画
	for (int i = 0; i < m_nEnemyCount; i++)
	{
		m_pEnemy[i]->Draw();
		if (IsKeyPress('0') || IsKeyPress('E') && IsKeyPress('3'))
		{
			if (m_pEnemy[i]->GetCornerCount() == 3)
				m_pEnemy[i]->CollisionDraw();
		}
		if (IsKeyPress('0') || IsKeyPress('E') && IsKeyPress('4'))
		{
			if (m_pEnemy[i]->GetCornerCount() == 4)
				m_pEnemy[i]->CollisionDraw();
		}
		if (IsKeyPress('0') || IsKeyPress('E') && IsKeyPress('5'))
		{
			if (m_pEnemy[i]->GetCornerCount() == 5)
				m_pEnemy[i]->CollisionDraw();
		}
		if (IsKeyPress('0') || IsKeyPress('E') && IsKeyPress('6'))
		{
			if (m_pEnemy[i]->GetCornerCount() == 6)
				m_pEnemy[i]->CollisionDraw();
		}
		if (IsKeyPress('0') || IsKeyPress('E') && IsKeyPress('7'))
		{
			if (m_pEnemy[i]->GetCornerCount() == 7)
				m_pEnemy[i]->CollisionDraw();
		}
		if (IsKeyPress('0') || IsKeyPress('E') && IsKeyPress('8'))
		{
			if (m_pEnemy[i]->GetCornerCount() == 8)
				m_pEnemy[i]->CollisionDraw();
		}
	}
}

void CBattle::NextWaveInit(void)
{
	//味方関係のデータア初期化
	for (int i = 0; i < MAX_ALLY; i++)
	{
		m_pAlly[i] = nullptr;

		m_tAllyData[i].nCornerCount = -1;

		m_tAllyData[i].m_tCreatePos.X = ALLYCREATE_POSX;
		m_tAllyData[i].m_tCreatePos.Y = ALLYCREATE_POSY_CENTER;
		m_tAllyData[i].m_tCreatePos.Z = 0.0f;
		m_tAllyData[i].Size = 1.0f;
	}
	m_nAllyCount = 0;
	m_nEnemyCount = 0;
	m_nAllyDateCount = 0;
	m_nCreateAllyNum = 0;
	m_nCreateEnemyNum = 0;
	m_nBattleTime = 0;
	m_bFirstFight = false;
}

void CBattle::SaveAllyData(int InCornerCount, float InSize)
{
	InSize = 1 + ((InSize - 1) / 10);	//1を基準として2で入ってきた場合1.1倍にするため

	m_tAllyData[m_nAllyDateCount].nCornerCount = InCornerCount;
	m_tAllyData[m_nAllyDateCount].Size = InSize;
	switch (m_tAllyData[m_nAllyDateCount].nCornerCount)
	{
	case 3:	m_tAllyData[m_nAllyDateCount].m_tCreatePos.Y = -800;
		break;
	case 4:m_tAllyData[m_nAllyDateCount].m_tCreatePos.Y = -500;
		break;
	case 5:m_tAllyData[m_nAllyDateCount].m_tCreatePos.Y = -200;
		break;
	case 6:m_tAllyData[m_nAllyDateCount].m_tCreatePos.Y =  200;
		break;
	case 7:m_tAllyData[m_nAllyDateCount].m_tCreatePos.Y =  500;
		break;
	case 8:m_tAllyData[m_nAllyDateCount].m_tCreatePos.Y =  800;
		break;
	}
	//保存数を加算
	m_nAllyDateCount++;
}

void CBattle::SaveEnemyData(int InCornerCount, int InWave, float InSize)
{
	InSize = 1 + ((InSize - 1) / 10);	//1を基準として2で入ってきた場合1.1倍にするため

	m_tEnemyData[InWave][m_nEnemyDateCount[InWave]].nCornerCount = InCornerCount;
	m_tEnemyData[InWave][m_nEnemyDateCount[InWave]].Size = InSize;
	//保存数を加算
	m_nEnemyDateCount[InWave]++;
}

void CBattle::TimeAxis(void)
{
	if (m_nBattleTime == 0 / Quick
		|| m_nBattleTime == 100 / Quick
		|| m_nBattleTime == 200 / Quick
		|| m_nBattleTime == 300 / Quick
		|| m_nBattleTime == 400 / Quick
		|| m_nBattleTime == 500 / Quick
		|| m_nBattleTime == 600 / Quick
		|| m_nBattleTime == 700 / Quick
		|| m_nBattleTime == 800 / Quick
		|| m_nBattleTime == 900 / Quick
		|| m_nBattleTime == 1000 / Quick
		|| m_nBattleTime == 1100 / Quick
		|| m_nBattleTime == 1200 / Quick
		|| m_nBattleTime == 1300 / Quick
		|| m_nBattleTime == 1400 / Quick
		|| m_nBattleTime == 1500 / Quick
		|| m_nBattleTime == 1600 / Quick
		|| m_nBattleTime == 1700 / Quick
		|| m_nBattleTime == 1800 / Quick
		|| m_nBattleTime == 1900 / Quick
		|| m_nBattleTime == 2000 / Quick
		)
	{
		//味方生成予定数を3に設定
		m_nCreateAllyNum = 1;

		//敵生成予定数を3に設定
		m_nCreateEnemyNum = 1;


		m_bFirstFight = true;
	}
}

void CBattle::CreateEntity(void)
{
	//指定された数だけ生成する
	while (m_nCreateAllyNum)
	{
		if (m_nCreateAllyNum <= m_nAllyDateCount)
		{
			//味方を生成する
			CreateAllyData(m_tAllyData[0]);

			//生成に使用したため情報を消して後ろの情報を前詰めにする
			for (int i = 0; i + 1 < MAX_ALLY; i++)
			{
				m_tAllyData[i] = m_tAllyData[i + 1];
			}
			//保存済みの総数を減らす
			m_nAllyDateCount--;
			//生成数を加算
			m_nAllyCount++;
		}
		//生成予定数を減らす
		m_nCreateAllyNum--;
	}

	//指定された数だけ生成する
	while (m_nCreateEnemyNum)
	{
		if (m_nCreateEnemyNum <= m_nEnemyDateCount[m_nNowWave])
		{
			//味方を生成する
			CreateEnemyData(m_tEnemyData[m_nNowWave][0]);

			//生成に使用したため情報を消して後ろの情報を前詰めにする
			for (int i = 0; i + 1 < MAX_ENEMY; i++)
			{
				m_tEnemyData[m_nNowWave][i] = m_tEnemyData[m_nNowWave][i + 1];
			}
			//保存済みの総数を減らす
			m_nEnemyDateCount[m_nNowWave]--;
			//生成数を加算
			m_nEnemyCount++;
		}
		//生成予定数を減らす
		m_nCreateEnemyNum--;
	}
}

void CBattle::CreateAllyData(EntityData InData)
{
	m_pAlly[m_nAllyCount] = new CAlly(InData.nCornerCount, InData.Size,InData.m_tCreatePos);
}

void CBattle::CreateEnemyData(EntityData InDate)
{
	m_pEnemy[m_nEnemyCount] = new CEnemy(InDate.nCornerCount,(int)InDate.Size,InDate.m_tCreatePos);
}

void CBattle::Search(int i, Entity Entity)
{
	switch (Entity)
	{
	case CBattle::Ally:/*味方の判定*/
		m_pAlly[i]->m_nTargetNumber = -1;

		for (int l = 0; l < m_nEnemyCount; l++)
		{
			if (m_pEnemy[l]->GetStatus() == CFighter::Battle)//相手のステータスがBattleかどうか
			{
				if (m_pAlly[i]->SearchCollisionCheck(m_pEnemy[l]->GetPos(), m_pEnemy[l]->GetSize()))//索敵当たり判定内に敵がいるかどうか
				{
					if (m_pAlly[i]->m_nTargetNumber != -1)
					{
						/*((敵のX位置 - 味方のX位置)+(敵のY位置 - 味方のY位置)) < ((標的のX位置 - 味方のX位置)+(標的のY位置 - 味方のY位置))*/
						if (((m_pEnemy[l]->GetPos().X - m_pAlly[i]->GetPos().X) + (m_pEnemy[l]->GetPos().Y - m_pAlly[i]->GetPos().Y))
							<
							((m_pEnemy[m_pAlly[i]->m_nTargetNumber]->GetPos().X - m_pAlly[i]->GetPos().X) + (m_pEnemy[m_pAlly[i]->m_nTargetNumber]->GetPos().Y - m_pAlly[i]->GetPos().Y)))
						{
							m_pAlly[i]->m_nTargetNumber = l;
						}
					}
					else
					{
						m_pAlly[i]->m_nTargetNumber = l;
					}
				}
			}
		}
		break;

	case CBattle::Enemy:/*敵の判定*/
		m_pEnemy[i]->m_nTargetNumber = -1;

		for (int l = 0; l < m_nAllyCount; l++)
		{
			if (m_pAlly[l]->GetStatus() == CFighter::Battle)//相手のステータスがBattleかどうか
			{
				if (m_pEnemy[i]->SearchCollisionCheck(m_pAlly[l]->GetPos(), m_pAlly[l]->GetSize()))//索敵当たり判定内に味方がいるかどうか
				{
					if (m_pEnemy[i]->m_nTargetNumber != -1)
					{
						/*((味方のX位置 - 敵のX位置)+(味方のY位置 - 敵のY位置)) < ((標的のX位置 - 敵のX位置)+(標的のY位置 - 敵のY位置))*/
						if (((m_pAlly[l]->GetPos().X - m_pEnemy[i]->GetPos().X) + (m_pAlly[l]->GetPos().Y - m_pEnemy[i]->GetPos().Y))
							<
							((m_pAlly[m_pEnemy[i]->m_nTargetNumber]->GetPos().X - m_pEnemy[i]->GetPos().X) + (m_pAlly[m_pEnemy[i]->m_nTargetNumber]->GetPos().Y - m_pEnemy[i]->GetPos().Y)))
						{
							m_pEnemy[i]->m_nTargetNumber = l;
						}
					}
					else
					{
						m_pEnemy[i]->m_nTargetNumber = l;
					}
				}
			}
		}
		break;
	}
}

void CBattle::Move(int i, Entity Entity)
{
	switch (Entity)
	{
		/*味方の判定*/
	case CBattle::Ally:
		if (m_pAlly[i]->m_nTargetNumber != -1)//標的を設定済み
		{
			if (m_pAlly[i]->m_nTargetNumber < m_nEnemyCount)//現在生存数よりも数字が大きくないかどうか
			{
				if (m_pEnemy[m_pAlly[i]->m_nTargetNumber]->GetPos().X < m_pAlly[i]->GetPos().X)m_pAlly[i]->AddPosX(-2.0f * Quick);
				if (m_pEnemy[m_pAlly[i]->m_nTargetNumber]->GetPos().X > m_pAlly[i]->GetPos().X)m_pAlly[i]->AddPosX(2.0f * Quick);

				if (m_pEnemy[m_pAlly[i]->m_nTargetNumber]->GetPos().Y < m_pAlly[i]->GetPos().Y)m_pAlly[i]->AddPosY(-2.0f * Quick);
				if (m_pEnemy[m_pAlly[i]->m_nTargetNumber]->GetPos().Y > m_pAlly[i]->GetPos().Y)m_pAlly[i]->AddPosY(2.0f * Quick);
			}
			else
			{
				if (SENTER_POSX < m_pEnemy[i]->GetPos().X)m_pEnemy[i]->AddPosX(-2.0f * Quick);
				if (SENTER_POSX > m_pEnemy[i]->GetPos().X)m_pEnemy[i]->AddPosX(2.0f * Quick);

				if (SENTER_POSY < m_pEnemy[i]->GetPos().Y)m_pEnemy[i]->AddPosY(-2.0f * Quick);
				if (SENTER_POSY > m_pEnemy[i]->GetPos().Y)m_pEnemy[i]->AddPosY(2.0f * Quick);
			}
		}
		else//標的がいないので敵のコアに向かって進む
		{
			if (SENTER_POSX < m_pAlly[i]->GetPos().X)m_pAlly[i]->AddPosX(-2.0f * Quick);
			if (SENTER_POSX > m_pAlly[i]->GetPos().X)m_pAlly[i]->AddPosX(2.0f * Quick);

			if (SENTER_POSY < m_pAlly[i]->GetPos().Y)m_pAlly[i]->AddPosY(-2.0f * Quick);
			if (SENTER_POSY > m_pAlly[i]->GetPos().Y)m_pAlly[i]->AddPosY(2.0f * Quick);
		}
		break;

		/*敵の判定*/
	case CBattle::Enemy:
		if (m_pEnemy[i]->m_nTargetNumber != -1)//標的を設定済み
		{
			if (m_pEnemy[i]->m_nTargetNumber < m_nAllyCount)//現在生存数よりも数字が大きくないかどうか
			{
				if (m_pAlly[m_pEnemy[i]->m_nTargetNumber]->GetPos().X < m_pEnemy[i]->GetPos().X)m_pEnemy[i]->AddPosX(-2.0f * Quick);
				if (m_pAlly[m_pEnemy[i]->m_nTargetNumber]->GetPos().X > m_pEnemy[i]->GetPos().X)m_pEnemy[i]->AddPosX(2.0f * Quick);

				if (m_pAlly[m_pEnemy[i]->m_nTargetNumber]->GetPos().Y < m_pEnemy[i]->GetPos().Y)m_pEnemy[i]->AddPosY(-2.0f * Quick);
				if (m_pAlly[m_pEnemy[i]->m_nTargetNumber]->GetPos().Y > m_pEnemy[i]->GetPos().Y)m_pEnemy[i]->AddPosY(2.0f * Quick);
			}
			else
			{
				if (ALLYCORE_POSX < m_pEnemy[i]->GetPos().X)m_pEnemy[i]->AddPosX(-2.0f * Quick);
				if (ALLYCORE_POSX > m_pEnemy[i]->GetPos().X)m_pEnemy[i]->AddPosX(2.0f * Quick);

				if (ALLYCORE_POSY < m_pEnemy[i]->GetPos().Y)m_pEnemy[i]->AddPosY(-2.0f * Quick);
				if (ALLYCORE_POSY > m_pEnemy[i]->GetPos().Y)m_pEnemy[i]->AddPosY(2.0f * Quick);
			}
		}
		else//標的がいないので敵のコアに向かって進む
		{
			if (ALLYCORE_POSX < m_pEnemy[i]->GetPos().X)m_pEnemy[i]->AddPosX(-2.0f * Quick);
			if (ALLYCORE_POSX > m_pEnemy[i]->GetPos().X)m_pEnemy[i]->AddPosX(2.0f * Quick);

			if (ALLYCORE_POSY < m_pEnemy[i]->GetPos().Y)m_pEnemy[i]->AddPosY(-2.0f * Quick);
			if (ALLYCORE_POSY > m_pEnemy[i]->GetPos().Y)m_pEnemy[i]->AddPosY(2.0f * Quick);

		}
		break;
	}
}

void CBattle::Battle(int i, int l, Entity Entity)
{
	switch (Entity)
	{
	case CBattle::Ally:		//味方の攻撃

		if (m_pAlly[i]->GetAtkCharge() >= m_pAlly[i]->GetCoolTime())//攻撃チャージがたまっているかどうか
		{
			if (m_pEnemy[l]->GetStatus() == CFighter::Battle)//相手のステータスがBattleかどうか
			{
				m_pEnemy[l]->Damage(m_pAlly[i]->GetAtk());//相手の体力を減らす
				m_pAlly[i]->ChargeReset();//攻撃したのでチャージをリセットする
			}
		}
		else//攻撃チャージが溜まっていなかったら
		{
			m_pAlly[i]->AddAtkCharge((float)Quick);//チャージする
		}
		break;

	case CBattle::Enemy://敵の攻撃
		if (m_pEnemy[i]->GetAtkCharge() >= m_pEnemy[i]->GetCoolTime())//攻撃チャージがたまっているかどうか
		{
			if (m_pEnemy[i]->AtkCollisionCheck(m_pAlly[l]->GetSize(), m_pAlly[l]->GetPos()))//当たり判定の中にいるかどうか
			{
				if (m_pAlly[l]->GetStatus() == CFighter::Battle)//相手のステータスがBattleかどうか
				{
					m_pAlly[l]->Damage(m_pEnemy[i]->GetAtk());//相手の体力を減らす
					m_pEnemy[i]->ChargeReset();//攻撃したのでチャージをリセットする
				}
			}
		}
		else//攻撃チャージが溜まっていなかったら
		{
			m_pEnemy[i]->AddAtkCharge((float)Quick);//チャージする
		}
	}
}

void CBattle::Alive(void)
{
	//味方判定
	for (int l = 0; l < m_nAllyCount; l++)
	{
		if (m_pAlly[l]->GetStatus() == CFighter::Battle)//味方のステータスがBattleかどうか
		{
			if (m_pAlly[l]->GetHp() <= 0.0f)//味方の体力が残っているかどうか
			{
				m_pAlly[l]->SetStatus(CFighter::Death);//ステータスを死亡状態にする
			}
		}
	}
	for (int l = 0; l < m_nEnemyCount; l++)
	{
		if (m_pEnemy[l]->GetStatus() == CFighter::Battle)//敵のステータスがBattleかどうか
		{
			if (m_pEnemy[l]->GetHp() <= 0.0f)//敵の体力が残っているかどうか
			{
				m_pEnemy[l]->SetStatus(CFighter::Death);//ステータスを死亡状態にする
			}
		}
	}
}

void CBattle::Delete(void)
{
	//味方の生存判定
	for (int i = 0; i < m_nAllyCount; i++)
	{
		if (m_pAlly[i]->GetStatus() == CFighter::Delete)					//ステータスがDeleteかどうか
		{
			m_pAlly[i] = nullptr;
			//配列前詰め
			for (int d = i; d < m_nAllyCount; d++)
			{
				if (m_pAlly[d] == nullptr)
				{
					m_pAlly[d] = m_pAlly[d + 1];							//一つ後ろの配列を自分に入れる
					m_pAlly[d + 1] = nullptr;								//一つ後ろをnullptrにする
				}
			}
			m_nAllyCount--;													//今いる生存数を減らす
			i--;															//前詰めされたのでもう一度同じ場所を処理する
		}
	}
	//敵の生存判定
	for (int i = 0; i < m_nEnemyCount; i++)
	{
		if (m_pEnemy[i]->GetStatus() == CFighter::Delete)		//ステータスがDeleteかどうか
		{
			m_pEnemy[i] = nullptr;
			//配列前詰め
			for (int d = i; d < m_nEnemyCount; d++)
			{
				if (m_pEnemy[d] == nullptr)
				{
					m_pEnemy[d] = m_pEnemy[d + 1];				//一つ後ろの配列を自分に入れる
					m_pEnemy[d + 1] = nullptr;					//一つ後ろをnullptrにする
				}
			}
			m_nEnemyCount--;									//今いる生存数を減らす
			i--;															//前詰めされたのでもう一度同じ場所を処理する
		}
	}
}

void CBattle::DebugMove(void)
{
	#define PosX			(1400)

	#define TrianglePosY	(-800)
	#define	SquarePosY		(-500)
	#define	PentagonPosY	(-200)
	#define	HexagonPosY		( 200)
	#define	HeptagonPosY	( 500)
	#define	OctagonPosY		( 800)

	for (int i = 0; i < m_nAllyCount; i++)
	{
		switch (m_pAlly[i]->GetCornerCount())
		{
		case 3:
			if (TrianglePosY < m_pAlly[i]->GetPos().Y)m_pAlly[i]->AddPosY(-2.0f);
			if (TrianglePosY > m_pAlly[i]->GetPos().Y)m_pAlly[i]->AddPosY(2.0f);
			break;
		case 4:
			if (SquarePosY < m_pAlly[i]->GetPos().Y)m_pAlly[i]->AddPosY(-2.0f);
			if (SquarePosY > m_pAlly[i]->GetPos().Y)m_pAlly[i]->AddPosY(2.0f);
			break;
		case 5:
			if (PentagonPosY < m_pAlly[i]->GetPos().Y)m_pAlly[i]->AddPosY(-2.0f);
			if (PentagonPosY > m_pAlly[i]->GetPos().Y)m_pAlly[i]->AddPosY(2.0f);
			break;
		case 6:
			if (HexagonPosY < m_pAlly[i]->GetPos().Y)m_pAlly[i]->AddPosY(-2.0f);
			if (HexagonPosY > m_pAlly[i]->GetPos().Y)m_pAlly[i]->AddPosY(2.0f);
			break;
		case 7:
			if (HeptagonPosY < m_pAlly[i]->GetPos().Y)m_pAlly[i]->AddPosY(-2.0f);
			if (HeptagonPosY > m_pAlly[i]->GetPos().Y)m_pAlly[i]->AddPosY(2.0f);
			break;
		case 8:
			if (OctagonPosY < m_pAlly[i]->GetPos().Y)m_pAlly[i]->AddPosY(-2.0f);
			if (OctagonPosY > m_pAlly[i]->GetPos().Y)m_pAlly[i]->AddPosY(2.0f);
			break;
		}
		if(PosX < m_pAlly[i]->GetPos().X)m_pAlly[i]->AddPosX(-2.0f);
		if(PosX > m_pAlly[i]->GetPos().X)m_pAlly[i]->AddPosX(2.0f);
	}

}

/*＝＝＝＝＝＝＝＝＝＝*/
/*	デバック画面処理  */
/*＝＝＝＝＝＝＝＝＝＝*/

/*＝＝＝＝＝バトル用＝＝＝＝＝*/

void CBattle::CreateAllyLogDraw(void)
{
	float fPosX[3] = {-1600.0f,-1700.0f,-1800.0f};
	float fPosY = 0.0f;

	CreateAllyLog();

	for (int i = 0; i < 6; i++)
	{
		int nDigits = 0;	//桁数
		int nNumbers[3];


		if (m_nAllyTypes[i] < 10)
		{
			nDigits = 1;
			nNumbers[0] = m_nAllyTypes[i];
		}
		if (m_nAllyTypes[i] > 9 && m_nAllyTypes[i] < 100)
		{
			nDigits = 2;
			nNumbers[1] = m_nAllyTypes[i] % 10;
			nNumbers[0] = (m_nAllyTypes[i] / 10) % 10;
		}
		if (m_nAllyTypes[i] > 99 && m_nAllyTypes[i] < 1000)
		{
			nDigits = 3;
			nNumbers[2] = m_nAllyTypes[i] % 10;
			nNumbers[1] = (m_nAllyTypes[i] / 10) % 10;
			nNumbers[0] = m_nAllyTypes[i] / 100;
		}

		switch (i)
		{
		case 0:
				fPosY = -800;
			break;
		case 1:
				fPosY = -500;
			break;
		case 2:
				fPosY = -200;
			break;
		case 3:
				fPosY = 200;
			break;
		case 4:
				fPosY = 500;
			break;
		case 5:
				fPosY = 800;
			break;
		}

		for (int l = 0; l < nDigits; l++)
		{

			//スプライトの座標の設定
			SetSpritePos(fPosX[l], fPosY);

			//大きさの設定
			SetSpriteScale(1.0f, 1.0f);

			//背景色の設定
			SetSpriteColor(1.0f, 1.0f, 1.0f, 1.0f);

			//その他、表示に必要なSpriteDrawer.hの各種関数を呼び出す
			SetSpriteTexture(m_pLogTex[nNumbers[l]]);

			// 描画
			DrawSprite(m_pLogVtx, sizeof(Vertex));

			// スプライト設定のリセット
			ReSetSprite();
		}
	}
}

void CBattle::CreateEnemyLogDraw(void)
{
	float fPosX[3] = {1600.0f,1700.0f,1800.0f};
	float fPosY = 0.0f;

	CreateEnemyLog();

	for (int i = 0; i < 3; i++)
	{
		int nDigits = 0;	//桁数
		int nNumbers[3];


		if (m_nEnemyTypes[i] < 10)
		{
			nDigits = 1;
			nNumbers[0] = m_nEnemyTypes[i];
		}
		if (m_nEnemyTypes[i] > 9 && m_nEnemyTypes[i] < 100)
		{
			nDigits = 2;
			nNumbers[1] = m_nEnemyTypes[i] % 10;
			nNumbers[0] = (m_nEnemyTypes[i] / 10) % 10;
		}
		if (m_nEnemyTypes[i] > 99 && m_nEnemyTypes[i] < 1000)
		{
			nDigits = 3;
			nNumbers[2] = m_nEnemyTypes[i] % 10;
			nNumbers[1] = (m_nEnemyTypes[i] / 10) % 10;
			nNumbers[0] = m_nEnemyTypes[i] / 100;
		}

		switch (i)
		{
		case 0:
				fPosY = -800;
			break;
		case 1:
				fPosY = -500;
			break;
		case 2:
				fPosY = -200;
			break;
		}

		for (int l = 0; l < nDigits; l++)
		{

			//スプライトの座標の設定
			SetSpritePos(fPosX[l], fPosY);

			//大きさの設定
			SetSpriteScale(1.0f, 1.0f);

			//背景色の設定
			SetSpriteColor(1.0f, 1.0f, 1.0f, 1.0f);

			//その他、表示に必要なSpriteDrawer.hの各種関数を呼び出す
			SetSpriteTexture(m_pLogTex[nNumbers[l]]);

			// 描画
			DrawSprite(m_pLogVtx, sizeof(Vertex));

			// スプライト設定のリセット
			ReSetSprite();
		}
	}
}

void CBattle::CreateAllyLog(void)
{
	m_nAllyTypes[0] = 0;
	m_nAllyTypes[1] = 0;
	m_nAllyTypes[2] = 0;
	m_nAllyTypes[3] = 0;
	m_nAllyTypes[4] = 0;
	m_nAllyTypes[5] = 0;
	for (int i = 0; i < m_nAllyCount; i++)
	{
		switch (m_pAlly[i]->GetCornerCount())
		{
		case 3:
			m_nAllyTypes[0]++;
			break;
		case 4:
			m_nAllyTypes[1]++;
			break;
		case 5:
			m_nAllyTypes[2]++;
			break;
		case 6:
			m_nAllyTypes[3]++;
			break;
		case 7:
			m_nAllyTypes[4]++;
			break;
		case 8:
			m_nAllyTypes[5]++;
			break;
		}
	}
}

void CBattle::CreateEnemyLog(void)
{
	m_nEnemyTypes[0] = 0;
	m_nEnemyTypes[1] = 0;
	m_nEnemyTypes[2] = 0;

	for (int i = 0; i < m_nEnemyCount; i++)
	{
		switch (m_pEnemy[i]->GetCornerCount())
		{
		case 3:
			m_nEnemyTypes[0]++;
			break;
		case 4:
			m_nEnemyTypes[1]++;
			break;
		case 5:
			m_nEnemyTypes[2]++;
			break;
		}
	}
}

/*＝＝＝＝＝図形判定用＝＝＝＝＝*/

void CBattle::SaveAllyLog(void)
{
	m_nAllyTypes[0] = 0;
	m_nAllyTypes[1] = 0;
	m_nAllyTypes[2] = 0;
	m_nAllyTypes[3] = 0;
	m_nAllyTypes[4] = 0;
	m_nAllyTypes[5] = 0;
	for (int i = 0; i < m_nAllyDateCount; i++)
	{
		switch (m_tAllyData[i].nCornerCount)
		{
		case 3:
			m_nAllyTypes[0]++;
			break;
		case 4:
			m_nAllyTypes[1]++;
			break;
		case 5:
			m_nAllyTypes[2]++;
			break;
		case 6:
			m_nAllyTypes[3]++;
			break;
		case 7:
			m_nAllyTypes[4]++;
			break;
		case 8:
			m_nAllyTypes[5]++;
			break;
		}
	}
}

void CBattle::SaveAllyLogDraw(void)
{
	float fPosX[3] = { 1600.0f,1700.0f,1800.0f };
	float fPosY = 0.0f;

	SaveAllyLog();

	for (int i = 0; i < 6; i++)
	{
		int nDigits = 0;	//桁数
		int nNumbers[3];


		if (m_nAllyTypes[i] < 10)
		{
			nDigits = 1;
			nNumbers[0] = m_nAllyTypes[i];
		}
		if (m_nAllyTypes[i] > 9 && m_nAllyTypes[i] < 100)
		{
			nDigits = 2;
			nNumbers[1] = m_nAllyTypes[i] % 10;
			nNumbers[0] = (m_nAllyTypes[i] / 10) % 10;
		}
		if (m_nAllyTypes[i] > 99 && m_nAllyTypes[i] < 1000)
		{
			nDigits = 3;
			nNumbers[2] = m_nAllyTypes[i] % 10;
			nNumbers[1] = (m_nAllyTypes[i] / 10) % 10;
			nNumbers[0] = m_nAllyTypes[i] / 100;
		}

		switch (i)
		{
		case 0:
			fPosY = -800;
			break;
		case 1:
			fPosY = -500;
			break;
		case 2:
			fPosY = -200;
			break;
		case 3:
			fPosY = 200;
			break;
		case 4:
			fPosY = 500;
			break;
		case 5:
			fPosY = 800;
			break;
		}

		for (int l = 0; l < nDigits; l++)
		{

			//スプライトの座標の設定
			SetSpritePos(fPosX[l], fPosY);

			//大きさの設定
			SetSpriteScale(1.0f, 1.0f);

			//背景色の設定
			SetSpriteColor(1.0f, 1.0f, 1.0f, 1.0f);

			//その他、表示に必要なSpriteDrawer.hの各種関数を呼び出す
			SetSpriteTexture(m_pLogTex[nNumbers[l]]);

			// 描画
			DrawSprite(m_pLogVtx, sizeof(Vertex));

			// スプライト設定のリセット
			ReSetSprite();
		}
	}
}
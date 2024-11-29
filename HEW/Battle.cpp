#include "Battle.h"
#include "SceneGame.h"
#include "Main.h"
#include "DirectXTex/TextureLoad.h"
#include "Controller.h"
#include"SpriteDrawer.h"
#include "Input.h"

//防衛最前線ライン
#define SENTER_POSX (0)
//#define SENTER_POSZ (0)

//味方コアの位置
#define ALLYCORE_POSX (100)
#define ALLYCORE_POSZ (0)

//味方の生成位置
#define ALLYCREATE_POSX (80)

#define ALLYCREATE_POSZ_1 (-100)
#define ALLYCREATE_POSZ_2 (-50)
#define ALLYCREATE_POSZ_3 (0)
#define ALLYCREATE_POSZ_4 (50)
#define ALLYCREATE_POSZ_5 (100)
//敵の生成位置
#define ENEMYCREATE_POSX (-80)

#define ENEMYCREATE_POSZ_1 (-100)
#define ENEMYCREATE_POSZ_2 (-50)
#define ENEMYCREATE_POSZ_3 (0)
#define ENEMYCREATE_POSZ_4 (50)
#define ENEMYCREATE_POSZ_5 (100)

#define MOVESPEED(Speed) Speed / 10

//戦闘フィールドのサイズ
#define BATTLEFIELDWIDTH (1920)
#define BATTLEFIELDHEIGHT (1080)

ID3D11ShaderResourceView* m_pLogTex[10];
float FirstPosX[5];
float FirstPosY[16];
int SelectFirstPosX;
int SelectFirstPosY;

CBattle::CBattle()
	: m_nAllyCount(0)
	, m_nAllyDateCount(0)
	, m_nEnemyCount(0)
	, m_nEnemyDateCount{ 0,0,0,0,0 }
	, m_nNowWave(0)
	, m_nMaxWave(0)
	, m_nSelectPattern(0)
	, m_nMaxPattern(0)
	, m_pAlly{}
	, m_pAllyBuffer{}
	, m_pEnemy{}
	, m_nAllyTypes{ 0,0,0,0,0,0 }
	, m_nEnemyTypes{ 0,0,0 }
	, m_nBattleTime(0)
	, m_bFirstFight(false)
	, m_nFirstPosPattern(0)
	, m_nOldEnemyCount(0)
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

		//m_tAllyData[i].m_tCreatePos.X = ALLYCREATE_POSX;
		//m_tAllyData[i].m_tCreatePos.Y = ALLYCREATE_POSY_CENTER;
		//m_tAllyData[i].m_tCreatePos.Z = 0.0f;

		m_tAllyData[i].Size = 1.0f;
	}
	for (int j = 0; j < MAX_WAVE; j++)
	{
		for (int l = 0; l < MAX_PATTERN; l++)
		{
			for (int i = 0; i < MAX_ENEMY; i++)
			{
				m_tEnemyData[j][l][i].nCornerCount = -1;

				m_tEnemyData[j][l][i].m_tCreatePos.X = ENEMYCREATE_POSX;
				m_tEnemyData[j][l][i].m_tCreatePos.Y = 0.0f;
				m_tEnemyData[j][l][i].m_tCreatePos.Z = 0.0f;
				m_tEnemyData[j][l][i].Size = 1.0f;
			}
		}
	}

	FirstPosX[0] = 0.0f;
	FirstPosX[1] = 10.0f;
	FirstPosX[2] = 20.0f;
	FirstPosX[3] = 30.0f;
	FirstPosX[4] = 40.0f;

	FirstPosY[0] = -700.0f;
	FirstPosY[1] = -600.0f;
	FirstPosY[2] = -500.0f;
	FirstPosY[3] = -400.0f;
	FirstPosY[4] = -30.0f;
	FirstPosY[5] = -200.0f;
	FirstPosY[6] = -100.0f;
	FirstPosY[7] =  0.0f;
	FirstPosY[8] = 100.0f;
	FirstPosY[9] = 200.0f;
	FirstPosY[10] = 300.0f;
	FirstPosY[11] = 400.0f;
	FirstPosY[12] = 500.0f;
	FirstPosY[13] = 600.0f;
	FirstPosY[14] = 700.0f;
	FirstPosY[15] = 800.0f;

	SelectFirstPosX = 0;
	SelectFirstPosY = 0;
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
	//バフの処理
	for (int i = 0; i < m_nAllyBufferCount; i++)
	{
		if (m_pAllyBuffer[i]->GetStatus() == St_Battle)//ステータスがバトルだったら
		{
			if (!m_pAllyBuffer[i]->IsBuff)
			{
				switch (m_pAllyBuffer[i]->GetBuffType())
				{
				case CAllyBuffer::BT_Shield:
					for (int i = 0; i < m_nAllyCount; i++)
					{
						if (m_pAlly[i]->GetStatus() == St_Battle)//ステータスがバトルだったら
						{
							m_pAlly[i]->SetShield(5.0f);
						}
					}
					break;
				case CAllyBuffer::BT_Attack:
					for (int i = 0; i < m_nAllyCount; i++)
					{
						if (m_pAlly[i]->GetStatus() == St_Battle)//ステータスがバトルだったら
						{
							m_pAlly[i]->AddAtk(1.0f);
						}
					}
					break;
				case CAllyBuffer::BT_ReSummon:
					//今存在している味方を生成予定情報に保存する
					for (int i = 0; i < m_nAllyCount; i++)
					{
						SaveAllyData(m_pAlly[i]->GetCornerCount(), m_pAlly[i]->GetSize().X / NORMAL_SIZE);
					}
					//指定された数だけ生成する
					while (m_nAllyDateCount)
					{
						CVector3<float> FirstPos;
						FirstPos.X = 0.0f;
						FirstPos.Y = 0.0f;
						FirstPos.Z = 0.0f;
						//味方を生成する
						CreateAllyData(m_tAllyData[0], FirstPos);

						//生成に使用したため情報を消して後ろの情報を前詰めにする
						for (int i = 0; i + 1 < MAX_ALLY; i++)
						{
							m_tAllyData[i] = m_tAllyData[i + 1];
						}
						//保存済みの総数を減らす
						m_nAllyDateCount--;
					}
					//全ての味方キャラクターのステータスをバトルに変更する
					for (int i = 0; i < m_nAllyCount; i++)
					{
						m_pAlly[i]->SetStatus(St_Battle);
					}
					break;
				}
			}
		}
	}

	//位置セッティング
	FirstPosSetting();

	//削除処理
	Delete();

	for (int i = 0; i < m_nAllyCount; i++)
	{
		if (m_pAlly[i]->GetStatus() == St_Battle)//ステータスがバトルだったら
		{
			//索敵処理
			Search(i, Ally);

			//攻撃判定
			for (int l = 0; l < m_nEnemyCount; l++)
			{
				m_pAlly[i]->m_bIsAttack = false;
				//攻撃当たり判定に入ってるかどうか
				if (m_pEnemy[l]->GetStatus() == St_Battle)
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
		if (m_pEnemy[i]->GetStatus() == St_Battle)//ステータスがバトルだったら
		{
			//索敵処理
			Search(i, Enemy);

			//攻撃判定
			for (int l = 0; l < m_nAllyCount; l++)
			{
				m_pEnemy[i]->m_bIsAttack = false;

				//攻撃当たり判定に入ってるかどうか
				if (m_pAlly[l]->GetStatus() == St_Battle)//ステータスがバトルだったら
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

	CharacterUpdate();

	m_nBattleTime++;//戦闘時間の更新

	//勝敗判定
	if (m_nNowWave == 0)
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
			if (m_pEnemyBoss == nullptr)
			{
				MessageBox(NULL, "ボスを倒したためステージクリア！！", "勝敗", MB_OK);
				ChangeScene(SCENE_TITLE);
			}
		}
	}
	if (GetNowPhase() == BATTLE)
	{
		if (m_pAllyPlayer == nullptr)
		{
			m_nNowWave = 0;
			MessageBox(NULL,"プレイヤーが倒されたため敗北", "勝敗", MB_OK);
			ChangeScene(SCENE_TITLE);
		}
	}

}

void CBattle::CharacterUpdate(void)
{
	//味方の更新(アニメーション)
	for (int i = 0; i < m_nAllyCount; i++)m_pAlly[i]->Update();

	for (int i = 0; i < m_nAllyBufferCount; i++)m_pAllyBuffer[i]->Update();

	//敵の更新(アニメーション)
	for (int i = 0; i < m_nEnemyCount; i++)m_pEnemy[i]->Update();

	if (m_nNowWave == 1)
	{
		m_pEnemyBoss->Update();
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
	}

	for (int i = 0; i < m_nAllyBufferCount; i++)
	{
		m_pAllyBuffer[i]->Draw();
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
	}

	if (m_nNowWave == 1)
	{
		m_pEnemyBoss->Draw();
	}
}

void CBattle::ReDrawingInit(void)
{
	m_nOldEnemyCount = m_nEnemyCount;//敵の生存数を保存

	for (int i = 0; i < MAX_ALLY; i++)
	{
		m_pAllyBuffer[i] = nullptr;
	}

	for (int i = 0; i < m_nAllyCount; i++)
	{
		m_pAlly[i]->SetShield(0.0f);
		m_pAlly[i]->SetAtk(1.0f);
	}

	RandomSelectPattern();
}

void CBattle::NextWaveInit(void)
{
	m_nEnemyCount = 0;
	m_nAllyDateCount = 0;
	m_nBattleTime = 0;
	m_bFirstFight = false;
}

void CBattle::SaveAllyData(int InCornerCount, float InSize)
{
	InSize = 1 + ((InSize - 1) / 10);	//1を基準として2で入ってきた場合1.1倍にするため

	m_tAllyData[m_nAllyDateCount].nCornerCount = InCornerCount;
	m_tAllyData[m_nAllyDateCount].Size = InSize;
	
	//保存数を加算
	m_nAllyDateCount++;
}

void CBattle::SaveEnemyData(int InCornerCount, int InWave,int InPattern, float InSize)
{
	InSize = 1 + ((InSize - 1) / 10);	//1を基準として2で入ってきた場合1.1倍にするため

	m_tEnemyData[InWave][InPattern][m_nEnemyDateCount[InWave][InPattern]].nCornerCount = InCornerCount;
	m_tEnemyData[InWave][InPattern][m_nEnemyDateCount[InWave][InPattern]].Size = InSize;
	//保存数を加算
	m_nEnemyDateCount[InWave][InPattern]++;
}

void CBattle::CreateEntity()
{
	if (m_pAllyPlayer == nullptr)
	{
		CVector3<float> InFirstPos;
		InFirstPos.X = ALLYCORE_POSX;
		InFirstPos.Y = 0.0f;
		InFirstPos.Z = ALLYCORE_POSZ;
		m_pAllyPlayer = new CAllyPlayer(1.0f, InFirstPos, m_pCamera);
	}
	//指定された数だけ生成する
	while (m_nAllyDateCount)
	{
		CVector3<float> InFirstPos;
		InFirstPos.X = FirstPosX[SelectFirstPosX];
		InFirstPos.Y = FirstPosY[SelectFirstPosY];

		//味方を生成する
		CreateAllyData(m_tAllyData[0], InFirstPos);

		//生成に使用したため情報を消して後ろの情報を前詰めにする
		for (int i = 0; i + 1 < MAX_ALLY; i++)
		{
			m_tAllyData[i] = m_tAllyData[i + 1];
		}
		//保存済みの総数を減らす
		m_nAllyDateCount--;

		SelectFirstPosX++;

		if (SelectFirstPosX >= 5)
		{
			SelectFirstPosX = 0;
			SelectFirstPosY++;
		}
	}

	//指定された数だけ生成する
	while (m_nEnemyDateCount[m_nNowWave][m_nSelectPattern] - (m_nEnemyCount - m_nOldEnemyCount))
	{
		//敵を生成する
		CreateEnemyData(m_tEnemyData[m_nNowWave][m_nSelectPattern][m_nEnemyCount - m_nOldEnemyCount]);

		////生成に使用したため情報を消して後ろの情報を前詰めにする
		//for (int i = 0; i + 1 < MAX_ENEMY; i++)
		//{
		//	m_tEnemyData[m_nNowWave][m_nSelectPattern][i] = m_tEnemyData[m_nNowWave][m_nSelectPattern][i + 1];
		//}
		////保存済みの総数を減らす
		//m_nEnemyDateCount[m_nNowWave]--;
		
		//生成数を加算
		m_nEnemyCount++;
	}

	if (m_nNowWave == 1)
	{
		CVector3<float> BossFirstPos;

		BossFirstPos.X = ENEMYCREATE_POSX;
		BossFirstPos.Y = -1.0f;
		BossFirstPos.Z = ENEMYCREATE_POSZ_3;

		m_pEnemyBoss = new CEnemyBoss(m_nStageNum, 2.0f, BossFirstPos, m_pCamera);
	}
}

void CBattle::CreateAllyData(EntityData InData, CVector3<float> InFirstPos)
{
	if (m_tAllyData[m_nAllyCount].nCornerCount < 6)
	{
		m_pAlly[m_nAllyCount] = new CAlly(InData.nCornerCount, InData.Size, InFirstPos,m_pCamera);
		//生成数を加算
		m_nAllyCount++;
	}
	else
	{
		m_pAllyBuffer[m_nAllyCount] = new CAllyBuffer(InData.nCornerCount, InData.Size, InFirstPos, m_pCamera);
		//生成数を加算
		m_nAllyBufferCount++;
	}
}

void CBattle::CreateEnemyData(EntityData InDate)
{
	m_pEnemy[m_nEnemyCount] = new CEnemy(InDate.nCornerCount,InDate.Size,InDate.m_tCreatePos, m_pCamera);
}

void CBattle::Search(int i, Entity Entity)
{
	switch (Entity)
	{
	case CBattle::Ally:/*味方の判定*/
		m_pAlly[i]->m_nTargetNumber = -1;

		for (int l = 0; l < m_nEnemyCount; l++)
		{
			if (m_pEnemy[l]->GetStatus() == St_Battle)//相手のステータスがBattleかどうか
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
			if (m_pAlly[l]->GetStatus() == St_Battle)//相手のステータスがBattleかどうか
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
				if (m_pEnemy[m_pAlly[i]->m_nTargetNumber]->GetPos().X < m_pAlly[i]->GetPos().X)m_pAlly[i]->AddPosX(-MOVESPEED(2.0f));
				if (m_pEnemy[m_pAlly[i]->m_nTargetNumber]->GetPos().X > m_pAlly[i]->GetPos().X)m_pAlly[i]->AddPosX(MOVESPEED(2.0f));

				if (m_pEnemy[m_pAlly[i]->m_nTargetNumber]->GetPos().Z < m_pAlly[i]->GetPos().Z)m_pAlly[i]->AddPosZ(-MOVESPEED(2.0f));
				if (m_pEnemy[m_pAlly[i]->m_nTargetNumber]->GetPos().Z > m_pAlly[i]->GetPos().Z)m_pAlly[i]->AddPosZ(MOVESPEED(2.0f));
			}
			else
			{
				if (SENTER_POSX < m_pEnemy[i]->GetPos().X)m_pEnemy[i]->AddPosX(-MOVESPEED(2.0f));
				if (SENTER_POSX > m_pEnemy[i]->GetPos().X)m_pEnemy[i]->AddPosX(MOVESPEED(2.0f));
			}
		}
		else//標的がいないので敵のコアに向かって進む
		{
			if (SENTER_POSX < m_pAlly[i]->GetPos().X)m_pAlly[i]->AddPosX(-MOVESPEED(2.0f));
			if (SENTER_POSX > m_pAlly[i]->GetPos().X)m_pAlly[i]->AddPosX(MOVESPEED(2.0f));
		}
		break;

		/*敵の判定*/
	case CBattle::Enemy:
		if (m_pEnemy[i]->m_nTargetNumber != -1)//標的を設定済み
		{
			if (m_pEnemy[i]->m_nTargetNumber < m_nAllyCount)//現在生存数よりも数字が大きくないかどうか
			{
				if (m_pAlly[m_pEnemy[i]->m_nTargetNumber]->GetPos().X < m_pEnemy[i]->GetPos().X)m_pEnemy[i]->AddPosX(-MOVESPEED(2.0f));
				if (m_pAlly[m_pEnemy[i]->m_nTargetNumber]->GetPos().X > m_pEnemy[i]->GetPos().X)m_pEnemy[i]->AddPosX(MOVESPEED(2.0f));

				if (m_pAlly[m_pEnemy[i]->m_nTargetNumber]->GetPos().Z < m_pEnemy[i]->GetPos().Z)m_pEnemy[i]->AddPosZ(-MOVESPEED(2.0f));
				if (m_pAlly[m_pEnemy[i]->m_nTargetNumber]->GetPos().Z > m_pEnemy[i]->GetPos().Z)m_pEnemy[i]->AddPosZ(MOVESPEED(2.0f));
			}
			else
			{
				if (ALLYCORE_POSX < m_pEnemy[i]->GetPos().X)m_pEnemy[i]->AddPosX(-MOVESPEED(2.0f));
				if (ALLYCORE_POSX > m_pEnemy[i]->GetPos().X)m_pEnemy[i]->AddPosX(MOVESPEED(2.0f));

				if (ALLYCORE_POSZ < m_pEnemy[i]->GetPos().Z)m_pEnemy[i]->AddPosZ(-MOVESPEED(2.0f));
				if (ALLYCORE_POSZ > m_pEnemy[i]->GetPos().Z)m_pEnemy[i]->AddPosZ(MOVESPEED(2.0f));
			}
		}
		//ここをプレイヤーの位置に進むに変更する
		else//標的がいないので敵のコアに向かって進む
		{
			if (ALLYCORE_POSX < m_pEnemy[i]->GetPos().X)m_pEnemy[i]->AddPosX(-MOVESPEED(2.0f));
			if (ALLYCORE_POSX > m_pEnemy[i]->GetPos().X)m_pEnemy[i]->AddPosX(MOVESPEED(2.0f));

			if (ALLYCORE_POSZ < m_pEnemy[i]->GetPos().Z)m_pEnemy[i]->AddPosZ(-MOVESPEED(2.0f));
			if (ALLYCORE_POSZ > m_pEnemy[i]->GetPos().Z)m_pEnemy[i]->AddPosZ(MOVESPEED(2.0f));
		}
		break;

	case EnemyBoss:
		if (m_pEnemyBoss->m_nTargetNumber != -1)//標的を設定済み
		{
			if (m_pEnemyBoss->m_nTargetNumber < m_nAllyCount)//現在生存数よりも数字が大きくないかどうか
			{
				if (m_pAlly[m_pEnemyBoss->m_nTargetNumber]->GetPos().X < m_pEnemyBoss->GetPos().X)m_pEnemyBoss->AddPosX(-MOVESPEED(2.0f));
				if (m_pAlly[m_pEnemyBoss->m_nTargetNumber]->GetPos().X > m_pEnemyBoss->GetPos().X)m_pEnemyBoss->AddPosX(MOVESPEED(2.0f));

				if (m_pAlly[m_pEnemyBoss->m_nTargetNumber]->GetPos().Z < m_pEnemyBoss->GetPos().Z)m_pEnemyBoss->AddPosZ(-MOVESPEED(2.0f));
				if (m_pAlly[m_pEnemyBoss->m_nTargetNumber]->GetPos().Z > m_pEnemyBoss->GetPos().Z)m_pEnemyBoss->AddPosZ(MOVESPEED(2.0f));
			}
			else
			{
				if (ALLYCORE_POSX < m_pEnemyBoss->GetPos().X)m_pEnemyBoss->AddPosX(-MOVESPEED(2.0f));
				if (ALLYCORE_POSX > m_pEnemyBoss->GetPos().X)m_pEnemyBoss->AddPosX(MOVESPEED(2.0f));

				if (ALLYCORE_POSZ < m_pEnemyBoss->GetPos().Z)m_pEnemyBoss->AddPosZ(-MOVESPEED(2.0f));
				if (ALLYCORE_POSZ > m_pEnemyBoss->GetPos().Z)m_pEnemyBoss->AddPosZ(MOVESPEED(2.0f));
			}
		}
		else//標的がいないので敵のコアに向かって進む
		{
			if (ALLYCORE_POSX < m_pEnemyBoss->GetPos().X)m_pEnemyBoss->AddPosX(-MOVESPEED(2.0f));
			if (ALLYCORE_POSX > m_pEnemyBoss->GetPos().X)m_pEnemyBoss->AddPosX(MOVESPEED(2.0f));

			if (ALLYCORE_POSZ < m_pEnemyBoss->GetPos().Z)m_pEnemyBoss->AddPosZ(-MOVESPEED(2.0f));
			if (ALLYCORE_POSZ > m_pEnemyBoss->GetPos().Z)m_pEnemyBoss->AddPosZ(MOVESPEED(2.0f));

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
			if (m_pEnemy[l]->GetStatus() == St_Battle)//相手のステータスがBattleかどうか
			{
				m_pEnemy[l]->Damage(m_pAlly[i]);//相手の体力を減らす
				m_pAlly[i]->ChargeReset();//攻撃したのでチャージをリセットする
			}
		}
		else//攻撃チャージが溜まっていなかったら
		{
			m_pAlly[i]->AddAtkCharge();//チャージする
		}
		break;

	case CBattle::Enemy://敵の攻撃
		if (m_pEnemy[i]->GetAtkCharge() >= m_pEnemy[i]->GetCoolTime())//攻撃チャージがたまっているかどうか
		{
			if (m_pEnemy[i]->AtkCollisionCheck(m_pAlly[l]->GetSize(), m_pAlly[l]->GetPos()))//当たり判定の中にいるかどうか
			{
				if (m_pAlly[l]->GetStatus() == St_Battle)//相手のステータスがBattleかどうか
				{
					m_pAlly[l]->Damage(m_pEnemy[i]);//相手の体力を減らす
					m_pEnemy[i]->ChargeReset();//攻撃したのでチャージをリセットする
				}
			}
		}
		else//攻撃チャージが溜まっていなかったら
		{
			m_pEnemy[i]->AddAtkCharge();//チャージする
		}
		break;
	case CBattle::EnemyBoss:
		if (m_pEnemyBoss->GetAtkCharge() >= m_pEnemyBoss->GetCoolTime())//攻撃チャージがたまっているかどうか
		{
			if (m_pEnemyBoss->AtkCollisionCheck(m_pAlly[l]->GetSize(), m_pAlly[l]->GetPos()))//当たり判定の中にいるかどうか
			{
				if (m_pAlly[l]->GetStatus() == St_Battle)//相手のステータスがBattleかどうか
				{
					m_pAlly[l]->Damage(m_pEnemyBoss);//相手の体力を減らす
					m_pEnemyBoss->ChargeReset();//攻撃したのでチャージをリセットする
				}
			}
		}
		else//攻撃チャージが溜まっていなかったら
		{
			m_pEnemyBoss->AddAtkCharge();//チャージする
		}
		break;
	}
}

void CBattle::Alive(void)
{
	//味方判定
	for (int l = 0; l < m_nAllyCount; l++)
	{
		if (m_pAlly[l]->GetStatus() == St_Battle)//味方のステータスがBattleかどうか
		{
			if (m_pAlly[l]->GetHp() <= 0.0f)//味方の体力が残っているかどうか
			{
				m_pAlly[l]->SetStatus(St_Death);//ステータスを死亡状態にする
			}
		}
	}
	for (int l = 0; l < m_nEnemyCount; l++)
	{
		if (m_pEnemy[l]->GetStatus() == St_Battle)//敵のステータスがBattleかどうか
		{
			if (m_pEnemy[l]->GetHp() <= 0.0f)//敵の体力が残っているかどうか
			{
				m_pEnemy[l]->SetStatus(St_Death);//ステータスを死亡状態にする
			}
		}
	}
}

void CBattle::Delete(void)
{
	//味方の生存判定
	for (int i = 0; i < m_nAllyCount; i++)
	{
		if (m_pAlly[i]->GetStatus() == St_Delete)					//ステータスがDeleteかどうか
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
		if (m_pEnemy[i]->GetStatus() == St_Delete)		//ステータスがDeleteかどうか
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
	//敵ボスの生存判定
	if (m_nNowWave == 1)
	{
		if (m_pEnemyBoss->GetStatus() == St_Delete)		//ステータスがDeleteかどうか
		{
			m_pEnemyBoss = nullptr;
		}
	}
}

void CBattle::FirstPosSetting()
{
	for (int i = 0; i < m_nAllyCount; i++)
	{
		if (!m_pAlly[i])continue;

		if (!m_pAlly[i]->m_bFirstBattlePosSetting)
		{
			m_pAlly[i]->SetPosX(ALLYCREATE_POSX);
			m_pAlly[i]->SetPosY(0.0f + m_pAlly[i]->GetSize().Y / 2);

			switch (m_nFirstPosPattern)
			{
			case 0:
				m_pAlly[i]->SetPosZ(ALLYCREATE_POSZ_1);
				m_pAlly[i]->m_bFirstBattlePosSetting = true;
				m_nFirstPosPattern = 1;
				break;
			case 1:
				m_pAlly[i]->SetPosZ(ALLYCREATE_POSZ_2);
				m_pAlly[i]->m_bFirstBattlePosSetting = true;
				m_nFirstPosPattern = 2;
				break;
			case 2:
				m_pAlly[i]->SetPosZ(ALLYCREATE_POSZ_3);
				m_pAlly[i]->m_bFirstBattlePosSetting = true;
				m_nFirstPosPattern = 3;
				break;
			case 3:
				m_pAlly[i]->SetPosZ(ALLYCREATE_POSZ_4);
				m_pAlly[i]->m_bFirstBattlePosSetting = true;
				m_nFirstPosPattern = 4;
				break;
			case 4:
				m_pAlly[i]->SetPosZ(ALLYCREATE_POSZ_5);
				m_pAlly[i]->m_bFirstBattlePosSetting = true;
				m_nFirstPosPattern = 0;
				break;
			}
		}
	}
	for (int i = 0; i < m_nEnemyCount; i++)
	{
		if (!m_pEnemy[i])continue;

		if (!m_pEnemy[i]->m_bFirstBattlePosSetting)
		{
			m_pEnemy[i]->SetPosX(ENEMYCREATE_POSX);
			m_pEnemy[i]->SetPosY(0.0f + m_pEnemy[i]->GetSize().Y / 2);

			switch (m_nFirstPosPattern)
			{
			case 0:
				m_pEnemy[i]->SetPosZ(ENEMYCREATE_POSZ_1);
				m_pEnemy[i]->m_bFirstBattlePosSetting = true;
				m_nFirstPosPattern = 1;
				break;
			case 1:
				m_pEnemy[i]->SetPosZ(ENEMYCREATE_POSZ_2);
				m_pEnemy[i]->m_bFirstBattlePosSetting = true;
				m_nFirstPosPattern = 2;
				break;
			case 2:
				m_pEnemy[i]->SetPosZ(ENEMYCREATE_POSZ_3);
				m_pEnemy[i]->m_bFirstBattlePosSetting = true;
				m_nFirstPosPattern = 3;
				break;
			case 3:
				m_pEnemy[i]->SetPosZ(ENEMYCREATE_POSZ_4);
				m_pEnemy[i]->m_bFirstBattlePosSetting = true;
				m_nFirstPosPattern = 4;
				break;
			case 4:
				m_pEnemy[i]->SetPosZ(ENEMYCREATE_POSZ_5);
				m_pEnemy[i]->m_bFirstBattlePosSetting = true;
				m_nFirstPosPattern = 0;
				break;
			}
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
		}
	}
	for (int i = 0; i < m_nAllyBufferCount; i++)
	{
		switch (m_pAllyBuffer[i]->GetCornerCount())
		{
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

void CBattle::RandomSelectPattern(void)
{
	m_nSelectPattern = rand() % m_nMaxPattern;
}
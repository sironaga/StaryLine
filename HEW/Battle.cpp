/*==============================================================
*
*  File：Battle.cpp
*
*  編集者：宇留野陸斗｜[作成][管理]
**
*  編集者：中嶋飛賀　｜[ 音 ]
*
================================================================*/

#include "Battle.h"
#include "SceneGame.h"
#include "Main.h"
#include "DirectXTex/TextureLoad.h"
#include "Controller.h"
#include"SpriteDrawer.h"
#include "Input.h"
#include "StageSelect.h"
#include "Defines.h"
#include "Pause.h"

//プレイヤーのX座標位置
#define ALLYCORE_POSX (-90)
//プレイヤーのZ座標位置
#define ALLYCORE_POSZ (0)

//敵ボスのX座標位置
#define ENEMYBOSSCORE_POSX (90)
//敵ボスのZ座標位置
#define ENEMYBOSSCORE_POSZ (0)

//味方の生成X座標位置
#define ALLYCREATE_POSX (-80)
//味方の生成Z座標位置
#define ALLYCREATE_POSZ_1 (20)
#define ALLYCREATE_POSZ_2 (10)
#define ALLYCREATE_POSZ_3 (0)
#define ALLYCREATE_POSZ_4 (-10)
#define ALLYCREATE_POSZ_5 (-20)

//敵の生成Z座標位置
#define ENEMYCREATE_POSZ_1 (20)
#define ENEMYCREATE_POSZ_2 (10)
#define ENEMYCREATE_POSZ_3 (0)
#define ENEMYCREATE_POSZ_4 (-10)
#define ENEMYCREATE_POSZ_5 (-20)

//敵の生成X座標位置
enum class ENEMYCREATE_POSX
{
	Stage1 =  80,
	Stage2 =  80,
	Stage3 =  60,
};

//戦闘範囲X軸
#define BATTLE_X (80)
//戦闘範囲Z軸
#define BATTLE_Z (30)
//移動のZ軸方向をし始めるライン
#define MOVEBATTLE_LINE (80)

//移動関係計算マクロ
#define MOVESPEED(Speed) Speed / 10
//移動力
#define MOVEPOWER (0.4f)

//時間の計算マクロ
#define Time(Num) Num * 60

//次に敵が生成される間隔
enum class NEXTSPAWN
{
	Stage1_1 = 10,
	Stage1_2 = 10,
	Stage1_3 = 10,
	Stage2_1 = 10,
	Stage2_2 = 10,
	Stage2_3 = 10,
	Stage3_1 = 10,
	Stage3_2 = 10,
	Stage3_3 = 10,
};
//一度に出てくる敵の数
enum class ENEMY_SPAWNNUM
{
	Stage1_1 = 3,
	Stage1_2 = 6,
	Stage1_3 = 8,
	Stage2_1 = 6,
	Stage2_2 = 8,
	Stage2_3 = 10,
	Stage3_1 = 8,
	Stage3_2 = 10,
	Stage3_3 = 12,
};
//敵の歩行タイプ雑魚敵の確率
enum class ENEMY_PROBABILITY
{
	Stage1_1 = 100,
	Stage1_2 = 100,
	Stage1_3 = 100,
	Stage2_1 = 80,
	Stage2_2 = 70,
	Stage2_3 = 60,
	Stage3_1 = 70,
	Stage3_2 = 30,
	Stage3_3 = 50,
};


//数字のテクスチャ保存用ポインタ
ID3D11ShaderResourceView* m_pLogTex[10];

//移動力構造体
struct MovePower
{
	float x;
	float z;
};

// 移動方向計算関数
MovePower MoveCalculation(DirectX::XMFLOAT3 nPos, DirectX::XMFLOAT3 nEnemyPos);
// 距離計算関数
float Distance(DirectX::XMFLOAT3 t1, DirectX::XMFLOAT3 t2);

// コンストラクタ
CBattle::CBattle(StageType In_StageType)
	: m_pFighter{}
	, m_nAllyTypes{ 0,0 }
	, m_nCreateEnemyNum(0)
	, m_pAllyLeader(nullptr)
	, m_pEnemyLeader(nullptr)
	, m_nEnemyTypes{ 0,0 }
	, m_nBattleTime(0)
	, m_bFirstFight(false)
	, m_nFirstPosPattern(0)
	, m_nStageNum(In_StageType)
	, m_bEnd(false)
	, m_bWin(false)
	, m_nSpawnTime(0)
	, m_nSummonAllyCount(0)
	, m_bWinCommand{false}
	, m_bLoseCommand{false}
{
	//数字テクスチャの読み込み
	HRESULT hr;

	Vertex vtx[] = {
		{{-WIDE_RATIO( 100.0f,GetNowWide()), -HEIGHT_RATIO( 100.0f,GetNowHeight()), 0.0f}, {0.0f, 0.0f}},
		{{-WIDE_RATIO( 100.0f,GetNowWide()),  HEIGHT_RATIO( 100.0f,GetNowHeight()), 0.0f}, {0.0f, 1.0f}},
		{{ WIDE_RATIO( 100.0f,GetNowWide()), -HEIGHT_RATIO( 100.0f,GetNowHeight()), 0.0f}, {1.0f, 0.0f}},
		{{ WIDE_RATIO( 100.0f,GetNowWide()),  HEIGHT_RATIO( 100.0f,GetNowHeight()), 0.0f}, {1.0f, 1.0f}},
	};
	m_pLogVtx = CreateVertexBuffer(vtx, 4);

	//音の初期化
	m_pSound = new CSoundList(SE_DEATH);
	m_pSound->SetMasterVolume();
	m_pDeathSE = m_pSound->GetSound(false);

	// リーダーの生成
	CreateLeader();
}

// デストラクタ
CBattle::~CBattle()
{
	//数字テクスチャの解放
	if (m_pLogVtx)
	{
		m_pLogVtx->Release();
	}
	for (int i = 0; i < 10; i++)
	{
		if (m_pLogTex[i])m_pLogTex[i]->Release();
	}
	//兵士ポインタの解放
	for (int i = 0; i < m_pFighter.size(); i++)
	{
		if (m_pFighter[i])
		{
			delete m_pFighter[i];
			m_pFighter[i] = nullptr;
		}
	}

	//味方リーダーポインタの解放
	if (m_pAllyLeader)
	{
		delete m_pAllyLeader;
		m_pAllyLeader = nullptr;
	}
	//敵リーダーポインタの解放
	if (m_pEnemyLeader)
	{
		delete m_pEnemyLeader;
		m_pEnemyLeader = nullptr;
	}
	//音の解放
	if(m_pSound)
	{
		m_pDeathSE->Stop();;
		SAFE_DELETE(m_pSound);
		m_pDeathSE = nullptr;
	}
}

// 更新処理
void CBattle::Update(void)
{
	//音の設定
	m_pSound->SetMasterVolume();
	CCharacterManager::GetInstance()->SetCharacterMasterVolume();

	//戦闘時間軸処理
	TimeLapse();
	//敵生成処理
	CreateEnemy();

	//初期位置セッティング
	FirstPosSetting();

	//削除処理
	Delete();

	//Z軸順に描画
	for (float Z = 20.0f; Z > -30.0f; Z -= 1.0f)
	{
		for (auto& fighter : m_pFighter)
		{
			// 固有処理
			switch (fighter->GetFighterType())
			{
			case FighterType::Enemy:
				break;
			case FighterType::Ally:
				if (fighter->GetStatus() == Status::Create)
				{
					if (m_pAllyLeader)
					{
						m_pAllyLeader->SetSummonFlag(true);
					}
				}
				break;
			}

			// 共通処理
			//Z軸順に処理
			if (fighter->GetPos().z > Z - 1.0f && fighter->GetPos().z < Z + 1.0f)
			{
				//索敵処理
				Search(fighter);
				//攻撃しているかの判断を初期化
				fighter->m_bIsAttack = false;

				//攻撃をしていなかったら
				if (!fighter->m_bIsAttack)
				{
					// 標的対象を取得
					auto targetList = GetFighterTypeList((int)fighter->GetFighterType() ? FighterType::Enemy : FighterType::Ally);

					if (!targetList.empty())
					{
						//相手を生成順に処理
						for (int l = 0; l < targetList.size(); l++)
						{
							//相手のステータスがBattleだったら
							if (targetList[l]->GetStatus() == Status::Battle)
							{
								//攻撃当たり判定に入ってるかどうか
								if (fighter->AtkCollisionCheck(targetList[l]->GetSize(), targetList[l]->GetPos()))
								{
									//攻撃しているかの判断をtrueにする
									fighter->m_bIsAttack = true;
									if (fighter->GetCornerCount() == targetList[l]->GetCornerCount())
									{
										fighter->m_bTypeAttack = true;
									}
									else
									{
										fighter->m_bTypeAttack = false;
									}
									//攻撃処理
									Battle(fighter, targetList[l]);
									break;
								}
							}
						}
					}
				}
				//攻撃をしていなかったら
				if (!fighter->m_bIsAttack)
				{
					CLeader* targetLeader = nullptr;
					switch (fighter->GetFighterType())
					{
					case FighterType::Enemy:
						targetLeader = m_pAllyLeader;
						break;
					case FighterType::Ally:
						targetLeader = m_pEnemyLeader;
						break;
					}

					//相手のリーダーが生成されているか
					if (targetLeader)
					{
						//相手のリーダーのステータスがBattleだったら
						if (targetLeader->GetStatus() == Status::Battle)
						{
							//攻撃の当たり判定に入っているか
							if (fighter->AtkCollisionCheck(targetLeader->GetSize(), targetLeader->GetPos()))
							{
								//攻撃しているかの判断をtrueにする
								fighter->m_bIsAttack = true;
								fighter->m_bTypeAttack = true;

								//攻撃処理
								Battle(fighter, targetLeader);
							}
						}
					}
				}
				//移動処理
				Move(fighter);
			}
		}
	}
	//範囲外に出ていたら範囲内に補正
	ScopeMove();

	//生存判定
	Alive();
	//キャラクターの更新処理
	CharacterUpdate();
	//戦闘時間の更新
	if(!m_bTutorialStop)m_nBattleTime++;

	/*勝敗判定*/
	//シーンがゲームだったら
	if (GetScene() == SCENE_GAME)
	{
		//敵のリーダーがnullptrになっていたら
		if (m_pEnemyLeader == nullptr)
		{
			if (m_pAllyLeader)
			{
				m_pAllyLeader->SetWinFlag(true);

				m_bEnd = true;
				m_bWin = true;
			}
		}
		//味方のリーダーがnullptrになっていたら
		if (m_pAllyLeader == nullptr)
		{
			SetVolumeSE(m_pDeathSE);
			m_pDeathSE->Start();
			m_bWin = false;
			m_bEnd = true;
		}
	}
}

// キャラクターの更新処理
void CBattle::CharacterUpdate(bool AnimationFlag)
{
	//兵士の更新処理(アニメーション)
	for (auto& fighter : m_pFighter)
	{
		if (!fighter)continue;
		fighter->Update();
	}

	//敵のリーダーの更新処理(アニメーション)
	if (m_pEnemyLeader)
	{
		m_pEnemyLeader->Update();
	}

	//味方リーダーの更新処理(アニメーション)
	if (m_pAllyLeader)
	{
		m_pAllyLeader->Update();
	}
	if (AnimationFlag)
	{
		if (m_pAllyLeader)
		{
			m_pAllyLeader->SetAnimationPlayFlag(false);
		}
	}
}

// Linieの描画
void CBattle::LinieDraw(void)
{
	if (m_pAllyLeader)
	{
		m_pAllyLeader->Draw();
	}
}

// 描画処理
void CBattle::Draw(void)
{
	std::lock_guard<std::mutex> lock(mtx);

	//Z軸順に描画
	for (float Z = 20.0f; Z > -30.0f; Z -= 1.0f)
	{
		//兵士の描画
		for (auto& fighter : m_pFighter)
		{
			//生成されているか
			if (!fighter)continue;
			//Z軸の範囲判定
			if (fighter->GetPos().z > Z - 1.0f && fighter->GetPos().z < Z + 1.0f)
			{
				//描画処理
				fighter->Draw();
			}
		}

		//味方リーダーが生成されているか
		if (m_pAllyLeader)
		{
			//Z軸の範囲判定
			if (m_pAllyLeader->GetPos().z > Z - 1.0f && m_pAllyLeader->GetPos().z < Z + 1.0f)
			{
				//描画処理
				m_pAllyLeader->Draw();
			}
		}
		//敵のリーダーが生成されているか
		if (m_pEnemyLeader)
		{
			//Z軸の範囲判定
			if (m_pEnemyLeader->GetPos().z > Z - 1.0f && m_pEnemyLeader->GetPos().z < Z + 1.0f)
			{
				//描画処理
				m_pEnemyLeader->Draw(m_nStageNum.StageMainNumber);
			}
		}

	}
	if (m_pAllyLeader)
	{
		m_pAllyLeader->HpDraw();
	}
	if (m_pEnemyLeader)
	{
		m_pEnemyLeader->HpDraw();
	}
	CreateEnemyLog();
	CreateAllyLog();
}

// 戦闘終了の取得
bool CBattle::GetEnd()
{
	return m_bEnd;
}

// テクスチャの再読み込み
void CBattle::ReLoadTexture(void)
{
	//兵士ポインタの解放
	for (int i = 0; i < m_pFighter.size(); i++)
	{
		if (m_pFighter[i])m_pFighter[i]->SetReLoadFlag();
	}

	//味方リーダーポインタの解放
	if(m_pAllyLeader)m_pAllyLeader->SetReLoadFlag();
	//敵リーダーポインタの解放
	if (m_pEnemyLeader)m_pEnemyLeader->SetReLoadFlag();
}

// 味方の情報保存処理
void CBattle::SaveAllyData(int InCornerCount,bool IsStella)
{
	EntityData tAllyData;

	//角数情報の格納
	tAllyData.nCornerCount = InCornerCount;
	
	//ステラ情報
	tAllyData.bStellaBuff = IsStella;

	//データリストに保存
	m_tAllyData.push_back(tAllyData);
}

// 戦闘時間軸処理
void CBattle::TimeLapse(void)
{
	if (m_nBattleTime == Time(m_nSpawnTime))
	{
		//敵の生成数を指定
		switch (m_nStageNum.StageMainNumber)
		{
		case (int)E_SELECT_STAGETYPE::GRASSLAND:
			switch (m_nStageNum.StageSubNumber)
			{
			case (int)E_SELECT_STAGENUMBER::GRASSLAND_STAGE1:
				m_nCreateEnemyNum = (int)ENEMY_SPAWNNUM::Stage1_1; 
				m_nSpawnTime += (int)NEXTSPAWN::Stage1_1; 
				break;
			case (int)E_SELECT_STAGENUMBER::GRASSLAND_STAGE2:
				m_nCreateEnemyNum = (int)ENEMY_SPAWNNUM::Stage1_2; 
				m_nSpawnTime += (int)NEXTSPAWN::Stage1_2;
				break;
			case (int)E_SELECT_STAGENUMBER::GRASSLAND_STAGE3:
				m_nCreateEnemyNum = (int)ENEMY_SPAWNNUM::Stage1_3;
				m_nSpawnTime += (int)NEXTSPAWN::Stage1_3;
				break;
			}
			break;
		case (int)E_SELECT_STAGETYPE::DESERT:
			switch (m_nStageNum.StageSubNumber)
			{
			case (int)E_SELECT_STAGENUMBER::DESERT_STAGE1:
				m_nCreateEnemyNum = (int)ENEMY_SPAWNNUM::Stage2_1;
				m_nSpawnTime += (int)NEXTSPAWN::Stage2_1;
				break;
			case (int)E_SELECT_STAGENUMBER::DESERT_STAGE2:
				m_nCreateEnemyNum = (int)ENEMY_SPAWNNUM::Stage2_2;
				m_nSpawnTime += (int)NEXTSPAWN::Stage2_2;
				break;
			case (int)E_SELECT_STAGENUMBER::DESERT_STAGE3:
				m_nCreateEnemyNum = (int)ENEMY_SPAWNNUM::Stage2_3;
				m_nSpawnTime += (int)NEXTSPAWN::Stage2_3;
				break;
			}
			break;
		case (int)E_SELECT_STAGETYPE::SNOWFIELD:
			switch (m_nStageNum.StageSubNumber)
			{
			case (int)E_SELECT_STAGENUMBER::SNOWFIELD_STAGE1:
				m_nCreateEnemyNum= (int)ENEMY_SPAWNNUM::Stage3_1;
				m_nSpawnTime += (int)NEXTSPAWN::Stage3_1;
				break;
			case (int)E_SELECT_STAGENUMBER::SNOWFIELD_STAGE2:
				m_nCreateEnemyNum= (int)ENEMY_SPAWNNUM::Stage3_2;
				m_nSpawnTime += (int)NEXTSPAWN::Stage3_2;
				break;
			case (int)E_SELECT_STAGENUMBER::SNOWFIELD_STAGE3:
				m_nCreateEnemyNum= (int)ENEMY_SPAWNNUM::Stage3_3;
				m_nSpawnTime += (int)NEXTSPAWN::Stage3_3;
				break;
			}
			break;
		}
	}
}

// 味方の生成処理
void CBattle::CreateAlly(void)
{
	if (m_pAllyLeader)
	{
		m_pAllyLeader->SetSummonFlag(true);
	}
	//保存済み数分処理する
	while (!m_tAllyData.empty())
	{
		//生成する
		m_pFighter.push_back(new CAlly(m_tAllyData[0].nCornerCount, m_tAllyData[0].bStellaBuff));
		
		//召喚総数を加算
		m_nSummonAllyCount++;
		//生成に使用したため情報を消して後ろの情報を前詰めにする
		m_tAllyData.erase(m_tAllyData.begin());
	}
}

// 敵の生成処理
void CBattle::CreateEnemy(void)
{
	// チュートリアル停止フラグが立っていたら処理しない
	if (!m_bTutorialStop)return;

	//生成予定数分処理する
	while (m_nCreateEnemyNum)
	{
		//ランダムピックした数字を格納する変数
		int nRandNum = rand() % 100;
		//敵の角数指定変数
		int EnemyCornerCount = 0;
		//歩行タイプの確率を設定
		int Probability = 0;

		switch (m_nStageNum.StageMainNumber)
		{
		case (int)E_SELECT_STAGETYPE::GRASSLAND:
			switch (m_nStageNum.StageSubNumber)
			{
			case (int)E_SELECT_STAGENUMBER::GRASSLAND_STAGE1:Probability = (int)ENEMY_PROBABILITY::Stage1_1; break;
			case (int)E_SELECT_STAGENUMBER::GRASSLAND_STAGE2:Probability = (int)ENEMY_PROBABILITY::Stage1_2; break;
			case (int)E_SELECT_STAGENUMBER::GRASSLAND_STAGE3:Probability = (int)ENEMY_PROBABILITY::Stage1_3; break;
			}
			break;
		case (int)E_SELECT_STAGETYPE::DESERT:
			switch (m_nStageNum.StageSubNumber)
			{
			case (int)E_SELECT_STAGENUMBER::DESERT_STAGE1:Probability = (int)ENEMY_PROBABILITY::Stage2_1; break;
			case (int)E_SELECT_STAGENUMBER::DESERT_STAGE2:Probability = (int)ENEMY_PROBABILITY::Stage2_2; break;
			case (int)E_SELECT_STAGENUMBER::DESERT_STAGE3:Probability = (int)ENEMY_PROBABILITY::Stage2_3; break;
			}
			break;
		case (int)E_SELECT_STAGETYPE::SNOWFIELD:
			switch (m_nStageNum.StageSubNumber)
			{
			case (int)E_SELECT_STAGENUMBER::SNOWFIELD_STAGE1:Probability = (int)ENEMY_PROBABILITY::Stage3_1; break;
			case (int)E_SELECT_STAGENUMBER::SNOWFIELD_STAGE2:Probability = (int)ENEMY_PROBABILITY::Stage3_2; break;
			case (int)E_SELECT_STAGENUMBER::SNOWFIELD_STAGE3:Probability = (int)ENEMY_PROBABILITY::Stage3_3; break;
			}
			break;
		}

		//0 ～ WALKENEMY_PROBABILITY まで
		if (nRandNum < Probability)EnemyCornerCount = 3;
		//上以降から99まで
		else EnemyCornerCount = 4;

		//生成する
		m_pFighter.push_back(new CEnemy(EnemyCornerCount));

		//生成予定数を減らす
		m_nCreateEnemyNum--;
	}
}

// 索敵処理
void CBattle::Search(CFighter* Searcher)
{
	//標的番号の初期化
	Searcher->m_nTargetNumber = -1;

	// 標的対象を取得
	auto targetList = GetFighterTypeList((int)Searcher->GetFighterType() ? FighterType::Enemy : FighterType::Ally);

	if (targetList.empty()) return;

	float minDistance = -1.0f;

	DirectX::XMFLOAT3 SelfPos = Searcher->GetPos();

	//相手を生成順に処理
	for (int l = 0; l < targetList.size(); l++)
	{
		//相手のステータスがBattleかどうか
		if (targetList[l]->GetStatus() == Status::Battle)
		{
			DirectX::XMFLOAT3 TargetPos = targetList[l]->GetPos();

			//索敵当たり判定内に敵がいるかどうか
			if (Searcher->SearchCollisionCheck(targetList[l]->GetSize(), TargetPos))
			{
				//標的番号がすでに保存済みかどうか
				if (Searcher->m_nTargetNumber != -1)
				{
					// 自身との距離を計算
					float distance = fabsf(Distance(SelfPos, TargetPos));

					// 最小距離と比較して小さかった場合
					if (minDistance > distance)
					{
						//標的番号を指定
						Searcher->m_nTargetNumber = l;

						// 最小距離を登録
						minDistance = distance;
					}
				}
				//保存していなかった場合は
				else
				{
					//標的番号を指定
					Searcher->m_nTargetNumber = l;

					// 最小距離を登録
					// 自身との距離を計算
					minDistance = fabsf(Distance(SelfPos, TargetPos));
				}
			}
		}
	}
}


// 移動処理
void CBattle::Move(CFighter* Mover)
{
	// チュートリアル停止フラグが立っていたら処理しない
	if (!m_bTutorialStop)return;

	//リーダーを標的にしているかどうか
	bool IsLeaderTargetFlag = true;

	Mover->SetMoveFlag(false);

	//移動フラグが立っていたら目的地指定の処理を飛ばす
	if (!Mover->GetMoveFlag())
	{
		//キャラクター同士が重なっていた場合の移動処理をしているかどうか
		if (OverlapMove(Mover))
		{
			//標的番号を設定済みだった場合
			if (Mover->m_nTargetNumber != -1)
			{
				IsLeaderTargetFlag = false;

				// 標的対象を取得
				auto targetList = GetFighterTypeList((int)Mover->GetFighterType() ? FighterType::Enemy : FighterType::Ally);

				if (targetList.empty()) return;


				Mover->SetTargetPos(targetList[Mover->m_nTargetNumber]->GetPos());

				//標的番号が現在生存数よりも数字が大きくないかどうか
				if (Mover->m_nTargetNumber < targetList.size())
				{
					DirectX::XMFLOAT3 SelfPos = Mover->GetPos();
					DirectX::XMFLOAT3 TargetPos = targetList[Mover->m_nTargetNumber]->GetPos();

					//敵の位置にMOVESPEEDの大きさで進む
					if (SelfPos.x > TargetPos.x - 10.0f)
					{
						Mover->AddPosX(MoveCalculation(SelfPos, TargetPos).x);
						Mover->AddPosZ(MoveCalculation(SelfPos, TargetPos).z);
					}
					else
					{
						if (Mover->GetPos().z != Mover->GetFirstPos().z)
						{
							Mover->AddPosX(MoveCalculation(SelfPos, { TargetPos.x,TargetPos.y, Mover->GetFirstPos().z }).x);
							Mover->AddPosZ(MoveCalculation(SelfPos, { TargetPos.x,TargetPos.y, Mover->GetFirstPos().z }).z);
						}
						else
						{
							Mover->AddPosX(MoveCalculation(SelfPos, TargetPos).x);
							Mover->AddPosZ(MoveCalculation(SelfPos, TargetPos).z);
						}
					}
				}
				//数字が大きかった場合
				else
				{
					IsLeaderTargetFlag = true;
				}
			}

			//標的番号を設定していなかった場合
			if(IsLeaderTargetFlag)
			{
				//自分のタイプを取得
				int SelfType = (int)Mover->GetFighterType();

				//リーダーが生成されているか
				CLeader* targetLeader = SelfType ? m_pEnemyLeader : m_pAllyLeader;

				//相手のリーダーが生成されているか
				if (targetLeader)
				{
					Mover->SetTargetPos(targetLeader->GetPos());

					DirectX::XMFLOAT3 SelfPos = Mover->GetPos();
					DirectX::XMFLOAT3 TargetPos = targetLeader->GetPos();

					//自分の位置が敵の一定のラインまで来たら
					if (SelfType ? Mover->GetPos().x < ALLYCORE_POSX + 10 : Mover->GetPos().x > ENEMYBOSSCORE_POSX - 10)
					{

						//標的がいないので相手のリーダーに向かってMOVESPEEDの大きさで進む
						Mover->AddPosX(MoveCalculation(SelfPos, TargetPos).x);
						Mover->AddPosX(MoveCalculation(SelfPos, TargetPos).z);
					}
					//そこより手前だったら
					else
					{
						if (Mover->m_bFirstBattlePosSetting)
						{
							if (Mover->GetPos().z != Mover->GetFirstPos().z)
							{
								Mover->AddPosX(MoveCalculation(SelfPos, { TargetPos.x,TargetPos.y, Mover->GetFirstPos().z }).x);
								Mover->AddPosZ(MoveCalculation(SelfPos, { TargetPos.x,TargetPos.y, Mover->GetFirstPos().z }).z);
							}
							else
							{
								Mover->AddPosX(MoveCalculation(SelfPos, TargetPos).x);
								Mover->AddPosZ(MoveCalculation(SelfPos, TargetPos).z);
							}
						}
						else
						{
							Mover->AddPosX(MOVESPEED(MOVEPOWER));
						}

					}
				}
			}
		}
	}
	else//目的地へ移動
	{
		DirectX::XMFLOAT3 SelfPos = Mover->GetPos();
		DirectX::XMFLOAT3 DestinationPos = Mover->GetDestinationPos();

		Mover->AddPosX(MoveCalculation(SelfPos, DestinationPos).x);
		Mover->AddPosZ(MoveCalculation(SelfPos, DestinationPos).z);
		//目的地の+-1.0fの範囲内に入ったら移動フラグをfalseにする
		if (DestinationPos.x + 1.0f < Mover->GetPos().x && DestinationPos.x - 1.0f > Mover->GetPos().x)
		{
			if (DestinationPos.y + 5.0f < Mover->GetPos().y && DestinationPos.y - 5.0f > Mover->GetPos().y)
			{
				if (DestinationPos.z + 5.0f < Mover->GetPos().z && DestinationPos.z - 5.0f > Mover->GetPos().z)
				{
					Mover->SetMoveFlag(false);
				}
			}
		}
	}
}

// 重なっていた場合の補正移動処理
bool CBattle::OverlapMove(CFighter* Mover)
{
	//移動先の座標保存変数
	DirectX::XMFLOAT3 m_tMovePos;
	float comparison;

	auto compatriotList = GetFighterTypeList(Mover->GetFighterType());

	//生成順に判定
	for (int l = 0; l < compatriotList.size(); l++)
	{
		//存在しているか確認
		if (!compatriotList[l])continue;
		//自分と同じ番号の場合は処理しない
		if (&compatriotList[l] == &Mover)continue;

		DirectX::XMFLOAT3 SelfPos = Mover->GetPos();
		DirectX::XMFLOAT3 AllyPos = compatriotList[l]->GetPos();
		DirectX::XMFLOAT3 AllySize = compatriotList[l]->GetSize();

		//自分の位置から圧倒的に離れているものは判定しない
		if (SelfPos.x + 1.0f < AllyPos.x || SelfPos.x - 1.0f > AllyPos.x)continue;
		if (SelfPos.z + 1.0f < AllyPos.z || SelfPos.z - 1.0f > AllyPos.z)continue;

		//中心位置から自分の位置が相手の位置より近いは処理しない
		float Z1 = SelfPos.z;
		float Z2 = AllyPos.z;

		if (Z1 < 0)Z1 *= -1.0f;
		if (Z2 < 0)Z2 *= -1.0f;

		if (Z1 >= Z2)continue;

		//重なっているか確認
		if (Mover->OverlapCheck(AllyPos, AllySize))
		{
			//目的地先の座標格納
			DirectX::XMFLOAT3 DestinationPos;
			DestinationPos.x = SelfPos.x + ((SelfPos.x - AllyPos.x));
			DestinationPos.y = SelfPos.y;
			DestinationPos.z = SelfPos.z + ((SelfPos.z - AllyPos.z));

			//補正移動先を目的地に移動
			Mover->SetDestinationPos(DestinationPos);
			//移動フラグを立てる
			Mover->SetMoveFlag(true);

			return false;
		}
	}
	//移動をしたかどうかを返す
	return true;
}

// 範囲内補正処理
void CBattle::ScopeMove()
{	
	// チュートリアル停止フラグが立っていたら処理しない
	if (!m_bTutorialStop)return;

	// 兵士
	for (auto fighter : m_pFighter)
	{
		//存在しているか確認
		if (!fighter)continue;
		float posz = fighter->GetPos().z;
		//奥行きが範囲外
		if (posz > BATTLE_Z)
		{
			fighter->SetPosZ(BATTLE_Z);
		}
		//手前が範囲外
		if (posz < -BATTLE_Z)
		{
			fighter->SetPosZ(-BATTLE_Z);
		}
	}
}

// 攻撃処理
template<class T>
void CBattle::Battle(CFighter* Attacker, T* Defender)
{
	if (typeid(T) != typeid(CFighter) && typeid(T) != typeid(CLeader))return;

	//味方のリーダーが生成されているか
	if (Defender)
	{
		//攻撃チャージがたまっているかどうか
		if (Attacker->GetAtkCharge() >= Attacker->GetCoolTime())
		{
			//相手の体力を減らす
			Defender->Damage(Attacker);
			//攻撃したのでチャージをリセットする
			Attacker->ChargeReset();
		}
		//攻撃チャージが溜まっていなかったら
		else
		{
			//チャージする
			Attacker->AddAtkCharge();
		}
	}
}

// 生存判定
void CBattle::Alive(void)
{
	// 兵士の判定
	for (auto fighter : m_pFighter)
	{
		//存在しているか確認
		if (!fighter)continue;
		//敵のステータスがBattleかどうか
		if (fighter->GetStatus() == Status::Battle)
		{
			//敵の体力が残っているかどうか
			if (fighter->GetHp() <= 0.0f)
			{
				switch (fighter->GetFighterType())
				{
				case FighterType::Ally:
					fighter->m_bIsDeath = true;
					break;
				}

				//ステータスを死亡状態にする
				fighter->SetStatus(Status::Death);
			}
		}
	}
	
	//敵のリーダーの判定
	if (m_pEnemyLeader)
	{
		//敵のステータスがBattleかどうか
		if (m_pEnemyLeader->GetStatus() == Status::Battle)
		{
			//敵の体力が残っているかどうか
			if (m_pEnemyLeader->GetHp() <= 0.0f || WinEndCommand())
			{
				//ステータスを死亡状態にする
				m_pEnemyLeader->SetStatus(Status::Death);
			}
		}
	}
	//味方のリーダーの判定
	if (m_pAllyLeader)
	{
		//味方のリーダーのステータスがBattleかどうか
		if (m_pAllyLeader->GetStatus() == Status::Battle)
		{
			//味方のリーダーの体力が残っているかどうか
			if (m_pAllyLeader->GetHp() <= 0.0f || LoseEndCommand())
			{
				//ステータスを死亡状態にする
				m_pAllyLeader->SetStatus(Status::Death);
			}
		}
	}
}

// 削除処理
void CBattle::Delete(void)
{	
	// 兵士の生存判定
	for (int i = 0; i < m_pFighter.size(); i++)
	{
		//存在しているか確認
		if (!m_pFighter[i])continue;
		//ステータスがDeleteかどうか
		if (m_pFighter[i]->GetStatus() == Status::Delete)
		{
			//解放処理
			delete m_pFighter[i];
			m_pFighter[i] = nullptr;
			m_pFighter.erase(m_pFighter.begin() + i);
			//前詰めされたのでもう一度同じ場所を処理する
			i--;
		}
	}

	//敵ボスの生存判定
	if (m_pEnemyLeader)
	{
		//ステータスがDeleteかどうか
		if (m_pEnemyLeader->GetStatus() == Status::Delete)
		{
			//解放処理
			m_fRinieMaxHp = m_pAllyLeader->GetMaxHp();
			m_fRinieLastHp = m_pAllyLeader->GetHp();

			delete m_pEnemyLeader;
			m_pEnemyLeader = nullptr;
		}
	}
	//プレイヤーの生存判定
	if (m_pAllyLeader)
	{
		//ステータスがDeleteかどうか
		if (m_pAllyLeader->GetStatus() == Status::Delete)
		{
			//解放処理

			//リザルトに渡すために情報を仮保存
			m_fRinieMaxHp = m_pAllyLeader->GetMaxHp();
			m_fRinieLastHp = m_pAllyLeader->GetHp();

			delete m_pAllyLeader;
			m_pAllyLeader = nullptr;
		}
	}
}

// 初期位置セッティング
void CBattle::FirstPosSetting()
{
	//X座標補正用変数
	int PosX = 0;

	auto AllyList = GetFighterTypeList(FighterType::Ally);

	//味方を生成順に処理
	for (int i = 0; i < AllyList.size(); i++)
	{
		//生成されていなかったら処理をしない
		if (!AllyList[i])continue;
		//初期位置設定をすでにしているかどうかを確認
		if (!AllyList[i]->m_bFirstBattlePosSetting)
		{
			//初期X位置を設定
			AllyList[i]->SetPosX(ALLYCREATE_POSX - PosX);
			//初期Y位置を設定
			AllyList[i]->SetPosY(0.0f + AllyList[i]->GetSize().y / 2);
			//Z座標をパターン別に設定
			switch (m_nFirstPosPattern)
			{
			case 0:
				//初期Z位置を設定
				AllyList[i]->SetPosZ(ALLYCREATE_POSZ_1);
				//初期位置設定を設定済みにする
				AllyList[i]->m_bFirstBattlePosSetting = true;
				//パターンを一つずらす
				m_nFirstPosPattern = 1;
				break;
			case 1:
				//初期Z位置を設定
				AllyList[i]->SetPosZ(ALLYCREATE_POSZ_2);
				//初期位置設定を設定済みにする
				AllyList[i]->m_bFirstBattlePosSetting = true;
				//パターンを一つずらす
				m_nFirstPosPattern = 2;
				break;
			case 2:
				//初期Z位置を設定
				AllyList[i]->SetPosZ(ALLYCREATE_POSZ_3);
				//初期位置設定を設定済みにする
				AllyList[i]->m_bFirstBattlePosSetting = true;
				//パターンを一つずらす
				m_nFirstPosPattern = 3;
				break;
			case 3:
				//初期Z位置を設定
				AllyList[i]->SetPosZ(ALLYCREATE_POSZ_4);
				//初期位置設定を設定済みにする
				AllyList[i]->m_bFirstBattlePosSetting = true;
				//パターンを一つずらす
				m_nFirstPosPattern = 4;
				break;
			case 4:
				//初期Z位置を設定
				AllyList[i]->SetPosZ(ALLYCREATE_POSZ_5);
				//初期位置設定を設定済みにする
				AllyList[i]->m_bFirstBattlePosSetting = true;
				//パターンを最初に戻す
				m_nFirstPosPattern = 0;
				//X座標補正用変数を加算する
				PosX += 5.0f;
				break;
			}

			AllyList[i]->SetFirstPos(AllyList[i]->GetPos());
		}
	}

	//X座標補正用変数を初期化
	PosX = 0;

	// 敵のリストを取得
	auto EnemyList = GetFighterTypeList(FighterType::Enemy);

	//敵を生成順に処理
	for (int i = 0; i < EnemyList.size(); i++)
	{
		//生成していなかったら処理しない
		if (!EnemyList[i])continue;
		//初期位置設定をすでにしているかどうかを確認
		if (!EnemyList[i]->m_bFirstBattlePosSetting)
		{
			//初期X位置を設定
			switch (m_nStageNum.StageMainNumber)
			{
			case 0:
				EnemyList[i]->SetPosX((float)ENEMYCREATE_POSX::Stage1 + PosX);
				break;
			case 1:
				EnemyList[i]->SetPosX((float)ENEMYCREATE_POSX::Stage2 + PosX);
				break;
			case 2:
				EnemyList[i]->SetPosX((float)ENEMYCREATE_POSX::Stage3 + PosX);
				break;
			}

			if (EnemyList[i]->GetCornerCount() == 3)
			{
				EnemyList[i]->SetPosY(0.0f + EnemyList[i]->GetSize().y / 2);
			}
			else if (EnemyList[i]->GetCornerCount() == 4)
			{
				EnemyList[i]->SetPosY(0.0f + EnemyList[i]->GetSize().y * 1.3);
			}

			//Z座標をパターン別に設定
			switch (m_nFirstPosPattern)
			{
			case 0:
				//初期Z位置を設定
				EnemyList[i]->SetPosZ(ENEMYCREATE_POSZ_1);
				//初期位置設定を設定済みにする
				EnemyList[i]->m_bFirstBattlePosSetting = true;
				//パターンを一つずらす
				m_nFirstPosPattern = 1;
				break;
			case 1:
				//初期Z位置を設定
				EnemyList[i]->SetPosZ(ENEMYCREATE_POSZ_2);
				//初期位置設定を設定済みにする
				EnemyList[i]->m_bFirstBattlePosSetting = true;
				//パターンを一つずらす
				m_nFirstPosPattern = 2;
				break;
			case 2:
				//初期Z位置を設定
				EnemyList[i]->SetPosZ(ENEMYCREATE_POSZ_3);
				//初期位置設定を設定済みにする
				EnemyList[i]->m_bFirstBattlePosSetting = true;
				//パターンを一つずらす
				m_nFirstPosPattern = 3;
				break;
			case 3:
				//初期Z位置を設定
				EnemyList[i]->SetPosZ(ENEMYCREATE_POSZ_4);
				//初期位置設定を設定済みにする
				EnemyList[i]->m_bFirstBattlePosSetting = true;
				//パターンを一つずらす
				m_nFirstPosPattern = 4;
				break;
			case 4:
				//初期Z位置を設定
				EnemyList[i]->SetPosZ(ENEMYCREATE_POSZ_5);
				//初期位置設定を設定済みにする
				EnemyList[i]->m_bFirstBattlePosSetting = true;
				//パターンを最初に戻す
				m_nFirstPosPattern = 0;
				//X座標補正用変数を加算する
				PosX += 5.0f;
				break;
			}
			EnemyList[i]->SetFirstPos(EnemyList[i]->GetPos());
		}
	}
}

// タイプ別のリストを取得
std::vector<CFighter*> CBattle::GetFighterTypeList(FighterType Type)
{
	std::vector<CFighter*> TypeList;	//タイプ別リスト

	for (auto& fighter : m_pFighter)
	{
		//生成されていなかったら処理をしない
		if (!fighter)continue;
		//タイプが一致していたら
		if (fighter->GetFighterType() == Type)
		{
			//リストに追加
			TypeList.push_back(fighter);
		}
	}

	return TypeList;
	
}

// リーダーの生成処理
void CBattle::CreateLeader(void)
{
	//味方のリーダーがnullptrだったら
	if (m_pAllyLeader == nullptr)
	{
		//初期位置設定用変数
		DirectX::XMFLOAT3 InFirstPos;

		//X座標を設定
		InFirstPos.x = ALLYCORE_POSX;
		//Y座標を設定
		InFirstPos.y = 8.0f;
		//Z座標を設定
		InFirstPos.z = ALLYCORE_POSZ;
		//味方のリーダーを生成
		m_pAllyLeader = new CLeader(LeaderSize, InFirstPos, 0);
	}
	//敵のリーダーがnullptrだったら
	if (m_pEnemyLeader == nullptr)
	{
		//初期位置設定用変数
		DirectX::XMFLOAT3 BossFirstPos;
		switch (m_nStageNum.StageMainNumber)
		{
		case 0:
			//Y座標を設定
			BossFirstPos.y = 15.0f;
			break;
		case 1:
			//Y座標を設定
			BossFirstPos.y = 16.0f;
			break;
		case 2:
			//Y座標を設定
			BossFirstPos.y = 16.0f;
			break;
		}

		//X座標を設定
		BossFirstPos.x = ENEMYBOSSCORE_POSX;
		//Z座標を設定
		BossFirstPos.z = ENEMYBOSSCORE_POSZ;

		//敵のリーダーを生成
		switch (m_nStageNum.StageMainNumber)
		{
		case 0://草原(クラッカー)
			m_pEnemyLeader = new CLeader(LeaderSize, BossFirstPos, 1, false);
			break;
		case 1://砂漠(ヌガー)
			m_pEnemyLeader = new CLeader(LeaderSize, BossFirstPos, 1, false);
			break;
		case 2://雪原(カヌレ＆ボルドー)
			m_pEnemyLeader = new CLeader(LeaderSize, BossFirstPos, 1, true);
			break;
		}
	}
}

/*＝＝＝＝＝バトル用＝＝＝＝＝*/

// 味方を角数別にカウントしたものの描画処理
void CBattle::CreateAllyLogDraw(void)
{
	float fPosX[3] = {-1800.0f,-1700.0f,-1600.0f};
	float fPosY = 0.0f;

	//生存数のカウント
	CreateAllyLog();

	//桁数別に数字テクスチャを指定を種類分行う
	for (int i = 0; i < 2; i++)
	{
		int nDigits = 0;	//桁数
		int nNumbers[3];

		//一桁
		if (m_nAllyTypes[i] < 10)
		{
			//桁数
			nDigits = 1;
			//一の桁のテクスチャ番号を指定
			nNumbers[0] = m_nAllyTypes[i];
		}
		//二桁
		if (m_nAllyTypes[i] > 9 && m_nAllyTypes[i] < 100)
		{
			//桁数
			nDigits = 2;
			//一の桁のテクスチャ番号を指定
			nNumbers[1] = m_nAllyTypes[i] % 10;
			//十の桁のテクスチャ番号を指定
			nNumbers[0] = (m_nAllyTypes[i] / 10) % 10;
		}
		//三桁
		if (m_nAllyTypes[i] > 99 && m_nAllyTypes[i] < 1000)
		{
			//桁数
			nDigits = 3;
			//一の桁のテクスチャ番号を指定
			nNumbers[2] = m_nAllyTypes[i] % 10;
			//十の桁のテクスチャ番号を指定
			nNumbers[1] = (m_nAllyTypes[i] / 10) % 10;
			//百の桁のテクスチャ番号を指定
			nNumbers[0] = m_nAllyTypes[i] / 100;
		}
		//表示するY軸の高さ指定
		switch (i)
		{
		case 0:
				fPosY =  050;
			break;
		case 1:
				fPosY = -100;
			break;
		}

		for (int l = 0; l < nDigits; l++)
		{

			//スプライトの座標の設定
			SetSpritePos(WIDE_RATIO(fPosX[l],GetNowWide()), HEIGHT_RATIO(fPosY,GetNowHeight()));

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
// 敵を種類別にカウントしたものの描画処理
void CBattle::CreateEnemyLogDraw(void)
{
	float fPosX[3] = {1600.0f,1700.0f,1800.0f};
	float fPosY = 0.0f;

	//生存数のカウント
	CreateEnemyLog();

	//桁数別に数字テクスチャを指定を種類分行う
	for (int i = 0; i < 2; i++)
	{
		//桁数
		int nDigits = 0;
		int nNumbers[3];

		if (m_nEnemyTypes[i] < 10)
		{
			//桁数
			nDigits = 1;
			//一の桁のテクスチャ番号を指定
			nNumbers[0] = m_nEnemyTypes[i];
		}
		if (m_nEnemyTypes[i] > 9 && m_nEnemyTypes[i] < 100)
		{
			//桁数
			nDigits = 2;
			//一の桁のテクスチャ番号を指定
			nNumbers[1] = m_nEnemyTypes[i] % 10;
			//十の桁のテクスチャ番号を指定
			nNumbers[0] = (m_nEnemyTypes[i] / 10) % 10;
		}
		if (m_nEnemyTypes[i] > 99 && m_nEnemyTypes[i] < 1000)
		{
			//桁数
			nDigits = 3;
			//一の桁のテクスチャ番号を指定
			nNumbers[2] = m_nEnemyTypes[i] % 10;
			//十の桁のテクスチャ番号を指定
			nNumbers[1] = (m_nEnemyTypes[i] / 10) % 10;
			//百の桁のテクスチャ番号を指定
			nNumbers[0] = m_nEnemyTypes[i] / 100;
		}
		//表示するY軸の高さ指定
		switch (i)
		{
		case 0:
			fPosY = 050;
			break;
		case 1:
			fPosY = -100;
			break;
		}

		for (int l = 0; l < nDigits; l++)
		{
			//スプライトの座標の設定
			SetSpritePos(WIDE_RATIO(fPosX[l], GetNowWide()), HEIGHT_RATIO(fPosY, GetNowHeight()));

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

// 味方を角数別にカウント
void CBattle::CreateAllyLog(void)
{
	auto AllyList = GetFighterTypeList(FighterType::Ally);

	//三角形のカウント格納用変数の初期化
	m_nAllyTypes[0] = 0;
	//四角形のカウント格納用変数の初期化
	m_nAllyTypes[1] = 0;
	//味方を生成順に処理
	for (int i = 0; i < AllyList.size(); i++)
	{
		//角数別に処理
		switch (AllyList[i]->GetCornerCount())
		{
		case 3:
			m_nAllyTypes[0]++;
			break;
		case 4:
			m_nAllyTypes[1]++;
			break;
		}
	}
}
// 敵を種類別にカウント
void CBattle::CreateEnemyLog(void)
{
	auto EnemyList = GetFighterTypeList(FighterType::Enemy);

	//三角形のカウント格納用変数の初期化
	m_nEnemyTypes[0] = 0;
	//四角形のカウント格納用変数の初期化
	m_nEnemyTypes[1] = 0;
	//敵を生成順に処理
	for (int i = 0; i < EnemyList.size(); i++)
	{
		//角数別に処理
		switch (EnemyList[i]->GetCornerCount())
		{
		case 3:
			m_nEnemyTypes[0]++;
			break;
		case 4:
			m_nEnemyTypes[1]++;
			break;
		}
	}
}

/*＝＝＝＝＝図形判定用＝＝＝＝＝*/

void CBattle::SaveAllyLog(void)
{
	m_nAllyTypes[0] = 0;
	m_nAllyTypes[1] = 0;
	for (int i = 0; i < m_tAllyData.size(); i++)
	{
		switch (m_tAllyData[i].nCornerCount)
		{
		case 3:
			m_nAllyTypes[0]++;
			break;
		case 4:
			m_nAllyTypes[1]++;
			break;
		}
	}
}

void CBattle::SaveAllyLogDraw(void)
{
	float fPosX[3] = { 1600.0f,1700.0f,1800.0f };
	float fPosY = 0.0f;

	SaveAllyLog();

	for (int i = 0; i < 2; i++)
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
			fPosY = -900;
			break;
		case 1:
			fPosY = -700;
			break;
		}

		for (int l = 0; l < nDigits; l++)
		{
			SetRender2D();

			//スプライトの座標の設定
			SetSpritePos(WIDE_RATIO(fPosX[l], GetNowWide()), HEIGHT_RATIO(fPosY, GetNowHeight()));

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

/*＝＝＝＝＝デバック用コマンド＝＝＝＝＝*/

bool CBattle::WinEndCommand(void)
{
	if (IsKeyTrigger('W')) {
		m_bWinCommand[0] = true;
		m_fWinCommandResetTimer = 0.0f;
	}
	if (IsKeyTrigger('A') && m_bWinCommand[0]) {
		m_bWinCommand[1] = true;
		m_fWinCommandResetTimer = 0.0f;
	}
	if (IsKeyTrigger('T') && m_bWinCommand[1]) {
		m_bWinCommand[2] = true;
		m_fWinCommandResetTimer = 0.0f;
	}
	if (IsKeyTrigger('A') && m_bWinCommand[2]) {
		m_bWinCommand[3] = true;
		m_fWinCommandResetTimer = 0.0f;
	}
	if (IsKeyTrigger('S') && m_bWinCommand[3]) {
		m_bWinCommand[4] = true;
		m_fWinCommandResetTimer = 0.0f;
	}
	if (IsKeyTrigger('I') && m_bWinCommand[4]) {
		m_bWinCommand[5] = true;
		m_fWinCommandResetTimer = 0.0f;
	}
	if (IsKeyTrigger('N') && m_bWinCommand[5]) {
		m_bWinCommand[6] = true;
		m_fWinCommandResetTimer = 0.0f;
	}
	if (IsKeyTrigger('O') && m_bWinCommand[6]) {
		m_bWinCommand[7] = true;
		m_fWinCommandResetTimer = 0.0f;
	}
	if (IsKeyTrigger('K') && m_bWinCommand[7]) {
		m_bWinCommand[8] = true;
		m_fWinCommandResetTimer = 0.0f;
	}
	if (IsKeyTrigger('A') && m_bWinCommand[8]) {
		m_bWinCommand[9] = true;
		m_fWinCommandResetTimer = 0.0f;
	}
	if (IsKeyTrigger('T') && m_bWinCommand[9]) {
		m_bWinCommand[10] = true;
		m_fWinCommandResetTimer = 0.0f;
	}
	if (IsKeyTrigger('I') && m_bWinCommand[10]) {
		m_bWinCommand[11] = true;
	}

	m_fWinCommandResetTimer += 1.0f / 60.0f;

	if (m_fWinCommandResetTimer > 4.0f)
	{
		for (int i = 0; i < 12; i++)m_bWinCommand[i] = false;
		m_fWinCommandResetTimer = 0.0f;
	}

	return m_bWinCommand[11];
}

bool CBattle::LoseEndCommand(void)
{
	if (IsKeyTrigger('W')) {
		m_bLoseCommand[0] = true;
		m_fLoseCommandResetTimer = 0.0f;
	}
	if (IsKeyTrigger('A') && m_bLoseCommand[0]) {
		m_bLoseCommand[1] = true;
		m_fLoseCommandResetTimer = 0.0f;
	}
	if (IsKeyTrigger('T') && m_bLoseCommand[1]) {
		m_bLoseCommand[2] = true;
		m_fLoseCommandResetTimer = 0.0f;
	}
	if (IsKeyTrigger('A') && m_bLoseCommand[2]) {
		m_bLoseCommand[3] = true;
		m_fLoseCommandResetTimer = 0.0f;
	}
	if (IsKeyTrigger('S') && m_bLoseCommand[3]) {
		m_bLoseCommand[4] = true;
		m_fLoseCommandResetTimer = 0.0f;
	}
	if (IsKeyTrigger('I') && m_bLoseCommand[4]) {
		m_bLoseCommand[5] = true;
		m_fLoseCommandResetTimer = 0.0f;
	}
	if (IsKeyTrigger('N') && m_bLoseCommand[5]) {
		m_bLoseCommand[6] = true;
		m_fLoseCommandResetTimer = 0.0f;
	}
	if (IsKeyTrigger('O') && m_bLoseCommand[6]) {
		m_bLoseCommand[7] = true;
		m_fLoseCommandResetTimer = 0.0f;
	}
	if (IsKeyTrigger('M') && m_bLoseCommand[7]) {
		m_bLoseCommand[8] = true;
		m_fLoseCommandResetTimer = 0.0f;
	}
	if (IsKeyTrigger('A') && m_bLoseCommand[8]) {
		m_bLoseCommand[9] = true;
		m_fLoseCommandResetTimer = 0.0f;
	}
	if (IsKeyTrigger('K') && m_bLoseCommand[9]) {
		m_bLoseCommand[10] = true;
		m_fLoseCommandResetTimer = 0.0f;
	}
	if (IsKeyTrigger('E') && m_bLoseCommand[10]) {
		m_bLoseCommand[11] = true;
	}

	m_fLoseCommandResetTimer += 1.0f / 60.0f;

	if (m_fLoseCommandResetTimer > 4.0f)
	{
		for (int i = 0; i < 12; i++)m_bLoseCommand[i] = false;
		m_fLoseCommandResetTimer = 0.0f;
	}

	return m_bLoseCommand[11];
}

/*＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝*/

//音の再読み込み
void CBattle::ReloadSound()
{
	if (m_pSound)
	{
		SAFE_DELETE(m_pSound);
		m_pDeathSE = nullptr;
	}
	//音の初期化
	m_pSound = new CSoundList(SE_DEATH);
	m_pSound->SetMasterVolume();
	m_pDeathSE = m_pSound->GetSound(false);
}

//移動方向計算関数
MovePower MoveCalculation(DirectX::XMFLOAT3 nPos, DirectX::XMFLOAT3 nEnemyPos)
{
	//移動力格納変数
	MovePower Movepower;
	//X軸移動力格納変数の初期化
	float X = 0.0f;
	//Z軸移動力格納変数の初期化
	float Z = 0.0f;
	//計算結果の格納変数の初期化
	float ShortDis = 0.0f;

	//X軸の設定
	X = (nEnemyPos.x - nPos.x);
	//Z軸の設定
	Z = (nEnemyPos.z - nPos.z);

	ShortDis = sqrtf(powf(X, 2) + powf(Z, 2));

	/*正規化*/
	float NorX = X / (ShortDis);
	float NorZ = Z / (ShortDis);

	/*絶対値*/
	float absX = fabs(NorX);
	float absZ = fabs(NorZ);

	float absXZ = absX + absZ;

	Movepower.x = (MOVESPEED(MOVEPOWER) * NorX) / absXZ;
	Movepower.z = (MOVESPEED(MOVEPOWER) * NorZ) / absXZ;

	return Movepower;
}

// 距離
float Distance(DirectX::XMFLOAT3 t1, DirectX::XMFLOAT3 t2)
{
	return sqrtf((t1.x - t2.x) * (t1.x - t2.x) + (t1.y - t2.y) * (t1.y - t2.y) + (t1.z - t2.z) * (t1.z - t2.z));
}
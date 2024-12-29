#include "Battle.h"
#include "SceneGame.h"
#include "Main.h"
#include "DirectXTex/TextureLoad.h"
#include "Controller.h"
#include"SpriteDrawer.h"
#include "Input.h"

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

//敵の生成位置
#define ENEMYCREATE_POSX (80)
//敵の生成Z座標位置
#define ENEMYCREATE_POSZ_1 (20)
#define ENEMYCREATE_POSZ_2 (10)
#define ENEMYCREATE_POSZ_3 (0)
#define ENEMYCREATE_POSZ_4 (-10)
#define ENEMYCREATE_POSZ_5 (-20)

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


//数字のテクスチャ保存用ポインタ
ID3D11ShaderResourceView* m_pLogTex[10];

//移動力構造体
struct MovePower
{
	float x;
	float z;
};

//移動方向計算関数
MovePower MoveCalculation(DirectX::XMFLOAT3 nPos, DirectX::XMFLOAT3 nEnemyPos);

//コンストラクタ
CBattle::CBattle()
	: m_pAlly{}
	, m_nAllyCount(0)
	, m_nAllyDateCount(0)
	, m_nAllyTypes{ 0,0 }
	, m_pEnemy{}
	, m_nEnemyCount(0)
	, m_nEnemyDateCount{ 0,0,0,0,0 }
	, m_nSelectPattern(0)
	, m_nMaxPattern(0)
	, m_nCreateEnemyNum(0)
	, m_pAllyLeader(nullptr)
	, m_pEnemyLeader(nullptr)
	, m_nEnemyTypes{ 0,0 }
	, m_nBattleTime(0)
	, m_bFirstFight(false)
	, m_nFirstPosPattern(0)
	, m_nStageNum{ 0,0 }
{
	//数字テクスチャの読み込み
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
		{{-100, -100, 0.0f}, {0.0f, 0.0f}},
		{{-100,  100, 0.0f}, {0.0f, 1.0f}},
		{{ 100, -100, 0.0f}, {1.0f, 0.0f}},
		{{ 100,  100, 0.0f}, {1.0f, 1.0f}},
	};
	m_pLogVtx = CreateVertexBuffer(vtx, 4);

	//味方データの初期化
	for (int i = 0; i < MAX_ALLY; i++)
	{
		m_tAllyData[i].nCornerCount = -1;
	}
	//敵データの初期化
	for (int l = 0; l < MAX_PATTERN; l++)
	{
		for (int i = 0; i < MAX_ENEMY; i++)
		{
			m_tEnemyData[l][i].nCornerCount = -1;
		}
	}
}

//デストラクタ
CBattle::~CBattle()
{
	//数字テクスチャの解放
	if (m_pLogVtx)m_pLogVtx->Release();
	for (int i = 0; i < 10; i++)
	{
		if (m_pLogTex[i])m_pLogTex[i]->Release();
	}
	//味方ポインタの解放
	for (int i = 0; i < MAX_ALLY; i++)
	{
		if (!m_pAlly[i])continue;
		delete m_pAlly[i];
		m_pAlly[i] = nullptr;
	}
	//敵ポインタの解放
	for (int l = 0; l < MAX_ENEMY; l++)
	{
		if (!m_tEnemyData[l])continue;
		delete m_pEnemy[l];
		m_pEnemy[l] = nullptr;

	}
}

void CBattle::Update(void)
{
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
		//味方を生成順に処理
		for (int i = 0; i < m_nAllyCount; i++)
		{
			//Z軸順に描画
			if (m_pAlly[i]->GetPos().z > Z - 1.0f && m_pAlly[i]->GetPos().z < Z + 1.0f)
			{
				//自分のステータスがBattleだったら
				if (m_pAlly[i]->GetStatus() == St_Battle)
				{
					//索敵処理
					Search(i, Ally);
					//攻撃しているかの判断を初期化
					m_pAlly[i]->m_bIsAttack = false;

					//攻撃をしていなかったら
					if (!m_pAlly[i]->m_bIsAttack)
					{
						//相手を生成順に処理
						for (int l = 0; l < m_nEnemyCount; l++)
						{
							//相手のステータスがBattleだったら
							if (m_pEnemy[l]->GetStatus() == St_Battle)
							{
								//攻撃当たり判定に入ってるかどうか
								if (m_pAlly[i]->AtkCollisionCheck(m_pEnemy[l]->GetSize(), m_pEnemy[l]->GetPos()))
								{
									//攻撃しているかの判断をtrueにする
									m_pAlly[i]->m_bIsAttack = true;
									//攻撃処理
									Battle(i, l, Enemy);
									break;
								}
							}
						}
					}
					//攻撃をしていなかったら
					if (!m_pAlly[i]->m_bIsAttack)
					{
						//相手のリーダーが生成されているか
						if (m_pEnemyLeader)
						{
							//相手のリーダーのステータスがBattleだったら
							if (m_pEnemyLeader->GetStatus() == St_Battle)
							{
								//攻撃の当たり判定に入っているか
								if (m_pAlly[i]->AtkCollisionCheck(m_pEnemyLeader->GetSize(), m_pEnemyLeader->GetPos()))
								{
									//攻撃しているかの判断をtrueにする
									m_pAlly[i]->m_bIsAttack = true;
									//攻撃処理
									Battle(i, -1, EnemyLeader);
								}
							}
						}
					}
						//移動処理
						Move(i, Ally);
				}
			}
		}
		//敵を生成順に処理
		for (int i = 0; i < m_nEnemyCount; i++)
		{
			//Z軸順に処理
			if (m_pEnemy[i]->GetPos().z > Z - 1.0f && m_pEnemy[i]->GetPos().z < Z + 1.0f)
			{
				//自分のステータスがバトルだったら
				if (m_pEnemy[i]->GetStatus() == St_Battle)
				{
					//索敵処理
					Search(i, Enemy);
					//攻撃しているかの判断を初期化
					m_pEnemy[i]->m_bIsAttack = false;
					//攻撃をしていなかったら
					if (!m_pEnemy[i]->m_bIsAttack)
					{
						//相手を生成順に処理
						for (int l = 0; l < m_nAllyCount; l++)
						{
							//相手のステータスがバトルだったら
							if (m_pAlly[l]->GetStatus() == St_Battle)
							{
								//攻撃当たり判定に入ってるかどうか
								if (m_pEnemy[i]->AtkCollisionCheck(m_pAlly[l]->GetSize(), m_pAlly[l]->GetPos()))
								{
									//攻撃しているかの判断をtrueにする
									m_pEnemy[i]->m_bIsAttack = true;
									//攻撃処理
									Battle(i, l, Ally);

									break;
								}
							}
						}
					}
					//攻撃をしていなかったら
					if (!m_pEnemy[i]->m_bIsAttack)
					{
						//相手のリーダーが生成されているか
						if (m_pAllyLeader)
						{
							//相手のリーダーのステータスがBattleだったら
							if (m_pAllyLeader->GetStatus() == St_Battle)
							{
								//攻撃当たり判定に入ってるかどうか
								if (m_pEnemy[i]->AtkCollisionCheck(m_pAllyLeader->GetSize(), m_pAllyLeader->GetPos()))
								{
									//攻撃しているかの判定をtrueにする
									m_pEnemy[i]->m_bIsAttack = true;
									//攻撃処理
									Battle(i, -1, AllyLeader);
								}
							}
						}
					}

				}
					//移動処理
					Move(i, Enemy);
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
	m_nBattleTime++;

	/*勝敗判定*/
	//シーンがゲームだったら
	if (GetScene() == SCENE_GAME)
	{
		//敵のリーダーがnullptrになっていたら
		if (m_pEnemyLeader == nullptr)
		{
			MessageBox(NULL, "ボスを倒したためステージクリア！！", "勝敗", MB_OK);
			//タイトルシーンへ移行
			ChangeScene(SCENE_TITLE);
		}
	}
	//シーンがゲームだったら
	if (GetScene() == SCENE_GAME)
	{
		//敵のリーダーがnullptrになっていたら
		if (m_pAllyLeader == nullptr)
		{
			MessageBox(NULL, "プレイヤーが倒されたため敗北", "勝敗", MB_OK);
			//タイトルシーンへ移行
			ChangeScene(SCENE_TITLE);
		}
	}
}

//キャラクターの更新処理
void CBattle::CharacterUpdate(void)
{
	//味方の更新処理(アニメーション)
	for (int i = 0; i < m_nAllyCount; i++)
	{
		if (!m_pAlly[i])continue;
		m_pAlly[i]->Update();
	}

	//敵の更新処理(アニメーション)
	for (int i = 0; i < m_nEnemyCount; i++)
	{
		if (!m_pEnemy[i])continue;
		m_pEnemy[i]->Update();
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
}

//味方キャラクターの描画処理
void CBattle::CharacterDraw(void)
{
	//味方の描画
	for (int i = 0; i < m_nAllyCount; i++)
	{
		m_pAlly[i]->Draw();
	}
}

//描画処理
void CBattle::Draw(void)
{
	//Z軸順に描画
	for (float Z = 20.0f; Z > -30.0f; Z -= 1.0f)
	{

		//味方の描画
		for (int i = 0; i < m_nAllyCount; i++)
		{
			//生成されているか
			if (!m_pAlly[i])continue;
			//Z軸の範囲判定
			if (m_pAlly[i]->GetPos().z > Z - 1.0f && m_pAlly[i]->GetPos().z < Z + 1.0f)
			{
				//描画処理
				m_pAlly[i]->Draw();
			}
		}

		//敵の描画
		for (int i = 0; i < m_nEnemyCount; i++)
		{
			//生成されているか
			if (!m_pEnemy[i])continue;
			//Z軸の範囲判定
			if (m_pEnemy[i]->GetPos().z > Z - 1.0f && m_pEnemy[i]->GetPos().z < Z + 1.0f)
			{
				//描画処理
				m_pEnemy[i]->Draw();
			}
		}

		//敵のリーダーが生成されているか
		if (m_pEnemyLeader)
		{
			//Z軸の範囲判定
			if (m_pEnemyLeader->GetPos().z > Z - 1.0f && m_pEnemyLeader->GetPos().z < Z + 1.0f)
			{
				//描画処理
				m_pEnemyLeader->Draw();
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
	}
	if (m_pAllyLeader)
	{
		m_pAllyLeader->HpDraw();
	}
	if (m_pEnemyLeader)
	{
		m_pEnemyLeader->HpDraw();
	}
	CreateEnemyLogDraw();
	CreateAllyLogDraw();
}

//味方の情報保存処理
void CBattle::SaveAllyData(int InCornerCount)
{
	//角数情報の格納
	m_tAllyData[m_nAllyDateCount].nCornerCount = InCornerCount;
	
	//保存数を加算
	m_nAllyDateCount++;
}
//敵の情報保存処理
void CBattle::SaveEnemyData(int InCornerCount,int InPattern)
{
	//角数情報の保存
	m_tEnemyData[InPattern][m_nEnemyDateCount[InPattern]].nCornerCount = InCornerCount;
	//保存数を加算
	m_nEnemyDateCount[InPattern]++;
}

//戦闘時間軸処理
void CBattle::TimeLapse(void)
{
	if (   m_nBattleTime == Time(0*2)
		|| m_nBattleTime == Time(1*2)
		|| m_nBattleTime == Time(2*2)
		|| m_nBattleTime == Time(3*2)
		|| m_nBattleTime == Time(4*2)
		|| m_nBattleTime == Time(5*2)
		|| m_nBattleTime == Time(6*2)
		|| m_nBattleTime == Time(7*2)
		|| m_nBattleTime == Time(8*2)
		|| m_nBattleTime == Time(9*2)
		|| m_nBattleTime == Time(10*2)
		|| m_nBattleTime == Time(11*2)
		|| m_nBattleTime == Time(12*2)
		|| m_nBattleTime == Time(13*2)
		|| m_nBattleTime == Time(14*2)
		|| m_nBattleTime == Time(15*2)
		)
	{
		//敵の生成数を指定
		m_nCreateEnemyNum = 4;
	}
}

//味方の生成処理
void CBattle::CreateAlly(void)
{
	//保存済み数分処理する
	while (m_nAllyDateCount)
	{
		//生成する
		m_pAlly[m_nAllyCount] = new CAlly(m_tAllyData[0].nCornerCount);
		//生成数を加算
		m_nAllyCount++;

		//生成に使用したため情報を消して後ろの情報を前詰めにする
		for (int i = 0; i + 1 < MAX_ALLY; i++)
		{
			m_tAllyData[i] = m_tAllyData[i + 1];
		}
		//保存済みの総数を減らす
		m_nAllyDateCount--;
	}
}

//敵の生成処理
void CBattle::CreateEnemy(void)
{
	//生成予定数分処理する
	while (m_nCreateEnemyNum)
	{
		//生成する
		m_pEnemy[m_nEnemyCount] = new CEnemy(m_tEnemyData[m_nSelectPattern][m_nCreateEnemyNum - 1].nCornerCount);
		//生成数を加算
		m_nEnemyCount++;
		//生成予定数を減らす
		m_nCreateEnemyNum--;
	}
}

//索敵処理
void CBattle::Search(int i, Entity Entity)
{
	//エンティティ番号で処理を分ける
	switch (Entity)
	{
	/*味方の判定*/
	case CBattle::Ally:
		//標的番号の初期化
		m_pAlly[i]->m_nTargetNumber = -1;

		//相手を生成順に処理
		for (int l = 0; l < m_nEnemyCount; l++)
		{
			//相手のステータスがBattleかどうか
			if (m_pEnemy[l]->GetStatus() == St_Battle)
			{
				//索敵当たり判定内に敵がいるかどうか
				if (m_pAlly[i]->SearchCollisionCheck(m_pEnemy[l]->GetSize(), m_pEnemy[l]->GetPos()))
				{
					//標的番号がすでに保存済みかどうか
					if (m_pAlly[i]->m_nTargetNumber != -1)
					{
						/*((敵のX位置 - 味方のX位置)+(敵のY位置 - 味方のY位置)) < ((標的のX位置 - 味方のX位置)+(標的のY位置 - 味方のY位置))*/
						if (((m_pEnemy[l]->GetPos().x - m_pAlly[i]->GetPos().x) + (m_pEnemy[l]->GetPos().z - m_pAlly[i]->GetPos().z))
							<
							((m_pEnemy[m_pAlly[i]->m_nTargetNumber]->GetPos().x - m_pAlly[i]->GetPos().x) + (m_pEnemy[m_pAlly[i]->m_nTargetNumber]->GetPos().z - m_pAlly[i]->GetPos().z)))
						{
							//標的番号を指定
							m_pAlly[i]->m_nTargetNumber = l;
						}
					}
					//保存していなかった場合は
					else
					{
						//標的番号を指定
						m_pAlly[i]->m_nTargetNumber = l;
					}
				}
			}
		}
		break;
		/*敵の判定*/
	case CBattle::Enemy:
		//標的番号の初期化
		m_pEnemy[i]->m_nTargetNumber = -1;
		//相手を生成順に処理
		for (int l = 0; l < m_nAllyCount; l++)
		{
			//相手のステータスがBattleかどうか
			if (m_pAlly[l]->GetStatus() == St_Battle)
			{
				//索敵当たり判定内に味方がいるかどうか
				if (m_pEnemy[i]->SearchCollisionCheck(m_pAlly[l]->GetSize(),m_pAlly[l]->GetPos()))
				{
					//標的番号がすでに保存済みかどうか
					if (m_pEnemy[i]->m_nTargetNumber != -1)
					{
						/*((味方のX位置 - 敵のX位置)+(味方のY位置 - 敵のY位置)) < ((標的のX位置 - 敵のX位置)+(標的のY位置 - 敵のY位置))*/
						if (((m_pAlly[l]->GetPos().x - m_pEnemy[i]->GetPos().x) + (m_pAlly[l]->GetPos().z - m_pEnemy[i]->GetPos().z))
							<
							((m_pAlly[m_pEnemy[i]->m_nTargetNumber]->GetPos().x - m_pEnemy[i]->GetPos().x) + (m_pAlly[m_pEnemy[i]->m_nTargetNumber]->GetPos().z - m_pEnemy[i]->GetPos().z)))
						{
							//標的番号に指定
							m_pEnemy[i]->m_nTargetNumber = l;
						}
					}
					//保存していなかった場合は
					else
					{
						//標的番号に指定
						m_pEnemy[i]->m_nTargetNumber = l;
					}
				}
			}
		}
		break;
	}
}

//移動処理
void CBattle::Move(int i, Entity Entity)
{
	//エンティティ番号別に処理
	switch (Entity)
	{
		/*味方の判定*/
	case CBattle::Ally:
		//移動フラグが立っていたら目的地指定の処理を飛ばす
		if (!m_pAlly[i]->GetMoveFlag())
		{
			//キャラクター同士が重なっていた場合の移動処理をしているかどうか
			if (OverlapMove(i, Ally))
			{
				//標的番号を設定済みだった場合
				if (m_pAlly[i]->m_nTargetNumber != -1)
				{
					//標的番号が現在生存数よりも数字が大きくないかどうか
					if (m_pAlly[i]->m_nTargetNumber < m_nEnemyCount)
					{
						DirectX::XMFLOAT3 iAllyPos = m_pAlly[i]->GetPos();
						DirectX::XMFLOAT3 EnemyPos = m_pEnemy[m_pAlly[i]->m_nTargetNumber]->GetPos();

						//敵の位置にMOVESPEEDの大きさで進む
						m_pAlly[i]->AddPosX(MoveCalculation(iAllyPos, EnemyPos).x);
						m_pAlly[i]->AddPosZ(MoveCalculation(iAllyPos, EnemyPos).z);
					}
					//数字が大きかった場合
					else
					{
						//相手のリーダーが生成されているか
						if (m_pEnemyLeader)
						{
							//自分の位置が敵の一定のラインまで来たら
							if (m_pAlly[i]->GetPos().x > ENEMYBOSSCORE_POSX - 10)
							{
								DirectX::XMFLOAT3 iAllyPos = m_pAlly[i]->GetPos();
								DirectX::XMFLOAT3 EnemyLeaderPos = m_pEnemyLeader->GetPos();

								//標的がいないので相手のリーダーに向かってMOVESPEEDの大きさで進む
								m_pAlly[i]->AddPosX(MoveCalculation(iAllyPos, EnemyLeaderPos).x);
								m_pAlly[i]->AddPosZ(MoveCalculation(iAllyPos, EnemyLeaderPos).z);
							}
							//そこより手前だったら
							else
							{
								//X軸を移動させる
								m_pAlly[i]->AddPosX(MOVESPEED(MOVEPOWER));
							}
						}
					}
				}
				//標的番号を設定していなかった場合
				else
				{
					//相手のリーダーが生成されているか
					if (m_pEnemyLeader)
					{
						//自分の位置が敵の一定のラインまで来たら
						if (m_pAlly[i]->GetPos().x > ENEMYBOSSCORE_POSX - 10)
						{
							DirectX::XMFLOAT3 iAllyPos = m_pAlly[i]->GetPos();
							DirectX::XMFLOAT3 EnemyLeaderPos = m_pEnemyLeader->GetPos();

							//標的がいないので相手のリーダーに向かってMOVESPEEDの大きさで進む
							m_pAlly[i]->AddPosX(MoveCalculation(iAllyPos, EnemyLeaderPos).x);
							m_pAlly[i]->AddPosZ(MoveCalculation(iAllyPos, EnemyLeaderPos).z);
						}
						//そこより手前だったら
						else
						{
							//X軸を移動させる
m_pAlly[i]->AddPosX(MOVESPEED(MOVEPOWER));
						}
					}
				}
			}
		}
		else//目的地へ移動
		{
			DirectX::XMFLOAT3 iEnemyPos = m_pAlly[i]->GetPos();
			DirectX::XMFLOAT3 DestinationPos = m_pAlly[i]->GetDestinationPos();

			m_pAlly[i]->AddPosX(MoveCalculation(iEnemyPos, DestinationPos).x);
			m_pAlly[i]->AddPosZ(MoveCalculation(iEnemyPos, DestinationPos).z);
			//目的地の+-1.0fの範囲内に入ったら移動フラグをfalseにする
			if (DestinationPos.x + 1.0f < m_pAlly[i]->GetPos().x && DestinationPos.x - 1.0f > m_pAlly[i]->GetPos().x)
			{
				if (DestinationPos.y + 1.0f < m_pAlly[i]->GetPos().y && DestinationPos.y - 1.0f > m_pAlly[i]->GetPos().y)
				{
					if (DestinationPos.z + 1.0f < m_pAlly[i]->GetPos().z && DestinationPos.z - 1.0f > m_pAlly[i]->GetPos().z)
					{
						m_pAlly[i]->SetMoveFlag(false);
					}
				}
			}
		}
		break;
		/*敵の判定*/
	case CBattle::Enemy:
		//移動フラグが立っていたら目的地指定の処理を飛ばす
		if (!m_pEnemy[i]->GetMoveFlag())
		{
			//キャラクター同士が重なっていた場合の移動処理をしているかどうか
			if (OverlapMove(i, Enemy))
			{
				//標的番号を設定済みだった場合
				if (m_pEnemy[i]->m_nTargetNumber != -1)
				{
					//標的番号が現在生存数よりも数字が大きくないかどうか
					if (m_pEnemy[i]->m_nTargetNumber < m_nAllyCount)
					{
						DirectX::XMFLOAT3 iEnemyPos = m_pEnemy[i]->GetPos();
						DirectX::XMFLOAT3 AllyPos = m_pAlly[m_pEnemy[i]->m_nTargetNumber]->GetPos();

						//標的の位置にMOVESPEEDの大きさで進む
						m_pEnemy[i]->AddPosX(MoveCalculation(iEnemyPos, AllyPos).x);
						m_pEnemy[i]->AddPosZ(MoveCalculation(iEnemyPos, AllyPos).z);
					}
					//数字が大きかった場合
					else
					{
						//相手のリーダーが生成されているか
						if (m_pAllyLeader)
						{
							//自分の位置が敵の一定のラインまで来たら
							if (m_pEnemy[i]->GetPos().x < ALLYCORE_POSX + 10)
							{
								DirectX::XMFLOAT3 iEnemyPos = m_pEnemy[i]->GetPos();
								DirectX::XMFLOAT3 AllyLeaderPos = m_pAllyLeader->GetPos();

								//標的がいないので相手のリーダーに向かってMOVESPEEDの大きさで進む
								m_pEnemy[i]->AddPosX(MoveCalculation(iEnemyPos, AllyLeaderPos).x);
								m_pEnemy[i]->AddPosZ(MoveCalculation(iEnemyPos, AllyLeaderPos).z);
							}
							//そこより手前だったら
							else
							{
								//X軸を移動させる
								m_pEnemy[i]->AddPosX(-MOVESPEED(MOVEPOWER));
							}
						}
					}
				}
				//標的番号を設定していなかった場合
				else
				{
					//相手のリーダーが生成されているか
					if (m_pAllyLeader)
					{
						//自分の位置が敵の一定のラインまで来たら
						if (m_pEnemy[i]->GetPos().x < ALLYCORE_POSX + 10)
						{
							DirectX::XMFLOAT3 iEnemyPos = m_pEnemy[i]->GetPos();
							DirectX::XMFLOAT3 AllyLeaderPos = m_pAllyLeader->GetPos();

							//標的がいないので相手のリーダーに向かってMOVESPEEDの大きさで進む
							m_pEnemy[i]->AddPosX(MoveCalculation(iEnemyPos, AllyLeaderPos).x);
							m_pEnemy[i]->AddPosZ(MoveCalculation(iEnemyPos, AllyLeaderPos).z);
						}
						//そこより手前だったら
						else
						{
							//X軸を移動させる
							m_pEnemy[i]->AddPosX(-MOVESPEED(MOVEPOWER));
						}
					}
				}
			}
		}
		else//目的地へ移動
		{
			DirectX::XMFLOAT3 iEnemyPos = m_pEnemy[i]->GetPos();
			DirectX::XMFLOAT3 DestinationPos = m_pEnemy[i]->GetDestinationPos();

			m_pEnemy[i]->AddPosX(MoveCalculation(iEnemyPos, DestinationPos).x);
			m_pEnemy[i]->AddPosZ(MoveCalculation(iEnemyPos, DestinationPos).z);
			//目的地の+-1.0fの範囲内に入ったら移動フラグをfalseにする
			if (DestinationPos.x + 1.0f < m_pEnemy[i]->GetPos().x && DestinationPos.x - 1.0f > m_pEnemy[i]->GetPos().x)
			{
				if (DestinationPos.y + 1.0f < m_pEnemy[i]->GetPos().y && DestinationPos.y - 1.0f > m_pEnemy[i]->GetPos().y)
				{
					if(DestinationPos.z + 1.0f < m_pEnemy[i]->GetPos().z && DestinationPos.z - 1.0f > m_pEnemy[i]->GetPos().z)
					{
						m_pEnemy[i]->SetMoveFlag(false);
					}
				}
			}
		}
		break;
	}
}

//重なっていた場合の補正移動処理
bool CBattle::OverlapMove(int i, Entity Entity)
{
	//移動先の座標保存変数
	DirectX::XMFLOAT3 m_tMovePos;
	float comparison;

	////Z軸順に確認
	//for (float Z = 20.0f; Z > -30.0f; Z -= 1.0f)
	//{
		//エンティティ番号別に処理
	switch (Entity)
	{
		/*味方の処理*/
	case CBattle::Ally:
		//生成順に判定
		for (int l = 0; l < m_nAllyCount; l++)
		{
			//存在しているか確認
			if (!m_pAlly[l])continue;
			//自分と同じ番号の場合は処理しない
			if (l == i)continue;

			DirectX::XMFLOAT3 iAllyPos = m_pAlly[i]->GetPos();
			DirectX::XMFLOAT3 AllyPos = m_pAlly[l]->GetPos();
			DirectX::XMFLOAT3 AllySize = m_pAlly[l]->GetSize();

			////Z軸の範囲判定
			//if (AllyPos.z > Z - 1.0f && AllyPos.z < Z + 1.0f)
			//{
				//自分の位置から圧倒的に離れているものは判定しない
			if (iAllyPos.x + 1.0f < AllyPos.x || iAllyPos.x - 1.0f > AllyPos.x)continue;
			if (iAllyPos.z + 1.0f < AllyPos.z || iAllyPos.z - 1.0f > AllyPos.z)continue;

			//中心位置から自分の位置が相手の位置より近いは処理しない
			float Z1 = iAllyPos.z;
			float Z2 = AllyPos.z;

			if (Z1 < 0)Z1 * -1.0f;
			if (Z2 < 0)Z2 * -1.0f;

			if (Z1 <= Z2)continue;

			//重なっているか確認
			if (m_pAlly[i]->OverlapCheck(AllyPos, AllySize))
			{
				//補正移動先を目的地に移動
				m_pAlly[i]->SetDestinationPos({ -AllyPos.x,AllyPos.y,-AllyPos.z });
				//移動フラグを立てる
				m_pAlly[i]->SetMoveFlag(true);

				return false;
			}
			//}
		}
		break;
		/*敵の処理*/
	case CBattle::Enemy:
		//前から順番に重なっている奴がいないか確認
		for (int l = 0; l < m_nEnemyCount; l++)
		{
			//存在しているか確認
			if (!m_pEnemy[l])continue;
			//自分と同じ番号の場合は処理しない
			if (l == i)continue;

			DirectX::XMFLOAT3 iEnemyPos = m_pEnemy[i]->GetPos();
			DirectX::XMFLOAT3 EnemyPos = m_pEnemy[l]->GetPos();
			DirectX::XMFLOAT3 EnemySize = m_pEnemy[l]->GetSize();

			////Z軸の範囲判定
			//if (EnemyPos.z > Z - 1.0f && EnemyPos.z < Z + 1.0f)
			//{
				//自分の位置から圧倒的に離れているものは判定しない
			if (iEnemyPos.x + 1.0f < EnemyPos.x || iEnemyPos.x - 1.0f > EnemyPos.x)continue;
			if (iEnemyPos.z + 1.0f < EnemyPos.z || iEnemyPos.z - 1.0f > EnemyPos.z)continue;

			//中心位置から自分の位置が相手の位置より近いは処理しない
			float Z1 = iEnemyPos.z;
			float Z2 = EnemyPos.z;

			if (Z1 < 0)Z1 * -1.0f;
			if (Z2 < 0)Z2 * -1.0f;

			if (Z1 <= Z2)continue;

			//重なっているか確認
			if (m_pEnemy[i]->OverlapCheck(EnemyPos, EnemySize))
			{
				//補正移動先を目的地に移動
				m_pEnemy[i]->SetDestinationPos({ -EnemyPos.x,EnemyPos.y,-EnemyPos.z });
				//移動フラグを立てる
				m_pEnemy[i]->SetMoveFlag(true);

				return false;
			}
			//}
		}
		break;
		//}
	}

	//移動をしたかどうかを返す
	return true;
}

//範囲内補正処理
void CBattle::ScopeMove()
{
	//生成された順に前にするために後ろのやつから補正していく
	for (int i = 0; i < m_nAllyCount; i++)
	{
		//存在しているか確認
		if (!m_pAlly[i])continue;

		float posz = m_pAlly[i]->GetPos().z;

		//奥行きが範囲外		
		if (posz > BATTLE_Z)
		{
			m_pAlly[i]->SetPosZ(BATTLE_Z);
		}
		//手前が範囲外
		if (posz < -BATTLE_Z)
		{
			m_pAlly[i]->SetPosZ(-BATTLE_Z);
		}
	}

	for (int i = 0; i < m_nEnemyCount; i++)
	{
		//存在しているか確認
		if (!m_pEnemy[i])continue;

		float posz = m_pEnemy[i]->GetPos().z;

		//奥行きが範囲外
		if (posz > BATTLE_Z)
		{
			m_pEnemy[i]->SetPosZ(BATTLE_Z);
		}
		//手前が範囲外
		if (posz < -BATTLE_Z)
		{
			m_pEnemy[i]->SetPosZ(-BATTLE_Z);
		}
	}
}

//戦闘処理
void CBattle::Battle(int i, int l, Entity Entity)
{
	//エンティティ番号別に処理
	switch (Entity)
	{
	//味方のリーダーが攻撃される
	case AllyLeader:
		//味方のリーダーが生成されているか
		if (m_pAllyLeader)
		{
			//攻撃チャージがたまっているかどうか
			if (m_pEnemy[i]->GetAtkCharge() >= m_pEnemy[i]->GetCoolTime())
			{
				//相手の体力を減らす
				m_pAllyLeader->Damage(m_pEnemy[i]);
				//攻撃したのでチャージをリセットする
				m_pEnemy[i]->ChargeReset();
			}
			//攻撃チャージが溜まっていなかったら
			else
			{
				//チャージする
				m_pEnemy[i]->AddAtkCharge();
			}
		}
		break;
		//敵のボスが攻撃される
	case CBattle::EnemyLeader:
		if (m_pEnemyLeader)
		{
			//攻撃チャージがたまっているかどうか
			if (m_pAlly[i]->GetAtkCharge() >= m_pAlly[i]->GetCoolTime())
			{
				//相手の体力を減らす
				m_pEnemyLeader->Damage(m_pAlly[i]);
				//攻撃したのでチャージをリセットする
				m_pAlly[i]->ChargeReset();
			}
			//攻撃チャージが溜まっていなかったら
			else
			{
				//チャージする
				m_pAlly[i]->AddAtkCharge();
			}
		}
		break;
		//味方が攻撃される
	case CBattle::Ally:
		//攻撃チャージがたまっているかどうか
		if (m_pEnemy[i]->GetAtkCharge() >= m_pEnemy[i]->GetCoolTime())
		{
			//相手の体力を減らす
			m_pAlly[l]->Damage(m_pEnemy[i]);
			//攻撃したのでチャージをリセットする
			m_pEnemy[i]->ChargeReset();
		}
		//攻撃チャージが溜まっていなかったら
		else
		{
			//チャージする
			m_pEnemy[i]->AddAtkCharge();
		}
		break;
		//敵の攻撃
	case CBattle::Enemy:
		//攻撃チャージがたまっているかどうか
		if (m_pAlly[i]->GetAtkCharge() >= m_pAlly[i]->GetCoolTime())
		{
			//相手の体力を減らす
			m_pEnemy[l]->Damage(m_pAlly[i]);
			//攻撃したのでチャージをリセットする
			m_pAlly[i]->ChargeReset();
		}
		//攻撃チャージが溜まっていなかったら
		else
		{
			//チャージする
			m_pAlly[i]->AddAtkCharge();
		}
		break;
	}
}

void CBattle::Alive(void)
{
	//味方判定
	for (int l = 0; l < m_nAllyCount; l++)
	{
		//味方のステータスがBattleかどうか
		if (m_pAlly[l]->GetStatus() == St_Battle)
		{
			//味方の体力が残っているかどうか
			if (m_pAlly[l]->GetHp() <= 0.0f)
			{
				//ステータスを死亡状態にする
				m_pAlly[l]->SetStatus(St_Death);
			}
		}
	}
	//敵の判定
	for (int l = 0; l < m_nEnemyCount; l++)
	{
		//敵のステータスがBattleかどうか
		if (m_pEnemy[l]->GetStatus() == St_Battle)
		{
			//敵の体力が残っているかどうか
			if (m_pEnemy[l]->GetHp() <= 0.0f)
			{
				//ステータスを死亡状態にする
				m_pEnemy[l]->SetStatus(St_Death);
			}
		}
	}
	//敵のリーダーの判定
	if (m_pEnemyLeader)
	{
		//敵のステータスがBattleかどうか
		if (m_pEnemyLeader->GetStatus() == St_Battle)
		{
			//敵の体力が残っているかどうか
			if (m_pEnemyLeader->GetHp() <= 0.0f)
			{
				//ステータスを死亡状態にする
				m_pEnemyLeader->SetStatus(St_Death);
			}
		}
	}
	//味方のリーダーの判定
	if (m_pAllyLeader)
	{
		//味方のリーダーのステータスがBattleかどうか
		if (m_pAllyLeader->GetStatus() == St_Battle)
		{
			//味方のリーダーの体力が残っているかどうか
			if (m_pAllyLeader->GetHp() <= 0.0f)
			{
				//ステータスを死亡状態にする
				m_pAllyLeader->SetStatus(St_Death);
			}
		}
	}
}

//削除処理
void CBattle::Delete(void)
{
	//味方の生存判定
	for (int i = 0; i < m_nAllyCount; i++)
	{
		//ステータスがDeleteかどうか
		if (m_pAlly[i]->GetStatus() == St_Delete)
		{
			//解放処理
			delete m_pAlly[i];
			m_pAlly[i] = nullptr;
			//配列前詰め
			for (int d = i; d < m_nAllyCount; d++)
			{
				if (m_pAlly[d] == nullptr)
				{
					//一つ後ろの配列を自分に入れる
					m_pAlly[d] = m_pAlly[d + 1];
					//一つ後ろをnullptrにする
					m_pAlly[d + 1] = nullptr;
				}
			}
			//今いる生存数を減らす
			m_nAllyCount--;
			//前詰めされたのでもう一度同じ場所を処理する
			i--;
		}
	}
	//敵の生存判定
	for (int i = 0; i < m_nEnemyCount; i++)
	{
		//ステータスがDeleteかどうか
		if (m_pEnemy[i]->GetStatus() == St_Delete)		
		{
			//解放処理
			delete m_pEnemy[i];
			m_pEnemy[i] = nullptr;
			//配列前詰め
			for (int d = i; d < m_nEnemyCount; d++)
			{
				if (m_pEnemy[d] == nullptr)
				{
					//一つ後ろの配列を自分に入れる
					m_pEnemy[d] = m_pEnemy[d + 1];
					//一つ後ろをnullptrにする
					m_pEnemy[d + 1] = nullptr;
				}
			}
			//今いる生存数を減らす
			m_nEnemyCount--;
			//前詰めされたのでもう一度同じ場所を処理する
			i--;
		}
	}
	//敵ボスの生存判定
	if (m_pEnemyLeader)
	{
		//ステータスがDeleteかどうか
		if (m_pEnemyLeader->GetStatus() == St_Delete)
		{
			//解放処理
			m_pEnemyLeader = nullptr;
		}
	}
	//プレイヤーの生存判定
	if (m_pAllyLeader)
	{
		//ステータスがDeleteかどうか
		if (m_pAllyLeader->GetStatus() == St_Delete)
		{
			//解放処理
			m_pAllyLeader = nullptr;
		}
	}
}

//初期位置セッティング
void CBattle::FirstPosSetting()
{
	//X座標補正用変数
	int PosX = 0;
	//味方を生成順に処理
	for (int i = 0; i < m_nAllyCount; i++)
	{
		//生成されていなかったら処理をしない
		if (!m_pAlly[i])continue;
		//初期位置設定をすでにしているかどうかを確認
		if (!m_pAlly[i]->m_bFirstBattlePosSetting)
		{
			//初期X位置を設定
			m_pAlly[i]->SetPosX(ALLYCREATE_POSX - PosX);
			//初期Y位置を設定
			if (m_pAlly[i]->GetCornerCount() == 3)
			{
				m_pAlly[i]->SetPosY(0.0f + m_pAlly[i]->GetSize().y / 2);
			}
			else if (m_pAlly[i]->GetCornerCount() == 4)
			{
				m_pAlly[i]->SetPosY(0.0f + m_pAlly[i]->GetSize().y * 2);
			}
			//Z座標をパターン別に設定
			switch (m_nFirstPosPattern)
			{
			case 0:
				//初期Z位置を設定
				m_pAlly[i]->SetPosZ(ALLYCREATE_POSZ_1);
				//初期位置設定を設定済みにする
				m_pAlly[i]->m_bFirstBattlePosSetting = true;
				//パターンを一つずらす
				m_nFirstPosPattern = 1;
				break;
			case 1:
				//初期Z位置を設定
				m_pAlly[i]->SetPosZ(ALLYCREATE_POSZ_2);
				//初期位置設定を設定済みにする
				m_pAlly[i]->m_bFirstBattlePosSetting = true;
				//パターンを一つずらす
				m_nFirstPosPattern = 2;
				break;
			case 2:
				//初期Z位置を設定
				m_pAlly[i]->SetPosZ(ALLYCREATE_POSZ_3);
				//初期位置設定を設定済みにする
				m_pAlly[i]->m_bFirstBattlePosSetting = true;
				//パターンを一つずらす
				m_nFirstPosPattern = 3;
				break;
			case 3:
				//初期Z位置を設定
				m_pAlly[i]->SetPosZ(ALLYCREATE_POSZ_4);
				//初期位置設定を設定済みにする
				m_pAlly[i]->m_bFirstBattlePosSetting = true;
				//パターンを一つずらす
				m_nFirstPosPattern = 4;
				break;
			case 4:
				//初期Z位置を設定
				m_pAlly[i]->SetPosZ(ALLYCREATE_POSZ_5);
				//初期位置設定を設定済みにする
				m_pAlly[i]->m_bFirstBattlePosSetting = true;
				//パターンを最初に戻す
				m_nFirstPosPattern = 0;
				//X座標補正用変数を加算する
				PosX += 5.0f;
				break;
			}
		}
	}

	//X座標補正用変数を初期化
	PosX = 0;

	//敵を生成順に処理
	for (int i = 0; i < m_nEnemyCount; i++)
	{
		//生成していなかったら処理しない
		if (!m_pEnemy[i])continue;
		//初期位置設定をすでにしているかどうかを確認
		if (!m_pEnemy[i]->m_bFirstBattlePosSetting)
		{
			//初期X位置を設定
			m_pEnemy[i]->SetPosX(ENEMYCREATE_POSX + PosX);
			////初期Y位置を設定
			//m_pEnemy[i]->SetPosY(0.0f + m_pEnemy[i]->GetSize().y / 2);
			//初期Y位置を設定

			if (m_pEnemy[i]->GetCornerCount() == 3)
			{
				m_pEnemy[i]->SetPosY(0.0f + m_pEnemy[i]->GetSize().y / 2);
			}
			else if (m_pEnemy[i]->GetCornerCount() == 4)
			{
				m_pEnemy[i]->SetPosY(0.0f + m_pEnemy[i]->GetSize().y * 2);
			}

			//Z座標をパターン別に設定
			switch (m_nFirstPosPattern)
			{
			case 0:
				//初期Z位置を設定
				m_pEnemy[i]->SetPosZ(ENEMYCREATE_POSZ_1);
				//初期位置設定を設定済みにする
				m_pEnemy[i]->m_bFirstBattlePosSetting = true;
				//パターンを一つずらす
				m_nFirstPosPattern = 1;
				break;
			case 1:
				//初期Z位置を設定
				m_pEnemy[i]->SetPosZ(ENEMYCREATE_POSZ_2);
				//初期位置設定を設定済みにする
				m_pEnemy[i]->m_bFirstBattlePosSetting = true;
				//パターンを一つずらす
				m_nFirstPosPattern = 2;
				break;
			case 2:
				//初期Z位置を設定
				m_pEnemy[i]->SetPosZ(ENEMYCREATE_POSZ_3);
				//初期位置設定を設定済みにする
				m_pEnemy[i]->m_bFirstBattlePosSetting = true;
				//パターンを一つずらす
				m_nFirstPosPattern = 3;
				break;
			case 3:
				//初期Z位置を設定
				m_pEnemy[i]->SetPosZ(ENEMYCREATE_POSZ_4);
				//初期位置設定を設定済みにする
				m_pEnemy[i]->m_bFirstBattlePosSetting = true;
				//パターンを一つずらす
				m_nFirstPosPattern = 4;
				break;
			case 4:
				//初期Z位置を設定
				m_pEnemy[i]->SetPosZ(ENEMYCREATE_POSZ_5);
				//初期位置設定を設定済みにする
				m_pEnemy[i]->m_bFirstBattlePosSetting = true;
				//パターンを最初に戻す
				m_nFirstPosPattern = 0;
				//X座標補正用変数を加算する
				PosX += 5.0f;
				break;
			}
		}
	}
}

//リーダーの生成処理
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
		m_pAllyLeader = new CLeader(0.5f, InFirstPos, 0);
	}
	//敵のリーダーがnullptrだったら
	if (m_pEnemyLeader == nullptr)
	{
		//初期位置設定用変数
		DirectX::XMFLOAT3 BossFirstPos;

		//X座標を設定
		BossFirstPos.x = ENEMYBOSSCORE_POSX;
		//Y座標を設定
		BossFirstPos.y = 8.0f;
		//Z座標を設定
		BossFirstPos.z = ENEMYBOSSCORE_POSZ;
		//敵のリーダーを生成
		m_pEnemyLeader = new CLeader(0.5f, BossFirstPos, 1);
	}
}

/*＝＝＝＝＝バトル用＝＝＝＝＝*/

//味方を角数別にカウントしたものの描画処理
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
//敵を種類別にカウントしたものの描画処理
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

//味方を角数別にカウント
void CBattle::CreateAllyLog(void)
{
	//三角形のカウント格納用変数の初期化
	m_nAllyTypes[0] = 0;
	//四角形のカウント格納用変数の初期化
	m_nAllyTypes[1] = 0;
	//味方を生成順に処理
	for (int i = 0; i < m_nAllyCount; i++)
	{
		//角数別に処理
		switch (m_pAlly[i]->GetCornerCount())
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
//敵を種類別にカウント
void CBattle::CreateEnemyLog(void)
{
	//三角形のカウント格納用変数の初期化
	m_nEnemyTypes[0] = 0;
	//四角形のカウント格納用変数の初期化
	m_nEnemyTypes[1] = 0;
	//敵を生成順に処理
	for (int i = 0; i < m_nEnemyCount; i++)
	{
		//角数別に処理
		switch (m_pEnemy[i]->GetCornerCount())
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

/*＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝*/

void CBattle::RandomSelectPattern(void)
{
 	m_nSelectPattern = rand() % m_nMaxPattern;
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

	//float hehe = Movepower.x + Movepower.z;
	//Movepower.z = MOVESPEED(MOVEPOWER) - Movepower.x;

	return Movepower;
}
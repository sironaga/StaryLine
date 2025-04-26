#pragma once
#include "Character.h"
#include "SoundList.h"
#include <vector>
#include <mutex>

//パターンの最大数
#define MAX_PATTERN (5)

class CBattle
{
	/*＝＝＝＝＝＝＝＝＝＝＝＝＝＝総合処理＝＝＝＝＝＝＝＝＝＝＝＝＝＝*/
private:
	//エンティティ番号
	enum Entity
	{
		AllyLeader,
		EnemyLeader,
		Ally,
		Enemy,
	};
private:
	struct EntityData
	{
		//角数番号
		int nCornerCount;
		//ステラフラグ
		bool bStellaBuff;
	};

public:
	//コンストラクタ
	CBattle();
	//デストラクタ
	~CBattle();	
	//更新処理
	void Update(void);
	//キャラクターたちの更新
	void CharacterUpdate(bool AnimationFlag = false);
	//Linieの描画
	void LinieDraw(void);
	//描画処理
	void Draw(void);		
	//リザルトにいく
	bool GetEnd();
	//テクスチャの再読み込み
	void ReLoadTexture(void);
	/*＝＝＝＝＝リザルトに渡す情報＝＝＝＝＝*/
	float m_fRinieMaxHp;
	float m_fRinieLastHp;
	//プレイヤーの体力の残り割合のGet
	int GetPlayerHpProportion(void) { return (m_fRinieLastHp / m_fRinieMaxHp) * 100; }
	//勝敗のGet
	bool GetWin(void) { return m_bWin; }
	//味方の召喚総数のGet
	int GetSummonAllyCount(void) { return m_nSummonAllyCount; }
	//勝利アニメーションが終了したかのGet
	bool GetWinAnimation() { return m_pAllyLeader->GetWinTImer() >= LINIEWINTIME; }

	//描画開始判定のSet
	void SetDrawingStart(bool IsStart) { m_bDrawingStart = IsStart; }
	//描画終了判定のSet
	void SetDrawingEnd(bool IsEnd) { m_bDrawingEnd = IsEnd; }
	//勝利アニメーションのSet
	void SetWinAnimation(bool IsWin) { m_pAllyLeader->SetWinFlag(IsWin); }
	//Linieの回転
	void LinieRotation(DirectX::XMFLOAT3 InRotate) { m_pAllyLeader->SetRotate(InRotate); };
private:
	//時間軸処理
	void TimeLapse(void);
	//索敵処理
	void Search(int i,Entity Entity);
	//移動処理
	void Move(int i, Entity Entity);
	//重なってた場合の補正移動処理
	bool OverlapMove(int i,Entity Entity);
	//範囲内補正
	void ScopeMove();
	//戦闘処理
	void Battle(int i, int l , Entity Entity);
	//生存判定
	void Alive(void);
	//削除処理と配列前詰め
	void Delete(void);
	//戦闘初期位置のセッティング
	void FirstPosSetting();

public:
	//ステージナンバー
	StageType m_nStageNum;
private:
	
	std::mutex mtx;

	//描画開始判定
	bool m_bDrawingStart;
	//描画終了判定
	bool m_bDrawingEnd;

	//戦闘時間
	int m_nBattleTime;
	//敵のひとつ前の出現時間
	int m_nSpawnTime;
	//初期戦闘したかどうか
	bool m_bFirstFight;
	//初期位置のパターン
	int m_nFirstPosPattern;
	//ゲームが終わったどうか
	bool m_bEnd;
	//勝敗フラグ
	bool m_bWin;
	//味方の召喚総数カウント
	int m_nSummonAllyCount;
	/*＝＝＝＝＝＝＝＝＝＝＝＝＝リーダー関係＝＝＝＝＝＝＝＝＝＝＝＝＝*/
private:
	//プレイヤー
	CLeader* m_pAllyLeader;
	//敵ボスのクラスポインタ
	CLeader* m_pEnemyLeader;
public:
	//リーダーの生成
	void CreateLeader(void);
	/*＝＝＝＝＝＝＝＝＝＝＝＝＝＝味方関係＝＝＝＝＝＝＝＝＝＝＝＝＝＝*/
private:
	//味方クラスポインタ
	//CAlly* m_pAlly[MAX_ALLY];
	std::vector<CAlly*> m_pAlly;

	//生成予定の味方情報
	EntityData m_tAllyData[MAX_ALLY];
	//生成した味方のカウント
	int m_nAllyCount;
	//保存した味方の情報数
	int m_nAllyDateCount;
	//現在生成している味方の種類別カウント変数
	int m_nAllyTypes[2];
public:
	//味方カウントのGet
	int GetAllyCount(void) { return m_nAllyCount; }	
	//味方の種類別カウントのGet
	int GetAllyTypeCount(int Num) { return m_nAllyTypes[Num]; }
	//味方要素保存
	void SaveAllyData(int InCornerCount, bool IsStella = false);
public:
	//味方作成
	void CreateAlly(void);

	/*＝＝＝＝＝＝＝＝＝＝＝＝＝＝ 敵関係 ＝＝＝＝＝＝＝＝＝＝＝＝＝＝*/
private:
	//敵クラスポインタ
	//CEnemy* m_pEnemy[MAX_ENEMY];
	std::vector<CEnemy*> m_pEnemy;

	//生成予定敵情報
	//EntityData m_tEnemyData[MAX_PATTERN][MAX_ENEMY];

	//生成した敵のカウント
	int m_nEnemyCount;
	//保存した敵の情報数
	//int m_nEnemyDateCount[MAX_PATTERN];

	//現在生成している敵の種類別カウント変数
	int m_nEnemyTypes[2];
	//生成してほしい数
	int m_nCreateEnemyNum;
public:
	//敵カウントのGet
	int GetEnemyCount(void) { return m_nEnemyCount; }
	//敵の種類別カウントのGet
	int GetEnemyTypeCount(int Num) { return m_nEnemyTypes[Num]; }
	//敵要素保存
	//void SaveEnemyData(int InCornerCount, int InPattern);		
private:
	//敵作成
	void CreateEnemy(void);

	/*＝＝＝＝＝＝＝＝＝＝＝＝＝ デバック関係 ＝＝＝＝＝＝＝＝＝＝＝＝＝*/
private:
	//数字のバッファーポインタ
	ID3D11Buffer* m_pLogVtx;
private:
	/*バトル用*/
	//味方生成種類別カウント
	void CreateAllyLog(void);
	//味方生成ログの描画
	void CreateAllyLogDraw(void);
	//敵生成種類別カウント
	void CreateEnemyLog(void);
	//敵生成ログの描画
	void CreateEnemyLogDraw(void);


	bool m_bWinCommand[12];
	float m_fWinCommandResetTimer;
	bool m_bLoseCommand[12];
	float m_fLoseCommandResetTimer;
	bool WinEndCommand(void);
	bool LoseEndCommand(void);

	/*図形判定用*/
private:
	//味方保存種類別カウント
	void SaveAllyLog(void);
public:
	//味方保存ログの描画
	void SaveAllyLogDraw(void);

	/*＝＝＝＝＝＝＝＝＝＝＝＝＝Pattern関係＝＝＝＝＝＝＝＝＝＝＝＝＝＝*/
private:
	//選択されたパターン
	int m_nSelectPattern;
	//最大パターン数
	int m_nMaxPattern;

public:
	//選択されたパターンのGet
	int GetSelectPattern(void) { return m_nSelectPattern; }
	//用意された範囲でパターンを選ぶ
	void RandomSelectPattern(void);
	//最大パターン数のSet
	void SetMaxPattern(int InMaxPattern) { m_nMaxPattern = InMaxPattern; }
	//最大パターン数のGet
	int GetMaxPattern(void) { return m_nMaxPattern; }
	/*＝＝＝＝＝＝＝＝＝＝＝＝＝音関係＝＝＝＝＝＝＝＝＝＝＝＝＝＝*/
	public:
		void ReloadSound();
	private:
		CSoundList* m_pSound;
		IXAudio2SourceVoice* m_pDeathSE;
};
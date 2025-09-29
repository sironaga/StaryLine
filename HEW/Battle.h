/*==============================================================
*
*  File：Battle.h
*
*  編集者：宇留野陸斗｜[作成][管理]
**
*  編集者：中嶋飛賀　｜[ 音 ]
*
================================================================*/

#pragma once
#include "CharacterInclude.h"
#include "SoundList.h"
#include <vector>
#include <mutex>
#include <memory>

//パターンの最大数
#define MAX_PATTERN (5)

class CBattle
{
	/*＝＝＝＝＝＝＝＝＝＝＝＝＝＝総合処理＝＝＝＝＝＝＝＝＝＝＝＝＝＝*/
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
	CBattle(StageType In_StageType);
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
	//勝利アニメーションのSet
	void SetWinAnimation(bool IsWin) { m_pAllyLeader->SetWinFlag(IsWin); }
	//Linieの回転
	void LinieRotation(DirectX::XMFLOAT3 InRotate) { m_pAllyLeader->SetRotate(InRotate); };

private:
	//時間軸処理
	void TimeLapse(void);
	//索敵処理
	void Search(CFighter* Searcher);
	//移動処理
	void Move(CFighter* Mover);
	//重なってた場合の補正移動処理
	bool OverlapMove(CFighter* Mover);
	//範囲内補正
	void ScopeMove();
	//戦闘処理
	template<class T>
	void Battle(CFighter* Attacker, T* Defender);
	//生存判定
	void Alive(void);
	//削除処理と配列前詰め
	void Delete(void);
	//戦闘初期位置のセッティング
	void FirstPosSetting();

	// 兵士を種類別に取得
	std::vector<CFighter*> GetFighterTypeList(FighterType Type);

public:
	//ステージナンバー
	StageType m_nStageNum;
private:

	std::mutex mtx;

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
private:
	//リーダーの生成
	void CreateLeader(void);

	/*＝＝＝＝＝＝＝＝＝＝＝＝＝＝兵士関係＝＝＝＝＝＝＝＝＝＝＝＝＝＝*/
private:
	//兵士クラスポインタ
	std::vector<CFighter*> m_pFighter;

	/*＝＝＝＝＝＝＝＝＝＝＝＝＝＝味方関係＝＝＝＝＝＝＝＝＝＝＝＝＝＝*/
private:
	//生成予定の味方情報
	std::vector <EntityData> m_tAllyData;
	//現在生成している味方の種類別カウント変数
	int m_nAllyTypes[2];
public:
	//味方の種類別カウントのGet
	int GetAllyTypeCount(int Num) { return m_nAllyTypes[Num]; }
	//味方要素保存
	void SaveAllyData(int InCornerCount, bool IsStella = false);
public:
	//味方作成
	void CreateAlly(void);

	/*＝＝＝＝＝＝＝＝＝＝＝＝＝＝ 敵関係 ＝＝＝＝＝＝＝＝＝＝＝＝＝＝*/
private:
	//現在生成している敵の種類別カウント変数
	int m_nEnemyTypes[2];
	//生成してほしい数
	int m_nCreateEnemyNum;
public:
	//敵の種類別カウントのGet
	int GetEnemyTypeCount(int Num) { return m_nEnemyTypes[Num]; }
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

	/*＝＝＝＝＝＝＝＝＝＝＝＝＝音関係＝＝＝＝＝＝＝＝＝＝＝＝＝＝*/
public:
	void ReloadSound();
private:
	CSoundList* m_pSound;
	IXAudio2SourceVoice* m_pDeathSE;

	/*＝＝＝＝＝＝＝＝＝＝＝チュートリアル関係＝＝＝＝＝＝＝＝＝＝＝＝*/
private:
	// チュートリアル時に止めるフラグ
	bool m_bTutorialMoveStop;
	// チュートリアル時に召喚処理を行わないようにするフラグ
	bool m_bTutorialStopAllySpown;
	bool m_bTutorialStopEnemySpown;
public:
	// チュートリアル関係のフラグをすべてクリアする
	void AllTutorialFlagClear();
	// すべてのファイターのクリア
	void AllFighterClear();

	// チュートリアル時に移動を止めるフラグ
	void SetTutorialMoveFlag(bool isStop) { m_bTutorialMoveStop = isStop; }
	// チュートリアルで召喚を行うかどうかのフラグのSet
	void SetTutorialSpownFlag(bool isSpown) { m_bTutorialStopAllySpown = isSpown; m_bTutorialStopEnemySpown = isSpown; }
};
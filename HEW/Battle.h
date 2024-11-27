#pragma once
#include "Character.h"
#include "VectorOperator.h"

#define MAX_WAVE  (5)	//ウェーブの最大数


class CBattle
{
	/*＝＝＝＝＝＝＝＝＝＝＝＝＝＝総合処理＝＝＝＝＝＝＝＝＝＝＝＝＝＝*/
private:
	enum Entity
	{
		Ally = 1,
		Enemy = 2,
	};
private:
	struct EntityData
	{
		int nCornerCount;				//バリエーション番号
		CVector3<float> m_tCreatePos;	//生成位置
		float Size;						//サイズ
	};

public:
	CBattle();				//コンストラクタ
	~CBattle();				//デストラクタ
	void Update(void);		//更新処理
	void Draw(void);		//描画処理

	void NextWaveInit(void);			//次のWaveに進むための初期化処理
private:
	void TimeAxis(void);				//時間軸処理
	void CreateEntity(void);			//エンティティ生成
	void Search(int i,Entity Entity);	//索敵処理
	void Move(int i, Entity Entity);	//移動処理
	void Battle(int i, int l, Entity Entity);	//戦闘処理
	void Alive(void);					//生存判定
	void Delete(void);					//削除処理と配列前詰め
private:
	int m_nBattleTime;					//戦闘時間
	bool m_bFirstFight;					//初期戦闘したかどうか

	/*＝＝＝＝＝＝＝＝＝＝＝＝＝＝味方関係＝＝＝＝＝＝＝＝＝＝＝＝＝＝*/
private:
	CAlly* m_pAlly[MAX_ALLY];							//味方クラスポインタ
	EntityData m_tAllyData[MAX_ALLY];					//生成予定の味方情報
	int m_nAllyCount;									//生成した味方のカウント
	int m_nAllyDateCount;								//保存した味方の情報数
	int m_nCreateAllyNum;								//味方を生成する数
	int m_nAllyTypes[6];								//現在生成している味方の種類別カウント変数
	int m_nFirstPosSetting;								//味方の初期位置を設定するときに使用する
public:
	int GetAllyCount(void) { return m_nAllyCount; }		//味方カウントのGet

	void SaveAllyData(int InCornerCount, float InSize);//味方要素保存
private:
	void CreateAllyData(EntityData InData);			//味方作成


	/*＝＝＝＝＝＝＝＝＝＝＝＝＝＝ 敵関係 ＝＝＝＝＝＝＝＝＝＝＝＝＝＝*/
private:
	CEnemy* m_pEnemy[MAX_ENEMY];					//敵クラスポインタ
	EntityData m_tEnemyData[MAX_WAVE][MAX_ENEMY];	//生成予定敵情報
	int m_nEnemyCount;								//生成した敵のカウント
	int m_nEnemyDateCount[MAX_WAVE];				//保存した敵の情報数
	int m_nCreateEnemyNum;							//敵を生成する数
	int m_nEnemyTypes[3];							//現在生成している敵の種類別カウント変数
public:
	int GetEnemyCount(void) { return m_nEnemyCount; }			//敵カウントのGet

	void SaveEnemyData(int InCornerCount, int Wave, float InSize);		//敵要素保存
private:
	void CreateEnemyData(EntityData InDate);	//敵作成


	/*＝＝＝＝＝＝＝＝＝＝＝＝＝ デバック関係 ＝＝＝＝＝＝＝＝＝＝＝＝＝*/
private:
	ID3D11Buffer* m_pLogVtx;
private:
	/*バトル用*/
	void DebugMove(void);							//デバック移動

	void CreateAllyLog(void);						//味方生成種類別カウント
	void CreateAllyLogDraw(void);					//味方生成ログの描画

	void CreateEnemyLog(void);						//敵生成種類別カウント
	void CreateEnemyLogDraw(void);					//敵生成ログの描画
	/*図形判定用*/
private:
	void SaveAllyLog(void);							//味方保存種類別カウント
public:
	void SaveAllyLogDraw(void);						//味方保存ログの描画

	/*＝＝＝＝＝＝＝＝＝＝＝＝＝＝Wave関係＝＝＝＝＝＝＝＝＝＝＝＝＝＝*/
private:
	int m_nNowWave;	//現在ウェーブ番号
	int m_nMaxWave;	//最大ウェーブ数

public:
	int GetNowWave(void) { return m_nNowWave; }					//現在Wave番号のGet

	void SetMaxWave(int InWaveNum) { m_nMaxWave = InWaveNum; }	//最大ウェーブ数のSet
	int GetMaxWave(void) { return m_nMaxWave; }					//最大ウェーブ数のGet	

	/*＝＝＝＝＝＝＝＝＝＝＝＝＝Pattern関係＝＝＝＝＝＝＝＝＝＝＝＝＝＝*/
private:
	int m_nSelectPattern;	//選択されたパターン
	int m_nMaxPattern;		//最大パターン数

public:
	int GetSelectPattern(void) { return m_nSelectPattern; }					//選択されたパターンのGet
	void RandomSelectPattern(void);											//用意された範囲でパターンを選ぶ

	void SetMaxPattern(int InMaxPattern) { m_nMaxPattern = InMaxPattern; }	//最大パターン数のSet
	int GetMaxPattern(void) { return m_nMaxPattern; }						//最大パターン数のGet

};
//FieldVertex.h
//編集者：AT12A25 中島聖羅
#pragma once
#define _DIRECTX_
#include "DirectX.h"
#include "Character.h"
#include "Battle.h"
#include <d3d11.h>
#include "Field.h"
#include "Sprite.h"
#include "ModelEx.h"
#include "StarLine.h" // ---- 3D用のやつ←これ
#include "EffectManager_sp.h"

#define MAX_VERTEX (25)//星(頂点)数
#define MAX_CENTER_VERTEX (16)//交点の数
#define MAX_SHAPE_VERTEX (10)//このゲームでつくれる最大N角形
#define MAX_LINE (300)//最大の線の本数
#define MAX_LOG (300) //ログの最大数

class CPlayer;//前方宣言(プレイヤーのアドレス保存用の変数を作成するため)

class CFieldVertex
{
public:
	CFieldVertex();//初期化コンストラクタ
	~CFieldVertex();//終了デストラクタ

	void Update();//FieldVertexのメイン更新処理
	void LogUpdate();//ログの更新処理

	void Draw();//FieldVertexのメイン描画処理
	void FeverDraw();//フィーバー時の描画用関数

	DirectX::XMFLOAT3 GetVertexPos(int);//引数の頂点番号の座標を返す関数
	bool GetRoadStop(int);//引数でもらった方向が進めるかどうかの情報を渡す関数
	float GetFeverPoint() { return nFeverPoint; }//フィーバーポイントを取得する関数

	void SetBattleAddress(CBattle*);//CBattleクラスのアドレス情報をセットする関数
	void SetPlayerAddress(CPlayer*);//CPlayerクラスのアドレス情報をセットする関数
	void SetStarLineAddress(StarLine*);//StarLineクラスのアドレス情報をセットする関数
	void SetMasterVolume();//マスターヴォリュームを設定

	void InitFieldVertex();//ゲーム中にFieldVertexの情報初期化する
	void ResetFeverPoint();//フィーバーポイントをリセットする関数

	void SetNowLine();//一番最後の線を表示しない用にする関数
	void SetSuperStar();//スーパースター(綺羅星)の位置を設定する関数

	void SoundStop();//FieldVertexの音を止める関数

	void SubtractFeverPoint();//フィーバー中にフィーバーゲージを減らす関数

	void InitTextureModel();//TextureとModelの初期化

	void InitSound();//音の初期化
private:

	// ログの構造体 //
	typedef struct Log 
	{
		DirectX::XMFLOAT3 Pos;
		int type;//0→三角形 1→四角形
		float time;
		int MoveType;//動きのタイプ 1→右から左 2→下から上 3→右から左
		float Alpha;//透明度
	};

	// 星(頂点)の構造体 //
	typedef struct FieldVertex 
	{
		DirectX::XMFLOAT3 Pos;//星(頂点)の座標
		bool Use;//星(頂点)が使われているか
		bool SuperStar;//スーパースター(綺羅星)
		bool SuperStarUse;//スーパースター(綺羅星)使ったか
		int Connect[8];//この星(頂点)に繋がっている星(頂点)番号保存用
		int Number;//星(頂点)の番号保存用
		float Angle[3];//星(頂点)を回すために使う配列(x,y,z)
	};

	// 交点の構造体 //
	typedef struct CenterVertex 
	{
		DirectX::XMFLOAT3 Pos;//交点の座標
		bool Use;//使用しているか
	};

private:

	void ShapesCheck(FieldVertex VertexNumber);//多角形判定再帰処理
	void DrawSetting(DirectX::XMFLOAT3 InPos, DirectX::XMFLOAT3 InSize ,DirectX::XMFLOAT3 InAngle, Sprite* InSprite);
	void DrawStarModel(int color, int Vertex);//色と頂点番号を使って描画

private:
	
	Log SummonLog[MAX_LOG];//召喚ログの情報保存
	FieldVertex m_tVertex[MAX_VERTEX];//星(頂点)情報保存
	CenterVertex m_tCenter_Vertex[MAX_CENTER_VERTEX];//交点の情報保存

	int NowSummonLog;//今の召喚ログの番号

	int OrderVertex[MAX_LINE + 1];//頂点たどる順番格納
	int OrderVertexCount;//頂点何回たどったか

	int StartVertex;//今の始点
	int GoalVertex;//たどり着いた最新の頂点

	DirectX::XMFLOAT2 Effect_Shapes_Pos[MAX_ALLY];//Effectの座標保存用xy
	int Effect_NowShapes;//今のEffectの開始番号
	int Shapes_Vertex[MAX_ALLY][MAX_VERTEX];//多角形の頂点保存用
	int NowShapes;//今の多角形の保存場所
	int Shapes_Count[MAX_ALLY];//何角形か保存する配列
	int Comparison_Shapes_Vertex_Save[MAX_ALLY][MAX_VERTEX];//比較用の図形の頂点保存
	int Shapes_Vertex_Save[MAX_ALLY][MAX_VERTEX]; //頂点を描画順に並べた頂点情報保存
	int Comparison[MAX_VERTEX];//できた図形の頂点を元々ある図形の頂点と比較
	float Shapes_Size[MAX_ALLY];//できた図形の面積

	int BreakVertex;//壊れる頂点情報

	int SuperStarCount;//スーパースターの個数
	int nFeverPoint;//ポイントカウント
	float fFeverPoint;//ポイントをちょっとづつ増やす用
	float Partition;//分割数
	int Ally_Count;//召喚の数

	int NowLine;//今何個目の線か
	
	DirectX::XMFLOAT3 PlayerPos;//プレイヤーの位置保存
	bool RoadStop;//プレイヤーの行ける方向

	DirectX::XMFLOAT3 Fever_Draw_Angle;//フィーバー時の回転させるための値保存用
	float Fever_Draw_Angle_Count;//フィーバーの描画UIを何回転させるか
	//float

	CBattle* m_pBattle;//バトルクラスのポインター
	CPlayer* m_pPlayer;//プレイヤークラスのポインター
	StarLine* m_pStarLine;

	Texture* m_pTex_SuperStar_Number[6];
	Texture* m_pTex_Fever_Gage[4];
	Texture* m_pTex_Fever_Player;
	Texture* m_pTex_Summon_Log[2];
	Texture* m_pTex_Ally_Count[4];
	Texture* m_pTex_Ally_Number[10];

	Sprite* m_pSprite_SuperStar_Number;//スーパースターの数
	Sprite* m_pSprite_Fever_Gage[4];//フィーバースター
	Sprite* m_pSprite_Fever_Player;//フィーバー時のプレイヤー表示
	Sprite* m_pSprite_Summon_Log;//召喚ログ
	Sprite* m_pSprite_Ally_Count[4];//召喚数
	Sprite* m_pSprite_Ally_Number[10];//召喚Number

	CEffectManager_sp* g_pLineEffects_Sprite;
	CEffectManager_sp* g_pLineEffects[MAX_ALLY];
	CModelEx* m_pStar_Model[3];//Starのモデル
	
};

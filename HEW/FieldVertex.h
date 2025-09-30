//FieldVertex.h
//編集者：AT12A25 中島聖羅
#pragma once
#define _DIRECTX_
#include "Battle.h"
#include "StarLine.h" 

#define MAX_VERTEX (25)//星(頂点)数
#define MAX_CENTER_VERTEX (16)//交点の数
#define MAX_SHAPE_VERTEX (8)//このゲームでつくれる最大N角形
#define MAX_LINE (85)//最大の線の本数
#define MAX_LOG (300) //ログの最大数

class CPlayer;//前方宣言(プレイヤーのアドレス保存用の変数を作成するため)

class CFieldVertex
{
public:
	CFieldVertex();//初期化コンストラクタ
	~CFieldVertex();//終了デストラクタ

	void Update();//FieldVertexのメイン更新処理
	void LogUpdate();//ログの更新処理

	void ShapesEffectDraw();//図形から召喚のエフェクト
	void Draw();//FieldVertexのメイン描画処理
	void FeverDraw();//フィーバー時の描画用関数

	void ShapesUpdate();//できた図形を更新する関数
	void ShapesDraw();//できた図形を描画する関数

	DirectX::XMFLOAT3 GetVertexPos(int);//引数の頂点番号の座標を返す関数
	bool GetRoadStop(int);//引数でもらった方向が進めるかどうかの情報を渡す関数
	float GetFeverPoint() { return m_nFeverPoint; }//フィーバーポイントを取得する関数
	void SetFeverPoint();//フィーバーポイント同期

	void SetBattleAddress(CBattle*);//CBattleクラスのアドレス情報をセットする関数
	void SetPlayerAddress(CPlayer*);//CPlayerクラスのアドレス情報をセットする関数
	void SetStarLineAddress(StarLine*);//StarLineクラスのアドレス情報をセットする関数
	void SetMasterVolume();//マスターヴォリュームを設定

	void InitFieldVertex();//ゲーム中にFieldVertexの情報初期化する
	void ResetFeverPoint();//フィーバーポイントをリセットする関数

	void SetNowLine();//一番最後の線を表示しない用にする関数
	void SetSuperStar();//スーパースター(綺羅星)の位置を設定する関数
	void ClearSuperStar();//スーパースター(綺羅星)をクリアする関数

	void SoundStop();//FieldVertexの音を止める関数

	void SubtractFeverPoint();//フィーバー中にフィーバーゲージを減らす関数

	void InitTextureModel();//TextureとModelの初期化

	void InitSound();//音の初期化

	void SetFeverInclease(bool isExec) { m_bFeverGaugeInclease = isExec; }
	void SetVertexStop(bool isMove, int Index) { m_bVertexLoadStop[Index] = isMove; }
private:

	// ログの構造体 //
	typedef struct Log
	{
		DirectX::XMFLOAT3 Pos;//ログの座標
		int type;//0→三角形 1→四角形
		float time;//ログの表示時間
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
		DirectX::XMFLOAT3 Angle;//星(頂点)を回すために使う配列(x,y,z)
	};

	// 交点の構造体 //
	typedef struct CenterVertex
	{
		DirectX::XMFLOAT3 Pos;//交点の座標
		bool Use;//使用しているか
	};

private:

	void ShapesCheck(FieldVertex VertexNumber);//多角形判定再帰処理
	void DrawSetting(DirectX::XMFLOAT3 InPos, DirectX::XMFLOAT3 InSize, DirectX::XMFLOAT3 InAngle, Sprite* InSprite);//描画時の座標と大きさをセットする
	void DrawStarModel(int color, int Vertex);//色と頂点番号を使って描画

private:
	//----- 星(頂点) -----
	FieldVertex m_tVertex[MAX_VERTEX];//星(頂点)情報保存
	CenterVertex m_tCenter_Vertex[MAX_CENTER_VERTEX];//交点の情報保存
	int m_OrderVertex[MAX_LINE + 1];//頂点たどる順番格納
	int m_OrderVertexCount;//頂点何回たどったか
	int m_StartVertex;//今の始点
	int m_GoalVertex;//たどり着いた最新の頂点

	//----- 図形 -----
	int m_Shapes_Vertex[MAX_ALLY][MAX_VERTEX];//多角形の頂点保存用
	int m_NowShapes;//今の多角形の保存場所
	int m_Shapes_Count[MAX_ALLY];//何角形か保存する配列
	int m_Shapes_Vertex_Save[MAX_ALLY][MAX_VERTEX]; //頂点を描画順に並べた頂点情報保存
	int m_Comparison_Shapes_Vertex_Save[MAX_ALLY][MAX_VERTEX];//比較用の図形の頂点保存
	int m_Comparison[MAX_VERTEX];//できた図形の頂点を元々ある図形の頂点と比較

	//----- ログ -----
	Log m_tSummonLog[MAX_LOG];//召喚ログの情報保存
	int m_NowSummonLog;//今の召喚ログの番号
	float m_MoveFlagStartTime = 0.0f;//ログの移動開始時間
	float m_MoveFlagEndTime = 0.0f;//ログの移動終了時間
	bool m_MoveFlagStart = true;//ログの移動開始タイミング
	bool m_MoveFlagEnd = true;//ログの移動終了タイミング

	//----- 図形のエフェクト -----
	DirectX::XMFLOAT2 m_Effect_Shapes_Pos[MAX_ALLY];//Effectの座標保存用xy
	int m_Effect_NowShapes;//今のEffectの開始番号
	DirectX::XMFLOAT3 m_ShapesEffects_Pos[MAX_ALLY][3];//図形から召喚のエフェクトの座標
	float m_ShapesEffects_Pos_Add[MAX_ALLY][2];//図形から召喚のエフェクトの座標の移動量

	//----- 図形描画 -----
	float m_Shapes_Draw_Time;//図形描画の間隔
	int m_Now_Shapes_Draw; //今表示している図形番号
	float m_Shapes_Size[MAX_ALLY];//できた図形の面積
	int m_Shapes_type_Angle[MAX_ALLY][2];//[0][0]=type; [0][1]=Angle;(0～3)
	float m_Shapes_Angle_Save[MAX_ALLY][4][2];
	float m_Shapes_Color_Time[MAX_ALLY];
	DirectX::XMFLOAT3 m_Shapes_Pos[MAX_ALLY];//図形の座標
	float m_Shapes_Length[MAX_ALLY][2];//縦と横の長さ
	
	//----- フィーバー・綺羅星 -----
	int m_SuperStarCount;//スーパースターの個数
	int m_nFeverPoint;//ポイントカウント(int型)
	float m_fFeverPoint;//ポイントをちょっとづつ増やす用(float型)
	float m_Partition;//フィーバーゲージ分割数
	DirectX::XMFLOAT3 m_Fever_Draw_Angle;//フィーバー時の回転させるための値保存用
	float m_Fever_Draw_Angle_Count;//フィーバーの描画UIを何回転させるか
	DirectX::XMFLOAT3 m_Fever_Player_Draw_Pos;//フィーバー時のプレイヤーの座標
	int m_Mode_Player_Move;//フィーバー時のプレイヤーの動き方
	float m_Fever_Stop_Player_time; //フィーバー時のプレイヤーの止まっている時間
	float m_Fever_Effects_Alpha;//フィーバーEffectの透明度
	float m_Efect_x = 0.0f;//フィーバーエフェクトの表示時間のイージング保存用
	float m_Efect_y = 0.0f;//フィーバーエフェクトの表示時間のイージング保存用
	bool m_bFeverGaugeInclease = true;
	bool m_bVertexLoadStop[MAX_VERTEX];
	bool m_bFaverStarCreate;
	
	//----- 線・プレイヤー -----
	int m_NowLine;//今何個目の線か
	Sprite::Vertex m_vtx_FieldLine[MAX_LINE][4]; //線の四頂点座標保存用
	DirectX::XMFLOAT3 m_PlayerPos;//プレイヤーの位置保存
	bool m_RoadStop;//プレイヤーの行ける方向

	//----- サウンド -----
	IXAudio2SourceVoice* m_FieldSe; //FieldVertexのサウンド音量
	CSoundList* m_Fieldsound; //FieldVertexのサウンドポインター

	//----- 他クラスポインター -----
	CBattle* m_pBattle;//バトルクラスのポインター
	CPlayer* m_pPlayer;//プレイヤークラスのポインター
	StarLine* m_pStarLine;//スターラインクラスのポインター

	//----- 描画関連 -----
	Texture* m_pTex_SuperStar_Number[6];
	Texture* m_pTex_Fever_Gage[4];
	Texture* m_pTex_Fever_Player;
	Texture* m_pTex_Summon_Log[2];
	Texture* m_pTex_Ally_Count[4];
	Texture* m_pTex_Ally_Number[10];
	Texture* m_pTex_Shapes[32];

	Sprite* m_pSprite_SuperStar_Number;//スーパースターの数
	Sprite* m_pSprite_Fever_Gage[4];//フィーバースター
	Sprite* m_pSprite_Fever_Player;//フィーバー時のプレイヤー表示
	Sprite* m_pSprite_Summon_Log;//召喚ログ
	Sprite* m_pSprite_Ally_Count[4];//召喚数
	Sprite* m_pSprite_Ally_Number[10];//召喚Number
	Sprite* m_pSprite_Shapes;

	CEffectManager_sp* m_pLineEffects_Sprite;
	CEffectManager_sp* m_pLineEffects[MAX_ALLY];
	CEffectManager_sp* m_pShapesEffects_Sprite;
	CEffectManager_sp* m_pShapesEffects[MAX_ALLY];
	CEffectManager_sp* m_pFeverEffects_Sprite;
	CEffectManager_sp* m_pFeverEffects[32];

	CModelEx* m_pStar_Model[3];//Starのモデル
	
	//----- その他 -----
	int m_BreakVertex;//壊れる頂点情報
	int m_Ally_Count;//召喚の数

	float Size = 0.0f;


	

	
	
};

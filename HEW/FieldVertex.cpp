//FieldVertex.cpp
//編集者：AT12A25 中島聖羅
#include "FieldVertex.h"
#include "DirectXTex/TextureLoad.h"
#include "Player.h"
#include <math.h>
#include "SceneGame.h"
#include "_StructList.h"
#include "Defines.h"
#include "Main.h"
#include "SoundList.h"

//図形のやつ
//三角形を回転させる(辺が一番長いところの2頂点が外心の上下左右を同時に満たす所を取得するればいけそう)
//四角形を回転させる(????)
//特殊な三角形の判定(1種類)
//特殊な四角形の判定(4種類)
//ひし形と正方形扱い変える
//正方形と長方形どうする


//↓FieldVertexのまだやってない事↓
//各変数名の修正
//コンストラクタとupdateのクリーンアップ
//ヘッダーのクリーンアップ
//グローバル領域のクリーンアップ
//全体のコメントアウト見直し

#define MAX_SHAPES_DRAW_TIME (0.75f)
#define TRIANGLE_SIZE (90.0f)//三角形のサイズ
#define SQUARE_SIZE (80.0f)//正方形のサイズ
#define TRAPEZOID_SIZE (80.0f)//台形のサイズ
#define PARALLELOGRAM_SIZE (80.0f)//平行四辺形のサイズ
#define SPECIAL_SHAPE_SIZE (80.0f)//カッター型の図形のサイズ

#define VERTEX_POS_X (-30.0f)//星(頂点)の描画位置X
#define VERTEX_POS_Y (95.0f)//星(頂点)の描画位置Y
#define VERTEX_SPACE_X (15.0f)//星(頂点)間の間隔X
#define VERTEX_SPACE_Y (15.0f)//星(頂点)間の間隔Y

#define LINE_SIZE (0.1f)//線のサイズ
#define STAR_SIZE (10.0f)//星(頂点)のサイズ

#define FEVER_ADD_TIME (1.0f)//フィーバーのステラ取った時の追加時間
#define FEVER_TIME (10.0f)//フィーバーの時間
#define MAX_FEVER_POINT (30.0f)//フィーバーゲージの上限ポイント
#define FEVER_GAGE_POS_X (-83.0f)//フィーバーゲージのX座標
#define FEVER_GAGE_POS_Y (65.0f)//フィーバーゲージのY座標

#define FEVER_PLAYER_DRAW_POS_X (0.0f)
#define FEVER_PLAYER_DRAW_POS_Y (-87.0f)//下限-63.0f 中限-30.0f　上限-27.0f
#define FEVER_STOP_PLAYER_TIME (1.0f)//フィーバー中プレイヤーが止まっている時間

#define SUMMON_LOG_SIZE_X (40.0f)//ログのXサイズ
#define SUMMON_LOG_SIZE_Y (10.0f)//ログのYサイズ
#define MAX_DRAW_LOG (15)//ログの描画数
#define DRAW_LOG_TIME (0.5f) //ログの終始の表示時間
#define DRAW_MAIN_LOG_TIME (2.0f) //ログの中間の表示時間
#define FADE_LOG_TIME (0.25f) //ログが移動する間隔
#define FADE_LOG_SPEED (0.5f) //ログの終始のスピード
#define MAIN_LOG_SPEED (1.0f) //ログの中間のスピード

#define BOARD_SIZE_X (20.0f)
#define BOARD_SIZE_Y (20.0f)

#define FEVER_DRAW_ANGLE_TIME (1.0f)//フィーバーの描画UIを何秒で1回転させるか
#define FEVER_DRAW_ANGLE_COUNT (2.0f)//フィーバーの描画UIを何回転させるか

Sprite::Vertex vtx_FieldLine[MAX_LINE][4];//線の四頂点座標保存用
IXAudio2SourceVoice* g_FieldSe;//FieldVertexのサウンド音量
CSoundList* g_Fieldsound;//FieldVertexのサウンドポインター

float Xx = 0.0f;
float Yy = 0.0f;

int SubDeleteCount = 0;
float MoveFlagStartTime = 0.0f;
float MoveFlagEndTime = 0.0f;
bool MoveFlagStart = true;
bool MoveFlagEnd = true;

float Size = 0.0f;

CFieldVertex::CFieldVertex()
	: m_tVertex{}
	, m_tCenter_Vertex{}
	, SummonLog{}
	, NowSummonLog(0)
	, OrderVertex{}
	, OrderVertexCount(0)
	, StartVertex()
	, GoalVertex()
	, Effect_Shapes_Pos{}
	, Effect_NowShapes(0)
	, NowShapes(0)
	, Shapes_Count{}
	, Comparison_Shapes_Vertex_Save{}
	, Shapes_Vertex_Save{}
	, Comparison{}
	, BreakVertex(-1)
	, SuperStarCount(0)
	, nFeverPoint(0)
	, fFeverPoint(0.0f)
	, Partition(MAX_FEVER_POINT)
	, Fever_Draw_Angle{}
	, Fever_Draw_Angle_Count (FEVER_DRAW_ANGLE_COUNT)
	, Fever_Player_Draw_Pos{}
	, Fever_Stop_Player_time(0.0f)
	, Mode_Player_Move(0)
	, Ally_Count(0)
	, NowLine(0)
	, Shapes_Draw_Time (0.0f)
	, Now_Shapes_Draw (0)
	, PlayerPos{}
	, RoadStop(false)
	, m_pBattle(nullptr)
	, m_pPlayer(nullptr)
	, m_pTex_SuperStar_Number{ nullptr }
	, m_pTex_Fever_Gage{ nullptr }
	, m_pTex_Fever_Player(nullptr)
	, m_pTex_Summon_Log{ nullptr }
	, m_pTex_Ally_Number{ nullptr }
	, m_pTex_Ally_Count{ nullptr }
	, m_pTex_Shapes{ nullptr }
	, m_pSprite_SuperStar_Number(nullptr)
	, m_pSprite_Fever_Gage{ nullptr }
	, m_pSprite_Fever_Player (nullptr)
	, m_pSprite_Summon_Log(nullptr)
	, m_pSprite_Ally_Number{ nullptr }
	, m_pSprite_Ally_Count{ nullptr }
	, m_pSprite_Shapes{ nullptr }
	, g_pFeverEffects_Sprite(nullptr)
	, g_pFeverEffects{nullptr}
	, g_pLineEffects_Sprite( nullptr)
	, g_pLineEffects{nullptr}
	, m_pStar_Model{ nullptr }
	, m_pStarLine(nullptr)
	, Fever_Effects_Alpha(1.0f)
	, Shapes_Pos{}
    , Shapes_type_Angle{}
	, Shapes_Angle_Save{}
	, Shapes_Length{}
	, Shapes_Color_Time{}
{
	//-----サウンドの初期化-----//
	{
		g_Fieldsound = new CSoundList(SE_COMPLETE);//サウンドのメモリ確保
		g_Fieldsound->SetMasterVolume();//
		g_FieldSe = g_Fieldsound->GetSound(false);//
	}

	//-----スプライトのメモリ確保-----//
	{
		/*m_pSprite_SuperStar_Number = new Sprite();*/
		m_pSprite_Fever_Gage[0] = new Sprite();
		m_pSprite_Fever_Gage[1] = new Sprite();
		m_pSprite_Fever_Gage[2] = new Sprite();
		m_pSprite_Fever_Gage[3] = new Sprite();
		m_pSprite_Fever_Player = new Sprite();
		m_pSprite_Summon_Log = new Sprite();
		m_pSprite_Ally_Count[0] = new Sprite();
		m_pSprite_Ally_Count[1] = new Sprite();
		m_pSprite_Ally_Count[2] = new Sprite();
		m_pSprite_Ally_Count[3] = new Sprite();
		m_pSprite_Shapes = new Sprite();
		for (int i = 0; i < 10; i++)
		{
			m_pSprite_Ally_Number[i] = new Sprite();
		}
	}

	//-----テクスチャのメモリ確保-----//
	{
		m_pTex_Fever_Gage[0] = new Texture();
		m_pTex_Fever_Gage[1] = new Texture();
		m_pTex_Fever_Gage[2] = new Texture();
		m_pTex_Fever_Gage[3] = new Texture();
		m_pTex_Fever_Player = new Texture();
		m_pTex_Summon_Log[0] = new Texture();
		m_pTex_Summon_Log[1] = new Texture();
		m_pTex_Ally_Count[0] = new Texture();
		m_pTex_Ally_Count[1] = new Texture();
		m_pTex_Ally_Count[2] = new Texture();
		m_pTex_Ally_Count[3] = new Texture();
		for (int i = 0; i < 30; i++)
		{
			m_pTex_Shapes[i] = new Texture();
		}
		for (int i = 0; i < 10; i++)
		{
			m_pTex_Ally_Number[i] = new Texture();
		}
		/*for (int i = 0; i < 6; i++)
		{
			m_pTex_SuperStar_Number[i] = new Texture();
		}*/
	}

	m_pStarLine = new StarLine();
	
	g_pFeverEffects_Sprite = new CEffectManager_sp(EFFECT_PASS("Sprite/fever.png"), 5, 10, 2.0f);
	g_pLineEffects_Sprite = new CEffectManager_sp(EFFECT_PASS("Sprite/図形生成.png"), 4, 8, 1.0f);
	for (int i = 0; i < 32; i++)
	{
		g_pFeverEffects[i] = new CEffectManager_sp(g_pFeverEffects_Sprite);
	}
	for (int i = 0; i < MAX_ALLY; i++)
	{
		g_pLineEffects[i] = new CEffectManager_sp(g_pLineEffects_Sprite);
	}

	m_pStar_Model[0] = new CModelEx(MODEL_PASS("Board_Star/Orange/Board_Star_Orange.fbx"));
	m_pStar_Model[1] = new CModelEx(MODEL_PASS("Board_Star/Blue/Board_Star_Blue.fbx"));
	m_pStar_Model[2] = new CModelEx(MODEL_PASS("Board_Star/Red/Board_Star_Red.fbx"));

	StartVertex = START_PLAYER;	// 始点初期化
	GoalVertex = START_PLAYER;	// 終点初期化
	NowShapes = 0;				// 格納した図形の数初期化

	Size = 0.0f;

	// 各配列を-1で初期化
	Fill(OrderVertex, -1);
	Fill(Shapes_Size, -1.0f);
	Fill(Comparison_Shapes_Vertex_Save, -1);
	Fill(Shapes_Vertex, -1);
	Fill(Effect_Shapes_Pos, -1.0f);
	Fill(Shapes_Vertex_Save, -1);
	Fill(Shapes_Count, -1);
	Fill(Comparison, -1);
	Fill(vtx_FieldLine, 0.0f);
	Fill(Shapes_type_Angle, -1);
	Fill(Shapes_Angle_Save, -1.0f);
	Fill(Shapes_Length, -1.0f);
	Fill(Shapes_Color_Time, MAX_SHAPES_DRAW_TIME);

	// 頂点２５個座標情報初期化
	{
		FieldVertex* Vertexp;
		Vertexp = m_tVertex;
		for (int j = 0; j < 5; j++)
		{
			for (int i = 0; i < 5; i++, Vertexp++)
			{
				Vertexp->Pos.x = i * VERTEX_SPACE_X + VERTEX_POS_X;
				Vertexp->Pos.y = -j * VERTEX_SPACE_Y + VERTEX_POS_Y;
				Vertexp->Pos.z = 0;
				Vertexp->Number = j * 5 + i;
				Vertexp->Use = false;
				Vertexp->SuperStar = false;
				Vertexp->Angle[0] = 0.0f;
				Vertexp->Angle[1] = 180.0f;
				Vertexp->Angle[2] = 0.0f;
				for (int k = 0; k < 8; k++)
				{
					Vertexp->Connect[k] = -1;
				}
			}
		}
	}

	//センター頂点16個座標情報初期化
	{
		CenterVertex* CenterVertexp;
		CenterVertexp = m_tCenter_Vertex;
		for (int j = 0; j < 4; j++)
		{
			for (int i = 0; i < 4; i++, CenterVertexp++)
			{
				CenterVertexp->Pos.x = i * VERTEX_SPACE_X + VERTEX_POS_X + VERTEX_SPACE_X / 2.0f;
				CenterVertexp->Pos.y = -j * VERTEX_SPACE_Y + VERTEX_POS_Y - VERTEX_SPACE_Y / 2.0f;
				CenterVertexp->Pos.z = 0;
				CenterVertexp->Use = false;
			}
		}
	}

	//たどる順にプレイヤーの最初の位置保存
	OrderVertex[0] = StartVertex;
	OrderVertexCount = 1;//たどった頂点の数初期化

	m_tVertex[START_PLAYER].Use = true;//最初の頂点を使用に

	SetSuperStar();

	//召喚ログ初期化
	{
		HRESULT hrSummon_Log;
		hrSummon_Log = m_pTex_Summon_Log[0]->Create(TEX_PASS("Summon_Log/Log_Triangular.png"));
		if (FAILED(hrSummon_Log)) {
			MessageBox(NULL, "Summon_Log_Triangular 画像", "Error", MB_OK);
		}
		hrSummon_Log = m_pTex_Summon_Log[1]->Create(TEX_PASS("Summon_Log/Log_Square.png"));
		if (FAILED(hrSummon_Log)) {
			MessageBox(NULL, "Summon_Log_Square 画像", "Error", MB_OK);
		}
	}

	//スーパースタの数字初期化
	{
		/*HRESULT hrSuperStar;
		for (int i = 0; i < 6; i++)
		{
			switch (i)
			{
			case 0:hrSuperStar = m_pTex_SuperStar_Number[0]->Create(TEX_PASS("Numbers/number_0.png")); break;
			case 1:hrSuperStar = m_pTex_SuperStar_Number[1]->Create(TEX_PASS("Numbers/number_1.png")); break;
			case 2:hrSuperStar = m_pTex_SuperStar_Number[2]->Create(TEX_PASS("Numbers/number_2.png")); break;
			case 3:hrSuperStar = m_pTex_SuperStar_Number[3]->Create(TEX_PASS("Numbers/number_3.png")); break;
			case 4:hrSuperStar = m_pTex_SuperStar_Number[4]->Create(TEX_PASS("Numbers/number_4.png")); break;
			case 5:hrSuperStar = m_pTex_SuperStar_Number[5]->Create(TEX_PASS("Numbers/number_5.png")); break;
			default:
				break;
			}
			if (FAILED(hrSuperStar)) {
				MessageBox(NULL, "Vertex 画像", "Error", MB_OK);
			}
		}*/
	}

	//召喚数の数字初期化
	{
		HRESULT hrNumber;
		for (int i = 0; i < 10; i++)
		{
			switch (i)
			{
			case 0:hrNumber = m_pTex_Ally_Number[0]->Create(TEX_PASS("Numbers/number_0.png")); break;
			case 1:hrNumber = m_pTex_Ally_Number[1]->Create(TEX_PASS("Numbers/number_1.png")); break;
			case 2:hrNumber = m_pTex_Ally_Number[2]->Create(TEX_PASS("Numbers/number_2.png")); break;
			case 3:hrNumber = m_pTex_Ally_Number[3]->Create(TEX_PASS("Numbers/number_3.png")); break;
			case 4:hrNumber = m_pTex_Ally_Number[4]->Create(TEX_PASS("Numbers/number_4.png")); break;
			case 5:hrNumber = m_pTex_Ally_Number[5]->Create(TEX_PASS("Numbers/number_5.png")); break;
			case 6:hrNumber = m_pTex_Ally_Number[6]->Create(TEX_PASS("Numbers/number_6.png")); break;
			case 7:hrNumber = m_pTex_Ally_Number[7]->Create(TEX_PASS("Numbers/number_7.png")); break;
			case 8:hrNumber = m_pTex_Ally_Number[8]->Create(TEX_PASS("Numbers/number_8.png")); break;
			case 9:hrNumber = m_pTex_Ally_Number[9]->Create(TEX_PASS("Numbers/number_9.png")); break;
			default:
				break;
			}
			if (FAILED(hrNumber)) {
				MessageBox(NULL, "Numbers 画像", "Error", MB_OK);
			}
		}
	}

	//フィーバーの初期化
	{
		HRESULT hrFeverStar;
		for (int i = 0; i < 4; i++)
		{
			switch (i)
			{
			case 0:hrFeverStar = m_pTex_Fever_Gage[0]->Create(TEX_PASS("Fever_Star/Gray_Fever_Star.png")); break;
			case 1:hrFeverStar = m_pTex_Fever_Gage[1]->Create(TEX_PASS("Fever_Star/Red_Fever_Star.png")); break;
			case 2:hrFeverStar = m_pTex_Fever_Gage[2]->Create(TEX_PASS("Fever_Star/UI11.png")); break;
			case 3:hrFeverStar = m_pTex_Fever_Gage[3]->Create(TEX_PASS("Fever_Star/UI10.png")); break;
			default:
				break;
			}
			if (FAILED(hrFeverStar)) {
				MessageBox(NULL, "Fever_Star 画像", "Error", MB_OK);
			}
		}
		HRESULT hrFeverPlayer;
		hrFeverPlayer = m_pTex_Fever_Player->Create(TEX_PASS("Fever_Star/FieldVertex_Player_UI.png"));
		if (FAILED(hrFeverStar)) {
			MessageBox(NULL, "Fever_Player 画像", "Error", MB_OK);
		}

		Fever_Player_Draw_Pos = { FEVER_PLAYER_DRAW_POS_X,FEVER_PLAYER_DRAW_POS_Y,0.0f};
	}

	//召喚数のボード初期化
	{
		HRESULT hrBoard;
		for (int i = 0; i < 4; i++)
		{
			switch (i)
			{
			case 0:hrBoard = m_pTex_Ally_Count[0]->Create(TEX_PASS("Summon_Count_Board/UI_Ally_3.png")); break;
			case 1:hrBoard = m_pTex_Ally_Count[1]->Create(TEX_PASS("Summon_Count_Board/UI_Ally_4.png")); break;
			case 2:hrBoard = m_pTex_Ally_Count[2]->Create(TEX_PASS("Summon_Count_Board/UI_Enemy_3.png")); break;
			case 3:hrBoard = m_pTex_Ally_Count[3]->Create(TEX_PASS("Summon_Count_Board/UI_Enemy_4.png")); break;
			default:
				break;
			}
			if (FAILED(hrBoard)) {
				MessageBox(NULL, "Summon_Count_Board 画像", "Error", MB_OK);
			}
		}
	}
	HRESULT hrShapes;
	for (int i = 0; i < 30; i++)
	{
		switch (i)
		{
		case 0:hrShapes = m_pTex_Shapes[i]->Create(TEX_PASS("Shapes/Tr00.png")); break;//三角形
		case 1:hrShapes = m_pTex_Shapes[i]->Create(TEX_PASS("Shapes/Tr01.png")); break;//▲

		case 2:hrShapes = m_pTex_Shapes[i]->Create(TEX_PASS("Shapes/Sq00.png")); break;//■正方形

		case 3:hrShapes = m_pTex_Shapes[i]->Create(TEX_PASS("Shapes/Sq24.png")); break;//1:3台形
		case 4:hrShapes = m_pTex_Shapes[i]->Create(TEX_PASS("Shapes/Sq25.png")); break;//2:4
		case 5:hrShapes = m_pTex_Shapes[i]->Create(TEX_PASS("Shapes/Sq11.png")); break;//1:2
		case 6:hrShapes = m_pTex_Shapes[i]->Create(TEX_PASS("Shapes/Sq13.png")); break;//1:3
		case 7:hrShapes = m_pTex_Shapes[i]->Create(TEX_PASS("Shapes/Sq16.png")); break;//1:4
		case 8:hrShapes = m_pTex_Shapes[i]->Create(TEX_PASS("Shapes/Sq07.png")); break;//2:3
		case 9:hrShapes = m_pTex_Shapes[i]->Create(TEX_PASS("Shapes/Sq09.png")); break;//3:4

		case 10:hrShapes = m_pTex_Shapes[i]->Create(TEX_PASS("Shapes/Sq01.png")); break;//1:1平行四辺形右下から左下
		case 11:hrShapes = m_pTex_Shapes[i]->Create(TEX_PASS("Shapes/Sq03.png")); break;//1:2
		case 12:hrShapes = m_pTex_Shapes[i]->Create(TEX_PASS("Shapes/Sq05.png")); break;//1:3
		case 13:hrShapes = m_pTex_Shapes[i]->Create(TEX_PASS("Shapes/Sq26.png")); break;//3:1
		case 14:hrShapes = m_pTex_Shapes[i]->Create(TEX_PASS("Shapes/Sq01.png")); break;//2:1

		case 15:hrShapes = m_pTex_Shapes[i]->Create(TEX_PASS("Shapes/Sq02.png")); break;//1:1平行四辺形左下から右下
		case 16:hrShapes = m_pTex_Shapes[i]->Create(TEX_PASS("Shapes/Sq04.png")); break;//1:2
		case 17:hrShapes = m_pTex_Shapes[i]->Create(TEX_PASS("Shapes/Sq06.png")); break;//1:3
		case 18:hrShapes = m_pTex_Shapes[i]->Create(TEX_PASS("Shapes/Sq27.png")); break;//3:1
		case 19:hrShapes = m_pTex_Shapes[i]->Create(TEX_PASS("Shapes/Sq02.png")); break;//2:1

		case 20:hrShapes = m_pTex_Shapes[i]->Create(TEX_PASS("Shapes/Sq19.png")); break;//1:2左上が尖っている
		case 21:hrShapes = m_pTex_Shapes[i]->Create(TEX_PASS("Shapes/Sq21.png")); break;//2:3
		case 22:hrShapes = m_pTex_Shapes[i]->Create(TEX_PASS("Shapes/Sq23.png")); break;//3:4
		case 23:hrShapes = m_pTex_Shapes[i]->Create(TEX_PASS("Shapes/Sq32.png")); break;//1:3 
		case 24:hrShapes = m_pTex_Shapes[i]->Create(TEX_PASS("Shapes/Sq34.png")); break;//1:4

		case 25:hrShapes = m_pTex_Shapes[i]->Create(TEX_PASS("Shapes/Sq18.png")); break;//1:2右上が尖っている
		case 26:hrShapes = m_pTex_Shapes[i]->Create(TEX_PASS("Shapes/Sq20.png")); break;//2:3
		case 27:hrShapes = m_pTex_Shapes[i]->Create(TEX_PASS("Shapes/Sq22.png")); break;//3:4
		case 28:hrShapes = m_pTex_Shapes[i]->Create(TEX_PASS("Shapes/Sq31.png")); break;//1:3 
		case 29:hrShapes = m_pTex_Shapes[i]->Create(TEX_PASS("Shapes/Sq33.png")); break;//1:4
		default:
			break;
		}
		
		if (FAILED(hrShapes)) {
			MessageBox(NULL, "Shapes 画像", "Error", MB_OK);
		}
	}

	Xx = 0.0f;
	Yy = 0.0f;
}

////=====FieldVertexのデストラクタ=====//
CFieldVertex::~CFieldVertex()
{
	g_FieldSe->Stop();
	g_FieldSe = nullptr;
	delete g_Fieldsound;
	g_FieldSe = nullptr;

	SAFE_DELETE(g_pFeverEffects_Sprite);
	for (int i = 0; i < 32; i++)
	{
		g_pFeverEffects[i] = nullptr;
	}
	SAFE_DELETE(g_pLineEffects_Sprite);
	for (int i = 0; i < MAX_ALLY; i++)
	{
		g_pLineEffects[i] = nullptr;
	}

	SAFE_DELETE(m_pTex_Fever_Gage[0]);
	SAFE_DELETE(m_pTex_Fever_Gage[1]);
	SAFE_DELETE(m_pTex_Fever_Gage[2]);
	SAFE_DELETE(m_pTex_Fever_Gage[3]);
	SAFE_DELETE(m_pTex_Fever_Player);
	SAFE_DELETE(m_pTex_Summon_Log[0]);
	SAFE_DELETE(m_pTex_Summon_Log[1]);
	SAFE_DELETE(m_pTex_Ally_Count[0]);
	SAFE_DELETE(m_pTex_Ally_Count[1]);
	SAFE_DELETE(m_pTex_Ally_Count[2]);
	SAFE_DELETE(m_pTex_Ally_Count[3]);
	for (int i = 0; i < 30; i++)
	{
		SAFE_DELETE(m_pTex_Shapes[i]);
	}
	for (int i = 0; i < 10; i++)
	{
		SAFE_DELETE(m_pTex_Ally_Number[i]);
	}
	/*for (int i = 0; i < 6; i++)
	{
		SAFE_DELETE(m_pTex_SuperStar_Number[i]);
	}*/

	SAFE_DELETE(m_pStarLine);

	/*SAFE_DELETE(m_pSprite_SuperStar_Number);*/
	SAFE_DELETE(m_pSprite_Fever_Gage[0]);
	SAFE_DELETE(m_pSprite_Fever_Gage[1]);
	SAFE_DELETE(m_pSprite_Fever_Gage[2]);
	SAFE_DELETE(m_pSprite_Fever_Gage[3]);
	SAFE_DELETE(m_pSprite_Fever_Player);
	SAFE_DELETE(m_pSprite_Summon_Log);
	SAFE_DELETE(m_pSprite_Ally_Count[0]);
	SAFE_DELETE(m_pSprite_Ally_Count[1]);
	SAFE_DELETE(m_pSprite_Ally_Count[2]);
	SAFE_DELETE(m_pSprite_Ally_Count[3]);
	SAFE_DELETE(m_pSprite_Shapes);

	for (int i = 0; i < 10; i++)
	{
		SAFE_DELETE(m_pSprite_Ally_Number[i]);
	}

	SAFE_DELETE(m_pStar_Model[0]);
	SAFE_DELETE(m_pStar_Model[1]);
	SAFE_DELETE(m_pStar_Model[2]);
}

void CFieldVertex::Update()
{
	FieldVertex* Vertexp;
	CenterVertex* CenterVertexp;
	//プレイヤーの位置の情報を取得
	PlayerPos = m_pPlayer->GetPlayerPos();

	//頂点の壊れる情報取得
	//BreakVertex = ;

	////頂点を壊す処理
	//if (BreakVertex != -1)
	//{
	//	m_tVertex[BreakVertex].Use = false;//使用して無いにする
	//	//壊れる頂点とつながっている頂点の壊れる頂点とのコネクト情報を消す
	//	for (int i = 0; i < 8; i++)
	//	{
	//		if (m_tVertex[BreakVertex].Connect[i] == -1)continue;
	//		for (int j = 0; j < 8; j++)
	//		{
	//			if (m_tVertex[m_tVertex[BreakVertex].Connect[i]].Connect[j] == BreakVertex)//壊れる頂点がコネクトに入っていたら
	//			{
	//				m_tVertex[m_tVertex[BreakVertex].Connect[i]].Connect[j] = -1;//初期化
	//			}
	//		}
	//	}
	//	Fill(m_tVertex[BreakVertex].Connect, -1);//壊れる頂点のコネクト情報消す
	//	for (int i = 0,j = 0, NowLine = 0; OrderVertex[i] != -1; i++)
	//	{
	//		if (OrderVertex[i] == BreakVertex || OrderVertex[i + 1] == BreakVertex)continue;
	//		vtx_FieldLine[j][0] = vtx_FieldLine[i][0];
	//		j++;//壊れた頂点を使用してないとき増やす
	//		NowLine++;
	//	}
	//}

	//今のフェーズがDrawの時、繋がっている頂点を各頂点に保存する

	Vertexp = m_tVertex;
	for (int i = 0; i < MAX_VERTEX; i++, Vertexp++)
	{
		//プレイヤーと頂点の場所が同じで、かつ、最終地点が今の地点でないかつ壊れている頂点でないとき
		if (PlayerPos.x == Vertexp->Pos.x && PlayerPos.y == Vertexp->Pos.y && m_tVertex[GoalVertex].Number != Vertexp->Number && Vertexp->Number != BreakVertex)
		{
			OrderVertex[OrderVertexCount] = Vertexp->Number;//着いた頂点の番号を保存
			OrderVertexCount++;//次の場所にカウントアップする

			int ConnectSave[2] = { -1,-1 };//コネクトを保存する位置を保存する

			//前の頂点とつなげる
			for (int j = 7; j >= 0; j--)//頂点の格納位置を決定する
			{
				if (m_tVertex[GoalVertex].Connect[j] == -1)ConnectSave[0] = j;
				if (Vertexp->Connect[j] == -1)ConnectSave[1] = j;
			}

			//お互いの頂点をつなげる
			m_tVertex[GoalVertex].Connect[ConnectSave[0]] = Vertexp->Number;
			Vertexp->Connect[ConnectSave[1]] = m_tVertex[GoalVertex].Number;

			//交点重複判定
			CenterVertexp = m_tCenter_Vertex;
			for (int j = 0; j < MAX_CENTER_VERTEX; j++, CenterVertexp++)
			{
				//繋がっている２点の中心がセンター頂点と等しいかどうか
				if (((m_tVertex[GoalVertex].Pos.x + Vertexp->Pos.x) / 2.0f == CenterVertexp->Pos.x) && ((m_tVertex[GoalVertex].Pos.y + Vertexp->Pos.y) / 2.0f == CenterVertexp->Pos.y))
				{
					if (!CenterVertexp->Use)CenterVertexp->Use = true;
				}
			}

			//コネクト処理が終わったので終点を今の地点に設定
			GoalVertex = Vertexp->Number;

			for (int i = 0; i < MAX_LINE + 1; i++)
			{
				if (OrderVertex[i] > 24)
				{
					MessageBox(NULL, "存在しない頂点をたどっています!", "Error", MB_OK);
				}
			}

			//多角形判定
			if (OrderVertex[3] != -1 && Vertexp->Use)//たどってきた頂点が４つ以上ならかつ今の頂点が過去に使われた時、多角形判定
			{
				ShapesCheck(m_tVertex[StartVertex]);//再帰処理(開始頂点のみ)
			}
			Vertexp->Use = true;//頂点が追加されたので今の頂点をtureに
			Vertexp->Angle[1] = 181.0f;
			NowLine++;//線の数増やす
		}
	}

	//描画のための更新処理
	int PlayerDestination;//プレイヤーの方向保存
	PlayerDestination = m_pPlayer->GetPlayerDestination();//プレイヤーの方向取得
	DirectX::XMFLOAT3 PosA[4];//回転後座標格納用
	DirectX::XMFLOAT3 Size;//線のサイズ

	Size.x = LINE_SIZE;
	Size.y = LINE_SIZE;

	//線の回転
	PosA[0].x = -Size.x * 0.5f * cosf(TORAD(45 * PlayerDestination)) - (-Size.y * 0.5f) * sinf(TORAD(45 * PlayerDestination));
	PosA[0].y = -Size.x * 0.5f * sinf(TORAD(45 * PlayerDestination)) + (-Size.y * 0.5f) * cosf(TORAD(45 * PlayerDestination));
	PosA[1].x = -Size.x * 0.5f * cosf(TORAD(45 * PlayerDestination)) - (Size.y * 0.5f) * sinf(TORAD(45 * PlayerDestination));
	PosA[1].y = -Size.x * 0.5f * sinf(TORAD(45 * PlayerDestination)) + (Size.y * 0.5f) * cosf(TORAD(45 * PlayerDestination));
	PosA[2].x = Size.x * 0.5f * cosf(TORAD(45 * PlayerDestination)) - (-Size.y * 0.5f) * sinf(TORAD(45 * PlayerDestination));
	PosA[2].y = Size.x * 0.5f * sinf(TORAD(45 * PlayerDestination)) + (-Size.y * 0.5f) * cosf(TORAD(45 * PlayerDestination));
	PosA[3].x = Size.x * 0.5f * cosf(TORAD(45 * PlayerDestination)) - (Size.y * 0.5f) * sinf(TORAD(45 * PlayerDestination));
	PosA[3].y = Size.x * 0.5f * sinf(TORAD(45 * PlayerDestination)) + (Size.y * 0.5f) * cosf(TORAD(45 * PlayerDestination));

	vtx_FieldLine[NowLine][0].pos[0] = PlayerPos.x + PosA[0].x;//左上のｘ座標
	vtx_FieldLine[NowLine][0].pos[1] = PlayerPos.y + PosA[0].y;//左上のｙ座標
	vtx_FieldLine[NowLine][2].pos[0] = PlayerPos.x + PosA[2].x;//右上のｘ座標
	vtx_FieldLine[NowLine][2].pos[1] = PlayerPos.y + PosA[2].y;//右上のｙ座標

	vtx_FieldLine[NowLine][1].pos[0] = m_tVertex[GoalVertex].Pos.x + PosA[1].x;//左下のｘ座標
	vtx_FieldLine[NowLine][1].pos[1] = m_tVertex[GoalVertex].Pos.y + PosA[1].y;//左下のｙ座標
	vtx_FieldLine[NowLine][3].pos[0] = m_tVertex[GoalVertex].Pos.x + PosA[3].x;//右下のｘ座標
	vtx_FieldLine[NowLine][3].pos[1] = m_tVertex[GoalVertex].Pos.y + PosA[3].y;//右下のｙ座標
}

////=====FieldVertexの描画処理の関数=====//
void CFieldVertex::Draw()
{
	if (GetFeverMode())
	{
		SetRender2D();
		for (int i = 0; i < 32; i++)
		{
			if (!g_pFeverEffects[i]->IsPlay())
			{
				g_pFeverEffects[i]->SetPos({ -100.0f + 30.0f * (i % 8),90.0f - 30.0f * (i / 8),0.0f });
				g_pFeverEffects[i]->SetSize({ 50.0f,50.0f,0.0f });

				g_pFeverEffects[i]->Play(true);
			}
			g_pFeverEffects[i]->SetColor({ 1.0f,1.0f,1.0f,Fever_Effects_Alpha });
			g_pFeverEffects[i]->Update();
			g_pFeverEffects[i]->Draw();
		}

		Xx = powf(2.71828, 1.0f - Yy);
		Yy += (15.0f / (60.0f * 10.0f));
		Fever_Effects_Alpha = Xx;
		//Fever_Effects_Alpha -= 1.0f / (60.0f * 10.0f);
		if (Fever_Effects_Alpha < 0.2f)Fever_Effects_Alpha = 0.2f;
	}

	SetRender3D();//3D描画準備

	//-----線の描画-----//
	{
		/*if (GetFeverMode())m_pStarLine->SetLineMode(1);
		else m_pStarLine->SetLineMode(0);*/
		for (int i = 0; i <= NowLine; i++)
		{
			m_pStarLine->SetLineInfo(//頂点情報格納
			{
					vtx_FieldLine[i][1].pos[0],
					vtx_FieldLine[i][1].pos[1],
					0.0f
			},
		    {
			        vtx_FieldLine[i][3].pos[0],
			        vtx_FieldLine[i][3].pos[1],
			        0.0f
		    },
		    {
			        vtx_FieldLine[i][0].pos[0],
			        vtx_FieldLine[i][0].pos[1],
			        0.0f
		    },
		    {
			        vtx_FieldLine[i][2].pos[0],
			        vtx_FieldLine[i][2].pos[1],
			        0.0f
		    });
			m_pStarLine->DispLine();//線の描画
		}
	}

	//-----頂点(星)の描画-----//
	{
		FieldVertex* Vertexp;//頂点の情報格納ポインター
		Vertexp = m_tVertex;//先頭の情報格納
		for (int i = 0; i < MAX_VERTEX; i++, Vertexp++)
		{
			if (Vertexp->SuperStar)DrawStarModel(2, i);//ステラの描画(レッド)
			else
			{
				if (Vertexp->Use)DrawStarModel(1, i);//使用中の頂点(星)の描画(ブルー)
				else DrawStarModel(0, i);//未使用の頂点(星)の描画(オレンジ)
			}
		}
	}

	SetRender2D();//2D描画準備

	//-----召喚ログ-----//
	{
		for (int i = 0; i < NowSummonLog; i++)
		{
			DrawSetting(SummonLog[i].Pos, { SUMMON_LOG_SIZE_X,SUMMON_LOG_SIZE_Y,1.0f }, {0.0f,0.0f,0.0f},m_pSprite_Summon_Log);//座標と大きさの設定
			m_pSprite_Summon_Log->SetColor({ 1.0f,1.0f,1.0f,SummonLog[i].Alpha });//色と透明度の設定
			if (SummonLog[i].type == 0)m_pSprite_Summon_Log->SetTexture(m_pTex_Summon_Log[0]);//三角形のテクスチャ設定
			else m_pSprite_Summon_Log->SetTexture(m_pTex_Summon_Log[1]);//四角形のテクスチャ設定ログ
			if (i < MAX_DRAW_LOG)m_pSprite_Summon_Log->Draw();//15個のみ描画
		}
		m_pSprite_Summon_Log->ReSetSprite();//スプライトのリセット
	}

	//-----ステラの数描画-----//
	{
		//DrawSetting({ -53.0f, 65.0f,10.0f }, { 20.0f,20.0f,1.0f }, m_pSprite_SuperStar_Number);//座標と大きさの設定
		//m_pSprite_SuperStar_Number->SetColor({ 1.0f,0.2f,0.2f,1.0f });//色と透明度の設定
		//m_pSprite_SuperStar_Number->SetTexture(m_pTex_SuperStar_Number[SuperStarCount]);//任意の数字のテクスチャ設定
		//m_pSprite_SuperStar_Number->Draw();//描画
		//m_pSprite_SuperStar_Number->ReSetSprite();//スプライトのリセット
	}

	//-----フィーバーゲージ描画-----//
	{
		//フィーバー背景//
		if (GetFeverMode())
		{
			Fever_Draw_Angle.y += (360.0f / (60.0f * FEVER_DRAW_ANGLE_TIME));//1.0f秒で１回転
			if (Fever_Draw_Angle.y >= 360.0f)
			{
				Fever_Draw_Angle.y = 0.0f;
				Fever_Draw_Angle_Count++;
			}
			if (Fever_Draw_Angle_Count >= FEVER_DRAW_ANGLE_COUNT)
			{
				Fever_Draw_Angle = { 0.0f,0.0f,0.0f };
			}
		}
		else
		{
			Fever_Draw_Angle = { 0.0f,0.0f,0.0f };
		}
		float Fever_Gage_Size = 50.0f;
		if (GetFeverMode())//フィーバータイムの時、後の背景出現
		{
			DrawSetting({ FEVER_GAGE_POS_X, FEVER_GAGE_POS_Y,10.0f }, { Fever_Gage_Size,Fever_Gage_Size,1.0f }, Fever_Draw_Angle, m_pSprite_Fever_Gage[2]);//座標と大きさの設定
			m_pSprite_Fever_Gage[2]->SetColor({ 1.0f,1.0f,1.0f,1.0f });//色と透明度の設定
			m_pSprite_Fever_Gage[2]->SetTexture(m_pTex_Fever_Gage[2]);//星形の背景のテクスチャ設定
			m_pSprite_Fever_Gage[2]->Draw();//描画
			m_pSprite_Fever_Gage[2]->ReSetSprite();//スプライトのリセット
		}

		DrawSetting({ FEVER_GAGE_POS_X, FEVER_GAGE_POS_Y,10.0f }, { Fever_Gage_Size,Fever_Gage_Size,1.0f }, Fever_Draw_Angle, m_pSprite_Fever_Gage[0]);//座標と大きさの設定
		m_pSprite_Fever_Gage[0]->SetColor({ 1.0f,1.0f,1.0f,1.0f });//色と透明度の設定
		m_pSprite_Fever_Gage[0]->SetTexture(m_pTex_Fever_Gage[0]);//星形の背景のテクスチャ設定
		m_pSprite_Fever_Gage[0]->Draw();//描画
		m_pSprite_Fever_Gage[0]->ReSetSprite();//スプライトのリセット


		if (!GetFeverMode())fFeverPoint += 0.2f;//フィーバータイムじゃないときふやす
		if (fFeverPoint > nFeverPoint)fFeverPoint = nFeverPoint;//値の補正
		DrawSetting({ FEVER_GAGE_POS_X, FEVER_GAGE_POS_Y - Fever_Gage_Size + (Fever_Gage_Size / Partition) * fFeverPoint  ,10.0f }, { Fever_Gage_Size,Fever_Gage_Size,1.0f }, Fever_Draw_Angle, m_pSprite_Fever_Gage[1]);//座標と大きさの設定
		m_pSprite_Fever_Gage[1]->SetUVPos({ 0.0f,1.0f - fFeverPoint / Partition });//UVの座標設定
		m_pSprite_Fever_Gage[1]->SetUVScale({ 1.0f,1.0f });//UVの大きさ設定
		m_pSprite_Fever_Gage[1]->SetColor({ 1.0f,1.0f,1.0f,1.0f });//色と透明度の設定
		m_pSprite_Fever_Gage[1]->SetTexture(m_pTex_Fever_Gage[1]);//星形のフィーバーゲージのテクスチャ設定
		m_pSprite_Fever_Gage[1]->Draw();//描画
		m_pSprite_Fever_Gage[1]->ReSetSprite();//スプライトのリセット

		DrawSetting({ FEVER_GAGE_POS_X, FEVER_GAGE_POS_Y,10.0f }, { Fever_Gage_Size,Fever_Gage_Size,1.0f }, Fever_Draw_Angle, m_pSprite_Fever_Gage[3]);//座標と大きさの設定
		m_pSprite_Fever_Gage[3]->SetColor({ 1.0f,1.0f,1.0f,1.0f });//色と透明度の設定
		m_pSprite_Fever_Gage[3]->SetTexture(m_pTex_Fever_Gage[3]);//星形の背景のテクスチャ設定
		m_pSprite_Fever_Gage[3]->Draw();//描画
		m_pSprite_Fever_Gage[3]->ReSetSprite();//スプライトのリセット
	}

	//-----現在のキャラクターの数描画のボード-----//
	{
		for (int i = 0; i < 4; i++)
		{
			switch (i)
			{
			case 0:DrawSetting({ -107.0f, 52.0f,10.0f }, { BOARD_SIZE_X,BOARD_SIZE_Y,1.0f }, { 0.0f,0.0f,0.0f }, m_pSprite_Ally_Count[i]);//座標と大きさの設定
				break;
			case 1:DrawSetting({ -107.0f, 78.0f,10.0f }, { BOARD_SIZE_X,BOARD_SIZE_Y,1.0f }, { 0.0f,0.0f,0.0f }, m_pSprite_Ally_Count[i]);//座標と大きさの設定
				break;
			case 2:DrawSetting({ 67.0f, 55.0f,10.0f }, { BOARD_SIZE_X,BOARD_SIZE_Y,1.0f }, { 0.0f,0.0f,0.0f }, m_pSprite_Ally_Count[i]);//座標と大きさの設定
				break;
			case 3:DrawSetting({ 67.0f, 75.0f,10.0f }, { BOARD_SIZE_X,BOARD_SIZE_Y,1.0f }, { 0.0f,0.0f,0.0f }, m_pSprite_Ally_Count[i]);//座標と大きさの設定
				break;
			default:
				break;
			}
			
			m_pSprite_Ally_Count[i]->SetColor({ 1.0f,1.0f,1.0f,1.0f });//色と透明度の設定
			m_pSprite_Ally_Count[i]->SetTexture(m_pTex_Ally_Count[i]);//星形の背景のテクスチャ設定
			m_pSprite_Ally_Count[i]->Draw();//描画
			m_pSprite_Ally_Count[i]->ReSetSprite();//スプライトのリセット
		}
	}

	//-----現在のキャラクターの数描画-----//
	{
		int Ally_Enemy_Count[4];
		Ally_Enemy_Count[0] = m_pBattle->GetAllyTypeCount(0);
		Ally_Enemy_Count[1] = m_pBattle->GetAllyTypeCount(1);
		Ally_Enemy_Count[2] = m_pBattle->GetEnemyTypeCount(0);
		Ally_Enemy_Count[3] = m_pBattle->GetEnemyTypeCount(1);

		float M = 0.0f;
		float Pos_Y[4];
		Pos_Y[0] = 52.0f;
		Pos_Y[1] = 78.0f;
		Pos_Y[2] = 55.0f;
		Pos_Y[3] = 75.0f;
		for (int i = 0; i < 4; i++)
		{
			if (i == 2)M = 174.0f;
			if (Ally_Enemy_Count[i] / 100 >= 1)
			{
				DrawSetting({ -107.0f + M - 4.0f, Pos_Y[i],10.0f}, {10.0f,10.0f,1.0f}, { 0.0f,0.0f,0.0f }, m_pSprite_Ally_Number[i]);//座標と大きさの設定
				m_pSprite_Ally_Number[i]->SetColor({ 1.0f,1.0f,1.0f,1.0f });//色と透明度の設定
				m_pSprite_Ally_Number[i]->SetTexture(m_pTex_Ally_Number[Ally_Enemy_Count[i] / 100]);//星形の背景のテクスチャ設定
				m_pSprite_Ally_Number[i]->Draw();//描画

				DrawSetting({ -107.0f + M, Pos_Y[i],10.0f }, { 10.0f,10.0f,1.0f }, { 0.0f,0.0f,0.0f }, m_pSprite_Ally_Number[i]);//座標と大きさの設定
				m_pSprite_Ally_Number[i]->SetColor({ 1.0f,1.0f,1.0f,1.0f });//色と透明度の設定
				m_pSprite_Ally_Number[i]->SetTexture(m_pTex_Ally_Number[(Ally_Enemy_Count[i] % 100) / 10]);//星形の背景のテクスチャ設定
				m_pSprite_Ally_Number[i]->Draw();//描画

				DrawSetting({ -107.0f + M + 3.5f, Pos_Y[i] ,10.0f }, { 10.0f,10.0f,1.0f }, { 0.0f,0.0f,0.0f }, m_pSprite_Ally_Number[i]);//座標と大きさの設定
				m_pSprite_Ally_Number[i]->SetColor({ 1.0f,1.0f,1.0f,1.0f });//色と透明度の設定
				m_pSprite_Ally_Number[i]->SetTexture(m_pTex_Ally_Number[Ally_Enemy_Count[i] % 10]);//星形の背景のテクスチャ設定
				m_pSprite_Ally_Number[i]->Draw();//描画
			}
			else
			{
				if (Ally_Enemy_Count[i] / 10 >= 1)
				{
					DrawSetting({ -107.0f + M - 2.5f, Pos_Y[i] ,10.0f }, { 10.0f,10.0f,1.0f }, { 0.0f,0.0f,0.0f }, m_pSprite_Ally_Number[i]);//座標と大きさの設定
					m_pSprite_Ally_Number[i]->SetColor({ 1.0f,1.0f,1.0f,1.0f });//色と透明度の設定
					m_pSprite_Ally_Number[i]->SetTexture(m_pTex_Ally_Number[Ally_Enemy_Count[i] / 10]);//星形の背景のテクスチャ設定
					m_pSprite_Ally_Number[i]->Draw();//描画

					DrawSetting({ -107.0f + M + 2.0f, Pos_Y[i] ,10.0f }, { 10.0f,10.0f,1.0f }, { 0.0f,0.0f,0.0f }, m_pSprite_Ally_Number[i]);//座標と大きさの設定
					m_pSprite_Ally_Number[i]->SetColor({ 1.0f,1.0f,1.0f,1.0f });//色と透明度の設定
					m_pSprite_Ally_Number[i]->SetTexture(m_pTex_Ally_Number[Ally_Enemy_Count[i] % 10]);//星形の背景のテクスチャ設定
					m_pSprite_Ally_Number[i]->Draw();//描画
				}
				else
				{
					DrawSetting({ -107.0f + M, Pos_Y[i],10.0f }, { 10.0f,10.0f,1.0f }, { 0.0f,0.0f,0.0f }, m_pSprite_Ally_Number[i]);//座標と大きさの設定
					m_pSprite_Ally_Number[i]->SetColor({ 1.0f,1.0f,1.0f,1.0f });//色と透明度の設定
					m_pSprite_Ally_Number[i]->SetTexture(m_pTex_Ally_Number[Ally_Enemy_Count[i] % 10]);//星形の背景のテクスチャ設定
					m_pSprite_Ally_Number[i]->Draw();//描画
				}
			}
			m_pSprite_Ally_Number[i]->ReSetSprite();//スプライトのリセット
		}
	}

	//-----Effectの描画-----//
	{
		for (int i = Effect_NowShapes; i < MAX_ALLY; i++)
		{
			if (Shapes_Count[i] != -1)
			{
				g_pLineEffects[Effect_NowShapes]->SetSize({ 100.0f + Shapes_Size[Effect_NowShapes] * 20.0f,100.0f + Shapes_Size[Effect_NowShapes] * 20.0f, 0.0f });
				g_pLineEffects[Effect_NowShapes]->SetPos({ Effect_Shapes_Pos[Effect_NowShapes].x - 0.766f, Effect_Shapes_Pos[Effect_NowShapes].y - 1.8f, 0.0f });
				if (Shapes_Count[i] == 3)g_pLineEffects[i]->SetColor({ 1.0f,1.0f,1.0f,1.0f });
				else g_pLineEffects[i]->SetColor({ 0.0f,0.3f,1.0f,1.0f });
				g_pLineEffects[Effect_NowShapes]->Play(false);
				Effect_NowShapes++;
			}
		}
		for (int i = 0; i < Effect_NowShapes; i++)
		{
			if (g_pLineEffects[i]->IsPlay())
			{	
				g_pLineEffects[i]->Update();
				g_pLineEffects[i]->Draw();
			}
		}
	}	
}

void CFieldVertex::FeverDraw()
{
	//-----フィーバープレイヤーの描画-----//
	{
		
		if (GetFeverMode())
		{	
			SetRender2D();
			switch (Mode_Player_Move)
			{
			case 0:
				Fever_Player_Draw_Pos.y += 240.0f / 60.0f;
				if (Fever_Player_Draw_Pos.y > -27.0f)Mode_Player_Move = 1;
				break;

			case 1:
				Fever_Stop_Player_time += 1.0f / 60.0f;
				Fever_Player_Draw_Pos.y -= 240.0f / 60.0f;
				if (Fever_Player_Draw_Pos.y < -30.0f)
				{
					Fever_Player_Draw_Pos.y = -30.0f;
				}
				if (Fever_Stop_Player_time >= FEVER_STOP_PLAYER_TIME)
				{
					Fever_Stop_Player_time = 0.0f;
					Mode_Player_Move = 2;
				}
				break;

			case 2:
				Fever_Player_Draw_Pos.y -= 360.0f / 60.0f;
				if (Fever_Player_Draw_Pos.y < FEVER_PLAYER_DRAW_POS_Y)
				{
					Fever_Player_Draw_Pos.y = FEVER_PLAYER_DRAW_POS_Y;
					Mode_Player_Move = 3;
				}
				break;

			case 3:break;
			default:
				break;
			}
			DrawSetting(Fever_Player_Draw_Pos, { 100.0f,150.0f,1.0f }, { 0.0f,0.0f,0.0f }, m_pSprite_Fever_Player);//座標と大きさの設定
			m_pSprite_Fever_Player->SetColor({ 1.0f,1.0f,1.0f,1.0f });//色と透明度の設定
			m_pSprite_Fever_Player->SetTexture(m_pTex_Fever_Player);//星形の背景のテクスチャ設定
			m_pSprite_Fever_Player->Draw();//描画
			m_pSprite_Fever_Player->ReSetSprite();//スプライトのリセット
		}
	}
}

void CFieldVertex::ShapesDraw()
{
	SetRender2D();
	float size = 0.0f;
	for (int i = 0;i < Effect_NowShapes;i++)
	{
		m_pSprite_Shapes->SetUVPos({ 0.0f,0.0f });		
		m_pSprite_Shapes->SetColor({ 0.5f,0.5f,0.5f,1.0f });
		if(Shapes_Color_Time[i] > 0.0f)m_pSprite_Shapes->SetColor({ 1.0f,1.0f,1.0f,1.0f });
		if (Shapes_Count[i] == 3)//三角形なら
		{
			if (Shapes_type_Angle[i][0] == 0)//直角二等辺三角形
			{
				DrawSetting({ Shapes_Pos[i].x, Shapes_Pos[i].y ,10.0f }, { TRIANGLE_SIZE + size + (TRIANGLE_SIZE + size) * (sqrtf(Shapes_Size[i] * 2.0f) - 1.0f),TRIANGLE_SIZE + size + (TRIANGLE_SIZE + size) * (sqrtf(Shapes_Size[i] * 2.0f) - 1.0f),1.0f }, { 0.0f,0.0f,-Shapes_type_Angle[i][1] * 90.0f }, m_pSprite_Shapes);//座標と大きさの設定
				m_pSprite_Shapes->SetTexture(m_pTex_Shapes[0]);
			}
			if (Shapes_type_Angle[i][0] == 1)//上向きの直角二等辺三角形
			{
				DrawSetting({ Shapes_Pos[i].x, Shapes_Pos[i].y ,10.0f }, { TRIANGLE_SIZE + size - 30.0f + (TRIANGLE_SIZE + size - 30.0f) * (sqrtf(Shapes_Size[i] * 2.0f) - 1.0f),TRIANGLE_SIZE + size - 30.0f + (TRIANGLE_SIZE + size - 30.0f) * (sqrtf(Shapes_Size[i] * 2.0f) - 1.0f),1.0f }, { 0.0f,0.0f,-Shapes_type_Angle[i][1] * 90.0f }, m_pSprite_Shapes);//座標と大きさの設定
				m_pSprite_Shapes->SetTexture(m_pTex_Shapes[1]);
			}
		}
		else //四角形なら
		{
			switch (Shapes_type_Angle[i][0])
			{
			case 2://正方形
				DrawSetting({ Shapes_Pos[i].x, Shapes_Pos[i].y ,10.0f }, { SQUARE_SIZE + size + (SQUARE_SIZE + size) * (sqrtf(Shapes_Size[i]) - 1),SQUARE_SIZE + size + (SQUARE_SIZE + size) * (sqrtf(Shapes_Size[i]) - 1),1.0f }, { 0.0f,0.0f,0.0f }, m_pSprite_Shapes);//座標と大きさの設定
				m_pSprite_Shapes->SetTexture(m_pTex_Shapes[2]);
				break;
			case 3://ひし形
				DrawSetting({ Shapes_Pos[i].x, Shapes_Pos[i].y ,10.0f }, { SQUARE_SIZE + size + (SQUARE_SIZE + size) * (sqrtf(Shapes_Size[i]) - 1),SQUARE_SIZE + size + (SQUARE_SIZE + size) * (sqrtf(Shapes_Size[i]) - 1),1.0f }, { 0.0f,0.0f,45.0f }, m_pSprite_Shapes);//座標と大きさの設定
				m_pSprite_Shapes->SetTexture(m_pTex_Shapes[2]);
				break;
			case 4://長方形 
				DrawSetting({ Shapes_Pos[i].x, Shapes_Pos[i].y ,10.0f }, { SQUARE_SIZE + size + (SQUARE_SIZE + size) * (Shapes_Length[i][0] - 1),SQUARE_SIZE + size + (SQUARE_SIZE + size) * (Shapes_Length[i][1] - 1),1.0f }, { 0.0f,0.0f,0.0f }, m_pSprite_Shapes);//座標と大きさの設定
				m_pSprite_Shapes->SetTexture(m_pTex_Shapes[2]);
				break;
			case 5://斜めの長方形
				DrawSetting({ Shapes_Pos[i].x, Shapes_Pos[i].y ,10.0f }, { (SQUARE_SIZE + size) * 1.4142f * (Shapes_Length[i][0]),(SQUARE_SIZE + size) * 1.4142f * (Shapes_Length[i][1]),1.0f }, { 0.0f,0.0f,-45.0f * Shapes_type_Angle[i][1] }, m_pSprite_Shapes);//座標と大きさの設定
				m_pSprite_Shapes->SetTexture(m_pTex_Shapes[2]);
				break;
			case 6://台形 //斜めの台形考える必要あり
				if (Shapes_type_Angle[i][1] < 4)
				{
					if (Shapes_Length[i][0] - 1 == 0)//1:3
					{
						DrawSetting({ Shapes_Pos[i].x, Shapes_Pos[i].y ,10.0f }, { TRAPEZOID_SIZE + size ,TRAPEZOID_SIZE + size,1.0f }, { 0.0f,0.0f,-Shapes_type_Angle[i][1] * 90.0f }, m_pSprite_Shapes);//座標と大きさの設定
						m_pSprite_Shapes->SetTexture(m_pTex_Shapes[3]);
					}
					else //3:4
					{
						DrawSetting({ Shapes_Pos[i].x, Shapes_Pos[i].y ,10.0f }, { TRAPEZOID_SIZE + size ,TRAPEZOID_SIZE + size,1.0f }, { 0.0f,0.0f,-Shapes_type_Angle[i][1] * 90.0f }, m_pSprite_Shapes);//座標と大きさの設定
						m_pSprite_Shapes->SetTexture(m_pTex_Shapes[4]);
					}
				}
				else //斜めの台形
				{
					if (Shapes_Length[i][0] == 1.0f && Shapes_Length[i][1] == 2.0f)//1:2
					{
						DrawSetting({ Shapes_Pos[i].x, Shapes_Pos[i].y ,10.0f }, { TRAPEZOID_SIZE + size ,TRAPEZOID_SIZE + size,1.0f }, { 0.0f,0.0f, (Shapes_type_Angle[i][1] - 4) * -90.0f }, m_pSprite_Shapes);//座標と大きさの設定
						m_pSprite_Shapes->SetTexture(m_pTex_Shapes[5]);
					}
					if (Shapes_Length[i][0] == 1.0f && Shapes_Length[i][1] == 3.0f)//1:3
					{
						DrawSetting({ Shapes_Pos[i].x, Shapes_Pos[i].y ,10.0f }, { TRAPEZOID_SIZE + size ,TRAPEZOID_SIZE + size,1.0f }, { 0.0f,0.0f, (Shapes_type_Angle[i][1] - 4) * -90.0f }, m_pSprite_Shapes);//座標と大きさの設定
						m_pSprite_Shapes->SetTexture(m_pTex_Shapes[6]);
					}
					if (Shapes_Length[i][0] == 1.0f && Shapes_Length[i][1] == 4.0f)//1:4
					{
						DrawSetting({ Shapes_Pos[i].x, Shapes_Pos[i].y ,10.0f }, { TRAPEZOID_SIZE + size ,TRAPEZOID_SIZE + size,1.0f }, { 0.0f,0.0f, (Shapes_type_Angle[i][1] - 4) * -90.0f }, m_pSprite_Shapes);//座標と大きさの設定
						m_pSprite_Shapes->SetTexture(m_pTex_Shapes[7]);
					}
					if (Shapes_Length[i][0] == 2.0f && Shapes_Length[i][1] == 3.0f)//2:3
					{
						DrawSetting({ Shapes_Pos[i].x, Shapes_Pos[i].y ,10.0f }, { TRAPEZOID_SIZE + size ,TRAPEZOID_SIZE + size,1.0f }, { 0.0f,0.0f, (Shapes_type_Angle[i][1] - 4) * -90.0f }, m_pSprite_Shapes);//座標と大きさの設定
						m_pSprite_Shapes->SetTexture(m_pTex_Shapes[8]);
					}
					if (Shapes_Length[i][0] == 2.0f && Shapes_Length[i][1] == 4.0f)//2:4
					{
						DrawSetting({ Shapes_Pos[i].x, Shapes_Pos[i].y ,10.0f }, { (TRAPEZOID_SIZE + size) * 2.0f ,(TRAPEZOID_SIZE + size) * 2.0f,1.0f }, { 0.0f,0.0f, (Shapes_type_Angle[i][1] - 4) * -90.0f }, m_pSprite_Shapes);//座標と大きさの設定
						m_pSprite_Shapes->SetTexture(m_pTex_Shapes[5]);
					}
					if (Shapes_Length[i][0] == 3.0f && Shapes_Length[i][1] == 4.0f)//3:4
					{
						DrawSetting({ Shapes_Pos[i].x, Shapes_Pos[i].y ,10.0f }, { TRAPEZOID_SIZE + size ,TRAPEZOID_SIZE + size,1.0f }, { 0.0f,0.0f, (Shapes_type_Angle[i][1] - 4) * -90.0f }, m_pSprite_Shapes);//座標と大きさの設定
						m_pSprite_Shapes->SetTexture(m_pTex_Shapes[9]);
					}
				}
				break;
			case 7://平行四辺形(右下から左上)
				if (Shapes_Length[i][0] == 1.0f && Shapes_Length[i][1] == 1.0f)//1:1
				{
					DrawSetting({ Shapes_Pos[i].x, Shapes_Pos[i].y ,10.0f }, { PARALLELOGRAM_SIZE + size ,PARALLELOGRAM_SIZE + size,1.0f }, { 0.0f,0.0f, Shapes_type_Angle[i][1] * -90.0f }, m_pSprite_Shapes);//座標と大きさの設定
					m_pSprite_Shapes->SetTexture(m_pTex_Shapes[10]);
				}
				if (Shapes_Length[i][0] == 1.0f && Shapes_Length[i][1] == 2.0f)//1:2
				{
					DrawSetting({ Shapes_Pos[i].x, Shapes_Pos[i].y ,10.0f }, { PARALLELOGRAM_SIZE + size,PARALLELOGRAM_SIZE + size,1.0f }, { 0.0f,0.0f, Shapes_type_Angle[i][1] * -90.0f }, m_pSprite_Shapes);//座標と大きさの設定
					m_pSprite_Shapes->SetTexture(m_pTex_Shapes[11]);
				}
				if (Shapes_Length[i][0] == 1.0f && Shapes_Length[i][1] == 3.0f)//1:3
				{
					DrawSetting({ Shapes_Pos[i].x, Shapes_Pos[i].y ,10.0f }, { PARALLELOGRAM_SIZE + size ,PARALLELOGRAM_SIZE + size,1.0f }, { 0.0f,0.0f, Shapes_type_Angle[i][1] * -90.0f }, m_pSprite_Shapes);//座標と大きさの設定
					m_pSprite_Shapes->SetTexture(m_pTex_Shapes[12]);
				}
				if (Shapes_Length[i][0] == 3.0f && Shapes_Length[i][1] == 1.0f)//3:1
				{
					DrawSetting({ Shapes_Pos[i].x, Shapes_Pos[i].y ,10.0f }, { PARALLELOGRAM_SIZE + size,PARALLELOGRAM_SIZE + size,1.0f }, { 0.0f,0.0f, Shapes_type_Angle[i][1] * -90.0f }, m_pSprite_Shapes);//座標と大きさの設定
					m_pSprite_Shapes->SetTexture(m_pTex_Shapes[13]);
				}
				if (Shapes_Length[i][0] == 2.0f && Shapes_Length[i][1] == 1.0f)//2:1
				{
					DrawSetting({ Shapes_Pos[i].x, Shapes_Pos[i].y ,10.0f }, { PARALLELOGRAM_SIZE + size ,PARALLELOGRAM_SIZE + size,1.0f }, { 0.0f,0.0f, Shapes_type_Angle[i][1] * -90.0f }, m_pSprite_Shapes);//座標と大きさの設定
					m_pSprite_Shapes->SetTexture(m_pTex_Shapes[14]);
				}
				if (Shapes_Length[i][0] == 2.0f && Shapes_Length[i][1] == 2.0f)//2:2
				{
					DrawSetting({ Shapes_Pos[i].x, Shapes_Pos[i].y ,10.0f }, { (PARALLELOGRAM_SIZE + size) * 2.0f ,(PARALLELOGRAM_SIZE + size) * 2.0f,1.0f }, { 0.0f,0.0f, Shapes_type_Angle[i][1] * -90.0f }, m_pSprite_Shapes);//座標と大きさの設定
					m_pSprite_Shapes->SetTexture(m_pTex_Shapes[10]);
				}
				break;
			case 8://平行四辺形(左下から右上)
				if (Shapes_Length[i][0] == 1.0f && Shapes_Length[i][1] == 1.0f)//1:1
				{
					DrawSetting({ Shapes_Pos[i].x, Shapes_Pos[i].y ,10.0f }, { PARALLELOGRAM_SIZE + size ,PARALLELOGRAM_SIZE + size,1.0f }, { 0.0f,0.0f, Shapes_type_Angle[i][1] * -90.0f }, m_pSprite_Shapes);//座標と大きさの設定
					m_pSprite_Shapes->SetTexture(m_pTex_Shapes[15]);
				}
				if (Shapes_Length[i][0] == 1.0f && Shapes_Length[i][1] == 2.0f)//1:2
				{
					DrawSetting({ Shapes_Pos[i].x, Shapes_Pos[i].y ,10.0f }, { PARALLELOGRAM_SIZE + size ,PARALLELOGRAM_SIZE + size,1.0f }, { 0.0f,0.0f, Shapes_type_Angle[i][1] * -90.0f }, m_pSprite_Shapes);//座標と大きさの設定
					m_pSprite_Shapes->SetTexture(m_pTex_Shapes[16]);
				}
				if (Shapes_Length[i][0] == 1.0f && Shapes_Length[i][1] == 3.0f)//1:3
				{
					DrawSetting({ Shapes_Pos[i].x, Shapes_Pos[i].y ,10.0f }, { PARALLELOGRAM_SIZE + size,PARALLELOGRAM_SIZE + size,1.0f }, { 0.0f,0.0f, Shapes_type_Angle[i][1] * -90.0f }, m_pSprite_Shapes);//座標と大きさの設定
					m_pSprite_Shapes->SetTexture(m_pTex_Shapes[17]);
				}
				if (Shapes_Length[i][0] == 3.0f && Shapes_Length[i][1] == 1.0f)//3:1
				{
					DrawSetting({ Shapes_Pos[i].x, Shapes_Pos[i].y ,10.0f }, { PARALLELOGRAM_SIZE + size ,PARALLELOGRAM_SIZE + size,1.0f }, { 0.0f,0.0f, Shapes_type_Angle[i][1] * -90.0f }, m_pSprite_Shapes);//座標と大きさの設定
					m_pSprite_Shapes->SetTexture(m_pTex_Shapes[18]);
				}
				if (Shapes_Length[i][0] == 2.0f && Shapes_Length[i][1] == 1.0f)//2:1
				{
					DrawSetting({ Shapes_Pos[i].x, Shapes_Pos[i].y ,10.0f }, { PARALLELOGRAM_SIZE + size,PARALLELOGRAM_SIZE + size,1.0f }, { 0.0f,0.0f, Shapes_type_Angle[i][1] * -90.0f }, m_pSprite_Shapes);//座標と大きさの設定
					m_pSprite_Shapes->SetTexture(m_pTex_Shapes[19]);
				}
				if (Shapes_Length[i][0] == 2.0f && Shapes_Length[i][1] == 2.0f)//2:2
				{
					DrawSetting({ Shapes_Pos[i].x, Shapes_Pos[i].y ,10.0f }, { (PARALLELOGRAM_SIZE + size) * 2.0f ,(PARALLELOGRAM_SIZE + size) * 2.0f,1.0f }, { 0.0f,0.0f, Shapes_type_Angle[i][1] * -90.0f }, m_pSprite_Shapes);//座標と大きさの設定
					m_pSprite_Shapes->SetTexture(m_pTex_Shapes[15]);
				}
				break;
			case 9://カッター型(左尖り)
				m_pSprite_Shapes->SetUVPos({ 0.04f,0.0f });
				if ((Shapes_Length[i][0] == 1.0f && Shapes_Length[i][1] == 2.0f) || (Shapes_Length[i][0] == 2.0f && Shapes_Length[i][1] == 1.0f))//1:2
				{
					DrawSetting({ Shapes_Pos[i].x, Shapes_Pos[i].y ,10.0f }, { SPECIAL_SHAPE_SIZE + size ,SPECIAL_SHAPE_SIZE + size,1.0f }, { 0.0f,0.0f, Shapes_type_Angle[i][1] * -90.0f }, m_pSprite_Shapes);//座標と大きさの設定
					m_pSprite_Shapes->SetTexture(m_pTex_Shapes[20]);
				}
				if ((Shapes_Length[i][0] == 2.0f && Shapes_Length[i][1] == 3.0f) || (Shapes_Length[i][0] == 3.0f && Shapes_Length[i][1] == 2.0f))//2:3
				{
					DrawSetting({ Shapes_Pos[i].x, Shapes_Pos[i].y ,10.0f }, { SPECIAL_SHAPE_SIZE + size,SPECIAL_SHAPE_SIZE + size,1.0f }, { 0.0f,0.0f, Shapes_type_Angle[i][1] * -90.0f }, m_pSprite_Shapes);//座標と大きさの設定
					m_pSprite_Shapes->SetTexture(m_pTex_Shapes[21]);
				}
				if ((Shapes_Length[i][0] == 3.0f && Shapes_Length[i][1] == 4.0f) || (Shapes_Length[i][0] == 4.0f && Shapes_Length[i][1] == 3.0f))//3:4
				{
					DrawSetting({ Shapes_Pos[i].x, Shapes_Pos[i].y ,10.0f }, { SPECIAL_SHAPE_SIZE + size ,SPECIAL_SHAPE_SIZE + size,1.0f }, { 0.0f,0.0f, Shapes_type_Angle[i][1] * -90.0f }, m_pSprite_Shapes);//座標と大きさの設定
					m_pSprite_Shapes->SetTexture(m_pTex_Shapes[22]);
				}
				if ((Shapes_Length[i][0] == 1.0f && Shapes_Length[i][1] == 3.0f) || (Shapes_Length[i][0] == 3.0f && Shapes_Length[i][1] == 1.0f))//1:3
				{
					m_pSprite_Shapes->SetUVPos({ 0.085f,0.0f });
					DrawSetting({ Shapes_Pos[i].x, Shapes_Pos[i].y ,10.0f }, { SPECIAL_SHAPE_SIZE + size,SPECIAL_SHAPE_SIZE + size,1.0f }, { 0.0f,0.0f, Shapes_type_Angle[i][1] * -90.0f }, m_pSprite_Shapes);//座標と大きさの設定
					m_pSprite_Shapes->SetTexture(m_pTex_Shapes[23]);
				}
				if ((Shapes_Length[i][0] == 1.0f && Shapes_Length[i][1] == 4.0f) || (Shapes_Length[i][0] == 4.0f && Shapes_Length[i][1] == 1.0f))//1:4
				{
					m_pSprite_Shapes->SetUVPos({ 0.13f,0.0f });
					DrawSetting({ Shapes_Pos[i].x, Shapes_Pos[i].y ,10.0f }, { SPECIAL_SHAPE_SIZE + size,SPECIAL_SHAPE_SIZE + size,1.0f }, { 0.0f,0.0f, Shapes_type_Angle[i][1] * -90.0f }, m_pSprite_Shapes);//座標と大きさの設定
					m_pSprite_Shapes->SetTexture(m_pTex_Shapes[24]);
				}
				if ((Shapes_Length[i][0] == 2.0f && Shapes_Length[i][1] == 4.0f) || (Shapes_Length[i][0] == 4.0f && Shapes_Length[i][1] == 2.0f))//2:4
				{
					DrawSetting({ Shapes_Pos[i].x, Shapes_Pos[i].y ,10.0f }, { (SPECIAL_SHAPE_SIZE + size) * 2.0f,(SPECIAL_SHAPE_SIZE + size) * 2.0f,1.0f }, { 0.0f,0.0f, Shapes_type_Angle[i][1] * -90.0f }, m_pSprite_Shapes);//座標と大きさの設定
					m_pSprite_Shapes->SetTexture(m_pTex_Shapes[20]);
				}
				break;
			case 10://カッター型(右尖り)
				m_pSprite_Shapes->SetUVPos({ -0.04f,0.0f });
				if ((Shapes_Length[i][0] == 1.0f && Shapes_Length[i][1] == 2.0f) || (Shapes_Length[i][0] == 2.0f && Shapes_Length[i][1] == 1.0f))//1:2
				{
					DrawSetting({ Shapes_Pos[i].x, Shapes_Pos[i].y ,10.0f }, { SPECIAL_SHAPE_SIZE + size ,SPECIAL_SHAPE_SIZE + size,1.0f }, { 0.0f,0.0f, Shapes_type_Angle[i][1] * -90.0f }, m_pSprite_Shapes);//座標と大きさの設定
					m_pSprite_Shapes->SetTexture(m_pTex_Shapes[25]);
				}
				if ((Shapes_Length[i][0] == 2.0f && Shapes_Length[i][1] == 3.0f) || (Shapes_Length[i][0] == 3.0f && Shapes_Length[i][1] == 2.0f))//2:3
				{
					DrawSetting({ Shapes_Pos[i].x, Shapes_Pos[i].y ,10.0f }, { SPECIAL_SHAPE_SIZE + size ,SPECIAL_SHAPE_SIZE + size,1.0f }, { 0.0f,0.0f, Shapes_type_Angle[i][1] * -90.0f }, m_pSprite_Shapes);//座標と大きさの設定
					m_pSprite_Shapes->SetTexture(m_pTex_Shapes[26]);
				}
				if ((Shapes_Length[i][0] == 3.0f && Shapes_Length[i][1] == 4.0f) || (Shapes_Length[i][0] == 4.0f && Shapes_Length[i][1] == 3.0f))//3:4
				{
					DrawSetting({ Shapes_Pos[i].x, Shapes_Pos[i].y ,10.0f }, { SPECIAL_SHAPE_SIZE + size ,SPECIAL_SHAPE_SIZE + size,1.0f }, { 0.0f,0.0f, Shapes_type_Angle[i][1] * -90.0f }, m_pSprite_Shapes);//座標と大きさの設定
					m_pSprite_Shapes->SetTexture(m_pTex_Shapes[27]);
				}
				if ((Shapes_Length[i][0] == 1.0f && Shapes_Length[i][1] == 3.0f) || (Shapes_Length[i][0] == 3.0f && Shapes_Length[i][1] == 1.0f))//1:3
				{
					m_pSprite_Shapes->SetUVPos({ -0.085f,0.0f });
					DrawSetting({ Shapes_Pos[i].x, Shapes_Pos[i].y ,10.0f }, { SPECIAL_SHAPE_SIZE + size ,SPECIAL_SHAPE_SIZE + size,1.0f }, { 0.0f,0.0f, Shapes_type_Angle[i][1] * -90.0f }, m_pSprite_Shapes);//座標と大きさの設定
					m_pSprite_Shapes->SetTexture(m_pTex_Shapes[28]);
				}
				if ((Shapes_Length[i][0] == 1.0f && Shapes_Length[i][1] == 4.0f) || (Shapes_Length[i][0] == 4.0f && Shapes_Length[i][1] == 1.0f))//1:4
				{
					m_pSprite_Shapes->SetUVPos({ -0.13f,0.0f });
					DrawSetting({ Shapes_Pos[i].x, Shapes_Pos[i].y ,10.0f }, { SPECIAL_SHAPE_SIZE + size,SPECIAL_SHAPE_SIZE + size,1.0f }, { 0.0f,0.0f, Shapes_type_Angle[i][1] * -90.0f }, m_pSprite_Shapes);//座標と大きさの設定
					m_pSprite_Shapes->SetTexture(m_pTex_Shapes[29]);
				}
				if ((Shapes_Length[i][0] == 2.0f && Shapes_Length[i][1] == 4.0f) || (Shapes_Length[i][0] == 4.0f && Shapes_Length[i][1] == 2.0f))//2:4
				{
					DrawSetting({ Shapes_Pos[i].x, Shapes_Pos[i].y ,10.0f }, { (SPECIAL_SHAPE_SIZE + size )* 2.0f,(SPECIAL_SHAPE_SIZE + size) * 2.0f,1.0f }, { 0.0f,0.0f, Shapes_type_Angle[i][1] * -90.0f }, m_pSprite_Shapes);//座標と大きさの設定
					m_pSprite_Shapes->SetTexture(m_pTex_Shapes[20]);
				}
				break;

			default:
				DrawSetting({ Shapes_Pos[i].x, Shapes_Pos[i].y ,10.0f }, { 50.0f * Shapes_Size[i],50.0f * Shapes_Size[i],1.0f }, { 0.0f,0.0f,0.0f }, m_pSprite_Shapes);//座標と大きさの設定
				m_pSprite_Shapes->SetTexture(m_pTex_Shapes[2]);
				break;
			}
		}
		m_pSprite_Shapes->Draw();
	}
}

void CFieldVertex::ShapesUpdate()
{
	for (int i = 0; i < Effect_NowShapes;i++)
	{
		if(Shapes_Color_Time[i] > 0.0f)Shapes_Color_Time[i] -= 1.0f / (60.0f * MAX_SHAPES_DRAW_TIME);
	}
	//Shapes_Draw_Time += 1.0f / 60.0f;
	////Size += (40.0f / (60.0f * MAX_SHAPES_DRAW_TIME));
	//if (Shapes_Draw_Time >= MAX_SHAPES_DRAW_TIME)
	//{
	//	Now_Shapes_Draw++;
	//	Shapes_Draw_Time = 0.0f;
	//	//Size = 0.0f;
	//}
	//if (Now_Shapes_Draw > Effect_NowShapes)return false;//全部描画しきったら終了フラグ返す
	//return true;
}

////=====ログの更新処理の関数=====//
void CFieldVertex::LogUpdate()
{
	int DeleteCount = 0;//削除する数を格納する変数

	//スタートログの間隔タイマー処理//
	if (MoveFlagStartTime > 0.0f)MoveFlagStartTime += 1.0f / 60.0f;
	if (MoveFlagStartTime > FADE_LOG_TIME)
	{
		MoveFlagStartTime = 0.0f;
		MoveFlagStart = true;
	}

	//エンドログの間隔タイマー処理//
	if (MoveFlagEndTime > 0.0f)MoveFlagEndTime += 1.0f / 60.0f;
	if (MoveFlagEndTime > FADE_LOG_TIME)
	{
		MoveFlagEndTime = 0.0f;
		MoveFlagEnd = true;
	}

	int SDC = 0;//３の処理の数を格納する変数
	for (int i = 0; SummonLog[i].MoveType == 3; i++)SDC++;//３の処理の数をふやす

	for (int i = 0; i < NowSummonLog; i++)
	{
		if (i - SDC < MAX_DRAW_LOG)
		{
			switch (SummonLog[i].MoveType)
			{
			case 1://スタートログの処理
				SummonLog[i].time -= 1.0f / 60.0f;//タイマー
				SummonLog[i].Alpha += 1.0f / (60.0f * FADE_LOG_SPEED);//透明度
				SummonLog[i].Pos.x -= 40.0f / (60.0f * FADE_LOG_SPEED);//ログの移動
				if (SummonLog[i].Pos.x < 97.0f)SummonLog[i].Pos.x = 97.0f;//値の補正
				if (SummonLog[i].time <= 0.0f && MoveFlagStart)
				{
					//１の処理を抜けて２の処理の初期化
					SummonLog[i].MoveType = 2;
					SummonLog[i].time = DRAW_MAIN_LOG_TIME;
					SummonLog[i].Alpha = 1.0f;
					MoveFlagStartTime += 1.0f / 60.0f;
					MoveFlagStart = false;
				}
				break;
			case 2://メインログの処理
				SummonLog[i].time -= 1.0f / 60.0f;//タイマー
				SummonLog[i].Pos.y += 0.5f;//ログの移動
				if (SummonLog[i].Pos.y > 100.0f - (i - SDC) * 5.0f)SummonLog[i].Pos.y -= 0.5f;//値の補正
				if (SummonLog[i].time <= 0.0f && SummonLog[i].Pos.y == 100.0f && MoveFlagEnd)
				{
					//２の処理を抜けて３の処理の初期化
					SummonLog[i].MoveType = 3;
					SummonLog[i].time = DRAW_LOG_TIME;
					MoveFlagEnd = false;
					MoveFlagEndTime += 1.0f / 60.0f;
				}
				break;
			case 3://エンドログの処理
				SummonLog[i].time -= 1.0f / 60.0f;//タイマー
				SummonLog[i].Alpha -= 1.0f / (60.0f * FADE_LOG_SPEED);//透明度
				SummonLog[i].Pos.x -= 40.0f / (60.0f * FADE_LOG_SPEED);//ログの移動
				//SummonLog[i].Pos.y += 40.0f / (60.0f * 1.0f);
				if (SummonLog[i].time <= 0.0f && SummonLog[i].Alpha <= 0.0f)DeleteCount = i + 1;//削除対象をカウント
				break;
			default:
				break;
			}
		}
	}

	//削除の数配列を前にずらす
	if (DeleteCount > 0)
	{
		for (int i = 0, j = DeleteCount; j < NowSummonLog; i++, j++)
		{
			SummonLog[i] = SummonLog[j];
		}
		NowSummonLog -= DeleteCount;
	}
}

////=====引数の頂点番号の座標を返す関数=====//
DirectX::XMFLOAT3 CFieldVertex::GetVertexPos(int VertexNumber)
{
	return m_tVertex[VertexNumber].Pos;//頂点座標を返す
}

////=====引数でもらった方向が進めるかどうかの情報を渡す関数=====//
bool CFieldVertex::GetRoadStop(int Direction)
{
	CenterVertex* CenterVertexp;//交点の情報を格納するポインター
	RoadStop = false;//初期化

	//行けない方向を判別
	switch (Direction)
	{
	case 0://上
		if (GoalVertex - 5 < 0)RoadStop = true;//頂点の番号が０より小さい時
		else
		{
			for (int i = 0; i < 8; i++)
			{
				if (m_tVertex[GoalVertex].Connect[i] == m_tVertex[GoalVertex - 5].Number)RoadStop = true;
			}
			if (GoalVertex - 5 == BreakVertex)RoadStop = true;//行き先が壊れた頂点なら
		}
		break;
	case 1://右上
		if (GoalVertex - 5 + 1 < 0 || GoalVertex % 5 == 4)RoadStop = true;//頂点の番号が０より小さい時または今いるとこが右端の時
		else
		{
			CenterVertexp = m_tCenter_Vertex;
			for (int j = 0; j < MAX_CENTER_VERTEX; j++, CenterVertexp++)
			{
				//繋がっている２点の中心がセンター頂点と等しいかどうか
				if (((m_tVertex[GoalVertex].Pos.x + m_tVertex[GoalVertex - 5 + 1].Pos.x) / 2 == CenterVertexp->Pos.x) && ((m_tVertex[GoalVertex].Pos.y + m_tVertex[GoalVertex - 5 + 1].Pos.y) / 2 == CenterVertexp->Pos.y))
				{
					if (CenterVertexp->Use)RoadStop = true;//交点使っていたら行けない
					break;
				}
			}
			//行きたい方向の頂点と繋がっていたら
			for (int i = 0; i < 8; i++)
			{
				if (m_tVertex[GoalVertex].Connect[i] == m_tVertex[GoalVertex - 5 + 1].Number)RoadStop = true;
			}
			if (GoalVertex - 5 + 1 == BreakVertex)RoadStop = true;//行き先が壊れた頂点なら
		}
		break;
	case 2://右
		if (GoalVertex + 1 > 24 || GoalVertex % 5 == 4)RoadStop = true;//頂点の番号が24より大きい時または今いるとこが右端の時
		else
		{
			//行きたい方向の頂点と繋がっていたら
			for (int i = 0; i < 8; i++)
			{
				if (m_tVertex[GoalVertex].Connect[i] == m_tVertex[GoalVertex + 1].Number)RoadStop = true;
			}
			if (GoalVertex + 1 == BreakVertex)RoadStop = true;//行き先が壊れた頂点なら
		}
		break;
	case 3://右下
		if (GoalVertex + 5 + 1 > 24 || GoalVertex % 5 == 4)RoadStop = true;//頂点の番号が24より大きい時または今いるとこが右端の時
		else
		{
			CenterVertexp = m_tCenter_Vertex;
			for (int j = 0; j < MAX_CENTER_VERTEX; j++, CenterVertexp++)
			{
				//繋がっている２点の中心がセンター頂点と等しいかどうか
				if (((m_tVertex[GoalVertex].Pos.x + m_tVertex[GoalVertex + 5 + 1].Pos.x) / 2 == CenterVertexp->Pos.x) && ((m_tVertex[GoalVertex].Pos.y + m_tVertex[GoalVertex + 5 + 1].Pos.y) / 2 == CenterVertexp->Pos.y))
				{
					if (CenterVertexp->Use)RoadStop = true;//交点使っていたら行けない
					break;
				}
			}
			//行きたい方向の頂点と繋がっていたら
			for (int i = 0; i < 8; i++)
			{
				if (m_tVertex[GoalVertex].Connect[i] == m_tVertex[GoalVertex + 5 + 1].Number)RoadStop = true;
			}
			if (GoalVertex + 5 + 1 == BreakVertex)RoadStop = true;//行き先が壊れた頂点なら
		}
		break;
	case 4://下
		if (GoalVertex + 5 > 24)RoadStop = true;//頂点の番号が24より大きい時
		else
		{
			//行きたい方向の頂点と繋がっていたら
			for (int i = 0; i < 8; i++)
			{
				if (m_tVertex[GoalVertex].Connect[i] == m_tVertex[GoalVertex + 5].Number)RoadStop = true;
			}
			if (GoalVertex + 5 == BreakVertex)RoadStop = true;//行き先が壊れた頂点なら
		}
		break;
	case 5://左下
		if (GoalVertex + 5 - 1 > 24 || GoalVertex % 5 == 0)RoadStop = true;//頂点の番号が24より大きい時または今いるとこが左端の時
		else
		{
			CenterVertexp = m_tCenter_Vertex;
			for (int j = 0; j < MAX_CENTER_VERTEX; j++, CenterVertexp++)
			{
				//繋がっている２点の中心がセンター頂点と等しいかどうか
				if (((m_tVertex[GoalVertex].Pos.x + m_tVertex[GoalVertex + 5 - 1].Pos.x) / 2 == CenterVertexp->Pos.x) && ((m_tVertex[GoalVertex].Pos.y + m_tVertex[GoalVertex + 5 - 1].Pos.y) / 2 == CenterVertexp->Pos.y))
				{
					if (CenterVertexp->Use)RoadStop = true;//交点使っていたら行けない
					break;
				}
			}
			//行きたい方向の頂点と繋がっていたら
			for (int i = 0; i < 8; i++)
			{
				if (m_tVertex[GoalVertex].Connect[i] == m_tVertex[GoalVertex + 5 - 1].Number)RoadStop = true;
			}
			if (GoalVertex + 5 - 1 == BreakVertex)RoadStop = true;//行き先が壊れた頂点なら
		}
		break;
	case 6://左
		if (GoalVertex - 1 < 0 || GoalVertex % 5 == 0)RoadStop = true;//頂点の番号が0より小さい時または今いるとこが左端の時
		else
		{
			//行きたい方向の頂点と繋がっていたら
			for (int i = 0; i < 8; i++)
			{
				if (m_tVertex[GoalVertex].Connect[i] == m_tVertex[GoalVertex - 1].Number)RoadStop = true;
			}
			if (GoalVertex - 1 == BreakVertex)RoadStop = true;//行き先が壊れた頂点なら
		}
		break;
	case 7://左上
		if (GoalVertex - 5 - 1 < 0 || GoalVertex % 5 == 0)RoadStop = true;//頂点の番号が0より小さい時または今いるとこが左端の時
		else
		{
			CenterVertexp = m_tCenter_Vertex;
			for (int j = 0; j < MAX_CENTER_VERTEX; j++, CenterVertexp++)
			{
				//繋がっている２点の中心がセンター頂点と等しいかどうか
				if (((m_tVertex[GoalVertex].Pos.x + m_tVertex[GoalVertex - 5 - 1].Pos.x) / 2 == CenterVertexp->Pos.x) && ((m_tVertex[GoalVertex].Pos.y + m_tVertex[GoalVertex - 5 - 1].Pos.y) / 2 == CenterVertexp->Pos.y))
				{
					if (CenterVertexp->Use)RoadStop = true;//交点使っていたら行けない
					break;
				}
			}
			//行きたい方向の頂点と繋がっていたらtrue
			for (int i = 0; i < 8; i++)
			{
				if (m_tVertex[GoalVertex].Connect[i] == m_tVertex[GoalVertex - 5 - 1].Number)RoadStop = true;
			}
			if (GoalVertex - 5 - 1 == BreakVertex)RoadStop = true;//行き先が壊れた頂点なら
		}
		break;
	default:
		break;
	}

	return RoadStop;//いけるかいけないかを返す
}

void CFieldVertex::SetFeverPoint()
{
	fFeverPoint = nFeverPoint;
}

////=====CBattleクラスのアドレス情報をセットする関数=====//
void CFieldVertex::SetBattleAddress(CBattle* InAddress)
{
	m_pBattle = InAddress;
}

////=====CPlayerクラスのアドレス情報をセットする関数=====//
void CFieldVertex::SetPlayerAddress(CPlayer* InAddress)
{
	m_pPlayer = InAddress;
}

void CFieldVertex::SetStarLineAddress(StarLine* InAddress)
{
	m_pStarLine = InAddress;
}

void CFieldVertex::SetMasterVolume()
{
	g_Fieldsound->SetMasterVolume();
}

////=====FieldVertexの頂点情報などを初期化する関数=====//
void CFieldVertex::InitFieldVertex()
{
	//-----頂点２５個初期化-----//
	{
		FieldVertex* Vertexp;//頂点の情報を格納するポインター
		Vertexp = m_tVertex;//先頭のアドレスをセット
		for (int j = 0; j < MAX_VERTEX; j++, Vertexp++)
		{
			Vertexp->Use = false;
			Vertexp->Angle[0] = 0.0f;
			Vertexp->Angle[1] = 180.0f;
			Vertexp->Angle[2] = 0.0f;
			for (int k = 0; k < 8; k++)
			{
				Vertexp->Connect[k] = -1;
			}
		}
	}

	//-----センター頂点16個初期化-----//
	{
		CenterVertex* CenterVertexp;//交点の情報を格納するポインター
		CenterVertexp = m_tCenter_Vertex;//先頭のアドレスをセット
		for (int j = 0; j < MAX_CENTER_VERTEX; j++, CenterVertexp++)
		{
			CenterVertexp->Use = false;
		}
	}

	//-----その他必要な初期化処理-----//
	{
		Fever_Stop_Player_time = 0.0f;
		Mode_Player_Move = 0;
		Fever_Player_Draw_Pos = { FEVER_PLAYER_DRAW_POS_X,FEVER_PLAYER_DRAW_POS_Y,0.0f };
		StartVertex = GoalVertex;//始点を今の地点に初期化
		NowShapes = 0;//格納した図形の数初期化
		Effect_NowShapes = 0;
		Fever_Effects_Alpha = 1.0f;
		Shapes_Draw_Time = 0.0f;
		Now_Shapes_Draw = 0;
		Size = 0.0f;

		Fever_Draw_Angle = { 0.0f,0.0f,0.0f };
		Fever_Draw_Angle_Count = 0.0f;

		Fill(OrderVertex, -1);//配列-1で初期化
		Fill(Shapes_Size, -1.0f);
		Fill(Comparison_Shapes_Vertex_Save, -1);
		Fill(Shapes_Vertex_Save, -1);
		Fill(Effect_Shapes_Pos, -1.0f);
		Fill(Shapes_Vertex, -1);
		Fill(Shapes_Count, -1);
		Fill(Comparison, -1);
		Fill(vtx_FieldLine, 0.0f);
		Fill(Shapes_Pos, -1.0f);
		Fill(Shapes_type_Angle, -1);
		Fill(Shapes_Angle_Save, -1.0f);
		Fill(Shapes_Length, -1.0f);
		Fill(Shapes_Color_Time, MAX_SHAPES_DRAW_TIME);
	
		OrderVertex[0] = StartVertex;//たどる順にプレイヤーの最初の位置保存
		OrderVertexCount = 1;//たどった頂点の数初期化
		m_tVertex[StartVertex].Use = true;//最初の頂点を使用に

		NowLine = 0;//線の情報を初期化

		SuperStarCount = 0;//ステラのカウントを初期化

		Xx = 0.0f;
		Yy = 0.0f;
	}
}

////=====ステラの位置を設定する関数=====//
void CFieldVertex::SetSuperStar()
{
	//頂点２５個初期化
	FieldVertex* Vertexp;//頂点の情報格納ポインター
	Vertexp = m_tVertex;//先頭のアドレスをセット
	int UseCount = 0;//使ったスーパースターを数える
	for (int i = 0; i < MAX_VERTEX; i++, Vertexp++)
	{
		//初期化//
		Vertexp->SuperStar = false;
		Vertexp->SuperStarUse = false;
	}
	
	//新しいステラの設定//
	for (int i = 0; i < 1;)
	{
		int Vertex;//ステラの頂点保存用変数
		Vertex = rand() % 25;//0～24でランダムにセット
		if (!m_tVertex[Vertex].SuperStar && Vertex != GoalVertex && !m_tVertex[Vertex].Use)//既にスーパースターか今いる頂点か既に使用している頂点ならもう一度抽選
		{
			m_tVertex[Vertex].SuperStar = true;
			i++;
		}
	}

}

////=====音を止める関数=====//
void CFieldVertex::SoundStop()
{
	g_FieldSe->Stop();
}

////=====フィーバー中にフィーバーゲージを減らす関数=====//
void CFieldVertex::SubtractFeverPoint()
{
	fFeverPoint -= (MAX_FEVER_POINT / 60.0f) / FEVER_TIME;
	if (fFeverPoint < 0.0f)//値の補正
	{
		nFeverPoint = 0;
		fFeverPoint = 0.0f;
	}
}

////=====モデルの初期化をする関数=====//
void CFieldVertex::InitTextureModel()
{

	//-----スプライトのメモリ確保-----//
	{
	  /*m_pSprite_SuperStar_Number = new Sprite();*/
		m_pSprite_Fever_Gage[0] = new Sprite();
		m_pSprite_Fever_Gage[1] = new Sprite();
		m_pSprite_Fever_Gage[2] = new Sprite();
		m_pSprite_Fever_Gage[3] = new Sprite();
		m_pSprite_Summon_Log = new Sprite();
		m_pSprite_Ally_Count[0] = new Sprite();
		m_pSprite_Ally_Count[1] = new Sprite();
		m_pSprite_Ally_Count[2] = new Sprite();
		m_pSprite_Ally_Count[3] = new Sprite();
		m_pSprite_Fever_Player = new Sprite();
		for (int i = 0; i < 10; i++)
		{
			m_pSprite_Ally_Number[i] = new Sprite();
		}
	}

	//-----テクスチャのメモリ確保-----//
	{
		m_pTex_Fever_Gage[0] = new Texture();
		m_pTex_Fever_Gage[1] = new Texture();
		m_pTex_Fever_Gage[2] = new Texture();
		m_pTex_Fever_Gage[3] = new Texture();
		m_pTex_Summon_Log[0] = new Texture();
		m_pTex_Summon_Log[1] = new Texture();
		m_pTex_Ally_Count[0] = new Texture();
		m_pTex_Ally_Count[1] = new Texture();
		m_pTex_Ally_Count[2] = new Texture();
		m_pTex_Ally_Count[3] = new Texture();
		for (int i = 0; i < 10; i++)
		{
			m_pTex_Ally_Number[i] = new Texture();
		}
	  /*for (int i = 0; i < 6; i++)
		{
			m_pTex_SuperStar_Number[i] = new Texture();
		}*/

		m_pStarLine = new StarLine();

		g_pFeverEffects_Sprite = new CEffectManager_sp(EFFECT_PASS("Sprite/fever.png"), 5, 11, 2.0f);
		g_pLineEffects_Sprite = new CEffectManager_sp(EFFECT_PASS("Sprite/図形生成.png"), 4, 8, 1.0f);
		for (int i = 0; i < 32; i++)
		{
			g_pFeverEffects[i] = new CEffectManager_sp(g_pFeverEffects_Sprite);
		}
		for (int i = 0; i < MAX_ALLY; i++)
		{
			g_pLineEffects[i] = new CEffectManager_sp(g_pLineEffects_Sprite);
		}

		m_pStar_Model[0] = new CModelEx(MODEL_PASS("Board_Star/Orange/Board_Star_Orange.fbx"));
		m_pStar_Model[1] = new CModelEx(MODEL_PASS("Board_Star/Blue/Board_Star_Blue.fbx"));
		m_pStar_Model[2] = new CModelEx(MODEL_PASS("Board_Star/Red/Board_Star_Red.fbx"));
	}

	//召喚ログ初期化
	{
		HRESULT hrSummon_Log;
		hrSummon_Log = m_pTex_Summon_Log[0]->Create(TEX_PASS("Summon_Log/Log_Triangular.png"));
		if (FAILED(hrSummon_Log)) {
			MessageBox(NULL, "Summon_Log_Triangular 画像", "Error", MB_OK);
		}
		hrSummon_Log = m_pTex_Summon_Log[1]->Create(TEX_PASS("Summon_Log/Log_Square.png"));
		if (FAILED(hrSummon_Log)) {
			MessageBox(NULL, "Summon_Log_Square 画像", "Error", MB_OK);
		}
	}

	//スーパースターの個数の数字初期化
	{
		/*HRESULT hrSuperStar;
		for (int i = 0; i < 6; i++)
		{
			switch (i)
			{
			case 0:hrSuperStar = m_pTex_SuperStar_Number[0]->Create(TEX_PASS("Numbers/number_0.png")); break;
			case 1:hrSuperStar = m_pTex_SuperStar_Number[1]->Create(TEX_PASS("Numbers/number_1.png")); break;
			case 2:hrSuperStar = m_pTex_SuperStar_Number[2]->Create(TEX_PASS("Numbers/number_2.png")); break;
			case 3:hrSuperStar = m_pTex_SuperStar_Number[3]->Create(TEX_PASS("Numbers/number_3.png")); break;
			case 4:hrSuperStar = m_pTex_SuperStar_Number[4]->Create(TEX_PASS("Numbers/number_4.png")); break;
			case 5:hrSuperStar = m_pTex_SuperStar_Number[5]->Create(TEX_PASS("Numbers/number_5.png")); break;
			default:
				break;
			}
			if (FAILED(hrSuperStar)) {
				MessageBox(NULL, "Vertex 画像", "Error", MB_OK);
			}
		}*/
	}

	//召喚数の数字の初期化
	{
		HRESULT hrNumber;
		for (int i = 0; i < 10; i++)
		{
			switch (i)
			{
			case 0:hrNumber = m_pTex_Ally_Number[0]->Create(TEX_PASS("Numbers/number_0.png")); break;
			case 1:hrNumber = m_pTex_Ally_Number[1]->Create(TEX_PASS("Numbers/number_1.png")); break;
			case 2:hrNumber = m_pTex_Ally_Number[2]->Create(TEX_PASS("Numbers/number_2.png")); break;
			case 3:hrNumber = m_pTex_Ally_Number[3]->Create(TEX_PASS("Numbers/number_3.png")); break;
			case 4:hrNumber = m_pTex_Ally_Number[4]->Create(TEX_PASS("Numbers/number_4.png")); break;
			case 5:hrNumber = m_pTex_Ally_Number[5]->Create(TEX_PASS("Numbers/number_5.png")); break;
			case 6:hrNumber = m_pTex_Ally_Number[6]->Create(TEX_PASS("Numbers/number_6.png")); break;
			case 7:hrNumber = m_pTex_Ally_Number[7]->Create(TEX_PASS("Numbers/number_7.png")); break;
			case 8:hrNumber = m_pTex_Ally_Number[8]->Create(TEX_PASS("Numbers/number_8.png")); break;
			case 9:hrNumber = m_pTex_Ally_Number[9]->Create(TEX_PASS("Numbers/number_9.png")); break;
			default:
				break;
			}
			if (FAILED(hrNumber)) {
				MessageBox(NULL, "Numbers 画像", "Error", MB_OK);
			}
		}
	}

	//フィーバーの初期化
	{
		HRESULT hrFeverStar;
		for (int i = 0; i < 4; i++)
		{
			switch (i)
			{
			case 0:hrFeverStar = m_pTex_Fever_Gage[0]->Create(TEX_PASS("Fever_Star/Gray_Fever_Star.png")); break;
			case 1:hrFeverStar = m_pTex_Fever_Gage[1]->Create(TEX_PASS("Fever_Star/Red_Fever_Star.png")); break;
			case 2:hrFeverStar = m_pTex_Fever_Gage[2]->Create(TEX_PASS("Fever_Star/UI11.png")); break;
			case 3:hrFeverStar = m_pTex_Fever_Gage[3]->Create(TEX_PASS("Fever_Star/UI10.png")); break;
			default:
				break;
			}
			if (FAILED(hrFeverStar)) {
				MessageBox(NULL, "Fever_Star 画像", "Error", MB_OK);
			}
		}
		m_pTex_Fever_Player->Create(TEX_PASS("Fever_Star/FieldVertex_Player_UI.png"));
	}

	//召喚数のボード初期化
	{
		HRESULT hrBoard;
		for (int i = 0; i < 4; i++)
		{
			switch (i)
			{
			case 0:hrBoard = m_pTex_Ally_Count[0]->Create(TEX_PASS("Summon_Count_Board/UI_Ally_3.png")); break;
			case 1:hrBoard = m_pTex_Ally_Count[1]->Create(TEX_PASS("Summon_Count_Board/UI_Ally_4.png")); break;
			case 2:hrBoard = m_pTex_Ally_Count[2]->Create(TEX_PASS("Summon_Count_Board/UI_Enemy_3.png")); break;
			case 3:hrBoard = m_pTex_Ally_Count[3]->Create(TEX_PASS("Summon_Count_Board/UI_Enemy_4.png")); break;
			default:
				break;
			}
			if (FAILED(hrBoard)) {
				MessageBox(NULL, "Summon_Count_Board 画像", "Error", MB_OK);
			}
		}
	}
}

void CFieldVertex::InitSound()
{
	SAFE_DELETE(g_Fieldsound);
	g_Fieldsound = new CSoundList(SE_COMPLETE);
	g_Fieldsound->SetMasterVolume();
	g_FieldSe = g_Fieldsound->GetSound(false);
}

////=====フィーバーポイントをリセットする関数=====//
void CFieldVertex::ResetFeverPoint()
{
	nFeverPoint = 0;
	fFeverPoint = 0.0f;
}

////=====一番最後の線を表示しない用にする関数=====//
void CFieldVertex::SetNowLine()
{
	NowLine = NowLine - 1;
	if (NowLine < 0)NowLine = 0;
}

////=====図形判定の再帰処理関数=====//
void CFieldVertex::ShapesCheck(FieldVertex VertexNumber)
{
	int NowVertex = 0;//仮頂点保存のどの位置に今格納したのかを保存する
	bool ShapesFlag = false;//保存済みのと比較して保存されていたらfalse
	bool ShapesSaveFalg = false;//保存を行ったかどうか

	for (int k = 0; k < MAX_VERTEX; k++)
	{
		if (NowShapes >= 200)
		{
			MessageBox(NULL, "200以上の図形が保存されました!これ以上保存できません!!", "Error", MB_OK);
		}
		if (Comparison[k] == VertexNumber.Number)//過去に自分と同じ頂点が格納されていたら図形としてみなし格納する	
		{
			int Comparison2[MAX_VERTEX];//過去に自分と同じ頂点が格納されている場所から今の場所までの頂点を保存する//降順に並び替える
			int Comparison3[MAX_VERTEX];//降順にする前の頂点情報
			for (int l = k; l < MAX_VERTEX; l++)
			{
				Comparison2[l - k] = Comparison[l];//Comparison2の先頭から格納する
				Comparison3[l - k] = Comparison[l];
			}
			for (int l = 0; l < k; l++)
			{
				Comparison2[MAX_VERTEX - 1 - l] = -1;//保存しきれなかった部分を初期化
				Comparison3[MAX_VERTEX - 1 - l] = -1;
			}
			for (int l = 0; l < MAX_VERTEX; l++)//降順に並び替える
			{
				for (int m = MAX_VERTEX - 1; m > l; m--)
				{
					if (Comparison2[m - 1] < Comparison2[m])
					{
						int ComparisonSave = Comparison2[m];
						Comparison2[m] = Comparison2[m - 1];
						Comparison2[m - 1] = ComparisonSave;
					}
				}
			}
			for (int l = 0; l < NowShapes; l++)//保存する図形が保存済みかどうか
			{
				int EqualCount = 0;//等しい回数
				for (int m = 0; m < MAX_VERTEX; m++)
				{
					if (Comparison_Shapes_Vertex_Save[l][m] == Comparison2[m])EqualCount++;//等しければカウント増やす
					if (EqualCount == MAX_VERTEX)ShapesFlag = true;//全て等しいなら図形を保存してはいけないフラグ立てる
				}
			}
			if (!ShapesFlag)//保存済みでなければ保存する
			{
				for (int l = 0; l < MAX_VERTEX; l++)
				{
					Comparison_Shapes_Vertex_Save[NowShapes][l] = Comparison2[l];//図形の頂点を保存
					Shapes_Vertex_Save[NowShapes][l] = Comparison3[l];//Draw順の図形頂点保存
				}
				//何角形か判定
				int Count = 0;
				for (int l = 2; l < MAX_VERTEX; l++)
				{
					if (Comparison3[l] != -1)//最後の点まで
					{
						//三点が縦か横の一直線上でないとき
						if (!((m_tVertex[Comparison3[l - 2]].Pos.x == m_tVertex[Comparison3[l - 1]].Pos.x && m_tVertex[Comparison3[l - 1]].Pos.x == m_tVertex[Comparison3[l]].Pos.x)
							|| (m_tVertex[Comparison3[l - 2]].Pos.y == m_tVertex[Comparison3[l - 1]].Pos.y && m_tVertex[Comparison3[l - 1]].Pos.y == m_tVertex[Comparison3[l]].Pos.y)))
						{
							//三点が斜めの一直線上でないとき
							if ((m_tVertex[Comparison3[l - 1]].Pos.x - m_tVertex[Comparison3[l - 2]].Pos.x) * (m_tVertex[Comparison3[l]].Pos.y - m_tVertex[Comparison3[l - 1]].Pos.y)
								!= (m_tVertex[Comparison3[l - 1]].Pos.y - m_tVertex[Comparison3[l - 2]].Pos.y) * (m_tVertex[Comparison3[l]].Pos.x - m_tVertex[Comparison3[l - 1]].Pos.x))
							{
								Shapes_Vertex[NowShapes][Count] = Comparison3[l - 1];//角の頂点番号を保存
								Count++;//辺が繋がってないとき角ができてる
							}
						}
					}
					else
					{
						//最後の辺とその一個前の辺の比較
						//三点が縦か横の一直線上でないとき
						if (!((m_tVertex[Comparison3[l - 2]].Pos.x == m_tVertex[Comparison3[l - 1]].Pos.x && m_tVertex[Comparison3[l - 1]].Pos.x == m_tVertex[Comparison3[0]].Pos.x)
							|| (m_tVertex[Comparison3[l - 2]].Pos.y == m_tVertex[Comparison3[l - 1]].Pos.y && m_tVertex[Comparison3[l - 1]].Pos.y == m_tVertex[Comparison3[0]].Pos.y)))
						{
							//三点が斜めの一直線上でないとき
							if ((m_tVertex[Comparison3[l - 1]].Pos.x - m_tVertex[Comparison3[l - 2]].Pos.x) * (m_tVertex[Comparison3[0]].Pos.y - m_tVertex[Comparison3[l - 1]].Pos.y)
								!= (m_tVertex[Comparison3[l - 1]].Pos.y - m_tVertex[Comparison3[l - 2]].Pos.y) * (m_tVertex[Comparison3[0]].Pos.x - m_tVertex[Comparison3[l - 1]].Pos.x))
							{
								Shapes_Vertex[NowShapes][Count] = Comparison3[l - 1];//角の頂点番号を保存
								Count++;//辺が繋がってないとき角ができてる	
							}
						}
						//最初の辺と最後の辺を比較
						//三点が縦か横の一直線上でないとき
						if (!((m_tVertex[Comparison3[l - 1]].Pos.x == m_tVertex[Comparison3[0]].Pos.x && m_tVertex[Comparison3[0]].Pos.x == m_tVertex[Comparison3[1]].Pos.x)
							|| (m_tVertex[Comparison3[l - 1]].Pos.y == m_tVertex[Comparison3[0]].Pos.y && m_tVertex[Comparison3[0]].Pos.y == m_tVertex[Comparison3[1]].Pos.y)))
						{
							//三点が斜めの一直線上でないとき
							if ((m_tVertex[Comparison3[l - 1]].Pos.x - m_tVertex[Comparison3[0]].Pos.x) * (m_tVertex[Comparison3[1]].Pos.y - m_tVertex[Comparison3[0]].Pos.y)
								!= (m_tVertex[Comparison3[l - 1]].Pos.y - m_tVertex[Comparison3[0]].Pos.y) * (m_tVertex[Comparison3[1]].Pos.x - m_tVertex[Comparison3[0]].Pos.x))
							{
								Shapes_Vertex[NowShapes][Count] = Comparison3[0];//角の頂点番号を保存
								Count++;//辺が繋がってないとき角ができてる
							}
						}
						break;//最後の頂点の処理を終えたら処理抜ける
					}
				}
				//凹角形判定
				bool BadShapes = false;
				if (Count == 3 || Count == 4)
				{
					DirectX::XMFLOAT2 FastVector;//先のベクトル
					DirectX::XMFLOAT2 SecondVector;//後のベクトル
					DirectX::XMFLOAT2 NormalizeFastVector;//正規化後の先ベクトル
					DirectX::XMFLOAT2 NormalizeSecondVector;//正規化後の後のベクトル
					float AngleSave[25];
					int AngleCount = 0;
					float Error = 0.01;//誤差
					int Save = 0;
					Fill(AngleSave, -1);

					for (int l = 2; l < MAX_VERTEX; l++)
					{
						int m = 0;
						DirectX::XMFLOAT2 RotationFastVector;//正規化した先ベクトルに回転行列を掛けたベクトル
						if (Comparison3[l] != -1)//最後の点まで
						{
							//各ベクトルの計算（取得）
							FastVector.x = m_tVertex[Comparison3[l]].Pos.x - m_tVertex[Comparison3[l - 1]].Pos.x;
							FastVector.y = m_tVertex[Comparison3[l]].Pos.y - m_tVertex[Comparison3[l - 1]].Pos.y;

							SecondVector.x = m_tVertex[Comparison3[l - 2]].Pos.x - m_tVertex[Comparison3[l - 1]].Pos.x;
							SecondVector.y = m_tVertex[Comparison3[l - 2]].Pos.y - m_tVertex[Comparison3[l - 1]].Pos.y;

							//求めたベクトルを正規化
							NormalizeFastVector.x = FastVector.x / sqrtf(powf(FastVector.x, 2) + powf(FastVector.y, 2));
							NormalizeFastVector.y = FastVector.y / sqrtf(powf(FastVector.x, 2) + powf(FastVector.y, 2));

							NormalizeSecondVector.x = SecondVector.x / sqrtf(powf(SecondVector.x, 2) + powf(SecondVector.y, 2));
							NormalizeSecondVector.y = SecondVector.y / sqrtf(powf(SecondVector.x, 2) + powf(SecondVector.y, 2));

							for (; m < 8; m++)
							{
								RotationFastVector.x = NormalizeFastVector.x * cosf(TORAD(float(45 * m))) - NormalizeFastVector.y * sinf(TORAD(float(45 * m)));
								RotationFastVector.y = NormalizeFastVector.x * sinf(TORAD(float(45 * m))) + NormalizeFastVector.y * cosf(TORAD(float(45 * m)));
								if (((RotationFastVector.x - Error <= NormalizeSecondVector.x) && (NormalizeSecondVector.x <= RotationFastVector.x + Error)) && ((RotationFastVector.y - Error <= NormalizeSecondVector.y) && (NormalizeSecondVector.y <= RotationFastVector.y + Error)))break;
							}
							AngleSave[AngleCount] = 45 * m;
							if (AngleSave[AngleCount] != 180.0f)
							{
								if (AngleSave[AngleCount] < 180.0f)
								{
									Shapes_Angle_Save[NowShapes][Save][0] = AngleSave[AngleCount];
									Shapes_Angle_Save[NowShapes][Save][1] = Comparison3[l - 1];
								}
								else
								{
									Shapes_Angle_Save[NowShapes][Save][0] = 360.0f - AngleSave[AngleCount];
									Shapes_Angle_Save[NowShapes][Save][1] = Comparison3[l - 1];
								}
								Save++;
							}
							AngleCount++;
							for (m = 1; (m < MAX_VERTEX - 1) && (AngleSave[m] != -1); m++)
							{
								if (fabsf(AngleSave[m] - AngleSave[m - 1]) >= 180.0f)BadShapes = true;
							}
							
						}
						else
						{
							//各ベクトルの計算（取得）
							FastVector.x = m_tVertex[Comparison3[0]].Pos.x - m_tVertex[Comparison3[l - 1]].Pos.x;
							FastVector.y = m_tVertex[Comparison3[0]].Pos.y - m_tVertex[Comparison3[l - 1]].Pos.y;

							SecondVector.x = m_tVertex[Comparison3[l - 2]].Pos.x - m_tVertex[Comparison3[l - 1]].Pos.x;
							SecondVector.y = m_tVertex[Comparison3[l - 2]].Pos.y - m_tVertex[Comparison3[l - 1]].Pos.y;

							//求めたベクトルを正規化
							NormalizeFastVector.x = FastVector.x / sqrtf(powf(FastVector.x, 2) + powf(FastVector.y, 2));
							NormalizeFastVector.y = FastVector.y / sqrtf(powf(FastVector.x, 2) + powf(FastVector.y, 2));

							NormalizeSecondVector.x = SecondVector.x / sqrtf(powf(SecondVector.x, 2) + powf(SecondVector.y, 2));
							NormalizeSecondVector.y = SecondVector.y / sqrtf(powf(SecondVector.x, 2) + powf(SecondVector.y, 2));

							for (m = 0; m < 8; m++)
							{
								RotationFastVector.x = NormalizeFastVector.x * cosf(TORAD(45 * m)) - NormalizeFastVector.y * sinf(TORAD(45 * m));
								RotationFastVector.y = NormalizeFastVector.x * sinf(TORAD(45 * m)) + NormalizeFastVector.y * cosf(TORAD(45 * m));
								if (((RotationFastVector.x - Error <= NormalizeSecondVector.x) && (NormalizeSecondVector.x <= RotationFastVector.x + Error)) && ((RotationFastVector.y - Error <= NormalizeSecondVector.y) && (NormalizeSecondVector.y <= RotationFastVector.y + Error)))break;
							}
							AngleSave[AngleCount] = 45 * m;
							if (AngleSave[AngleCount] != 180.0f)
							{
								if (AngleSave[AngleCount] < 180.0f)
								{
									Shapes_Angle_Save[NowShapes][Save][0] = AngleSave[AngleCount];
									Shapes_Angle_Save[NowShapes][Save][1] = Comparison3[l - 1];
								}
								else
								{
									Shapes_Angle_Save[NowShapes][Save][0] = 360.0f - AngleSave[AngleCount];
									Shapes_Angle_Save[NowShapes][Save][1] = Comparison3[l - 1];
								}
								Save++;
							}
							AngleCount++;
							for (m = 1; (m < MAX_VERTEX - 1) && (AngleSave[m] != -1); m++)
							{
								if (fabsf(AngleSave[m] - AngleSave[m - 1]) >= 180.0f)BadShapes = true;
							}

							//各ベクトルの計算（取得）
							FastVector.x = m_tVertex[Comparison3[1]].Pos.x - m_tVertex[Comparison3[0]].Pos.x;
							FastVector.y = m_tVertex[Comparison3[1]].Pos.y - m_tVertex[Comparison3[0]].Pos.y;

							SecondVector.x = m_tVertex[Comparison3[l - 1]].Pos.x - m_tVertex[Comparison3[0]].Pos.x;
							SecondVector.y = m_tVertex[Comparison3[l - 1]].Pos.y - m_tVertex[Comparison3[0]].Pos.y;

							//求めたベクトルを正規化
							NormalizeFastVector.x = FastVector.x / sqrtf(powf(FastVector.x, 2) + powf(FastVector.y, 2));
							NormalizeFastVector.y = FastVector.y / sqrtf(powf(FastVector.x, 2) + powf(FastVector.y, 2));

							NormalizeSecondVector.x = SecondVector.x / sqrtf(powf(SecondVector.x, 2) + powf(SecondVector.y, 2));
							NormalizeSecondVector.y = SecondVector.y / sqrtf(powf(SecondVector.x, 2) + powf(SecondVector.y, 2));

							for (m = 0; m < 8; m++)
							{
								RotationFastVector.x = NormalizeFastVector.x * cosf(TORAD(45 * m)) - NormalizeFastVector.y * sinf(TORAD(45 * m));
								RotationFastVector.y = NormalizeFastVector.x * sinf(TORAD(45 * m)) + NormalizeFastVector.y * cosf(TORAD(45 * m));
								if (((RotationFastVector.x - Error <= NormalizeSecondVector.x) && (NormalizeSecondVector.x <= RotationFastVector.x + Error)) && ((RotationFastVector.y - Error <= NormalizeSecondVector.y) && (NormalizeSecondVector.y <= RotationFastVector.y + Error)))break;
							}
							AngleSave[AngleCount] = 45 * m;
							if (AngleSave[AngleCount] != 180.0f)
							{
								if (AngleSave[AngleCount] < 180.0f)
								{
									Shapes_Angle_Save[NowShapes][Save][0] = AngleSave[AngleCount];
									Shapes_Angle_Save[NowShapes][Save][1] = Comparison3[0];
								}
								else
								{
									Shapes_Angle_Save[NowShapes][Save][0] = 360.0f - AngleSave[AngleCount];
									Shapes_Angle_Save[NowShapes][Save][1] = Comparison3[0];
								}
								Save++;
							}
							AngleCount++;
							for (m = 1; (m < MAX_VERTEX - 1) && (AngleSave[m] != -1); m++)
							{
								if (fabsf(AngleSave[m] - AngleSave[m - 1]) >= 180.0f)BadShapes = true;
							}
							break;
						}
					}
				}
				if (Count > 4)BadShapes = true;//図形とみなさない(9角形以上の凹角形が存在しないため)BadShapesをtrueにする
				if (BadShapes)//ダメな図形を保存しない（消去する)
				{
					Fill(Shapes_Vertex[NowShapes], -1);
					Fill(Comparison_Shapes_Vertex_Save[NowShapes], -1);
					Fill(Shapes_Vertex_Save[NowShapes], -1);
					Fill(Shapes_Angle_Save[NowShapes], -1);
				}
				if (!BadShapes)
				{
					Shapes_Count[NowShapes] = Count;
					float InVertex = 0;//中の頂点
					float OutVertex = 0;//辺上の頂点
					int l = 0;
					//頂点の個数調べ
					for (; l < MAX_VERTEX; l++)
					{
						if (Comparison2[l] == -1)break;
						OutVertex++;//辺上の頂点の数
					}
					//図形内点判定（同一線上は抜き）

					for (int m = Comparison2[0]; m > Comparison2[l - 1]; m--)
					{
						bool Out = false;
						for (int n = Comparison2[0]; n > Comparison2[l - 1]; n--)
						{
							if (m == Comparison2[n])
							{
								Out = true;//辺上の頂点なら
								break;
							}
						}
						if (!Out)//辺上の頂点じゃないなら
						{
							bool UpVertex = false;
							bool DownVertex = false;
							bool LeftVertex = false;
							bool RightVertex = false;
							//比較頂点を一番左からにする
							for (int n = (m / 5) * 5; n < m; n++)
							{
								if (m_tVertex[n].Use)
								{
									for (int o = 0; o < MAX_VERTEX; o++)//図形に使用している頂点のみ
									{
										if (n == Comparison2[o])
										{
											LeftVertex = true;
										}
									}		
								}
							}
							//比較頂点を一番右からにする
							for (int n = (m / 5) * 5 + 4; n > m; n--)
							{
								if (m_tVertex[n].Use)
								{
									for (int o = 0; o < MAX_VERTEX; o++)//図形に使用している頂点のみ
									{
										if (n == Comparison2[o])
										{
											RightVertex = true;
										}
									}
								}
							}
							//比較頂点を一番上からにする
							for (int n = m % 5; n < m; n += 5)
							{
								if (m_tVertex[n].Use)
								{
									for (int o = 0; o < MAX_VERTEX; o++)//図形に使用している頂点のみ
									{
										if (n == Comparison2[o])
										{
											UpVertex = true;
										}
									}
								}
							}
							//比較頂点を一番下からにする
							for (int n = m % 5 + 5 * 4; n > m; n -= 5)
							{
								if (m_tVertex[n].Use)
								{
									for (int o = 0; o < MAX_VERTEX; o++)//図形に使用している頂点のみ
									{
										if (n == Comparison2[o])
										{
											DownVertex = true;
										}
									}
								}
							}
							if (UpVertex && DownVertex && LeftVertex && RightVertex)InVertex++;//全方向に使っている頂点があれば囲まれているので内側の頂点
						}
					}
					Shapes_Size[NowShapes] = InVertex + OutVertex / 2.0f - 1.0f;//ピックの定理から面積を導く
					m_pBattle->SaveAllyData(Shapes_Count[NowShapes]);//図形の頂点と角数を渡す

					//std::thread Th_Ally_Load(LoadAllyTextureThread);
					//Th_Ally_Load.join();

					Ally_Count++;//召喚数増やす
					//召喚ログセット
					//SummonLog[NowSummonLog].Pos = DirectX::XMFLOAT3(140.0f, 100.0f - 5.0f * NowSummonLog, 10.0f);
					SummonLog[NowSummonLog].Pos = DirectX::XMFLOAT3(137.0f, 50.0f, 10.0f);
					SummonLog[NowSummonLog].time = DRAW_LOG_TIME;
					SummonLog[NowSummonLog].type = Shapes_Count[NowShapes] - 3;//画数から引く (0か1)
					SummonLog[NowSummonLog].Alpha = 0.0f;
					SummonLog[NowSummonLog].MoveType = 1;
					NowSummonLog++;//召喚ログを増やす
					//if (Shapes_Count[NowShapes] == 3)//三角形なら
					//{
					//	DirectX::XMFLOAT2 pos[3];
					//	for (int m = 0; m < 3; m++)
					//	{
					//		pos[m].x = m_tVertex[Shapes_Vertex[NowShapes][m]].Pos.x;
					//		pos[m].y = m_tVertex[Shapes_Vertex[NowShapes][m]].Pos.y;
					//	}
					//	Effect_Shapes_Pos[NowShapes].x = (pos[0].x + pos[1].x + pos[2].x) / 3.0f;
					//	Effect_Shapes_Pos[NowShapes].y = (pos[0].y + pos[1].y + pos[2].y) / 3.0f;
					//}
					//else
					//{
					//	DirectX::XMFLOAT2 pos[4];
					//	for (int m = 0; m < 4; m++)
					//	{
					//		pos[m].x = m_tVertex[Shapes_Vertex[NowShapes][m]].Pos.x;
					//		pos[m].y = m_tVertex[Shapes_Vertex[NowShapes][m]].Pos.y;
					//	}
					//	Effect_Shapes_Pos[NowShapes].x = (pos[0].x + pos[1].x + pos[2].x + pos[3].x) / 4.0f;
					//	Effect_Shapes_Pos[NowShapes].y = (pos[0].y + pos[1].y + pos[2].y + pos[3].y) / 4.0f;
					//}

					if (Shapes_Count[NowShapes] == 3)//三角形なら
					{
						//三角形の外心の座標を求める
						DirectX::XMFLOAT2 pos[3];
						for (int m = 0; m < 3; m++)
						{
							pos[m].x = m_tVertex[Shapes_Vertex[NowShapes][m]].Pos.x;
							pos[m].y = m_tVertex[Shapes_Vertex[NowShapes][m]].Pos.y;
						}
						
						//Shapes_Pos[NowShapes].x = ((powf(pos[0].x, 2) + powf(pos[0].y, 2)) * (pos[1].y - pos[2].y) + (powf(pos[1].x, 2) + powf(pos[1].y, 2)) * (pos[2].y - pos[0].y) + (powf(pos[2].x, 2) + powf(pos[2].y, 2)) * (pos[0].y - pos[1].y)) / (2.0f * ((pos[0].x - pos[1].x) * (pos[1].y - pos[2].y) - (pos[1].x - pos[2].x) * (pos[0].y - pos[1].y)));
						//Shapes_Pos[NowShapes].y = ((powf(pos[0].x, 2) + powf(pos[0].y, 2)) * (pos[1].x - pos[2].x) + (powf(pos[1].x, 2) + powf(pos[1].y, 2)) * (pos[2].x - pos[0].x) + (powf(pos[2].x, 2) + powf(pos[2].y, 2)) * (pos[0].x - pos[1].x)) / (2.0f * ((pos[1].x - pos[2].x) * (pos[0].y - pos[1].y) - (pos[0].x - pos[1].x) * (pos[1].y - pos[2].y)));
						Effect_Shapes_Pos[NowShapes].x = (pos[0].x + pos[1].x + pos[2].x) / 3.0f;
						Effect_Shapes_Pos[NowShapes].y = (pos[0].y + pos[1].y + pos[2].y) / 3.0f;
						bool typeflag = false;
						float CornerAngle = -1.0f;
						int CornerNumber = -1;
						int VertexSub[2] = { -1,-1 };
						float Vx = 0.0f;
						float Vy = 0.0f;
						float Px = 0.0f;
						float Py = 0.0f;
						for (int m = 0,n = 0; m < 3; m++)
						{
							if (Shapes_Angle_Save[NowShapes][m][0] == 90.0f)
							{
								typeflag = true;
								CornerAngle = 90.0f;
								CornerNumber = Shapes_Angle_Save[NowShapes][m][1];
							}
							else
							{						
								VertexSub[n] = Shapes_Angle_Save[NowShapes][m][1];
								n++;
							}
						}
						Px = m_tVertex[VertexSub[0]].Pos.x + m_tVertex[VertexSub[1]].Pos.x;
						Py = m_tVertex[VertexSub[0]].Pos.y + m_tVertex[VertexSub[1]].Pos.y;
						Shapes_Pos[NowShapes].x = Px / 2.0f;
						Shapes_Pos[NowShapes].y = Py / 2.0f;
						Vx = m_tVertex[VertexSub[0]].Pos.x - m_tVertex[CornerNumber].Pos.x + m_tVertex[VertexSub[1]].Pos.x - m_tVertex[CornerNumber].Pos.x;
						Vy = m_tVertex[VertexSub[0]].Pos.y - m_tVertex[CornerNumber].Pos.y + m_tVertex[VertexSub[1]].Pos.y - m_tVertex[CornerNumber].Pos.y;
						//横向きの直角二等辺三角形なら type 0
						if ((Vx > 0.0f && Vy > 0.0f) || (Vx > 0.0f && Vy < 0.0f)|| (Vx < 0.0f && Vy < 0.0f) || (Vx < 0.0f && Vy > 0.0f))
						{
							if (Vx > 0.0f && Vy > 0.0f)Shapes_type_Angle[NowShapes][1] = 0;
							if (Vx > 0.0f && Vy < 0.0f)Shapes_type_Angle[NowShapes][1] = 1;
							if (Vx < 0.0f && Vy < 0.0f)Shapes_type_Angle[NowShapes][1] = 2;
							if (Vx < 0.0f && Vy > 0.0f)Shapes_type_Angle[NowShapes][1] = 3;
							Shapes_type_Angle[NowShapes][0] = 0;
						}
						//上向きの直角二等辺三角形なら type 1
						if ((Vx == 0.0f && Vy > 0.0f)|| (Vx > 0.0f && Vy == 0.0f)|| (Vx == 0.0f && Vy < 0.0f) || (Vx < 0.0f && Vy == 0.0f))
						{
							if (Vx == 0.0f && Vy > 0.0f)Shapes_type_Angle[NowShapes][1] = 2;
							if (Vx > 0.0f && Vy == 0.0f)Shapes_type_Angle[NowShapes][1] = 3;
							if (Vx == 0.0f && Vy < 0.0f)Shapes_type_Angle[NowShapes][1] = 0;
							if (Vx < 0.0f && Vy == 0.0f)Shapes_type_Angle[NowShapes][1] = 1;
							Shapes_type_Angle[NowShapes][0] = 1;
						}	
					}
					else//四角形なら
					{
						DirectX::XMFLOAT2 pos[4];
						for (int m = 0; m < 4; m++)
						{
							pos[m].x = m_tVertex[Shapes_Vertex[NowShapes][m]].Pos.x;
							pos[m].y = m_tVertex[Shapes_Vertex[NowShapes][m]].Pos.y;
						}
						Shapes_Pos[NowShapes].x = (pos[0].x + pos[1].x + pos[2].x + pos[3].x) / 4.0f;
						Shapes_Pos[NowShapes].y = (pos[0].y + pos[1].y + pos[2].y + pos[3].y) / 4.0f;
						Effect_Shapes_Pos[NowShapes].x = (pos[0].x + pos[1].x + pos[2].x + pos[3].x) / 4.0f;
						Effect_Shapes_Pos[NowShapes].y = (pos[0].y + pos[1].y + pos[2].y + pos[3].y) / 4.0f;

						float Length[4][2];//四辺の長さを求める
						for (int m = 0; m < 3; m++)
						{
							if (pos[m].x == pos[m + 1].x)
							{
								Length[m][0] = fabsf(pos[m].y - pos[m + 1].y);
								Length[m][1] = 1;
							}
							else
							{
								if (pos[m].y == pos[m + 1].y)
								{
									Length[m][0] = fabsf(pos[m].x - pos[m + 1].x);
									Length[m][1] = 0;
								}
								else//直角じゃないとき
								{
									Length[m][0] = sqrtf(powf(pos[m].x - pos[m + 1].x, 2) + powf(pos[m].y - pos[m + 1].y, 2));
								}
							}
						}
						if (pos[3].x == pos[0].x)
						{
							Length[3][0] = fabsf(pos[3].y - pos[0].y);
							Length[3][1] = 1;
						}
						else
						{
							if (pos[3].y == pos[0].y)
							{
								Length[3][0] = fabsf(pos[3].x - pos[0].x);
								Length[3][1] = 0;
							}
							else //直角じゃないとき
							{
								Length[3][0] = sqrtf(powf(pos[3].x - pos[0].x, 2) + powf(pos[3].y - pos[0].y, 2));
							}
						}
						//正方形か長方形またはひし形の時
						if (Shapes_Angle_Save[NowShapes][0][0] == 90.0f && Shapes_Angle_Save[NowShapes][1][0] == 90.0f && Shapes_Angle_Save[NowShapes][2][0] == 90.0f && Shapes_Angle_Save[NowShapes][3][0] == 90.0f)
						{

							//各辺の長さによって図形の形を区別する
							if (Length[0][0] == Length[1][0] && Length[2][0] == Length[3][0] && Length[1][0] == Length[2][0])//正方形orひし形
							{
								if ((int)Length[0][0] % 15 == 0)//上下の頂点の間隔と同じなら正方形
								{
									Shapes_type_Angle[NowShapes][0] = 2;//正方形
									Shapes_type_Angle[NowShapes][1] = 0;
								}
								else
								{
									Shapes_type_Angle[NowShapes][0] = 3;//ひし形
									Shapes_type_Angle[NowShapes][1] = 0;
								}
							}
							else
							{

								if (pos[0].x == pos[1].x || pos[0].y == pos[1].y)
								{
									Shapes_type_Angle[NowShapes][0] = 4;//長方形
									Shapes_type_Angle[NowShapes][1] = 0;
									if (Length[0][1] == 0)
									{
										Shapes_Length[NowShapes][0] = Length[0][0] / 15.0f;
										Shapes_Length[NowShapes][1] = Length[1][0] / 15.0f;
									}
									else
									{
										Shapes_Length[NowShapes][0] = Length[1][0] / 15.0f;
										Shapes_Length[NowShapes][1] = Length[0][0] / 15.0f;
									}
								}
								else
								{
									//斜めの長方形
									DirectX::XMFLOAT2 dir;
									dir.x = (pos[0].x + pos[1].x + pos[2].x + pos[3].x) / 4.0f;
									dir.y = (pos[0].y + pos[1].y + pos[2].y + pos[3].y) / 4.0f;
									if ((pos[0].x - dir.x > 0.0f && pos[0].y - dir.y > 0.0f) || (pos[0].x - dir.x < 0.0f && pos[0].y - dir.y < 0.0f))//左から右
									{
										Shapes_type_Angle[NowShapes][0] = 5;//斜めの長方形
										Shapes_type_Angle[NowShapes][1] = -1;
									}
									else
									{
										Shapes_type_Angle[NowShapes][0] = 5;//斜めの長方形
										Shapes_type_Angle[NowShapes][1] = 1;
									}
									if (Length[0][0] == 0)
									{
										Shapes_Length[NowShapes][0] = Length[0][0] / sqrtf(450.0f);
										Shapes_Length[NowShapes][1] = Length[1][0] / sqrtf(450.0f);
									}
									else
									{
										Shapes_Length[NowShapes][0] = Length[1][0] / sqrtf(450.0f);
										Shapes_Length[NowShapes][1] = Length[0][0] / sqrtf(450.0f);
									}
								}
							}
						}
						else
						{
							//カッター型と台形のとき
							if (Length[0][0] != Length[2][0] || Length[1][0] != Length[3][0])
							{
								//一つでも直角があればカッター型
								if (Shapes_Angle_Save[NowShapes][0][0] == 90.0f || Shapes_Angle_Save[NowShapes][1][0] == 90.0f || Shapes_Angle_Save[NowShapes][2][0] == 90.0f || Shapes_Angle_Save[NowShapes][3][0] == 90.0f)
								{
									//図形の中点を求める
									DirectX::XMFLOAT2 dir;
									dir.x = (pos[0].x + pos[1].x + pos[2].x + pos[3].x) / 4.0f;
									dir.y = (pos[0].y + pos[1].y + pos[2].y + pos[3].y) / 4.0f;
									int AngleNumber = 0;
									//鋭角の頂点を求める//辺の比を求める
									for (int m = 0; m < 4; m++)
									{
										if ((int)Length[m][0] % 15 == 0)Length[m][0] = Length[m][0] / 15.0f;
										else Length[m][0] = Length[m][0] / sqrtf(450.0f);
								
										if(Shapes_Angle_Save[NowShapes][m][0] < 90.0f)
										{
											AngleNumber = m;
										}
									}
									//各辺の長さを求める(対辺の比が等しくないものがその図形の辺の比になる)
									if (Length[0][0] == Length[2][0])
									{
										Shapes_Length[NowShapes][0] = Length[1][0];
										Shapes_Length[NowShapes][1] = Length[3][0];
									}
									else
									{
										Shapes_Length[NowShapes][0] = Length[0][0];
										Shapes_Length[NowShapes][1] = Length[2][0];
									}
									if (pos[AngleNumber].x - dir.x > 0.0f && pos[AngleNumber].y - dir.y > 0.0f)
									{
										if (fabsf(pos[AngleNumber].x - dir.x) > fabsf(pos[AngleNumber].y - dir.y))
										{
											Shapes_type_Angle[NowShapes][0] = 9;//カッター型(左凸)
											Shapes_type_Angle[NowShapes][1] = 2;
										}
										else
										{
											Shapes_type_Angle[NowShapes][0] = 10;//カッター型(右凸)
											Shapes_type_Angle[NowShapes][1] = 3;
										}
									}
									if (pos[AngleNumber].x - dir.x > 0.0f && pos[AngleNumber].y - dir.y < 0.0f)
									{
										if (fabsf(pos[AngleNumber].x - dir.x) < fabsf(pos[AngleNumber].y - dir.y))
										{
											Shapes_type_Angle[NowShapes][0] = 9;//カッター型(左凸)
											Shapes_type_Angle[NowShapes][1] = 3;
										}
										else
										{
											Shapes_type_Angle[NowShapes][0] = 10;//カッター型(右凸)
											Shapes_type_Angle[NowShapes][1] = 0;
										}
									}
									if (pos[AngleNumber].x - dir.x < 0.0f && pos[AngleNumber].y - dir.y < 0.0f)
									{
										if (fabsf(pos[AngleNumber].x - dir.x) > fabsf(pos[AngleNumber].y - dir.y))
										{
											Shapes_type_Angle[NowShapes][0] = 9;//カッター型(左凸)
											Shapes_type_Angle[NowShapes][1] = 0;
										}
										else
										{
											Shapes_type_Angle[NowShapes][0] = 10;//カッター型(右凸)
											Shapes_type_Angle[NowShapes][1] = 1;
										}	
									}
									if (pos[AngleNumber].x - dir.x < 0.0f && pos[AngleNumber].y - dir.y > 0.0f)
									{
										if (fabsf(pos[AngleNumber].x - dir.x) < fabsf(pos[AngleNumber].y - dir.y))
										{
											Shapes_type_Angle[NowShapes][0] = 9;//カッター型(左凸)
											Shapes_type_Angle[NowShapes][1] = 1;
										}
										else
										{
											Shapes_type_Angle[NowShapes][0] = 10;//カッター型(右凸)
											Shapes_type_Angle[NowShapes][1] = 2;
										}	
									}
								}
								else //台形
								{
									Shapes_type_Angle[NowShapes][0] = 6;//台形
									float Num[2];
									Num[0] = Length[0][0];
									Num[1] = 0;
									for (int m = 1; m < 4; m++)
									{
										if (Num[0] < Length[m][0])
										{
											Num[0] = Length[m][0];
											Num[1] = m;
										}
									}
									
									DirectX::XMFLOAT2 dirpos;
									
									Num[1] = Num[1] + 2;
									if (Num[1] > 3) Num[1] = Num[1] - 4;
									if (Num[1] == 3)
									{
										dirpos.x = (pos[(int)Num[1]].x + pos[0].x) / 2.0f;
										dirpos.y = (pos[(int)Num[1]].y + pos[0].y) / 2.0f;
									}
									else
									{
										dirpos.x = (pos[(int)Num[1]].x + pos[(int)Num[1] + 1].x) / 2.0f;
										dirpos.y = (pos[(int)Num[1]].y + pos[(int)Num[1] + 1].y) / 2.0f;
									}
									dirpos.x = dirpos.x - Shapes_Pos[NowShapes].x;
									dirpos.y = dirpos.y - Shapes_Pos[NowShapes].y;
									if (dirpos.x == 0.0f && dirpos.y > 0.0f)Shapes_type_Angle[NowShapes][1] = 0;
									if (dirpos.x > 0.0f && dirpos.y == 0.0f)Shapes_type_Angle[NowShapes][1] = 1;
									if (dirpos.x == 0.0f && dirpos.y < 0.0f)Shapes_type_Angle[NowShapes][1] = 2;
									if (dirpos.x < 0.0f && dirpos.y == 0.0f)Shapes_type_Angle[NowShapes][1] = 3;
									if (dirpos.x > 0.0f && dirpos.y > 0.0f)Shapes_type_Angle[NowShapes][1] = 4;
									if (dirpos.x > 0.0f && dirpos.y < 0.0f)Shapes_type_Angle[NowShapes][1] = 5;
									if (dirpos.x < 0.0f && dirpos.y < 0.0f)Shapes_type_Angle[NowShapes][1] = 6;
									if (dirpos.x < 0.0f && dirpos.y > 0.0f)Shapes_type_Angle[NowShapes][1] = 7;
									if (Shapes_type_Angle[NowShapes][1] < 4)
									{
										Shapes_Length[NowShapes][0] = Length[(int)Num[1]][0]/ 15.0f;//上底の長さ保存
										Shapes_Length[NowShapes][1] = Num[0] / 15.0f;//下底の長さ保存
									}
									else
									{
										Shapes_Length[NowShapes][0] = Length[(int)Num[1]][0] / sqrtf(450.0f);//上底の長さ保存
										Shapes_Length[NowShapes][1] = Num[0] / sqrtf(450.0f);//下底の長さ保存
									}
								}
							}
							else//平行四辺形
							{		
								DirectX::XMFLOAT2 dir;
								dir.x = (pos[0].x + pos[1].x + pos[2].x + pos[3].x) / 4.0f;
								dir.y = (pos[0].y + pos[1].y + pos[2].y + pos[3].y) / 4.0f;
								float Num[2];
								int SaveNumber = 0;
								//平行四辺形の中心からの距離が一番遠い頂点を調べる
								Num[0] = sqrtf(powf((pos[0].x - dir.x), 2) + powf((pos[0].y - dir.y), 2));
								Num[1] = sqrtf(powf((pos[1].x - dir.x), 2) + powf((pos[1].y - dir.y), 2));
								if (Num[0] > Num[1])SaveNumber = 0;
								else SaveNumber = 1;
								if ((pos[0].y == pos[1].y || pos[0].y == pos[3].y ) && (pos[2].y == pos[1].y || pos[2].y == pos[3].y))//角度０の平行四辺形
								{
									//下の条件を満たせば左下から右上の平行四辺形
									if ((pos[SaveNumber].x - dir.x > 0.0f && pos[SaveNumber].y - dir.y > 0.0f) || (pos[SaveNumber].x - dir.x < 0.0f && pos[SaveNumber].y - dir.y < 0.0f))
									{
										Shapes_type_Angle[NowShapes][0] = 8;
										Shapes_type_Angle[NowShapes][1] = 0;
									}
									else//右下から左上の平行四辺形
									{
										Shapes_type_Angle[NowShapes][0] = 7;
										Shapes_type_Angle[NowShapes][1] = 0;
									}
									if ((int)Length[0][0] % 15 == 0)
									{
										Shapes_Length[NowShapes][0] = Length[0][0] / 15.0f;
										Shapes_Length[NowShapes][1] = Length[1][0] / sqrtf(450.0f);
									}
									else
									{
										Shapes_Length[NowShapes][0] = Length[1][0] / 15.0f;
										Shapes_Length[NowShapes][1] = Length[0][0] / sqrtf(450.0f);
									}
								}
								else //角度９０の平行四辺形
								{
									//x,y共にプラスが1つでもあるなら左下から右上の平行四辺形
									if ((pos[SaveNumber].x - dir.x > 0.0f && pos[SaveNumber].y - dir.y > 0.0f) || (pos[SaveNumber].x - dir.x < 0.0f && pos[SaveNumber].y - dir.y < 0.0f))
									{
										Shapes_type_Angle[NowShapes][0] = 7;
										Shapes_type_Angle[NowShapes][1] = 1;
									}
									else
									{
										Shapes_type_Angle[NowShapes][0] = 8;
										Shapes_type_Angle[NowShapes][1] = 1;
									}
									if ((int)Length[0][0] % 15 == 0)
									{
										Shapes_Length[NowShapes][0] = Length[0][0] / 15.0f;
										Shapes_Length[NowShapes][1] = Length[1][0] / sqrtf(450.0f);
									}
									else
									{
										Shapes_Length[NowShapes][0] = Length[1][0] / 15.0f;
										Shapes_Length[NowShapes][1] = Length[0][0] / sqrtf(450.0f);
									}
								}
							}
						}
					}

					//音を再生
					g_FieldSe->Stop();
					XAUDIO2_BUFFER buffer;
					buffer =g_Fieldsound->GetBuffer(false);
					g_FieldSe->FlushSourceBuffers();
					g_FieldSe->SubmitSourceBuffer(&buffer);
					if(g_FieldSe)SetVolumeSE(g_FieldSe);
					g_FieldSe->Start();

					for (int m = 0; Comparison2[m] != -1; m++)
					{
						//保存した各頂点のスーパースターを使用に変える
						//のちの処理でスーパースターがtrueでないならSuperStarUseがなんであろうとはじかれる
						if (m_tVertex[Comparison2[m]].SuperStar)
						{
							m_tVertex[Comparison2[m]].SuperStarUse = true;
							SuperStarCount++;
							if (GetFeverMode())
							{
								fFeverPoint += (MAX_FEVER_POINT / FEVER_TIME) * FEVER_ADD_TIME;
								float PointAjust = 0.0f;
								if (fFeverPoint > MAX_FEVER_POINT)
								{
									PointAjust = MAX_FEVER_POINT - fFeverPoint;
									fFeverPoint = MAX_FEVER_POINT;
								}
								SetFeverStellaTime(FEVER_ADD_TIME * 60.0f - PointAjust);
							}
							SetSuperStar();
							if (!GetFeverMode())
							{
								nFeverPoint += 1.0f;
								m_pBattle->SaveAllyData(Shapes_Count[NowShapes],true);//図形の頂点と角数を渡す
								Ally_Count++;//召喚数増やす
								SummonLog[NowSummonLog].Pos = DirectX::XMFLOAT3(137.0f, 50.0f, 10.0f);
								SummonLog[NowSummonLog].time = DRAW_LOG_TIME;
								SummonLog[NowSummonLog].type = Shapes_Count[NowShapes] - 3;//画数から引く (0か1)
								SummonLog[NowSummonLog].Alpha = 0.0f;
								SummonLog[NowSummonLog].MoveType = 1;
								NowSummonLog++;//召喚ログを増やす
							}
						}
						m_tVertex[Comparison2[m]].Angle[1] = 181.0f;
					}
					if (!GetFeverMode())
					{
						nFeverPoint += 1.0f;
					}
					if (nFeverPoint > MAX_FEVER_POINT)nFeverPoint = MAX_FEVER_POINT;
					
					NowShapes++;//保存場所を次の場所にする
				}
				
			}
			ShapesSaveFalg = true;//重複した図形または認めない図形は保存はしないが保存したとしてみなしtrueにする
			ShapesFlag = false;//初期化
			break;
		}
		else
		{
			if (Comparison[k] == -1)
			{
				Comparison[k] = VertexNumber.Number;//図形の頂点保存に探索時の頂点を保存する
				NowVertex = k;//仮頂点保存配列の何番目に格納したのかを保存
				break;
			}
		}
	}
	//図形として保存したらさらに探索してはいけない
	if (!ShapesSaveFalg)
	{
		for (int k = 0; k < 8; k++)
		{
			//ｋ＝０のときはがいる
			
			if (NowVertex == 0)
			{
				if (VertexNumber.Connect[k] != -1)ShapesCheck(m_tVertex[VertexNumber.Connect[k]]);
			}
			else
			{
				if (VertexNumber.Connect[k] != -1 && VertexNumber.Connect[k] != Comparison[NowVertex - 1])ShapesCheck(m_tVertex[VertexNumber.Connect[k]]);
			}
		}
		Comparison[NowVertex] = -1;//繋がっている頂点が無ければ行き止まりなので一つ前の頂点に戻る
	}
}

////=====描画時の座標と大きさをセットする関数=====//
void CFieldVertex::DrawSetting(DirectX::XMFLOAT3 InPos, DirectX::XMFLOAT3 InSize, DirectX::XMFLOAT3 InAngle, Sprite* InSprite)
{
	//移動行列(Translation)
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(InPos.x,InPos.y,InPos.z);
	//回転行列
	DirectX::XMMATRIX Rx = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(InAngle.x));
	DirectX::XMMATRIX Ry = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(InAngle.y));
	DirectX::XMMATRIX Rz = DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(InAngle.z));
	//拡大縮小行列
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(InSize.x, InSize.y, InSize.z);

	DirectX::XMMATRIX mat = S * Rx * Ry * Rz * T;//それぞれの行列を掛け合わせて格納

	DirectX::XMFLOAT4X4 wvp[3];
	DirectX::XMMATRIX world;
	world = mat;

	DirectX::XMStoreFloat4x4(&wvp[0], DirectX::XMMatrixTranspose(world));
	wvp[1] = GetView();
	wvp[2] = GetProj();

	InSprite->SetWorld(wvp[0]);
	InSprite->SetView(wvp[1]);
	InSprite->SetProjection(wvp[2]);
}

////頂点(星)の動きと描画をする関数=====// 
void CFieldVertex::DrawStarModel(int color, int Vertex)
{
	//-----描画関連の処理-----//
	{
		m_pStar_Model[color]->SetPostion(m_tVertex[Vertex].Pos.x, m_tVertex[Vertex].Pos.y, 10.0f);//座標設定
		m_pStar_Model[color]->SetRotation(TORAD(m_tVertex[Vertex].Angle[0]), TORAD(m_tVertex[Vertex].Angle[1]), TORAD(m_tVertex[Vertex].Angle[2]));//角度設定
		m_pStar_Model[color]->SetScale(STAR_SIZE, STAR_SIZE, STAR_SIZE);//大きさ設定
		m_pStar_Model[color]->SetViewMatrix(GetView());//view情報をセット
		m_pStar_Model[color]->SetProjectionMatrix(GetProj());//proj情報をセット
		m_pStar_Model[color]->Draw();//描画
	}
		
	//-----頂点(星)の動き処理-----//
	{
		if (m_tVertex[Vertex].SuperStar)//ステラは常に回転
		{
			m_tVertex[Vertex].Angle[1] += (360.0f / 60.0f) * 0.75f;
		}
		else
		{
			if (!(m_tVertex[Vertex].Angle[1] == 180.0f))
			{
				m_tVertex[Vertex].Angle[1] += (360.0f / 60.0f);//１周で止める
			}
		}
		if (m_tVertex[Vertex].Angle[1] > 360.0f)
		{
			m_tVertex[Vertex].Angle[1] = 0.0f;//値の補正
		}
		//if (GetFeverMode())
		//{
		//	//m_tVertex[Vertex].Angle[0] += (360.0 / 30.0f);
		//	m_tVertex[Vertex].Angle[2] += (360.0 / 60.0f);
		//	/*if (m_tVertex[Vertex].Angle[0] > 360.0f)
		//	{
		//		m_tVertex[Vertex].Angle[0] = 0.0f;
		//	}*/
		//	if (m_tVertex[Vertex].Angle[2] > 360.0f)
		//	{
		//		m_tVertex[Vertex].Angle[2] = 0.0f;
		//	}
		//}
		//else
		//{
		//	m_tVertex[Vertex].Angle[0] = 0.0f;
		//	m_tVertex[Vertex].Angle[2] = 0.0f;
		//}
	}
}



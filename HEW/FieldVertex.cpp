//Character.cpp
//編集者：AT12A25 中島聖羅
#include "FieldVertex.h"
#include "DirectXTex/TextureLoad.h"
#include "Player.h"
#include <math.h>
#include "SceneGame.h"
#include "SpriteDrawer.h"
#include "_StructList.h"
#include "Defines.h"
#include "Main.h"

E_GAME_PHASE NowPhase;//今のフェーズ
E_GAME_PHASE PrevPhase;//過去のフェーズ

Sprite::Vertex vtx_FieldLine[MAX_LINE][4];

CFieldVertex::CFieldVertex()
	:RoadStop(false)
	, m_tVertex{}
	, m_tCenter_Vertex{}
	, m_pTex_FieldVertex(nullptr)
	, m_pTex_FieldUseVertex(nullptr)
	, m_offsetU_Field(0.0f)
	, m_pTex_FieldLine{ nullptr }
	, NowLine(0)
	, DrawLinePos{}
	, BreakVertex(-1)
	, m_pVtx_FieldLine{nullptr}
	, m_pSprite_Line{nullptr}
	, SuperStarCount(1)
	, m_pTex_SuperStar_Number{nullptr}
	, m_pSprite_SuperStar_Number(nullptr)
	, m_pStar_Model{nullptr}
{
	// 星の描画用
	// スプライト
	m_pSprite_Star = new Sprite();
	m_pSprite_SuperStar_Number = new Sprite();
	// テクスチャ
	m_pTex_FieldVertex = new Texture();
	m_pTex_FieldUseVertex = new Texture();
	
	// 線の描画用
	// スプライト
	for (int i = 0; i < MAX_LINE; i++)
	{
		m_pSprite_Line[i] = new Sprite();
	}
	// テクスチャ
	m_pTex_FieldLine = new Texture();
	for (int i = 0; i < 5; i++)
	{
		m_pTex_SuperStar_Number[i] = new Texture();
	}
	
	m_pStar_Model[0] = new CModelEx(MODEL_PASS("Board_Star/Orange/Board_Star_Orange.fbx"));
	m_pStar_Model[1] = new CModelEx(MODEL_PASS("Board_Star/Blue/Board_Star_Blue.fbx"));
	m_pStar_Model[2] = new CModelEx(MODEL_PASS("Board_Star/Red/Board_Star_Red.fbx"));

	StartVertex = START_PLAYER;	// 始点初期化
	GoalVertex = START_PLAYER;	// 終点初期化
	NowShapes = 0;				// 格納した図形の数初期化

	NowPhase = E_GAME_PHASE::DRAWING;	// 今のフェーズ初期化
	PrevPhase = E_GAME_PHASE::DRAWING;	// 過去のフェーズ初期化

	// 各配列を-1で初期化
	Fill(OrderVertex, -1);
	Fill(Comparison_Shapes_Vertex_Save, -1);
	Fill(Shapes_Vertex_Save, -1);
	Fill(Shapes_Count, -1);
	Fill(Comparison, -1);

	// 頂点２５個座標情報初期化
	FieldVertex* Vertexp;
	Vertexp = m_tVertex;
	for (int j = 0; j < 5; j++)
	{
		for (int i = 0; i < 5; i++, Vertexp++)
		{
			Vertexp->Pos.x = i * VERTEX_SIZE_X + VERTEX_POS_X;
			Vertexp->Pos.y = -j * VERTEX_SIZE_Y + VERTEX_POS_Y;
			Vertexp->Pos.z = 0;
			Vertexp->Number = j * 5 + i;
			Vertexp->Use = false;
			Vertexp->SuperStar = false;
			for (int k = 0; k < 8; k++)
			{
				Vertexp->Connect[k] = -1;
			}
		}
	}

	//センター頂点16個座標情報初期化
	CenterVertex* CenterVertexp;
	CenterVertexp = m_tCenter_Vertex;
	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < 4; i++, CenterVertexp++)
		{
			CenterVertexp->Pos.x = i * VERTEX_SIZE_X + VERTEX_POS_X + VERTEX_SIZE_X / 2.0f;
			CenterVertexp->Pos.y = -j * VERTEX_SIZE_Y + VERTEX_POS_Y - VERTEX_SIZE_Y / 2.0f;
			CenterVertexp->Pos.z = 0;
			CenterVertexp->Use = false;
		}
	}

	//たどる順にプレイヤーの最初の位置保存
	OrderVertex[0] = StartVertex;
	OrderVertexCount = 1;//たどった頂点の数初期化

	m_tVertex[START_PLAYER].Use = true;//最初の頂点を使用に

	//頂点描画初期化
	HRESULT hrVertex;
	hrVertex = m_pTex_FieldVertex->Create("Asset/Star/star.png");
	if (FAILED(hrVertex)) {
		MessageBox(NULL, "Vertex 画像", "Error", MB_OK);
	}
	hrVertex = m_pTex_FieldUseVertex->Create("Asset/Star/star2.png");
	if (FAILED(hrVertex)) {
		MessageBox(NULL, "UseVertex 画像", "Error", MB_OK);
	}

	//線描画初期化
	HRESULT hrLine;
	hrLine = m_pTex_FieldLine->Create("Asset/Line/Line.png");
	if (FAILED(hrLine)) {
		MessageBox(NULL, "Field 画像", "Error", MB_OK);
	}

	//スーパースター初期化
	HRESULT hrSuperStar;
	for (int i = 0; i < 5; i++)
	{
		switch (i)
		{
		case 0:hrSuperStar = m_pTex_SuperStar_Number[0]->Create("Asset/Numbers/number_1.png"); break;
		case 1:hrSuperStar = m_pTex_SuperStar_Number[1]->Create("Asset/Numbers/number_2.png"); break;
		case 2:hrSuperStar = m_pTex_SuperStar_Number[2]->Create("Asset/Numbers/number_3.png"); break;
		case 3:hrSuperStar = m_pTex_SuperStar_Number[3]->Create("Asset/Numbers/number_4.png"); break;
		case 4:hrSuperStar = m_pTex_SuperStar_Number[4]->Create("Asset/Numbers/number_5.png"); break;
		default:
			break;
		}
		if (FAILED(hrSuperStar)) {
			MessageBox(NULL, "Vertex 画像", "Error", MB_OK);
		}
	}

	//Sprite::Vertex vtx_FieldVertex[] = {
	//	//背景表示の座標
	//	{{-1.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},
	//	{{-1.0f,  1.0f, 0.0f}, {0.0f, 1.0f}},
	//	{{ 1.0f, -1.0f, 0.0f}, {1.0f, 0.0f}},
	//	{{ 1.0f,  1.0f, 0.0f}, {1.0f, 1.0f}},
	//};

	//for (int i = 0; i < MAX_LINE; i++)
	//{
	//	//背景表示の座標
	//	vtx_FieldLine[i][0] = {{-1.0f, -1.0f, 0.0f}, {0.0f, 0.0f}};
	//	vtx_FieldLine[i][1] = {{-1.0f,  1.0f, 0.0f}, {0.0f, 1.0f}};
	//	vtx_FieldLine[i][2] = {{ 1.0f, -1.0f, 0.0f}, {1.0f, 0.0f}};
	//	vtx_FieldLine[i][3] = {{ 1.0f,  1.0f, 0.0f}, {1.0f, 1.0f}};
	//}

}

CFieldVertex::~CFieldVertex()
{
	SAFE_DELETE(m_pTex_FieldLine);
	SAFE_DELETE(m_pTex_FieldUseVertex);
	SAFE_DELETE(m_pTex_FieldVertex);
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

			//多角形判定
			if (OrderVertex[3] != -1 && Vertexp->Use)//たどってきた頂点が４つ以上ならかつ今の頂点が過去に使われた時、多角形判定
			{
				ShapesCheck(m_tVertex[StartVertex]);//再帰処理(開始頂点のみ)
			}
			Vertexp->Use = true;//頂点が追加されたので今の頂点をtureに
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

void CFieldVertex::Draw()
{
	SetRender2D();
	//今のフェーズがDrawの時フィールドの頂点描画
	if (NowPhase == E_GAME_PHASE::DRAWING)
	{
		FieldVertex* Vertexp;
		Vertexp = m_tVertex;
	
		for (int i = 0; i <= NowLine; i++)
		{			
			//線の描画
			m_pSprite_Line[i]->SetCenterPosAndRotation(
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
			{ vtx_FieldLine[i][0].pos[0],
				vtx_FieldLine[i][0].pos[1],
				0.0f
			},
			{
				vtx_FieldLine[i][2].pos[0],
				vtx_FieldLine[i][2].pos[1],
				0.0f
			}
			);
			//背景色の設定
			m_pSprite_Line[i]->SetColor({1.0f,1.0f,1.0f,1.0f});
			//その他、表示に必要なSpriteDrawer.hの各種関数を呼び出す
			m_pSprite_Line[i]->SetTexture(m_pTex_FieldLine);
			m_pSprite_Line[i]->Draw();
		}

		for (int i = 0; i < MAX_VERTEX; i++, Vertexp++)
		{
			if (Vertexp->SuperStar)DrawStarModel(2, i);
			else
			{
				if (Vertexp->Use)DrawStarModel(1, i);
				else DrawStarModel(0, i);
			}

			//// スプライトの設定		// 座標の設定						// 大きさの設定
			//DrawSetting({ Vertexp->Pos.x, Vertexp->Pos.y,10.0f }, { STAR_SIZE,STAR_SIZE,1.0f }, m_pSprite_Star);

			//// 背景色の設定
			//m_pSprite_Star->SetColor({ 1.0f,1.0f,1.0f,1.0f });

			////その他、表示に必要なSpriteDrawer.hの各種関数を呼び出す
			//if (!Vertexp->Use)m_pSprite_Star->SetTexture(m_pTex_FieldVertex);
			//else m_pSprite_Star->SetTexture(m_pTex_FieldUseVertex);
			//if (Vertexp->SuperStar)
			//{
			//	m_pSprite_Star->SetTexture(m_pTex_FieldUseVertex);
			//	m_pSprite_Star->SetColor({ 1.0f,0.2f,0.2f,1.0f });
			//}
			////if(Vp->Number == BreakVertex)SetSpriteTexture(m_pTex_FieldVertex);//壊れた頂点
			//m_pSprite_Star->Draw();
		}

		//スーパースターの数描画
		// スプライトの設定		// 座標の設定						// 大きさの設定
		DrawSetting({ -80.0f, 60.0f,10.0f }, { 100.0f,100.0f,1.0f }, m_pSprite_SuperStar_Number);

		// 背景色の設定
		m_pSprite_SuperStar_Number->SetColor({ 1.0f,0.2f,0.2f,1.0f });

		//その他、表示に必要なSpriteDrawer.hの各種関数を呼び出す
		m_pSprite_SuperStar_Number->SetTexture(m_pTex_SuperStar_Number[SuperStarCount - 1]);
		
		//if(Vp->Number == BreakVertex)SetSpriteTexture(m_pTex_FieldVertex);//壊れた頂点
		m_pSprite_SuperStar_Number->Draw();
		m_pSprite_SuperStar_Number->ReSetSprite();
	}

	//SetRender3D();

	if (NowPhase == E_GAME_PHASE::DRAWING || NowPhase == E_GAME_PHASE::SHAPESCHECK)
	{
		m_pBattle->SaveAllyLogDraw();
	}
}

DirectX::XMFLOAT3 CFieldVertex::GetVertexPos(int VertexNumber)
{
	return m_tVertex[VertexNumber].Pos;//頂点座標ゲット
}

bool CFieldVertex::GetRoadStop(int Direction)
{
	CenterVertex* CenterVertexp;
	RoadStop = false;//初期化

	//行けない方向を判別
	switch (Direction)
	{
	case 0:
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
	case 1:
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
	case 2:
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
	case 3:
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
	case 4:
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
	case 5:
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
	case 6:
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
	case 7:
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
			//行きたい方向の頂点と繋がっていたら
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
	return RoadStop;
}

void CFieldVertex::SetBattleAddress(CBattle* InAddress)
{
	m_pBattle = InAddress;
}

void CFieldVertex::SetPlayerAddress(CPlayer* InAddress)
{
	m_pPlayer = InAddress;
}

void CFieldVertex::InitFieldVertex()
{
	//頂点２５個初期化
	FieldVertex* Vertexp;
	Vertexp = m_tVertex;
	for (int j = 0; j < MAX_VERTEX; j++, Vertexp++)
	{
		Vertexp->Use = false;
		for (int k = 0; k < 8; k++)
		{
			Vertexp->Connect[k] = -1;
		}
	}

	//センター頂点16個初期化
	CenterVertex* CenterVertexp;
	CenterVertexp = m_tCenter_Vertex;
	for (int j = 0; j < MAX_CENTER_VERTEX; j++, CenterVertexp++)
	{
		CenterVertexp->Use = false;
	}

	//線の描画情報消す
	for (int i = 0; i < NowLine; i++)
	{
		//背景表示の座標
		
	}

	StartVertex = GoalVertex;//始点を今の地点に初期化
	NowShapes = 0;//格納した図形の数初期化

	//配列-1で初期化
	Fill(OrderVertex, -1);
	Fill(Comparison_Shapes_Vertex_Save, -1);
	Fill(Shapes_Vertex_Save, -1);
	Fill(Shapes_Count, -1);
	Fill(Comparison, -1);

	//たどる順にプレイヤーの最初の位置保存
	OrderVertex[0] = StartVertex;
	OrderVertexCount = 1;//たどった頂点の数初期化

	//最初の頂点を使用に
	m_tVertex[StartVertex].Use = true;

	NowLine = 0;
}

void CFieldVertex::SetSuperStar()
{
	//頂点２５個初期化
	FieldVertex* Vertexp;
	Vertexp = m_tVertex;
	int UseCount = 0;//使ったスーパースターを数える
	if (SuperStarCount == 2)
	{
		UseCount = 0;
	}
	for (int i = 0; i < MAX_VERTEX; i++, Vertexp++)
	{
		
		if (Vertexp->SuperStar && Vertexp->SuperStarUse)
		{
			UseCount++;//スーパースターを使っていたら使用回数増やす
		}
		Vertexp->SuperStar = false;
		Vertexp->SuperStarUse = false;
	}
	if (UseCount == SuperStarCount)SuperStarCount++;//盤面のスーパースターの数と使った数が同じなら増やす
	else SuperStarCount = 1;
	if (SuperStarCount > 5)SuperStarCount = 5;//盤面のスーパースターは5を超えない
	for (int i = 0; i < SuperStarCount;)
	{
		int Vertex;
		Vertex = rand() % 25;
		if (!m_tVertex[Vertex].SuperStar && Vertex != GoalVertex)//すでにスーパースターか今いる頂点ならもう一度抽選
		{
			m_tVertex[Vertex].SuperStar = true;
			i++;
		}
	}
}

void CFieldVertex::ShapesCheck(FieldVertex VertexNumber)
{
	int NowVertex = 0;//仮頂点保存のどの位置に今格納したのかを保存する
	bool ShapesFlag = false;//保存済みのと比較して保存されていたらfalse
	bool ShapesSaveFalg = false;//保存を行ったかどうか

	for (int k = 0; k < MAX_VERTEX; k++)
	{
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
								Count++;//辺が繋がってないとき角ができてる
							}
						}
						break;//最後の頂点の処理を終えたら処理抜ける
					}
				}
				//凹角形判定
				bool BadShapes = false;
				if (Count != 3 && !(Count > 8))
				{
					DirectX::XMFLOAT2 FastVector;//先のベクトル
					DirectX::XMFLOAT2 SecondVector;//後のベクトル
					DirectX::XMFLOAT2 NormalizeFastVector;//正規化後の先ベクトル
					DirectX::XMFLOAT2 NormalizeSecondVector;//正規化後の後のベクトル
					float AngleSave[25];
					int AngleCount = 0;
					float Error = 0.01;//誤差
					Fill(AngleSave, -1);
					if (Count == 5)
					{
						Count = Count;
					}
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
							AngleCount++;
							for (m = 1; (m < MAX_VERTEX - 1) && (AngleSave[m] != -1); m++)
							{
								if (fabsf(AngleSave[m] - AngleSave[m - 1]) >= 180.0f)BadShapes = true;
							}

							break;
						}
					}
				}
				if (Count > 8)BadShapes = true;//図形とみなさない(9角形以上の凹角形が存在しないため)BadShapesをtrueにする
				/*if (BadShapes)
				{
					MessageBox(NULL, "凹角形でちゃった(はーと)","バカな君へ", MB_OK);
				}*/
				if (!BadShapes)
				{
					Shapes_Count[NowShapes] = Count;
					//図形の面積
					Shapes_Size = 0;
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
								if (m_tVertex[n].Use)LeftVertex = true;
							}
							//比較頂点を一番右からにする
							for (int n = (m / 5) * 5 + 4; n > m; n--)
							{
								if (m_tVertex[n].Use)RightVertex = true;
							}
							//比較頂点を一番上からにする
							for (int n = m % 5; n < m; n += 5)
							{
								if (m_tVertex[n].Use)UpVertex = true;
							}
							//比較頂点を一番下からにする
							for (int n = m % 5 + 5 * 4; n > m; n -= 5)
							{
								if (m_tVertex[n].Use)DownVertex = true;
							}
							if (UpVertex && DownVertex && LeftVertex && RightVertex)InVertex++;//全方向に使っている頂点があれば囲まれているので内側の頂点
						}
					}
					Shapes_Size = InVertex + OutVertex / 2.0f - 1.0f;
					m_pBattle->SaveAllyData(Shapes_Count[NowShapes], Shapes_Size);//図形の頂点と角数を渡す
					for (int m = 0; Comparison2[m] != -1; m++)
					{
						m_tVertex[Comparison2[m]].SuperStarUse = true;//保存した各頂点のスーパースターを使用に変える
						//のちの処理でスーパースターがtrueでないならSuperStarUseがなんであろうとはじかれる
					}
				}
				NowShapes++;//保存場所を次の場所にする
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

void CFieldVertex::DrawSetting(DirectX::XMFLOAT3 InPos, DirectX::XMFLOAT3 InSize, Sprite* InSprite)
{
	//移動行列(Translation)
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslationFromVector(DirectX::XMVectorSet(
		InPos.x,
		InPos.y,
		InPos.z,
		0.0f
	));

	DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMVectorSet(
		0.0f,
		0.0f,
		0.0f,
		0.0f
	));

	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(InSize.x, InSize.y, InSize.z);
	//それぞれの行列を掛け合わせて格納
	DirectX::XMMATRIX mat = S * R * T;

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

void CFieldVertex::DrawStarModel(int color, int Vertex)
{
		SetRender3D();
		m_pStar_Model[color]->SetPostion(m_tVertex[Vertex].Pos.x, m_tVertex[Vertex].Pos.y, 10.0f);
		m_pStar_Model[color]->SetRotation(0.0f,TORAD(180), 0.0f);
		m_pStar_Model[color]->SetScale(STAR_SIZE, STAR_SIZE, STAR_SIZE);
		m_pStar_Model[color]->SetViewMatrix(GetView());
		m_pStar_Model[color]->SetProjectionMatrix(GetProj());
		m_pStar_Model[color]->Draw();
}



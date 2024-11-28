//Character.cpp
//編集者：AT12A25 中島聖羅
#include "FieldVertex.h"
#include "DirectXTex/TextureLoad.h"
#include "Player.h"
#include <math.h>
#include "SceneGame.h"
#include"SpriteDrawer.h"
#include"_StructList.h"
#include"Defines.h"

E_GAME_PHASE NowPhase;//今のフェーズ
E_GAME_PHASE PrevPhase;//過去のフェーズ

Vertex vtx_FieldLine[MAX_LINE][4];

CFieldVertex::CFieldVertex()
	:RoadStop(false)
	,m_tVertex{}
	,m_tCenter_Vertex{}
	,m_pTex_FieldVertex(nullptr)
	,m_pVtx_FieldVertex(nullptr)
	,m_offsetU_Field(0.0f)
	,m_pTex_FieldLine{ nullptr }
	,m_pVtx_FieldLine{ nullptr }
	,NowLine(0)
	,DrawLinePos{}
	,BreakVertex(-1)
{
	StartVertex = START_PLAYER;//始点初期化
	GoalVertex = START_PLAYER;//終点初期化
	NowShapes = 0;//格納した図形の数初期化

	NowPhase = E_GAME_PHASE::DRAWING;//今のフェーズ初期化
	PrevPhase = E_GAME_PHASE::DRAWING;//過去のフェーズ初期化
	
	//配列-1で初期化
	Fill(OrderVertex, -1);
	Fill(Comparison_Shapes_Vertex_Save, -1);
	Fill(Shapes_Vertex_Save, -1);
	Fill(Shapes_Count, -1);
	Fill(Comparison, -1);

	//頂点２５個座標初期化
	FieldVertex* Vertexp;
	Vertexp = m_tVertex;
	for (int j = 0; j < 5; j++)
	{
		for (int i = 0; i < 5; i++, Vertexp++)
		{
			Vertexp->Pos.X = i * VERTEX_SIZE + VERTEX_POS_X;
			Vertexp->Pos.Y = j * VERTEX_SIZE + VERTEX_POS_Y;
			Vertexp->Pos.Z = 0;
			Vertexp->Number = j * 5 + i;
			Vertexp->Use = false;
			for (int k = 0; k < 8; k++)
			{
				Vertexp->Connect[k] = -1;
			}
		}
	}

	//センター頂点16個座標初期化
	CenterVertex* CenterVertexp;
	CenterVertexp = m_tCenter_Vertex;
	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < 4; i++, CenterVertexp++)
		{
			CenterVertexp->Pos.X = i * VERTEX_SIZE + VERTEX_POS_X + VERTEX_SIZE / 2;
			CenterVertexp->Pos.Y = j * VERTEX_SIZE + VERTEX_POS_Y + VERTEX_SIZE / 2;
			CenterVertexp->Pos.Z = 0;
			CenterVertexp->Use = false;
		}
	}

	//たどる順にプレイヤーの最初の位置保存
	OrderVertex[0] = StartVertex;
	OrderVertexCount = 1;//たどった頂点の数初期化

	m_tVertex[START_PLAYER].Use = true;//最初の頂点を使用に

	//頂点描画初期化
	HRESULT hrVertex;
	hrVertex = LoadTextureFromFile(GetDevice(), "Asset/Star/星.png", &m_pTex_FieldVertex);
	if (FAILED(hrVertex)) {
		MessageBox(NULL, "Field 画像", "Error", MB_OK);
	}
	hrVertex = LoadTextureFromFile(GetDevice(), "Asset/Star/星2.png", &m_pTex_FieldUseVertex);
	if (FAILED(hrVertex)) {
		MessageBox(NULL, "UseVertex 画像", "Error", MB_OK);
	}
	Vertex vtx_FieldVertex[] = {
		//背景表示の座標
		{{-STAR_SIZE, -STAR_SIZE, 0.0f}, {0.0f, 0.0f}},
		{{-STAR_SIZE,  STAR_SIZE, 0.0f}, {0.0f, 1.0f}},
		{{ STAR_SIZE, -STAR_SIZE, 0.0f}, {1.0f, 0.0f}},
		{{ STAR_SIZE,  STAR_SIZE, 0.0f}, {1.0f, 1.0f}}, 
	};
    m_pVtx_FieldVertex = CreateVertexBuffer(vtx_FieldVertex, 4);

	//線描画初期化
	HRESULT hrLine;
	
	hrLine = LoadTextureFromFile(GetDevice(), "Asset/Line/Line.png", &m_pTex_FieldLine);
	if (FAILED(hrLine)) {
		MessageBox(NULL, "Field 画像", "Error", MB_OK);
	}
	
	for(int i = 0; i < MAX_LINE; i++)
	{
		//背景表示の座標
		vtx_FieldLine[i][0] = { {-LINE_SIZE, -LINE_SIZE, 0.0f}, {0.0f, 0.0f} };
		vtx_FieldLine[i][1] = { {-LINE_SIZE,  LINE_SIZE, 0.0f}, {0.0f, 1.0f} };
		vtx_FieldLine[i][2] = { { LINE_SIZE, -LINE_SIZE, 0.0f}, {1.0f, 0.0f} };
		vtx_FieldLine[i][3] = { { LINE_SIZE,  LINE_SIZE, 0.0f}, {1.0f, 1.0f} };
		m_pVtx_FieldLine[i] = CreateVertexBuffer(vtx_FieldLine[i], 4);
	}
}

CFieldVertex::~CFieldVertex()
{
	for (int i = 0; i < MAX_LINE; i++)
	{
		if (m_pVtx_FieldLine[i]) m_pVtx_FieldLine[i]->Release();
	}
	if (m_pTex_FieldLine)m_pTex_FieldLine->Release();

	if (m_pTex_FieldUseVertex)m_pTex_FieldUseVertex->Release();
	if (m_pVtx_FieldVertex)m_pVtx_FieldVertex->Release();
	if (m_pTex_FieldVertex)m_pTex_FieldVertex->Release();
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
		if (PlayerPos.X == Vertexp->Pos.X && PlayerPos.Y == Vertexp->Pos.Y && m_tVertex[GoalVertex].Number != Vertexp->Number && Vertexp->Number != BreakVertex)
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
				if (((m_tVertex[GoalVertex].Pos.X + Vertexp->Pos.X) / 2 == CenterVertexp->Pos.X) && ((m_tVertex[GoalVertex].Pos.Y + Vertexp->Pos.Y) / 2 == CenterVertexp->Pos.Y))
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
	FLOAT3 PosA[4];//回転後座標格納用
	FLOAT3 Size;//線のサイズ

	Size.X = LINE_SIZE;
	Size.Y = LINE_SIZE;

	//線の回転
	PosA[0].X = -Size.X * 0.5f * cosf(TORAD(45 * PlayerDestination)) - (-Size.Y * 0.5f) * sinf(TORAD(45 * PlayerDestination));
	PosA[0].Y = -Size.X * 0.5f * sinf(TORAD(45 * PlayerDestination)) + (-Size.Y * 0.5f) * cosf(TORAD(45 * PlayerDestination));
	PosA[1].X = -Size.X * 0.5f * cosf(TORAD(45 * PlayerDestination)) - ( Size.Y * 0.5f) * sinf(TORAD(45 * PlayerDestination));
	PosA[1].Y = -Size.X * 0.5f * sinf(TORAD(45 * PlayerDestination)) + ( Size.Y * 0.5f) * cosf(TORAD(45 * PlayerDestination));
	PosA[2].X =  Size.X * 0.5f * cosf(TORAD(45 * PlayerDestination)) - (-Size.Y * 0.5f) * sinf(TORAD(45 * PlayerDestination));
	PosA[2].Y =  Size.X * 0.5f * sinf(TORAD(45 * PlayerDestination)) + (-Size.Y * 0.5f) * cosf(TORAD(45 * PlayerDestination));
	PosA[3].X =  Size.X * 0.5f * cosf(TORAD(45 * PlayerDestination)) - ( Size.Y * 0.5f) * sinf(TORAD(45 * PlayerDestination));
	PosA[3].Y =  Size.X * 0.5f * sinf(TORAD(45 * PlayerDestination)) + ( Size.Y * 0.5f) * cosf(TORAD(45 * PlayerDestination));

	vtx_FieldLine[NowLine][0].pos[0] = PlayerPos.X + PosA[0].X;//左上のｘ座標
	vtx_FieldLine[NowLine][0].pos[1] = PlayerPos.Y + PosA[0].Y;//左上のｙ座標
	vtx_FieldLine[NowLine][1].pos[0] = m_tVertex[GoalVertex].Pos.X + PosA[1].X;//左下のｘ座標
	vtx_FieldLine[NowLine][1].pos[1] = m_tVertex[GoalVertex].Pos.Y + PosA[1].Y;//左下のｙ座標
	vtx_FieldLine[NowLine][2].pos[0] = PlayerPos.X + PosA[2].X;//右上のｘ座標
	vtx_FieldLine[NowLine][2].pos[1] = PlayerPos.Y + PosA[2].Y;//右上のｙ座標
	vtx_FieldLine[NowLine][3].pos[0] = m_tVertex[GoalVertex].Pos.X + PosA[3].X;//右下のｘ座標
	vtx_FieldLine[NowLine][3].pos[1] = m_tVertex[GoalVertex].Pos.Y + PosA[3].Y;//右下のｙ座標
	m_pVtx_FieldLine[NowLine] = CreateVertexBuffer(vtx_FieldLine[NowLine], 4);

}

void CFieldVertex::Draw()
{
	//今のフェーズがDrawの時フィールドの頂点描画
	if (NowPhase == E_GAME_PHASE::DRAWING)
	{
		FieldVertex* Vertexp;
		Vertexp = m_tVertex;
		for (int i = 0; i < MAX_VERTEX; i++, Vertexp++)
		{
			//スプライトの設定
			SetSpritePos(Vertexp->Pos.X, Vertexp->Pos.Y);//各頂点に位置を設定

			//大きさの設定
			SetSpriteScale(1.0f, 1.0f);

			//背景色の設定
			SetSpriteColor(1.0f, 1.0f, 1.0f, 1.0f);

			//その他、表示に必要なSpriteDrawer.hの各種関数を呼び出す
			if (!Vertexp->Use)SetSpriteTexture(m_pTex_FieldVertex);
			else SetSpriteTexture(m_pTex_FieldUseVertex);
			//if(Vp->Number == BreakVertex)SetSpriteTexture(m_pTex_FieldVertex);//壊れた頂点


			DrawSprite(m_pVtx_FieldVertex, sizeof(Vertex));
		}
		//線の描画
		for (int i = 0; i <= NowLine; i++)
		{
			//スプライトの設定
			SetSpritePos(0.0f, 0.0f);

			//大きさの設定
			SetSpriteScale(1.0f, 1.0f);

			//背景色の設定
			SetSpriteColor(1.0f, 1.0f, 1.0f, 1.0f);

			//その他、表示に必要なSpriteDrawer.hの各種関数を呼び出す
			SetSpriteTexture(m_pTex_FieldLine);

			DrawSprite(m_pVtx_FieldLine[i], sizeof(Vertex));
		}
	}
		
	if (NowPhase == E_GAME_PHASE::DRAWING || NowPhase == E_GAME_PHASE::SHAPESCHECK)
	{
		m_pBattle->SaveAllyLogDraw();
	}
}

CVector3<float> CFieldVertex::GetVertexPos(int VertexNumber)
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
				if (((m_tVertex[GoalVertex].Pos.X + m_tVertex[GoalVertex - 5 + 1].Pos.X) / 2 == CenterVertexp->Pos.X) && ((m_tVertex[GoalVertex].Pos.Y + m_tVertex[GoalVertex - 5 + 1].Pos.Y) / 2 == CenterVertexp->Pos.Y))
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
				if (((m_tVertex[GoalVertex].Pos.X + m_tVertex[GoalVertex + 5 + 1].Pos.X) / 2 == CenterVertexp->Pos.X) && ((m_tVertex[GoalVertex].Pos.Y + m_tVertex[GoalVertex + 5 + 1].Pos.Y) / 2 == CenterVertexp->Pos.Y))
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
			if (GoalVertex + 5 + 1== BreakVertex)RoadStop = true;//行き先が壊れた頂点なら
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
				if (((m_tVertex[GoalVertex].Pos.X + m_tVertex[GoalVertex + 5 - 1].Pos.X) / 2 == CenterVertexp->Pos.X) && ((m_tVertex[GoalVertex].Pos.Y + m_tVertex[GoalVertex + 5 - 1].Pos.Y) / 2 == CenterVertexp->Pos.Y))
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
				if (((m_tVertex[GoalVertex].Pos.X + m_tVertex[GoalVertex - 5 - 1].Pos.X) / 2 == CenterVertexp->Pos.X) && ((m_tVertex[GoalVertex].Pos.Y + m_tVertex[GoalVertex - 5 - 1].Pos.Y) / 2 == CenterVertexp->Pos.Y))
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
		vtx_FieldLine[i][0] = { {-LINE_SIZE, -LINE_SIZE, 0.0f}, {0.0f, 0.0f} };
		vtx_FieldLine[i][1] = { {-LINE_SIZE,  LINE_SIZE, 0.0f}, {0.0f, 1.0f} };
		vtx_FieldLine[i][2] = { { LINE_SIZE, -LINE_SIZE, 0.0f}, {1.0f, 0.0f} };
		vtx_FieldLine[i][3] = { { LINE_SIZE,  LINE_SIZE, 0.0f}, {1.0f, 1.0f} };
		m_pVtx_FieldLine[i] = CreateVertexBuffer(vtx_FieldLine[i], 4);
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
				bool BadShapes = false;
				for (int l = 2; l < MAX_VERTEX; l++)
				{
					if (Comparison3[l] != -1)//最後の点まで
					{
						//三点が縦か横の一直線上でないとき
						if (!((m_tVertex[Comparison3[l - 2]].Pos.X == m_tVertex[Comparison3[l - 1]].Pos.X && m_tVertex[Comparison3[l - 1]].Pos.X == m_tVertex[Comparison3[l]].Pos.X)
							|| (m_tVertex[Comparison3[l - 2]].Pos.Y == m_tVertex[Comparison3[l - 1]].Pos.Y && m_tVertex[Comparison3[l - 1]].Pos.Y == m_tVertex[Comparison3[l]].Pos.Y)))
						{
							//三点が斜めの一直線上でないとき
							if ((m_tVertex[Comparison3[l - 1]].Pos.X - m_tVertex[Comparison3[l - 2]].Pos.X) * (m_tVertex[Comparison3[l]].Pos.Y - m_tVertex[Comparison3[l - 1]].Pos.Y)
								!= (m_tVertex[Comparison3[l - 1]].Pos.Y - m_tVertex[Comparison3[l - 2]].Pos.Y) * (m_tVertex[Comparison3[l]].Pos.X - m_tVertex[Comparison3[l - 1]].Pos.X))
							{
								Count++;//辺が繋がってないとき角ができてる
							}
						}
					}
					else 
					{   
						//最後の辺とその一個前の辺の比較
						//三点が縦か横の一直線上でないとき
						if (!((m_tVertex[Comparison3[l - 2]].Pos.X == m_tVertex[Comparison3[l - 1]].Pos.X && m_tVertex[Comparison3[l - 1]].Pos.X == m_tVertex[Comparison3[0]].Pos.X)
							|| (m_tVertex[Comparison3[l - 2]].Pos.Y == m_tVertex[Comparison3[l - 1]].Pos.Y && m_tVertex[Comparison3[l - 1]].Pos.Y == m_tVertex[Comparison3[0]].Pos.Y)))
						{
							//三点が斜めの一直線上でないとき
							if ((m_tVertex[Comparison3[l - 1]].Pos.X - m_tVertex[Comparison3[l - 2]].Pos.X) * (m_tVertex[Comparison3[0]].Pos.Y - m_tVertex[Comparison3[l - 1]].Pos.Y)
								!= (m_tVertex[Comparison3[l - 1]].Pos.Y - m_tVertex[Comparison3[l - 2]].Pos.Y) * (m_tVertex[Comparison3[0]].Pos.X - m_tVertex[Comparison3[l - 1]].Pos.X))
							{
								Count++;//辺が繋がってないとき角ができてる	
							}
						}
						//最初の辺と最後の辺を比較
						//三点が縦か横の一直線上でないとき
						if (!((m_tVertex[Comparison3[l - 1]].Pos.X == m_tVertex[Comparison3[0]].Pos.X && m_tVertex[Comparison3[0]].Pos.X == m_tVertex[Comparison3[1]].Pos.X)
							|| (m_tVertex[Comparison3[l - 1]].Pos.Y == m_tVertex[Comparison3[0]].Pos.Y && m_tVertex[Comparison3[0]].Pos.Y == m_tVertex[Comparison3[1]].Pos.Y)))
						{
							//三点が斜めの一直線上でないとき
							if ((m_tVertex[Comparison3[l - 1]].Pos.X - m_tVertex[Comparison3[0]].Pos.X) * (m_tVertex[Comparison3[1]].Pos.Y - m_tVertex[Comparison3[0]].Pos.Y)
								!= (m_tVertex[Comparison3[l - 1]].Pos.Y - m_tVertex[Comparison3[0]].Pos.Y) * (m_tVertex[Comparison3[1]].Pos.X - m_tVertex[Comparison3[0]].Pos.X))
							{
								Count++;//辺が繋がってないとき角ができてる
							}		
						}
						////凹角形判定
						//if (Count != 3 && !(Count > 8))
						//{
						//	float Angle[2][8];//マックス角形の角度 右回りと左回りの角度を保存
						//	float AngleSum[2] = { 0.0f,0.0f };//角度の合計
						//	float ShapesAngle;//図形の内角の和
						//	int AngleDirectionNumber;//右回りの角度か左回りの角度か
						//	float AngleSave;
						//	float AngleDirection;
						//	int AngleSaveCount = 0;
						//	for (int l = 2; l < MAX_VERTEX; l++)
						//	{
						//		if (Comparison3[l] != -1)//最後の点まで
						//		{
						//			//内積の公式から
						//			AngleSave = ((m_tVertex[Comparison3[l - 2]].Pos.X - m_tVertex[Comparison3[l - 1]].Pos.X) * (m_tVertex[Comparison3[l]].Pos.X - m_tVertex[Comparison3[l - 1]].Pos.X) + (m_tVertex[Comparison3[l - 2]].Pos.Y - m_tVertex[Comparison3[l - 1]].Pos.Y) * (m_tVertex[Comparison3[l]].Pos.Y - m_tVertex[Comparison3[l - 1]].Pos.Y))/(sqrtf(powf((m_tVertex[Comparison3[l]].Pos.X - m_tVertex[Comparison3[l - 1]].Pos.X), 2) + powf((m_tVertex[Comparison3[l]].Pos.Y - m_tVertex[Comparison3[l - 1]].Pos.Y), 2)) * sqrtf(powf((m_tVertex[Comparison3[l - 2]].Pos.X - m_tVertex[Comparison3[l - 1]].Pos.X), 2) + powf((m_tVertex[Comparison3[l - 2]].Pos.Y - m_tVertex[Comparison3[l - 1]].Pos.Y), 2)));
						//			AngleDirection = (m_tVertex[Comparison3[l - 2]].Pos.X - m_tVertex[Comparison3[l - 1]].Pos.X) * (m_tVertex[Comparison3[l]].Pos.Y - m_tVertex[Comparison3[l - 1]].Pos.Y) + (m_tVertex[Comparison3[l - 2]].Pos.Y - m_tVertex[Comparison3[l - 1]].Pos.Y) * (m_tVertex[Comparison3[l]].Pos.X - m_tVertex[Comparison3[l - 1]].Pos.X);
						//			if (AngleDirection == 0)continue;//外積がゼロなら平行なので角度が存在しない
						//			if (1/*AngleDirection > 0*/)
						//			{
						//				//左回り
						//				Angle[0][AngleSaveCount] = TODEF(2 * PI - acosf(AngleSave));
						//				Angle[1][AngleSaveCount] = TODEF(acosf(AngleSave));
						//				AngleSum[0] += Angle[0][AngleSaveCount];
						//				AngleSum[1] += Angle[1][AngleSaveCount];
						//			}
						//			//else
						//			//{
						//			//	//右回り
						//			//	Angle[1][AngleSaveCount] = TODEF(2 * PI - acosf(AngleSave));
						//			//	Angle[0][AngleSaveCount] = TODEF(acosf(AngleSave));
						//			//	AngleSum[0] += Angle[0][AngleSaveCount];
						//			//	AngleSum[1] += Angle[1][AngleSaveCount];
						//			//}
						//			AngleSaveCount++;
						//		}
						//		else
						//		{
						//			AngleSave = ((m_tVertex[Comparison3[l - 2]].Pos.X - m_tVertex[Comparison3[l - 1]].Pos.X) * (m_tVertex[Comparison3[0]].Pos.X - m_tVertex[Comparison3[l - 1]].Pos.X) + (m_tVertex[Comparison3[l - 2]].Pos.Y - m_tVertex[Comparison3[l - 1]].Pos.Y) * (m_tVertex[Comparison3[0]].Pos.Y - m_tVertex[Comparison3[l - 1]].Pos.Y)) / (sqrtf(powf((m_tVertex[Comparison3[0]].Pos.X - m_tVertex[Comparison3[l - 1]].Pos.X), 2) + powf((m_tVertex[Comparison3[0]].Pos.Y - m_tVertex[Comparison3[l - 1]].Pos.Y), 2)) * sqrtf(powf((m_tVertex[Comparison3[l - 2]].Pos.X - m_tVertex[Comparison3[l - 1]].Pos.X), 2) + powf((m_tVertex[Comparison3[l - 2]].Pos.Y - m_tVertex[Comparison3[l - 1]].Pos.Y), 2)));
						//			AngleDirection = (m_tVertex[Comparison3[l - 2]].Pos.X - m_tVertex[Comparison3[l - 1]].Pos.X) * (m_tVertex[Comparison3[0]].Pos.Y - m_tVertex[Comparison3[l - 1]].Pos.Y) + (m_tVertex[Comparison3[l - 2]].Pos.Y - m_tVertex[Comparison3[l - 1]].Pos.Y) * (m_tVertex[Comparison3[0]].Pos.X - m_tVertex[Comparison3[l - 1]].Pos.X);
						//			if (AngleDirection != 0)
						//			{//外積がゼロなら平行なので角度が存在しない
						//				if (1/*AngleDirection > 0*/)
						//				{
						//					Angle[0][AngleSaveCount] = TODEF(2 * PI - acosf(AngleSave));
						//					Angle[1][AngleSaveCount] = TODEF(acosf(AngleSave));
						//					AngleSum[0] += Angle[0][AngleSaveCount];
						//					AngleSum[1] += Angle[1][AngleSaveCount];
						//				}
						//				/*else
						//				{
						//					Angle[1][AngleSaveCount] = TODEF(2 * PI - acosf(AngleSave));
						//					Angle[0][AngleSaveCount] = TODEF(acosf(AngleSave));
						//					AngleSum[0] += Angle[0][AngleSaveCount];
						//					AngleSum[1] += Angle[1][AngleSaveCount];
						//				}*/
						//				AngleSaveCount++;
						//			}

						//			AngleSave = ((m_tVertex[Comparison3[l - 1]].Pos.X - m_tVertex[Comparison3[0]].Pos.X) * (m_tVertex[Comparison3[1]].Pos.X - m_tVertex[Comparison3[0]].Pos.X) + (m_tVertex[Comparison3[l - 1]].Pos.Y - m_tVertex[Comparison3[0]].Pos.Y) * (m_tVertex[Comparison3[1]].Pos.Y - m_tVertex[Comparison3[0]].Pos.Y)) / (sqrtf(powf((m_tVertex[Comparison3[1]].Pos.X - m_tVertex[Comparison3[0]].Pos.X), 2) + powf((m_tVertex[Comparison3[1]].Pos.Y - m_tVertex[Comparison3[0]].Pos.Y), 2)) * sqrtf(powf((m_tVertex[Comparison3[l - 1]].Pos.X - m_tVertex[Comparison3[0]].Pos.X), 2) + powf((m_tVertex[Comparison3[l - 1]].Pos.Y - m_tVertex[Comparison3[0]].Pos.Y), 2)));
						//			AngleDirection = (m_tVertex[Comparison3[l - 1]].Pos.X - m_tVertex[Comparison3[0]].Pos.X) * (m_tVertex[Comparison3[1]].Pos.Y - m_tVertex[Comparison3[0]].Pos.Y) + (m_tVertex[Comparison3[l - 1]].Pos.Y - m_tVertex[Comparison3[0]].Pos.Y) * (m_tVertex[Comparison3[1]].Pos.X - m_tVertex[Comparison3[0]].Pos.X);
						//			if (AngleDirection != 0)
						//			{//外積がゼロなら平行なので角度が存在しない
						//				if (1/*AngleDirection > 0*/)
						//				{
						//					Angle[0][AngleSaveCount] = TODEF(2 * PI - acosf(AngleSave));
						//					Angle[1][AngleSaveCount] = TODEF(acosf(AngleSave));
						//					AngleSum[0] += Angle[0][AngleSaveCount];
						//					AngleSum[1] += Angle[1][AngleSaveCount];
						//				}
						//				/*else
						//				{
						//					Angle[1][AngleSaveCount] = TODEF(2 * PI - acosf(AngleSave));
						//					Angle[0][AngleSaveCount] = TODEF(acosf(AngleSave));
						//					AngleSum[0] += Angle[0][AngleSaveCount];
						//					AngleSum[1] += Angle[1][AngleSaveCount];
						//				}*/
						//				AngleSaveCount++;
						//			}
						//			break;
						//		}
						//	}
						//	ShapesAngle = (Count - 2) * 180.0f;//図形の内角を求める
						//	if (AngleSum[0] == ShapesAngle)AngleDirectionNumber = 0;//内角の和と等しい方を調べる
						//	else
						//	{
						//		if (AngleSum[1] == ShapesAngle)AngleDirectionNumber = 1;
						//		else BadShapes = true;
						//	}

						//	//for (int l = 0; l < AngleSaveCount; l++)
						//	//{
						//	//	if (Angle[AngleDirectionNumber][l] > 180)
						//	//	{
						//	//		BadShapes = true;//内角のどれかが180度を超えるなら図形とみなさない
						//	//		break;//一つでも見つければ処理抜ける
						//	//	}
						//	//}
						//}
						//if (Count > 8)BadShapes = true;//図形とみなさない(9角形以上の凹角形が存在しないため)
						////BadShapesをtrueにする
						break;//最後の点を見つけたら処理を終わってほしい
					}
				}
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
					m_pBattle->SaveAllyData(Shapes_Count[NowShapes],Shapes_Size);//図形の頂点と角数を渡す
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



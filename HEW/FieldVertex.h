//Character.h
//編集者：AT12A25 中島聖羅
#pragma once
#define _DIRECTX_
#include "DirectX.h"
#include "Character.h"
#include "Battle.h"
#include <d3d11.h>

#define MAX_VERTEX (25)//頂点数
#define MAX_CENTER_VERTEX (16)
#define MAX_SHAPE_VERTEX (10)//最大N角形
#define MAX_LINE (300)//最大の線の本数
#define LINE_SIZE (50)//線のサイズ
#define STAR_SIZE (100)//頂点の星のサイズ
#define VERTEX_POS_X (-1500)//頂点の描画位置X
#define VERTEX_POS_Y (-750)//頂点の描画位置Y
#define VERTEX_SIZE (400)//頂点間の間隔

class CPlayer;

class CFieldVertex
{
public:
	CFieldVertex();
	~CFieldVertex();

	void Update();
	void Draw();

	CVector3<float> GetVertexPos(int);
	bool GetRoadStop(int);

	void SetBattleAddress(CBattle*);
	void SetPlayerAddress(CPlayer*);
	
private:

	typedef struct FieldVertex
	{
		CVector3<float> Pos;
		bool Use;
		int Connect[8];
		int Number;
	};

	typedef struct CenterVertex
	{
		CVector3<float> Pos;
		bool Use;
	};

	FieldVertex m_tVertex[MAX_VERTEX];//フィールドの頂点情報保存
	CenterVertex m_tCenter_Vertex[MAX_CENTER_VERTEX];//フィールドのセンターの頂点情報保存

	int OrderVertex[MAX_LINE + 1];//頂点たどる順番格納
	int OrderVertexCount;//頂点何回たどったか

	int StartVertex;//今の始点
	int GoalVertex;//たどり着いた最新の頂点

	int NowShapes;//今の多角形の保存場所
	int Shapes_Count[MAX_ALLY];//何角形か
	float Shapes_Size;//図形の面積(大きさ)
	int Comparison_Shapes_Vertex_Save[MAX_ALLY][MAX_VERTEX];//比較用の図形の頂点保存
	int Shapes_Vertex_Save[MAX_ALLY][MAX_VERTEX]; //頂点をDraw順に並べた頂点情報保存
	int Comparison[MAX_VERTEX];//できた図形の頂点を元々ある図形の頂点と比較

	int BreakVertex;//壊れる頂点情報

	ID3D11Buffer* m_pVtx_FieldVertex;
	ID3D11ShaderResourceView* m_pTex_FieldVertex;
	ID3D11ShaderResourceView* m_pTex_FieldUseVertex;
	float m_offsetU_Field;

	ID3D11Buffer* m_pVtx_FieldLine[MAX_LINE];
	ID3D11ShaderResourceView* m_pTex_FieldLine;
	int NowLine;//何個目の線か
	CVector3<float> DrawLinePos[MAX_LINE];

	bool RoadStop;//プレイヤーの行ける方向
	CVector3<float> PlayerPos;//プレイヤーの位置保存

	CBattle* m_pBattle;//バトルクラスのポインター

	CPlayer* m_pPlayer;//プレイヤークラスのポインター

	void ShapesCheck(FieldVertex VertexNumber);//多角形判定再帰処理
};

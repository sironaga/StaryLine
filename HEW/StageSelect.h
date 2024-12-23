
#pragma once

#define _DIRECTX_
#include"DirectX.h"

enum E_SELECT_TYPE
{
	STAGE1 = 0,
	STAGE2,
	STAGE3
};

class CStageSelect
{
public:
	CStageSelect();
	~CStageSelect();

	//プロトタイプ宣言
	void UpdateStageSelect();
	void DrawStageSelect();

	int GetStageNum();
private:
	POLYGON_BUFFER* m_vSTAGE1_BackGround;
	POLYGON_TEXTURE* m_tStage1_BackGround;
	POLYGON_BUFFER* m_vSTAGE2_BackGround;
	POLYGON_TEXTURE* m_tStage2_BackGround;
	POLYGON_BUFFER* m_vSTAGE3_BackGround;
	POLYGON_TEXTURE* m_tStage3_BackGround;
	POLYGON_BUFFER* m_vRight_Select;
	POLYGON_TEXTURE* m_tRight_Select;
	POLYGON_BUFFER* m_vLeft_Select;
	POLYGON_TEXTURE* m_tLeft_Select;
	float f_Rotation;
	float f_Rad;
	float f_SelectX;
	float f_SelectY;

};

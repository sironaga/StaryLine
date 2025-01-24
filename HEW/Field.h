#pragma once
#include "Model.h"
#include "Main.h"

#define MAX_MODEL (1)
#define MAX_FIELD_MODEL (1)

class Field
{
public:
enum class EStageType
{
	STAGE_GRASSLAND = 0,
	STAGE_DESERT = 1,
	STAGE_SNOWFIELD = 2,
	MAX,
};
	Field(StageType StageNum);
	~Field();

	//void FieldModelInit(FieldModel*, int);

	void Update();

	void Draw();


	void Reload(StageType StageNum);
	//void ModelDraw(FieldModel*, int);

	//void FieldModelUpdate(FieldModel*, int);

private:
	DirectX::XMFLOAT3 m_Pos;
	DirectX::XMFLOAT3 m_Angle;
	DirectX::XMFLOAT3 m_Scale;

	Model* m_pModel;
};
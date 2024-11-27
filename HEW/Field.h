#pragma once
#include "Model.h"
#include "Camera.h"

#define MAX_MODEL (1)
#define MAX_FIELD_MODEL (1)

class Field
{
public:
	enum ModelName
	{
		TILELOW_FOREST = 0,
		MODEL_END
	};
	struct FieldModel
	{
		DirectX::XMFLOAT3 m_Pos;
		DirectX::XMFLOAT3 m_Angle;
		DirectX::XMFLOAT3 m_Scale;
		bool m_Use;//Žg‚Á‚Ä‚¢‚é‚©
		ModelName m_Model;//ƒ‚ƒfƒ‹‚ÌŽí—Þ

	};

	Field();
	~Field();

	void FieldModelInit(FieldModel*, int);

	void Update();

	void Draw();

	void ModelDraw(FieldModel*, int);

	void FieldModelUpdate(FieldModel*, int);

private:

	FieldModel m_Field_Model[MAX_FIELD_MODEL];

	Camera* m_pCamera;

	Model* m_pModel[MAX_MODEL];
};
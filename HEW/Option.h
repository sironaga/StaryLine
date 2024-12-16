#pragma once

#include "SpriteEx.h"

#define OPTION_KIND (2)

class COption
{
public:
	COption();
	~COption();
	void Update();
	void Draw();

	void SetOption();
	bool GetOption();
	float GetBGMVoluem();
	float GetSEVoluem();
private:
	enum E_OPTION_IMAGE
	{
		IMAGE_BACKGROUND = 0,
		IMAGE_OPTION_BACK,
		IMAGE_POINT,



		IMAGE_MAX
	};

	enum E_OPTION_KIND
	{
		KIND_BGM = 0,
		KIND_SE,


		KIND_MAX
	};

	DirectX::XMFLOAT3 m_tOptionPos[KIND_MAX];
	DirectX::XMFLOAT3 m_tBarPos[2];

	SpriteEx* m_pOptions[IMAGE_MAX];
	bool m_bOption;
	int m_nSelect;
	int m_nOptionValue[OPTION_KIND];
};
#include "WriteName.h"
#include "DirectX.h"
#include "Main.h"

CWriteName::CWriteName()
	:m_pWrite(nullptr)
{
	m_pWrite = new DirectWriteCustomFont(&data);
	m_pWrite->Init(GetSwapChain());

	// フォントデータを改変
	data.fontSize = 60;
	data.fontWeight = DWRITE_FONT_WEIGHT_ULTRA_BLACK;
	data.Color = D2D1::ColorF(D2D1::ColorF::Red);
	data.font = L"Assets\\Font\\Kazesawa-Extrabold.ttf";
	data.shadowColor = D2D1::ColorF(D2D1::ColorF::White);
	data.shadowOffset = D2D1::Point2F(5.0f, -5.0f);

	// フォントをセット
	m_pWrite->SetFont(data);
}

CWriteName::~CWriteName()
{
}

void CWriteName::Update()
{
	std::string checkkey = GetStringForKey();

	if (checkkey != "Error" && checkkey != "Back")
	{
		m_Name += checkkey;
	}
	else if (checkkey == "Back" && m_Name.size() != 0)
	{
		m_Name.erase(m_Name.size() - 1);
	}
}

void CWriteName::Draw()
{
	m_pWrite->DrawString(m_Name, DirectX::XMFLOAT2(90, 90), D2D1_DRAW_TEXT_OPTIONS_NONE);
}

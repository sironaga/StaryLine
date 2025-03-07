#include "WriteName.h"
#include "DirectX.h"
#include "Main.h"
#include "Defines.h"

std::string CWriteName::m_Name;

CWriteName::CWriteName()
	:m_pWrite(nullptr)
{
	m_pWrite = new DirectWriteCustomFont(&data);
	m_pWrite->Init(GetSwapChain());

	// フォントデータを改変
	data.fontSize = 80;
	data.fontWeight = DWRITE_FONT_WEIGHT_ULTRA_BLACK;
	data.Color = D2D1::ColorF(D2D1::ColorF::Blue);
	data.font = m_pWrite->GetFontName(1);
	data.shadowColor = D2D1::ColorF(D2D1::ColorF::White);
	data.shadowOffset = D2D1::Point2F(5.0f, -5.0f);

	// フォントをセット
	m_pWrite->SetFont(data);

	m_Name = "";
}

CWriteName::~CWriteName()
{
}

void CWriteName::Update()
{
	std::string checkkey = GetStringForKey();

	if (checkkey != "Error" && checkkey != "Back" && m_Name.size() < 10)
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
	m_pWrite->DrawString(m_Name, DirectX::XMFLOAT2( SCREEN_CENTER.x - 270.0f,SCREEN_CENTER.y + 100.0f ), D2D1_DRAW_TEXT_OPTIONS::D2D1_DRAW_TEXT_OPTIONS_NONE);
}

std::string CWriteName::GetPlayerName()
{
	return m_Name;
}

void CWriteName::SetPlayerName(std::string name)
{
	m_Name = name;
}

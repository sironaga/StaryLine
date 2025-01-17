#include "NumberUI.h"
#include "Main.h"

CNumberUI::CNumberUI()
{
	m_pNumber[0][0][nColor] = new SpriteEx("Assets/Texture/Number/Num_0.png");
	m_pNumber[0][1][nColor] = new SpriteEx("Assets/Texture/Number/Num_1.png");
	m_pNumber[0][2][nColor] = new SpriteEx("Assets/Texture/Number/Num_2.png");
	m_pNumber[0][3][nColor] = new SpriteEx("Assets/Texture/Number/Num_3.png");
	m_pNumber[0][4][nColor] = new SpriteEx("Assets/Texture/Number/Num_4.png");
	m_pNumber[0][5][nColor] = new SpriteEx("Assets/Texture/Number/Num_5.png");
	m_pNumber[0][6][nColor] = new SpriteEx("Assets/Texture/Number/Num_6.png");
	m_pNumber[0][7][nColor] = new SpriteEx("Assets/Texture/Number/Num_7.png");
	m_pNumber[0][8][nColor] = new SpriteEx("Assets/Texture/Number/Num_8.png");
	m_pNumber[0][9][nColor] = new SpriteEx("Assets/Texture/Number/Num_9.png");
	
	m_pNumber[1][0][nColor] = new SpriteEx("Assets/Texture/Number/Num_0_Bold.png");
	m_pNumber[1][1][nColor] = new SpriteEx("Assets/Texture/Number/Num_1_Bold.png");
	m_pNumber[1][2][nColor] = new SpriteEx("Assets/Texture/Number/Num_2_Bold.png");
	m_pNumber[1][3][nColor] = new SpriteEx("Assets/Texture/Number/Num_3_Bold.png");
	m_pNumber[1][4][nColor] = new SpriteEx("Assets/Texture/Number/Num_4_Bold.png");
	m_pNumber[1][5][nColor] = new SpriteEx("Assets/Texture/Number/Num_5_Bold.png");
	m_pNumber[1][6][nColor] = new SpriteEx("Assets/Texture/Number/Num_6_Bold.png");
	m_pNumber[1][7][nColor] = new SpriteEx("Assets/Texture/Number/Num_7_Bold.png");
	m_pNumber[1][8][nColor] = new SpriteEx("Assets/Texture/Number/Num_8_Bold.png");
	m_pNumber[1][9][nColor] = new SpriteEx("Assets/Texture/Number/Num_9_Bold.png");
					
	m_pNumber[0][0][nColor] = new SpriteEx("Assets/Texture/Number/White/Num_0.png");
	m_pNumber[0][1][nColor] = new SpriteEx("Assets/Texture/Number/White/Num_1.png");
	m_pNumber[0][2][nColor] = new SpriteEx("Assets/Texture/Number/White/Num_2.png");
	m_pNumber[0][3][nColor] = new SpriteEx("Assets/Texture/Number/White/Num_3.png");
	m_pNumber[0][4][nColor] = new SpriteEx("Assets/Texture/Number/White/Num_4.png");
	m_pNumber[0][5][nColor] = new SpriteEx("Assets/Texture/Number/White/Num_5.png");
	m_pNumber[0][6][nColor] = new SpriteEx("Assets/Texture/Number/White/Num_6.png");
	m_pNumber[0][7][nColor] = new SpriteEx("Assets/Texture/Number/White/Num_7.png");
	m_pNumber[0][8][nColor] = new SpriteEx("Assets/Texture/Number/White/Num_8.png");
	m_pNumber[0][9][nColor] = new SpriteEx("Assets/Texture/Number/White/Num_9.png");

	color.X = color.Y = color.Z = fAlpha = 1.0f;			 
}

CNumberUI::~CNumberUI()
{
	for (int nLoopY = 0; nLoopY < 2; nLoopY++)
	{
		for (int nLoopX = 0; nLoopX < 10; nLoopX++)
		{
			for(int nLoopZ = 0; nLoopZ < 10; nLoopZ++)
			{ 
				if (m_pNumber[nLoopY][nLoopX][nLoopZ])
				{
					delete m_pNumber[nLoopY][nLoopX][nLoopZ];
					m_pNumber[nLoopY][nLoopX][nLoopZ] = nullptr;
				}
			}
		}
	}
}

void CNumberUI::Draw()
{
	int x, y, z, l;
	x = y = z = l = 0;
	x = y = nNumber;

	for (int nLoop = 0; nLoop < nLong; nLoop++)
	{
		y = y / 10;
		z = y;

		for (int nLoopZ = 0; nLoopZ < (nLoop + 1); nLoopZ++)
		{
			z = z * (10);
		}

		l = x - z;
		for (int nLoopZ = 0; nLoopZ < nLoop; nLoopZ++)
		{
			l = l / 10;
		}
		m_pNumber[0][l][nColor]->Setcolor(color.X, color.Y, color.Z, fAlpha);
		m_pNumber[0][l][nColor]->SetRotation(0.0f, TORAD(180.0f), TORAD(180.0f));
		m_pNumber[0][l][nColor]->SetTexture();
		m_pNumber[0][l][nColor]->SetView(Get2DView());
		m_pNumber[0][l][nColor]->SetProjection(Get2DProj());
		m_pNumber[0][l][nColor]->SetPositon(Pos.X + ((Scale.X * 1000.0f) * (nLong / 2)) - (Scale.X * 1000.0f) * nLoop, Pos.Y, Pos.Z);
		m_pNumber[0][l][nColor]->SetSize(Scale.X, Scale.Y, Scale.Z);
		m_pNumber[0][l][nColor]->Disp();
		m_pNumber[0][l][nColor]->Setcolor(1.0f, 1.0f, 1.0f, 1.0f);
	}

}

void CNumberUI::Update()
{

}

void CNumberUI::SetNumber(int number)
{
	nNumber = number;
	nLong = 1;
	int x;
	x = number;

	while (1)
	{
		x = x / 10;
		if (x)
		{
			nLong++;
		}
		else
		{
			break;
		}
	}
}

void CNumberUI::SetPos(FLOAT3 pos)
{
	Pos = pos;
}

void CNumberUI::SetScale(FLOAT3 scale)
{
	Scale = scale;
}

void CNumberUI::SetColor(float R, float G, float B, float A)
{
	color.X = R;
	color.Y = G;
	color.Z = B;
	fAlpha = A;
}

void CNumberUI::SetLend(int InData)
{
	nLong = InData;
}

void CNumberUI::SetNumberColor(int InColor)
{
	nColor = InColor;
}

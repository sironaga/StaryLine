#include "NumberUI.h"
#include "Main.h"

CNumberUI::CNumberUI()
{
	m_pNumber[0][0] = new SpriteEx("Assets/Texture/Number/Num_0.png");
	m_pNumber[0][1] = new SpriteEx("Assets/Texture/Number/Num_1.png");
	m_pNumber[0][2] = new SpriteEx("Assets/Texture/Number/Num_2.png");
	m_pNumber[0][3] = new SpriteEx("Assets/Texture/Number/Num_3.png");
	m_pNumber[0][4] = new SpriteEx("Assets/Texture/Number/Num_4.png");
	m_pNumber[0][5] = new SpriteEx("Assets/Texture/Number/Num_5.png");
	m_pNumber[0][6] = new SpriteEx("Assets/Texture/Number/Num_6.png");
	m_pNumber[0][7] = new SpriteEx("Assets/Texture/Number/Num_7.png");
	m_pNumber[0][8] = new SpriteEx("Assets/Texture/Number/Num_8.png");
	m_pNumber[0][9] = new SpriteEx("Assets/Texture/Number/Num_9.png");

	m_pNumber[1][0] = new SpriteEx("Assets/Texture/Number/Num_0_Bold.png");
	m_pNumber[1][1] = new SpriteEx("Assets/Texture/Number/Num_1_Bold.png");
	m_pNumber[1][2] = new SpriteEx("Assets/Texture/Number/Num_2_Bold.png");
	m_pNumber[1][3] = new SpriteEx("Assets/Texture/Number/Num_3_Bold.png");
	m_pNumber[1][4] = new SpriteEx("Assets/Texture/Number/Num_4_Bold.png");
	m_pNumber[1][5] = new SpriteEx("Assets/Texture/Number/Num_5_Bold.png");
	m_pNumber[1][6] = new SpriteEx("Assets/Texture/Number/Num_6_Bold.png");
	m_pNumber[1][7] = new SpriteEx("Assets/Texture/Number/Num_7_Bold.png");
	m_pNumber[1][8] = new SpriteEx("Assets/Texture/Number/Num_8_Bold.png");
	m_pNumber[1][9] = new SpriteEx("Assets/Texture/Number/Num_9_Bold.png");
}

CNumberUI::~CNumberUI()
{
	for (int nLoopY = 0; nLoopY < 2; nLoopY++)
	{
		for (int nLoopX = 0; nLoopX < 10; nLoopX++)
		{
			if (m_pNumber[nLoopY][nLoopX])
			{
				delete m_pNumber[nLoopY][nLoopX];
				m_pNumber[nLoopY][nLoopX] = nullptr;
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
		m_pNumber[0][l]->SetTexture();
		m_pNumber[0][l]->SetView(Get2DView());
		m_pNumber[0][l]->SetProjection(Get2DProj());
		m_pNumber[0][l]->SetPositon(Pos.X - (SIZE * (nLong / 2)) + SIZE * nLoop, Pos.Y, Pos.Z);
		m_pNumber[0][l]->SetSize(SIZE, SIZE, SIZE);
		m_pNumber[0][l]->Disp();
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

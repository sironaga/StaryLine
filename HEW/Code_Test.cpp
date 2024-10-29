/* コードのテスト用 */
#include"Code_Test.h"
#include"DirectXPlus.h"

FLOAT3 TestPositon;

void CodeTestInit()
{



}

void CodeTestUpdate()
{

	if (IsKeyPress(VK_UP))
	{
		TestPositon.Z += 0.1f;
	}

	if (IsKeyPress(VK_DOWN))
	{
		TestPositon.Z -= 0.1f;
	}
	if (IsKeyPress(VK_RIGHT))
	{
		TestPositon.X += 0.1f;
	}

	if (IsKeyPress(VK_LEFT))
	{
		TestPositon.X -= 0.1f;
	}

}

void CodeTestDraw()
{
	SetTranslation(TestPositon);
	Geometory::DrawBox();
	ResetData();
}

void CodeTestUnInit()
{



}
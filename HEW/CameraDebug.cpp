#include "CameraDebug.h"
#include "Defines.h"
#define MOVESPEED (1.0f)


CameraDebug::CameraDebug()
	:m_radXZ(TORAD(0.0f)), m_radY(TORAD(0.0f)), m_radius(100.0f) //0.0f,0.0f,10.0f
{

}

void CameraDebug::Update()
{
	// 注視点の移動		    
	if (IsKeyPress(VK_UP)) { m_look.z += 0.1f; }
	if (IsKeyPress(VK_DOWN)) { m_look.z -= 0.1f; }
	if (IsKeyPress(VK_RIGHT)) { m_look.x += 0.1f; }
	if (IsKeyPress(VK_LEFT)) { m_look.x -= 0.1f; }
	if (IsKeyPress(VK_SHIFT)) { m_look.y += 0.1f; }
	if (IsKeyPress(VK_CONTROL)) { m_look.y -= 0.1f; }

	//カメラの位置の移動//
	// 回り込みの処理
	if (IsKeyPress('U') && IsKeyPress('A')) { m_radXZ += 0.01f; }
	if (IsKeyPress('U') && IsKeyPress('D')) { m_radXZ -= 0.01f; }
	if (IsKeyPress('U') && IsKeyPress('W')) { m_radY += 0.01f; }
	if (IsKeyPress('U') && IsKeyPress('S')) { m_radY -= 0.01f; }

	//カメラの距離を変える処理
	if (IsKeyPress('U') && IsKeyPress('E')) { m_radius += 0.1f; }
	if (IsKeyPress('U') && IsKeyPress('Q')) { m_radius -= 0.1f; }

	//カメラ位置の計算
	m_pos.x = cos(m_radY) * sin(m_radXZ) * m_radius + m_look.x;
	m_pos.y = sin(m_radY) * m_radius + m_look.y;
	m_pos.z = cos(m_radY) * cos(m_radXZ) * m_radius + m_look.z;
}

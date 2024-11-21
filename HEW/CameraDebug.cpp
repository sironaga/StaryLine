#include "CameraDebug.h"
#define MOVESPEED (1.0f)


CameraDebug::CameraDebug()
	:m_radXZ(0.0f),m_radY(0.0f),m_radius(10.0f)
{
}

void CameraDebug::Update()
{
    // ’‹“_‚ÌˆÚ“®
    if (IsKeyPress(VK_UP)) { m_look.z += 0.1f; }
    if (IsKeyPress(VK_DOWN)) { m_look.z -= 0.1f; }
    if (IsKeyPress(VK_LEFT)) { m_look.x -= 0.1f; }
    if (IsKeyPress(VK_RIGHT)) { m_look.x += 0.1f; }
    if (IsKeyPress(VK_SHIFT)) { m_look.y += 0.1f; }
    if (IsKeyPress(VK_CONTROL)) { m_look.y -= 0.1f; }

    // ƒJƒƒ‰‚ÌˆÚ“®
    if (IsKeyPress('A')) { m_radXZ += 0.01f; }
    if (IsKeyPress('D')) { m_radXZ -= 0.01f; }
    if (IsKeyPress('W')) { m_radY += 0.01f; }
    if (IsKeyPress('S')) { m_radY -= 0.01f; }
    // ƒJƒƒ‰‚Ì‹——£
    if (IsKeyPress('E')) { m_radius += 1.0f; }
    if (IsKeyPress('Q')) { m_radius -= 1.0f; }

    m_pos.x = cosf(m_radY) * sinf(m_radXZ) * m_radius + m_look.x;
    m_pos.y = sinf(m_radY) * m_radius + m_look.y;
    m_pos.z = cosf(m_radY) * cosf(m_radXZ) * m_radius + m_look.z;

}

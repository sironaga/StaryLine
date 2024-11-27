#include "Camera.h"
#include"Defines.h"

Camera::Camera()
	: m_pos(2.5f, 30.5f, -40.0f)
	, m_look(0.0f, 1.0f, 0.0f)
	, m_up(0.0f, 1.0f, 0.0f)
	, m_fovy(DirectX::XMConvertToRadians(60.0f))
	, m_aspect(16.0f / 9.0f)
	, m_near(CMETER(30.0f))
	, m_far(METER(1000.0f))
{}

Camera::~Camera()
{
}
/* ビュー行列の取得(デフォルトでは転置済みの行列を計算) */
DirectX::XMFLOAT4X4 Camera::GetViewMatrix(bool transpose)
{
	DirectX::XMFLOAT4X4 mat;
	DirectX::XMMATRIX view;

	DirectX::XMVECTOR vEyePosition = DirectX::XMVectorSet(m_look.x, m_look.y, m_look.z, 0.0f);
	DirectX::XMVECTOR vFocusPosition = DirectX::XMVectorSet(m_pos.x, m_pos.y, m_pos.z, 0.0f);
	DirectX::XMVECTOR vUpDirection = DirectX::XMVectorSet(m_up.x, m_up.y, m_up.z, 0);

	view = DirectX::XMMatrixLookAtLH(vEyePosition, vFocusPosition, vUpDirection);

	if (transpose)
	{
		view = DirectX::XMMatrixTranspose(view);
	}
	DirectX::XMStoreFloat4x4(&mat, view);
	return mat;
}
/* プロジェクション行列の取得(デフォルトでは転置済みの行列を計算) */
DirectX::XMFLOAT4X4 Camera::GetProjectionMatrix(bool transpose)
{
	DirectX::XMFLOAT4X4 mat;
	DirectX::XMMATRIX proj;
	proj = DirectX::XMMatrixPerspectiveFovLH(TORAD(m_fovy),m_aspect,m_near,m_far);
	if (transpose) {
		proj = DirectX::XMMatrixTranspose(proj);
	}
	DirectX::XMStoreFloat4x4(&mat, proj);
	return mat;
}

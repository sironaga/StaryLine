#pragma once
#include<DirectXMath.h>

class Camera
{
public:
	Camera();
	virtual ~Camera();
	virtual void Update() = 0;
	DirectX::XMFLOAT4X4 GetViewMatrix(bool transpose = true);
	DirectX::XMFLOAT4X4 GetProjectionMatrix(bool transpose = true);
	DirectX::XMFLOAT3 GetPos() { return m_pos; }
	DirectX::XMFLOAT3 GetLook() { return m_look; }
	DirectX::XMMATRIX GetView();
	DirectX::XMMATRIX GetProjection();
	virtual void SetCameraPos(DirectX::XMFLOAT3 look);
	virtual void SetCameraRotate(DirectX::XMFLOAT3 rotate);
	void SetFovY(float radFov);
protected:
	DirectX::XMFLOAT3 m_pos;	/* 座標 */
	DirectX::XMFLOAT3 m_look;	/* 注視点 */
	DirectX::XMFLOAT3 m_up;		/* 上方ベクトル*/
	float m_fovy;				/* 画角 */
	float m_aspect;				/* アスペクト比 */
	float m_near;				/* ニアクリップ */
	float m_far;				/* ファークリップ*/
};


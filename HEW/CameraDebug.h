#pragma once
#include"Camera.h"
#include"Input.h"

class CameraDebug :public Camera
{
public:
	CameraDebug();
	~CameraDebug() {};
	virtual void Update()override;
	virtual void SetCameraPos(DirectX::XMFLOAT3 look)override;

private:
	float m_radXZ; /* カメラの横回転 */
	float m_radY;  /* カメラの横回転 */
	float m_radius;/* カメラと注視点の距離 */

};


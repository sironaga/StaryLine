#pragma once
/* 構造体リスト */
#include <DirectXMath.h>

// === Float_Struct
struct FLOAT2
{
	float X;
	float Y;
};

struct FLOAT3
{
	float X;
	float Y;
	float Z;
};

// === UDP_Conecting_System_Struct

#define IPV4_LEN (20)

enum CSEC
{
	CSEC_OK,
	CSEC_ERROR,
};

enum SPR
{
	SAVER_MODE_1,
	SAVER_MODE_2,
};

enum STM
{
	TIME_OUT_NOPROSESS,
	TIME_OUT_CALLING,
	TIME_OUT_RESPONSE,
};

/* ユーザーの個人データ */
struct Personal_Information_Data
{
	int nConnctinKey;				/* ユーザー認証キー */
	char IPv4[IPV4_LEN];			/* IPv4アドレス */
};

/* システムデータ */
struct Connecting_System_Data
{
	bool bUse = false;				/* 使用判定 */
	SPR eProsessMode;				/* 処理モードの選択 */
	STM eTimeOutProsess;			/* 接続確認処理 */
	int nTimeOutTImer;				/* 接続確認用タイマー */
	int nCalling_Number;			/* 呼出番号 */


};

/* Result用データ */
struct ResultGameInfo
{
	bool bWin;
	int nTime; 
	int nHitPoint;
	int nSpawnCount;
	int nAverageSpwn;
	int nDrawCount;
};

struct Communication_Data
{
	Personal_Information_Data PersonalData;
	Connecting_System_Data ConectionData;
};

struct SpriteParam
{
public:
	DirectX::XMFLOAT2 pos;
	DirectX::XMFLOAT2 size;
	DirectX::XMFLOAT4 color;
	DirectX::XMFLOAT3 rotate;
	DirectX::XMFLOAT2 uvPos;
	DirectX::XMFLOAT2 uvSize;

	DirectX::XMFLOAT4X4 world;
	DirectX::XMFLOAT4X4 view;
	DirectX::XMFLOAT4X4 proj;

	SpriteParam()
	{
		pos = { 0.0f,0.0f };
		size = { 1.0f,1.0f };
		rotate = { 0.0f,0.0f,0.0f };
		color = { 1.0f,1.0f,1.0f,1.0f };
		uvPos = { 0.0f,0.0f };
		uvSize = { 1.0f,1.0f };
		DirectX::XMMATRIX mView = DirectX::XMMatrixLookAtLH(
			DirectX::XMVectorSet(0.0f, 0.0f, -0.3f, 0.0f),
			DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
			DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
		DirectX::XMStoreFloat4x4(&view, mView);
		DirectX::XMMATRIX mProj = DirectX::XMMatrixOrthographicOffCenterLH(
			0.0f, 1920.0f, 1080.0f, 0.0f, 0.1f, 100.0f);
		mProj = DirectX::XMMatrixTranspose(mProj);
		DirectX::XMStoreFloat4x4(&proj, mProj);
		world = {};
		mWorld = {};
	}

	DirectX::XMFLOAT4X4 operator()(
		DirectX::XMFLOAT3 rotateWorld = { 0.0f,0.0f,0.0f },
		DirectX::XMFLOAT2 scaleWorld = { 1.0f,1.0f},
		DirectX::XMFLOAT2 posWorld = { 1920.0f / 2.0f, 1080.0f / 2.0f }
		)
	{
		DirectX::XMFLOAT4X4 outWorld;
		DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(posWorld.x, posWorld.y, 0.0f);
		DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scaleWorld.x, scaleWorld.y * -1.0f, 1.0f);
		DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMVectorSet(rotateWorld.x, rotateWorld.y, rotateWorld.z, 0.0f));
		mWorld = S * R * T;
		mWorld = DirectX::XMMatrixTranspose(mWorld);
		DirectX::XMStoreFloat4x4(&outWorld, mWorld);
		return outWorld;
	}
private:
	DirectX::XMMATRIX mWorld;
};
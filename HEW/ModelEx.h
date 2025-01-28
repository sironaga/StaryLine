#pragma once

#include "ShaderList.h"
#include "Model.h"
#include "Defines.h"
#include <vector>

class CModelEx
{
public:
	CModelEx(const char* ModelFile, bool isAnime = false, Model::Flip flip = Model::None);
	CModelEx(const char* ModelFile, const char** AnimeDataArray, int AnimeArraySize, Model::Flip flip = Model::ZFlip,float size = 1.0f);
	~CModelEx();
	Model* GetModelData();
	void Draw();
	void SetViewMatrix(DirectX::XMFLOAT4X4 Camera);
	void SetProjectionMatrix(DirectX::XMFLOAT4X4 Camera);
	void SetScale(float X, float Y, float Z);
	void SetPostion(float X, float Y, float Z);
	void SetRotation(float X, float Y, float Z);
	void SetWorldMatrix(DirectX::XMFLOAT4X4 InWorld);

	bool IsAnimePlay(Model::AnimeNo PlayAnimeNo);
	void PlayAnime(Model::AnimeNo PlayAnimeNo, bool isLoop, float speed = 1.0f);
	void Step(float tick);
	void SetAnimeTime(Model::AnimeNo PlayAnimeNo, float SetTime);
private:
	DirectX::XMMATRIX T;
	DirectX::XMMATRIX S;
	DirectX::XMMATRIX R;
	DirectX::XMMATRIX mat;
	DirectX::XMFLOAT4X4 wvp[3];
	DirectX::XMMATRIX world, view, proj;
	float sX, sY, sZ;
	float tX, tY, tZ;
	DirectX::XMFLOAT4X4 ViewMatrix;
	DirectX::XMFLOAT4X4 ProjectionMatrix;
	Model* CModel;
	bool m_bAnime;
};


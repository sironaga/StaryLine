#include "ModelEx.h"

CModelEx::CModelEx(const char *ModelFile, bool isAnime)
	:m_bAnime(isAnime)
{

	CModel = new Model();
	CModel->Load(ModelFile);
	if (isAnime)
	{
		Model::AnimeNo anime = CModel->AddAnimation(ModelFile);
		CModel->Play(anime, true);
	}
	sX = sY = sZ = 0.0f;

	T = DirectX::XMMatrixTranslation(sX,sY,sZ);
	S = DirectX::XMMatrixScaling(0.1f, 0.1f, 0.1f);

	DirectX::XMMATRIX Rx = DirectX::XMMatrixRotationX(0.0f);//X軸回転行列
	DirectX::XMMATRIX Ry = DirectX::XMMatrixRotationY(0.0f);//Y軸回転行列
	DirectX::XMMATRIX Rz = DirectX::XMMatrixRotationZ(0.0f);//Z軸回転行列
	R = Rx * Ry * Rz;


	mat = S * R * T;


	EyePosition = DirectX::XMVectorSet(0.0f, 10.0f, -10.0f, 0.0f);
	FocusPosition = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
    UpDirection = DirectX::XMVectorSet(0, 1, 0, 0);


	world = mat;
	view = DirectX::XMMatrixLookAtLH(EyePosition, FocusPosition, UpDirection);
	proj = DirectX::XMMatrixPerspectiveFovLH(TORAD(60.0f), 16.0f / 9.0f, 10.0f, 90.0f);

	DirectX::XMStoreFloat4x4(&wvp[0], DirectX::XMMatrixTranspose(world));
	DirectX::XMStoreFloat4x4(&wvp[1], DirectX::XMMatrixTranspose(view));
	DirectX::XMStoreFloat4x4(&wvp[2], DirectX::XMMatrixTranspose(proj));
}

CModelEx::~CModelEx()
{
	if (CModel)
	{
		delete CModel;
		CModel = nullptr;
	}
}

void CModelEx::Draw()
{

	//シェーダーへ変換行列を設定
	ShaderList::SetWVP(wvp);	//引数にはXMFLOAT4x4型の、要素数３の配列のアドレスを渡すこと

	//モデルに使用する頂点シェーダー、ピクセルシェーダーを設定
	if (m_bAnime)CModel->SetVertexShader(ShaderList::GetVS(ShaderList::VS_ANIME));
	else CModel->SetVertexShader(ShaderList::GetVS(ShaderList::VS_WORLD));
	CModel->SetPixelShader(ShaderList::GetPS(ShaderList::PS_LAMBERT));


	//複数のメッシュで構成されている場合ある部分は金属的な表現、ある部分は非金属的な表現と
	//分ける場合がある前回の表示は同じマテリアルで一括表示していたため、メッシュごとにマテリアルを
	//切り替える
	for (int i = 0; i < CModel->GetMeshNum(); ++i)
	{
		//モデルのメッシュを取得
		Model::Mesh mesh = *CModel->GetMesh(i);
		//メッシュに割り当てられているマテリアルを取得
		Model::Material material = *CModel->GetMaterial(mesh.materialID);
		//シェーダーへマテリアルを設定
		ShaderList::SetMaterial(material);

		// ボーンの情報をシェーダーに送る
		if (m_bAnime)
		{
			DirectX::XMFLOAT4X4 bones[200];
			for (int i = 0; i < mesh.bones.size(); ++i)
			{
				DirectX::XMStoreFloat4x4(&bones[i], DirectX::XMMatrixTranspose(
					mesh.bones[i].invOffset * CModel->GetBone(mesh.bones[i].index)
				));
				ShaderList::SetBones(bones);
			}
		}
		
		//モデルの描画
		CModel->Draw(i);
	}

}

void CModelEx::SetViewMatrix(DirectX::XMFLOAT4X4 Camera)
{
	ViewMatrix = Camera;
	wvp[1] = ViewMatrix;
}

void CModelEx::SetProjection(DirectX::XMFLOAT4X4 Camera)
{
	ProjectionMatric = Camera;
	wvp[2] = ProjectionMatric;
}

void CModelEx::SetScale(float X, float Y, float Z)
{
	S = DirectX::XMMatrixScaling(X, Y, Z);
	mat = S * R * T;
	world = mat;
	DirectX::XMStoreFloat4x4(&wvp[0], DirectX::XMMatrixTranspose(world));
}

void CModelEx::SetPostion(float X, float Y, float Z)
{
	T = DirectX::XMMatrixTranslation(X, Y, Z);
	mat = S * R * T;
	world = mat;
	DirectX::XMStoreFloat4x4(&wvp[0], DirectX::XMMatrixTranspose(world));
}

void CModelEx::SetRotation(float X, float Y, float Z)
{
	DirectX::XMMATRIX Rx = DirectX::XMMatrixRotationX(X);//X軸回転行列
	DirectX::XMMATRIX Ry = DirectX::XMMatrixRotationY(Y);//Y軸回転行列
	DirectX::XMMATRIX Rz = DirectX::XMMatrixRotationZ(Z);//Z軸回転行列
	R = Rx * Ry * Rz;
	mat = S * R * T;
	world = mat;
	DirectX::XMStoreFloat4x4(&wvp[0], DirectX::XMMatrixTranspose(world));
}

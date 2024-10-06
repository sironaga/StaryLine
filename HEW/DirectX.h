//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
// * DirectXを使用するための機能をまとめたヘッダー
// *使えそうな機能を追加してあります
// * 複数のファイルで使用する際は #_DIRECTX_ を宣言してください
//
// ＊file		DirectX
// ＊author	30015秋野翔太
// *Data 2024/06/07
// *Data 2024/06/08 設定項目の追加
// *Data 2024/06/10 デファインの追加
// *Data 2024/06/13 関数複数追加 アニメーションの実装
// *Data 2024/06/14 FLOAT3,4の構造体の追加、MakeFloat関数の追加
// *Data 2024/07/04 入力に関する関数の追加
// *Data 2024/09/16 サウンドに関する関数の追加
// *Data 2024/10/06 CONIOEX関数の削除
//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー

/*Looks*/
/*
#define _DEF
#define _ANIMETION
#define _FLOATS
*/

#ifndef __DIRECTX_
#define __DIRECTX_
// = Include
#include<math.h>
#include<d3d11.h>
#include <DirectXMath.h>
#include<timeapi.h>
#include <d3dcompiler.h>
#include<Windows.h>
#include<Digitalv.h>
#pragma comment(lib,"Winmm.lib")
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <mmsystem.h>
#include <mmreg.h>
#include <MSAcm.h>
#include <Shlwapi.h>
#include <vector>
#include <map>
#include <string>
#include <xaudio2.h>
#pragma comment(lib, "xaudio2.lib")

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "msacm32.lib")
#pragma comment(lib, "shlwapi.lib")

// = Define

//  ■　= Seting =
// 
//  ■　 画面サイズ
#define WIND_W (1920)
#define WIND_H (1080)

//  ■　 フルスクリーン設定
#define  fullScreen (false)

//  ■　クラスネーム
#define CLASS_NAME "ClassNmae"

//  ■　ウィンドウネーム
#define WIND_NAME "HEW"

//  ■　sampDesc.AddressU
#define ADDRESS_U D3D11_TEXTURE_ADDRESS_WRAP

//  ■　sampDesc.AddressV
#define ADDRESS_V D3D11_TEXTURE_ADDRESS_WRAP

//  ■　sampDesc.AddressW
#define ADDRESS_W D3D11_TEXTURE_ADDRESS_WRAP

//  ■　sampDesc.Filter
// 補正あり D3D11_FILTER_MIN_MAG_MIP_LINEAR
// 補正なし D3D11_FILTER_MIN_MAG_MIP_POINT
#define FILTER D3D11_FILTER_MIN_MAG_MIP_LINEAR

//	テクスチャデータ用
#define POLYGON_TEXTURE ID3D11ShaderResourceView
//	ポリゴンデータ用
#define POLYGON_BUFFER	ID3D11Buffer
// WinMain
#define WINMAIN int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)

// ============================

struct Vertex {
	float pos[3];
	float uv[2];
};

//　Animation管理用
typedef struct
{
	int n_AnimeTime;
	int n_AnimeFrame;

}Animation;


typedef struct
{
	float X;
	float Y;
}FLOAT2;

typedef struct
{
	float X;
	float Y;
	float Z;
}FLOAT3;

typedef struct
{
	float X;
	float Y;
	float Z;
	float W;
}FLOAT4;

// Entity用
typedef struct
{
	float PosX;
	float PosY;
	float PosZ;
	bool bMove;
	bool bAttack;
	bool bEvent;
}Entity;

typedef struct
{
	DWORD time;
	DWORD oldTime;
	DWORD fpsCount;
	DWORD FPS;
	DWORD fpsTime;
}FPSTIMER;

// = Global
#ifndef _DIRECTX_



struct SpriteParameter
{
	DirectX::XMFLOAT2 pos;
	DirectX::XMFLOAT2 scale;
	DirectX::XMFLOAT2 uvPos;
	DirectX::XMFLOAT2 uvScale;
	DirectX::XMFLOAT4 color;
	DirectX::XMFLOAT2 screen;
	float rad;
	float dummy;
};

typedef struct {
	int				nDevType;
	MCIDEVICEID		wDeviceID;
	char			szPath[MAX_PATH];
	int				nRepeat;
} __conioex_h_SoundInfo;

struct SoundData
{
	WAVEFORMATEX	format;		// WAVフォーマット
	BYTE* pBuffer;	// サウンドデータ
	DWORD			bufSize;	// データサイズ
	XAUDIO2_BUFFER	sound;		// サウンドバッファ
};
struct MP3FormatInfo
{
	DWORD offset;
	DWORD dataSize;
};
struct MP3FrameInfo
{
	BYTE channel;
	BYTE padding;
	DWORD sampleRate;
	DWORD bitRate;
	DWORD frameSize;
};



SpriteParameter g_SpriteParam;
ID3D11DeviceContext* g_pSpriteContext;
ID3D11ShaderResourceView* g_pSpriteTexture;
ID3D11ShaderResourceView* g_pSpriteWhiteTex;
ID3D11InputLayout* g_pSpriteIL;
ID3D11VertexShader* g_pSpriteVS;
ID3D11PixelShader* g_pSpritePS;
ID3D11Buffer* g_pSpriteBuffer;
ID3D11RasterizerState* g_pSpriteRasterizer;
ID3D11SamplerState* g_pSpriteSampler;
ID3D11Device* g_pDevice;
ID3D11DeviceContext* g_pContext;
IDXGISwapChain* g_pSwapChain;
ID3D11RenderTargetView* g_pRTV;
ID3D11BlendState* g_pBlendState;
ID3D11SamplerState* g_pSamplerState;
ID3D11BlendState* g_pAddBlend; // 加算合成の設定
HWND hWnd;
float AnimetionTimer;
BYTE g_KeyTable[256];
BYTE g_oldKeyTable[256];
using SoundKey = std::pair<std::string, SoundData>;
using SoundMap = std::map<std::string, SoundData>;
IXAudio2* g_pXAudio;
IXAudio2MasteringVoice* g_pMasterVoice;
SoundMap				g_soundMap;

const BYTE CMP_MATCH = 0;


/*PROTOTYPE*/
HRESULT ShaderCompile(ID3D11Device* pDevice, const char* vs, const char* ps);
HRESULT CreateVertexShader(ID3D11Device* pDevice, void* data, UINT size);
HRESULT CreatePixelShader(ID3D11Device* pDevice, void* data, UINT size);
ID3D11DeviceContext* GetContext(void);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
ID3D11Device* GetDevice(void);
HRESULT LoadWav(const char* file, SoundData* pData);
HRESULT LoadMP3(const char* file, SoundData* pData);
DWORD ReadMP3Format(HANDLE hFile, MP3FormatInfo* pFormat);
DWORD ReadMP3FrameHeader(HANDLE hFile, DWORD seek, MP3FrameInfo* pFrame);
DWORD ReadMP3Data(HANDLE hFile, DWORD seek, DWORD size, MP3FrameInfo* pFrame, SoundData* pData);

#endif /* _DIRECTX_ */
#endif /* __DIRECTX */

#ifndef _DEF

/*Sprite*/

void DrawSprite(ID3D11Buffer* pSprite, UINT vtxSize)
#ifdef _DIRECTX_
;
#else
{
	D3D11_BUFFER_DESC desc;
	pSprite->GetDesc(&desc);
	UINT num = desc.ByteWidth / vtxSize;
	UINT offset = 0;

	g_pSpriteContext->IASetInputLayout(g_pSpriteIL);
	g_pSpriteContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	g_pSpriteContext->IASetVertexBuffers(0, 1, &pSprite, &vtxSize, &offset);
	g_pSpriteContext->VSSetShader(g_pSpriteVS, NULL, 0);
	g_pSpriteContext->PSSetShader(g_pSpritePS, NULL, 0);
	g_pSpriteContext->UpdateSubresource(g_pSpriteBuffer, 0, nullptr, &g_SpriteParam, 0, 0);
	g_pSpriteContext->VSSetConstantBuffers(0, 1, &g_pSpriteBuffer);
	g_pSpriteContext->PSSetShaderResources(0, 1, &g_pSpriteTexture);

	g_pSpriteContext->Draw(num, 0);
}
#endif 

void SetSpritePos(float x, float y)
#ifdef _DIRECTX_
;
#else
{
	g_SpriteParam.pos.x = x;
	g_SpriteParam.pos.y = y;
}
#endif

void SetSpriteAngle(float rad)
#ifdef _DIRECTX_
;
#else
{
	g_SpriteParam.rad = rad;
}
#endif

void SetSpriteScale(float x, float y)
#ifdef _DIRECTX_
;
#else
{
	g_SpriteParam.scale.x = x;
	g_SpriteParam.scale.y = y;
}
#endif

void SetSpriteUVPos(float u, float v)
#ifdef _DIRECTX_
;
#else
{
	g_SpriteParam.uvPos.x = u;
	g_SpriteParam.uvPos.y = v;
}
#endif

void SetSpriteUVScale(float u, float v)
#ifdef _DIRECTX_
;
#else
{
	g_SpriteParam.uvScale.x = u;
	g_SpriteParam.uvScale.y = v;
}
#endif

void SetSpriteColor(float r, float g, float b, float a)
#ifdef _DIRECTX_
;
#else
{
	g_SpriteParam.color.x = r;
	g_SpriteParam.color.y = g;
	g_SpriteParam.color.z = b;
	g_SpriteParam.color.w = a;
}
#endif

void SetSpriteTexture(ID3D11ShaderResourceView* pTexture)
#ifdef _DIRECTX_
;
#else
{
	g_pSpriteTexture = pTexture;
}
#endif

void InitSpriteDrawer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UINT width, UINT height)
#ifdef _DIRECTX_
;
#else
{
	const char* VS = R"EOT(
struct VS_IN {
	float3 pos : POSITION0;
	float2 uv : TEXCOORD0;
};
struct VS_OUT {
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float4 color : COLOR0;
};
cbuffer Param : register(b0) {
	float2 pos;
	float2 scale;
	float2 uvPos;
	float2 uvScale;
	float4 color;
	float2 screen;
	float rad;
	float dummy;
};
VS_OUT main(VS_IN vin) {
	VS_OUT vout;
	vout.pos = float4(vin.pos, 1.0f);
	vout.pos.xy *= scale;
	float x = vout.pos.x * cos(rad) - vout.pos.y * sin(rad);
	float y = vout.pos.x * sin(rad) + vout.pos.y * cos(rad);
	vout.pos.x = x;
	vout.pos.y = y;
	vout.pos.xy += pos;
	vout.pos.xy /= screen;
	vout.pos.y = -vout.pos.y;
	vout.uv = vin.uv;
	vout.uv *= uvScale;
	vout.uv += uvPos;
	vout.color = color;
	return vout;
})EOT";
	const char* PS = R"EOT(
struct PS_IN {
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float4 color : COLOR0;
};
Texture2D tex : register(t0);
SamplerState samp : register(s0);
float4 main(PS_IN pin) : SV_TARGET {
	return tex.Sample(samp, pin.uv) * pin.color;
})EOT";

	g_SpriteParam.pos = { 0.0f, 0.0f };
	g_SpriteParam.scale = { 1.0f, 1.0f };
	g_SpriteParam.uvPos = { 0.0f, 0.0f };
	g_SpriteParam.uvScale = { 1.0f, 1.0f };
	g_SpriteParam.color = { 1.0f, 1.0f, 1.0f, 1.0f };
	g_SpriteParam.screen = { (float)width, (float)height };
	g_SpriteParam.rad = 0.0f;

	g_pSpriteContext = pContext;

	ShaderCompile(pDevice, VS, PS);


	HRESULT hr = E_FAIL;
	// カリング設定
	D3D11_RASTERIZER_DESC rasterizer = {};
	rasterizer.CullMode = D3D11_CULL_NONE;
	rasterizer.FillMode = D3D11_FILL_SOLID;
	rasterizer.FrontCounterClockwise = true;
	hr = pDevice->CreateRasterizerState(&rasterizer, &g_pSpriteRasterizer);
	if (FAILED(hr)) { return; }
	g_pSpriteContext->RSSetState(g_pSpriteRasterizer);

	// サンプラー
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	hr = pDevice->CreateSamplerState(&samplerDesc, &g_pSpriteSampler);
	if (FAILED(hr)) { return; }
	//g_pSpriteContext->PSSetSamplers(0, 1, &g_pSpriteSampler);
}
#endif 

void UninitSpriteDrawer()
#ifdef _DIRECTX_
;
#else
{
	if (g_pSpriteWhiteTex) g_pSpriteWhiteTex->Release();
	if (g_pSpriteVS) g_pSpriteVS->Release();
	if (g_pSpritePS) g_pSpritePS->Release();
	if (g_pSpriteIL) g_pSpriteIL->Release();
	if (g_pSpriteBuffer) g_pSpriteBuffer->Release();
	if (g_pSpriteRasterizer) g_pSpriteRasterizer->Release();
	if (g_pSpriteSampler) g_pSpriteSampler->Release();
}
#endif

HRESULT ShaderCompile(ID3D11Device* pDevice, const char* vs, const char* ps)
#ifdef _DIRECTX_
;
#else
{
	const char* ppCode[] = { vs, ps };
	const char* pTargetList[] = { "vs_5_0", "ps_5_0" };
	HRESULT hr;
	ID3DBlob* pBlob;
	ID3DBlob* error;

	for (int i = 0; i < 2; ++i)
	{
		// シェーダー解析
		UINT compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
		hr = D3DCompile(ppCode[i], strlen(ppCode[i]), nullptr, nullptr, nullptr,
			"main", pTargetList[i], compileFlag, 0, &pBlob, &error);
		if (FAILED(hr)) { return hr; }

		if (i == 0)
			hr = CreateVertexShader(pDevice, pBlob->GetBufferPointer(), (UINT)pBlob->GetBufferSize());
		else
			hr = CreatePixelShader(pDevice, pBlob->GetBufferPointer(), (UINT)pBlob->GetBufferSize());
		if (FAILED(hr)) { return hr; }

		if (pBlob) pBlob->Release();
		if (error) error->Release();
	}

	// 定数バッファ作成
	D3D11_BUFFER_DESC bufDesc = {};
	bufDesc.ByteWidth = sizeof(SpriteParameter);
	bufDesc.Usage = D3D11_USAGE_DEFAULT;
	bufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	// バッファの作成
	hr = pDevice->CreateBuffer(&bufDesc, nullptr, &g_pSpriteBuffer);
	if (FAILED(hr)) { return hr; }

	return hr;
}
#endif

HRESULT CreateVertexShader(ID3D11Device* pDevice, void* pData, UINT size)
#ifdef _DIRECTX_
;
#else
{
	// シェーダー作成
	HRESULT hr;
	hr = pDevice->CreateVertexShader(pData, size, NULL, &g_pSpriteVS);
	if (FAILED(hr)) { return hr; }

	ID3D11ShaderReflection* pReflection;
	D3D11_SHADER_DESC shaderDesc;
	D3D11_INPUT_ELEMENT_DESC* pInputDesc;
	D3D11_SIGNATURE_PARAMETER_DESC sigDesc;

	DXGI_FORMAT formats[][4] =
	{
		{
			DXGI_FORMAT_R32_UINT,
			DXGI_FORMAT_R32G32_UINT,
			DXGI_FORMAT_R32G32B32_UINT,
			DXGI_FORMAT_R32G32B32A32_UINT,
		}, {
			DXGI_FORMAT_R32_SINT,
			DXGI_FORMAT_R32G32_SINT,
			DXGI_FORMAT_R32G32B32_SINT,
			DXGI_FORMAT_R32G32B32A32_SINT,
		}, {
			DXGI_FORMAT_R32_FLOAT,
			DXGI_FORMAT_R32G32_FLOAT,
			DXGI_FORMAT_R32G32B32_FLOAT,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
		},
	};

	hr = D3DReflect(pData, size, IID_PPV_ARGS(&pReflection));
	if (FAILED(hr)) { return hr; }

	pReflection->GetDesc(&shaderDesc);
	pInputDesc = new D3D11_INPUT_ELEMENT_DESC[shaderDesc.InputParameters];
	for (UINT i = 0; i < shaderDesc.InputParameters; ++i)
	{
		pReflection->GetInputParameterDesc(i, &sigDesc);
		pInputDesc[i].SemanticName = sigDesc.SemanticName;
		pInputDesc[i].SemanticIndex = sigDesc.SemanticIndex;

		// http://marupeke296.com/TIPS_No17_Bit.html
		BYTE elementCount = sigDesc.Mask;
		elementCount = (elementCount & 0x05) + ((elementCount >> 1) & 0x05);
		elementCount = (elementCount & 0x03) + ((elementCount >> 2) & 0x03);

		switch (sigDesc.ComponentType)
		{
		case D3D_REGISTER_COMPONENT_UINT32:
			pInputDesc[i].Format = formats[0][elementCount - 1];
			break;
		case D3D_REGISTER_COMPONENT_SINT32:
			pInputDesc[i].Format = formats[1][elementCount - 1];
			break;
		case D3D_REGISTER_COMPONENT_FLOAT32:
			pInputDesc[i].Format = formats[2][elementCount - 1];
			break;
		}
		pInputDesc[i].InputSlot = 0;
		pInputDesc[i].AlignedByteOffset = i == 0 ? 0 : D3D11_APPEND_ALIGNED_ELEMENT;
		pInputDesc[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		pInputDesc[i].InstanceDataStepRate = 0;
	}

	hr = pDevice->CreateInputLayout(
		pInputDesc, shaderDesc.InputParameters,
		pData, size, &g_pSpriteIL
	);

	delete[] pInputDesc;
	return hr;
}
#endif

HRESULT CreatePixelShader(ID3D11Device* pDevice, void* data, UINT size)
#ifdef _DIRECTX_
;
#else
{
	return pDevice->CreatePixelShader(data, size, NULL, &g_pSpritePS);
}
#endif

/*DirectX*/
HRESULT InitDirectX()
#ifdef _DIRECTX_
;
#else
{
	


	//HRESULT...DirectXの処理結果を表すデータ
	//S_OKはプログラムの成功を表す
	//

	HRESULT hr = E_FAIL;

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferDesc.Width = WIND_W; //フレームバッファの幅
	sd.BufferDesc.Height = WIND_H;//フレームバッファの高さ
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//フレームバッファの色情報(各8bit)
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.SampleDesc.Count = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;//バックバッファの使用方法
	sd.BufferCount = 1;//バックバッファの数
	sd.OutputWindow = hWnd;//関連付けるウインドウ
	sd.Windowed = fullScreen ? FALSE : TRUE;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	//ドライバの種類
	D3D_DRIVER_TYPE driverType;
	D3D_DRIVER_TYPE driverTypes[] = {
		D3D_DRIVER_TYPE_HARDWARE,D3D_DRIVER_TYPE_WARP,D3D_DRIVER_TYPE_REFERENCE
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	//機能レベル
	D3D_FEATURE_LEVEL featureLevel;
	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_1,D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,D3D_FEATURE_LEVEL_9_2,D3D_FEATURE_LEVEL_9_1
	};

	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	//作成可能な設定でDirectXの機能を有効化
	for (UINT i = 0; i < numDriverTypes; i++)
	{
		driverType = driverTypes[i];
		hr = D3D11CreateDeviceAndSwapChain(
			NULL, driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pDevice, &featureLevel, &g_pContext
		);
		if (SUCCEEDED(hr)) { break; }
	}
	if (FAILED(hr)) { return hr; }

	//フレームバッファへのポインタを取得
	ID3D11Texture2D* pBackBuffer = nullptr;
	hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	//フレームバッファへのポインタで紐づけて、レンダーターゲットビューを作成
	if (SUCCEEDED(hr)) {
		hr = g_pDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_pRTV);
		g_pContext->OMSetRenderTargets(1, &g_pRTV, nullptr);//GPUの出力先
	}

	D3D11_VIEWPORT vp;
	ZeroMemory(&vp, sizeof(vp));
	vp.TopLeftX = 0.0f; // 表示位置 X
	vp.TopLeftY = 0.0f; // 表示位置 Y
	vp.Width = WIND_W; // 表示する幅
	vp.Height = WIND_H; // 表示する高さ
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	g_pContext->RSSetViewports(1, &vp);

	// 透過

	
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	hr = GetDevice()->CreateBlendState(&blendDesc, &g_pBlendState);
	

	// = テクスチャの貼り付けに関する設定
	// Filter ~_LENEAR 滑らかに
	//		  ~_POINT そのまま
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(D3D11_SAMPLER_DESC));
	sampDesc.Filter = FILTER; // 拡大・縮小を行ったとき、どう補間するか
	sampDesc.AddressU = ADDRESS_U; // Ｕ(横)方向の繰り返し設定
	sampDesc.AddressV = ADDRESS_V; // Ｖ(縦)方向の繰り返し設定
	sampDesc.AddressW = ADDRESS_W; // Ｗ(奥)方向の繰り返し設定(2Dでも設定が必要
	hr = g_pDevice->CreateSamplerState(&sampDesc, &g_pSamplerState);
	if (FAILED(hr)) { return hr; }
	GetContext()->PSSetSamplers(0, 1, &g_pSamplerState); // サンプラーステートを設定

	D3D11_BLEND_DESC addDesc;
	ZeroMemory(&addDesc, sizeof(addDesc));
	addDesc.RenderTarget[0].BlendEnable = TRUE;
	addDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	addDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	addDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	addDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	addDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	addDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	addDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	hr = GetDevice()->CreateBlendState(&addDesc, &g_pAddBlend);


	return hr;
}
#endif

// 通常のブレンドの設定
void SetAlphaBlend()
#ifdef _DIRECTX_
;
#else
{
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	GetContext()->OMSetBlendState(g_pBlendState, blendFactor, 0xffffffff);
}
#endif
// 加算合成の設定
void SetAddBlend()
#ifdef _DIRECTX_
;
#else
{
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	GetContext()->OMSetBlendState(g_pAddBlend, blendFactor, 0xffffffff);
}
#endif


void UnInitDirectX()
#ifdef _DIRECTX_
;
#else
{
	g_pRTV->Release();
	g_pSwapChain->SetFullscreenState(false, NULL);
	g_pSwapChain->Release();
	g_pContext->ClearState();
	g_pContext->Release();
	g_pDevice->Release();
}
#endif

void BeginDrawDirectX()
#ifdef _DIRECTX_
;
#else
{

	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	GetContext()->OMSetBlendState(g_pBlendState, blendFactor, 0xffffffff);

	//描画開始時に画面をクリア
	//                  R     G     B   透明度
	float color[4] = { 0.5f, 0.5f, 0.8f, 1.0f };
	g_pContext->ClearRenderTargetView(g_pRTV, color);
}
#endif

void EndDrawDirectX()
#ifdef _DIRECTX_
;
#else
{
	//描画完了時に画面へ出力
	g_pSwapChain->Present(0, 0);
}
#endif

ID3D11Device* GetDevice(void)
#ifdef _DIRECTX_
;
#else
{
	return g_pDevice;
}
#endif

ID3D11DeviceContext* GetContext(void)
#ifdef _DIRECTX_
;
#else
{
	return g_pContext;
}
#endif

/*VertTex*/

ID3D11Buffer* CreateVertexBuffer(void* vtxData, UINT vtxNum)
#ifdef _DIRECTX_
;
#else
{
	// バッファ情報 設定
	D3D11_BUFFER_DESC vtxBufDesc;
	ZeroMemory(&vtxBufDesc, sizeof(vtxBufDesc));
	vtxBufDesc.ByteWidth = sizeof(Vertex) * vtxNum; // バッファの大きさ
	vtxBufDesc.Usage = D3D11_USAGE_DEFAULT; // メモリ上での管理方法
	vtxBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER; // GPU上での利用方法
	// バッファ初期データ 設定
	D3D11_SUBRESOURCE_DATA vtxSubResource;
	ZeroMemory(&vtxSubResource, sizeof(vtxSubResource));
	vtxSubResource.pSysMem = vtxData; // バッファに流し込むデータ
	// 作成
	HRESULT hr;
	ID3D11Buffer* pVtxBuf;
	hr = GetDevice()->CreateBuffer(&vtxBufDesc, &vtxSubResource, &pVtxBuf);
	if (FAILED(hr)) { return nullptr; }
	return pVtxBuf;
}
#endif

/*Windows*/
void CreatWind(HINSTANCE hInstance_Wind, int nCmdShow_Wind)
#ifdef _DIRECTX_
;
#else
{
	WNDCLASSEX wcex;
	ZeroMemory(&wcex, sizeof(wcex));

	wcex.hInstance = hInstance_Wind;
	wcex.lpszClassName = CLASS_NAME;
	wcex.lpfnWndProc = WndProc;
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hIconSm = wcex.hIcon;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	//ウインドウの登録
	if (!RegisterClassEx(&wcex))
	{
		return;
	}
	//ウインドウの作成
	hWnd = CreateWindowEx(
		WS_EX_OVERLAPPEDWINDOW,
		wcex.lpszClassName, WIND_NAME,
		WS_CAPTION | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT,
		WIND_W, WIND_H,
		HWND_DESKTOP, NULL, hInstance_Wind, NULL);
	if (hWnd == NULL)
	{
		return;
	}
	//ウインドウの表示
	ShowWindow(hWnd, nCmdShow_Wind);
	UpdateWindow(hWnd);

	return;
}
#endif

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
#ifdef _DIRECTX_
;
#else
{
	switch (message)
	{
		//×ボタンが押されたときのメッセージ
	case WM_CLOSE:

		if (IDNO == MessageBox(hWnd, "終了しますか", "", MB_YESNO))
		{
			return 0;
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}
#endif

HWND GetHWND()
#ifdef _DIRECTX_
;
#else
{
	return hWnd;
}
#endif

/*keyboard*/



void KeyboardUpdate()
#ifdef _DIRECTX_
;
#else
{
	memcpy(g_oldKeyTable, g_KeyTable, sizeof(g_oldKeyTable));
	GetKeyboardState(g_KeyTable);
}
#endif

bool IsKeyPress(int nVirtKey)
#ifdef _DIRECTX_
;
#else
{
	return g_KeyTable[nVirtKey] & 0x80;
}
#endif

bool IsKeyTrigger(int nVirtKey)
#ifdef _DIRECTX_
;
#else
{
	return (g_KeyTable[nVirtKey] ^ g_oldKeyTable[nVirtKey]) & g_KeyTable[nVirtKey] & 0x80;

}
#endif

bool IsKeyRepeat(int nVirtKey)
#ifdef _DIRECTX_
;
#else
{
	return (g_KeyTable[nVirtKey] ^ g_oldKeyTable[nVirtKey]) & g_oldKeyTable[nVirtKey] & 0x80;
}
#endif








#endif /* _DEF */


/*自作関数*/
#ifndef _ANIMETION_

// Animation関数を使用する際に使う、
// Frame管理されているメインループないに使用してください
void SetFreamTime(void)
#ifdef _DIRECTX_
;
#else
{
	AnimetionTimer++;


	if (AnimetionTimer > 1200000)
	{
		AnimetionTimer = 0;
	}

}
#endif

float GetFreamTime(void)
#ifdef _DIRECTX_
;
#else
{
	return AnimetionTimer;
}
#endif

//角度からラジアン角に変更
float RadianConversion(float angle)
#ifdef _DIRECTX_
;
#else
{
	float add;
	add = 3.14f * angle;
	add = add / 180.0f;
	return add;
}
#endif

//ラジアン角から角度に変更
float AngleConversion(float angle)
#ifdef _DIRECTX_
;
#else
{
	float add;
	add = 180.0f * angle;
	add = add / 3.14f;
	return add;

}
#endif

// 振り子運動のアニメーション　SetSpritePosで使用
float Animation_Pendulum(float Frame, float Move)
#ifdef _DIRECTX_
;
#else
{
	float offsetX;
	offsetX = sin(AnimetionTimer * 360.0f / Frame * 3.14f / 180.0f) * Move;
	return offsetX;
}
#endif

// 揺れ物アニメーション SetSpriteAngleで使用
float Animetion_Angle(float Frame, float Move)
#ifdef _DIRECTX_
;
#else
{
	float Dangle;
	Dangle = sin(AnimetionTimer * 360.0f / Frame * 3.14f / 180.0f) * Move;
	return Dangle;
}
#endif

// 軟体アニメーション SetSpriteScale
FLOAT2 Animetion_softbody(float Frame,float MoveX,float MoveY,float litleScaleX,float litleScaleY)
#ifdef _DIRECTX_
;
#else
{
	FLOAT2 scale;
	float rad;
	rad = AnimetionTimer * 360.0f / Frame * 3.14f / 180.0f;
	scale.X = (sin(rad) * 0.5f + 0.5f) * MoveX + litleScaleX;
	scale.Y = (sin(rad + 3.14f) * 0.5f + 0.5f) * MoveY + litleScaleY;
	return scale;


}
#endif

void ReSetSprite(void)
#ifdef _DIRECTX_
;
#else
{
	SetSpriteScale(1.0f, 1.0f);
	SetSpriteAngle(0.0f);
	SetSpritePos(0.0f, 0.0f);
	SetSpriteUVPos(0.0f, 0.0f);
	SetSpriteUVScale(1.0f, 1.0f);
	SetSpriteColor(1.0f, 1.0f, 1.0f, 1.0f);
}
#endif

void ALLInit(HINSTANCE IhInstance, int Cmd)
#ifdef _DIRECTX_
;
#else
{
	CreatWind(IhInstance,Cmd);
	InitDirectX();
	InitSpriteDrawer(GetDevice(), GetContext(), WIND_W, WIND_H);

}
#endif

#endif /*_ANIMETION_*/

#ifndef _FLOATS_

FLOAT2 MakeFloat(float x, float y)
#ifdef _DIRECTX_
;
#else
{
	FLOAT2 Set;
	Set.X = x;
	Set.Y = y;
	return Set;
	
}
#endif

FLOAT3 MakeFloat(float x, float y, float z)
#ifdef _DIRECTX_
;
#else
{
	FLOAT3 Set;
	Set.X = x;
	Set.Y = y;
	Set.Z = z;
	return Set;
}
#endif

FLOAT4 MakeFloat(float x, float y, float z, float w)
#ifdef _DIRECTX_
;
#else
{
	FLOAT4 Set;
	Set.X = x;
	Set.Y = y;
	Set.Z = z;
	Set.W = w;
	return Set;
	
}
#endif

#endif/*_FLOATS_*/

/* Sound */
#ifndef _SOUND_
#ifdef _DIRECTX_
;
#else
/**
 * @brief 初期化
 * @return 処理結果
 */


HRESULT InitSound(void)
#
{
	HRESULT hr = E_FAIL;

	// COMオブジェクト初期化
	hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (FAILED(hr))
	{
		return hr;
	}

	// XAUDIO2初期化
	hr = XAudio2Create(&g_pXAudio);
	if (FAILED(hr))
	{
		return hr;
	}

	// マスターボイス(ソース)作成
	hr = g_pXAudio->CreateMasteringVoice(&g_pMasterVoice);

	return hr;
}
#endif
#ifdef _DIRECTX_
;
#else
/**
 * @brief 終了処理
 */
void UninitSound(void)
{
	// サウンドデータの削除
	SoundMap::iterator it = g_soundMap.begin();
	while (it != g_soundMap.end())
	{
		delete[] it->second.pBuffer;
		++it;
	}

	// XAudio2のオブジェクトを削除
	if (g_pMasterVoice != NULL)
	{
		g_pMasterVoice->DestroyVoice();
		g_pMasterVoice = NULL;
	}
	if (g_pXAudio != NULL)
	{
		g_pXAudio->Release();
		g_pXAudio = NULL;
	}
}
#endif

#ifdef _DIRECTX_
;
#else
/**
 * @brief サウンドファイルの読み込み
 * @param[in] file 読み込むファイル
 * @param[in] loop 繰り返し再生
 * @return サウンドバッファ
 */
XAUDIO2_BUFFER* LoadSound(const char* file, bool loop)
{
	SoundData data;

	// 読み込み済みのサウンドファイルか確認
	SoundMap::iterator it = g_soundMap.find(file);
	if (it != g_soundMap.end())
	{
		// 読み込み済みのサウンドファイルを返す
		return &it->second.sound;
	}

	// 拡張子ごとに読み込み処理実行
	HRESULT hr = E_FAIL;
	LPSTR ext = PathFindExtension(file);
	if (ext != NULL) {
		if (memcmp(ext, ".wav", 4) == 0) {
			hr = LoadWav(file, &data);
		}
		else if (memcmp(ext, ".mp3", 4) == 0) {
			hr = LoadMP3(file, &data);
		}
	}
	if (FAILED(hr)) {
		return NULL;
	}

	//--- バッファー作成
	ZeroMemory(&data.sound, sizeof(data.sound));
	// サウンドデータのバイト数
	data.sound.AudioBytes = data.bufSize;
	// サウンドデータの先頭アドレス
	data.sound.pAudioData = data.pBuffer;
	// ループ指定
	if (loop)
	{
		data.sound.LoopCount = XAUDIO2_LOOP_INFINITE;
	}
	data.sound.Flags = XAUDIO2_END_OF_STREAM;

	// 読み込み済みデータとして登録
	g_soundMap.insert(SoundKey(file, data));
	it = g_soundMap.find(file);

	return &it->second.sound;
}
#endif

#ifdef _DIRECTX_
;
#else
/**
 * @brief サウンド再生
 * @param[in] pSound サウンドバッファ
 */
IXAudio2SourceVoice* PlaySound(XAUDIO2_BUFFER* pSound)
{
	HRESULT hr;
	IXAudio2SourceVoice* pSource;

	// 再生するデータを探索
	SoundMap::iterator soundIt = g_soundMap.begin();
	while (soundIt != g_soundMap.end())
	{
		if (&soundIt->second.sound == pSound)
		{
			break;
		}
		++soundIt;
	}
	if (soundIt == g_soundMap.end())
	{
		// 該当のデータなし
		return NULL;
	}

	// フォーマットを指定してソースを作成
	/*----------
	* WAVEFORMATEX
	* wFormatTag
	*  フォーマット
	* nChannels
	*  チャンネル数
	*  1... モノラル
	*  2... ステレオ
	* wBitsPerSample
	*  1サンプルあたりのビット数
	*  8...128を無音とする0~255
	*  16...0を無音とする-32768~32767
	* nSamplesPerSec
	*  サンプリングレート
	*  (1秒あたりのサンプル数
	*  単位はHz(ヘルツ
	*----------*/
	hr = g_pXAudio->CreateSourceVoice(&pSource, &soundIt->second.format);
	if (FAILED(hr)) {
		return NULL;
	}
	pSource->SubmitSourceBuffer(pSound);

	// 再生
	pSource->Start();


	return pSource;
}
#endif

#ifdef _DIRECTX_
;
#else
/**
 * @brief wavファイル読み込み
 * @param[in] file 読み込むファイル
 * @param[out] pData サウンドデータ
 * @return 処理結果
 */
HRESULT LoadWav(const char* file, SoundData* pData)
{
	HMMIO hMmio = NULL;
	MMIOINFO mmioInfo;
	MMRESULT mmRes;

	// WAVEファイルオープン
	memset(&mmioInfo, 0, sizeof(MMIOINFO));
	hMmio = mmioOpen(const_cast<char*>(file), &mmioInfo, MMIO_READ);
	if (hMmio == NULL) {
		return E_FAIL;
	}

	// RIFFチャンク検索
	MMCKINFO riffChunk;
	riffChunk.fccType = mmioFOURCC('W', 'A', 'V', 'E');
	mmRes = mmioDescend(hMmio, &riffChunk, NULL, MMIO_FINDRIFF);
	if (mmRes != MMSYSERR_NOERROR) {
		mmioClose(hMmio, 0);
		return E_FAIL;
	}

	// フォーマットチャンク検索
	MMCKINFO formatChunk;
	formatChunk.ckid = mmioFOURCC('f', 'm', 't', ' ');
	mmRes = mmioDescend(hMmio, &formatChunk, &riffChunk, MMIO_FINDCHUNK);
	if (mmRes != MMSYSERR_NOERROR) {
		mmioClose(hMmio, 0);
		return E_FAIL;
	}

	// フォーマット取得
	DWORD formatSize = formatChunk.cksize;
	DWORD size = mmioRead(hMmio, reinterpret_cast<HPSTR>(&pData->format), formatSize);
	if (size != formatSize) {
		mmioClose(hMmio, 0);
		return E_FAIL;
	}

	// RIFFチャンクに移動
	mmioAscend(hMmio, &formatChunk, 0);


	// データチャンク検索
	MMCKINFO dataChunk;
	dataChunk.ckid = mmioFOURCC('d', 'a', 't', 'a');
	mmRes = mmioDescend(hMmio, &dataChunk, &riffChunk, MMIO_FINDCHUNK);
	if (mmRes != MMSYSERR_NOERROR) {
		mmioClose(hMmio, 0);
		return E_FAIL;
	}

	// データ取得
	pData->bufSize = dataChunk.cksize;
	pData->pBuffer = new BYTE[pData->bufSize];
	size = mmioRead(hMmio, reinterpret_cast<HPSTR>(pData->pBuffer), pData->bufSize);
	if (size != dataChunk.cksize) {
		pData->bufSize = 0;
		if (pData->pBuffer != NULL)
		{
			delete[] pData->pBuffer;
			pData->pBuffer;
		}
		return E_FAIL;
	}

	mmioClose(hMmio, 0);
	return S_OK;
}
#endif

#ifdef _DIRECTX_
;
#else
/**
 * @brief mp3ファイル読み込み
 * @param[in] file 読み込むファイル
 * @param[out] pData サウンドデータ
 * @return 処理結果
 */
HRESULT LoadMP3(const char* file, SoundData* pData)
{
	HANDLE hFile; // ファイルポインタ
	DWORD readSize; // 読み込みサイズ

	// 読み込み
	hFile = CreateFile(
		file, GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL
	);
	if (hFile == INVALID_HANDLE_VALUE) {
		return E_FAIL;
	}

	// ファイルフォーマット読み込み
	MP3FormatInfo format;
	readSize = ReadMP3Format(hFile, &format);
	if (readSize == 0) {
		return E_FAIL;
	}

	// サウンドフレームヘッダ読み込み
	MP3FrameInfo frame;
	readSize = ReadMP3FrameHeader(hFile, format.offset, &frame);
	if (readSize == 0) {
		return E_FAIL;
	}

	// サウンドデータ読み込み
	readSize = ReadMP3Data(hFile, format.offset, format.dataSize, &frame, pData);
	if (readSize == 0) {
		return E_FAIL;
	}

	return S_OK;
}
#endif

#ifdef _DIRECTX_
;
#else
/**
 * @brief MP3フォーマットチェック
 * @param[in] hFile ファイルポインタ
 * @param[out] pFormat ファイルフォーマット
 * @return データサイズ
 */
DWORD ReadMP3Format(HANDLE hFile, MP3FormatInfo* pFormat)
{
	DWORD readSize;

	// MP3ファイルサイズ
	DWORD fileSize = GetFileSize(hFile, NULL);

	// ①ヘッダー/フッターの有無を調べる
	/*----------
	* ID3v1
	*  -末尾にタグがあるorタグなし
	*  [MP3データ]
	*		or
	*	[MP3データ]["TAG"(3byte)][タグ情報(125byte)]
	* ID3v2
	*  -先頭にタグがある
	*	["ID3"(3byte)][バージョン(2byte)][フラグ(1byte)][タグサイズ(4byte)][拡張ヘッダ][MP3データ]
	*----------*/
	const BYTE ID3V2_HEADER_SIZE = 10;
	BYTE header[ID3V2_HEADER_SIZE];	// ヘッダー情報
	ReadFile(hFile, header, sizeof(header), &readSize, NULL);

	// タグをチェックし、MP3データの位置、サイズを計算
	const char* ID3V1_TAG = "TAG";
	const char* ID3V2_TAG = "ID3";
	const BYTE MP3_TAG_SIZE = 3;
	if (memcmp(header, ID3V2_TAG, MP3_TAG_SIZE) == CMP_MATCH)
	{
		// ID3v2ヘッダー情報解析
		/*----------
		* [拡張ヘッダ]のデータサイズについて
		*  [タグサイズ]のデータ構造
		*	 [0AAAAAAA][0BBBBBBB][0CCCCCCC][0DDDDDDD]
		*	実際のデータサイズ
		*	 0x0000AAAAAAABBBBBBBCCCCCCCDDDDDDD
		*
		* - データの最上位ビットは必ず0
		*	 実際のサイズは、0を省いて詰めたもの
		*----------*/
		DWORD exHeaderSize =
			(header[6] << 21) |
			(header[7] << 14) |
			(header[8] << 7) |
			(header[9]);
		pFormat->offset = exHeaderSize + ID3V2_HEADER_SIZE;
		pFormat->dataSize = fileSize - pFormat->offset;
	}
	else
	{
		// ID3v1フッター情報解析
		const BYTE ID3V1_FOOTER_SIZE = 128;
		BYTE tag[MP3_TAG_SIZE];
		SetFilePointer(hFile, fileSize - ID3V1_FOOTER_SIZE, NULL, FILE_BEGIN);
		ReadFile(hFile, tag, MP3_TAG_SIZE, &readSize, NULL);
		pFormat->offset = 0;
		if (memcmp(tag, ID3V1_TAG, MP3_TAG_SIZE) == CMP_MATCH)
		{
			pFormat->dataSize = fileSize - 128;
		}
		else
		{
			pFormat->dataSize = fileSize;
		}
	}
	return pFormat->dataSize;
}
#endif


#ifdef _DIRECTX_
;
#else
/**
 * @brief MP3サウンドフレームヘッダ読み込み
 * @param[in] hFile ファイルポインタ
 * @param[in] seek フレーム読み込み位置
 * @param[out] pFrame フレーム情報
 * @return 読み込みサイズ
 */
DWORD ReadMP3FrameHeader(HANDLE hFile, DWORD seek, MP3FrameInfo* pFrame)
{
	DWORD readSize;

	// ①フレームヘッダから情報を取得
	/*----------
	 * MP3データの構造
	 *	[フレームヘッダ(4byte)][データ]
	 *	[フレームヘッダ(4byte)][データ]
	 *	[フレームヘッダ(4byte)][データ]
	 *	...(繰り返し
	 *----------*/
	 // MP3データ位置へ移動
	SetFilePointer(hFile, seek, NULL, FILE_BEGIN);

	/*----------
	 * フレームヘッダ情報
	 * http://www.cactussoft.co.jp/Sarbo/divMPeg3UnmanageHeader.html
	 *	[AAAAAAAA][AAABBCCD][EEEEFFGH][IIJJKLMM]
	 *	 A - 同期ビット(必ず１)
	 *	 B - MP3のバージョン
	 *		00 - MPEG2.5
	 *		01 - 予約
	 *		10 - MPEG2
	 *		11 - MPEG1
	 *	 C - レイヤー
	 *		00 - 予約
	 *		01 - Layer3
	 *		10 - Layer2
	 *		11 - Layer1
	 *	 D - CRC誤り検出の有無
	 *	 E - ビットレート
	 *	 F - サンプリング周波数
	 *	 G - パディング(フレームの最後のデータが0xFFだった場合、
	 *		 次のフレームの頭とつながるため、フレームの最後にNULLが挿入される
	 *		 実際にデータが挿入されたかどうかの情報をもつ
	 *	 H - 拡張(未使用
	 *	 I - チャンネルモード
	 *	 J - 拡張
	 *	 K - 著作権の有無
	 *	 L - オリジナル(コピーorオリジナル
	 *	 M - 強調
	 *
	 *	- 最低限必要な情報はB,C,E,F,G,I
	 *----------*/
	const BYTE FRAME_HEADER_SIZE = 4;
	BYTE frameHeader[FRAME_HEADER_SIZE];
	ReadFile(hFile, frameHeader, FRAME_HEADER_SIZE, &readSize, NULL);

	// 同期ビットチェック
	if (!(frameHeader[0] == 0xFF && (frameHeader[1] & 0xE0) == 0xE0)) {
		return 0;
	}

	// バージョン
	BYTE version = (frameHeader[1] >> 3) & 0b11;

	// レイヤー
	BYTE layer = (frameHeader[1] >> 1) & 0b11;

	// ビットレート(単位はkbit/sec
	const int bitRateTable[][16] = {
		// MPEG1, Layer1
		{ 0, 32, 64, 96, 128, 160, 192, 224, 256, 288, 320, 352, 384, 416, 448, -1 },
		// MPEG1, Layer2
		{ 0, 32, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320, 384, -1 },
		// MPEG1, Layer3
		{ 0, 32, 40, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320, -1 },
		// MPEG2, Layer1
		{ 0, 32, 48, 56, 64, 80, 96, 112, 128, 144, 160, 176, 192, 224, 256, -1 },
		// MPEG2/2.5, Layer2,3
		{ 0, 8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160, -1 }
	};
	BYTE bitRateTableIndex;
	if (version == 0b11) {
		bitRateTableIndex = 0b11 - layer;
	}
	else if (version == 0b10 && layer == 0b11) {
		bitRateTableIndex = 3;
	}
	else {
		bitRateTableIndex = 4;
	}
	WORD bitRate = bitRateTable[bitRateTableIndex][frameHeader[2] >> 4];

	// サンプリングレート
	const int sampleRateTable[][4] = {
		// MPEG1
		{ 44100, 48000, 32000, -1 },
		// MPEG2
		{ 22050, 24000, 16000, -1 },
		// MPEG2.5
		{ 11025, 12000, 8000, -1 },
	};
	BYTE sampleRateTableIndex;
	switch (version) {
	case 0b00: sampleRateTableIndex = 2; break;
	case 0b10: sampleRateTableIndex = 1; break;
	case 0b11: sampleRateTableIndex = 0; break;
	default: return E_FAIL;
	}
	WORD sampleRate = sampleRateTable[sampleRateTableIndex][(frameHeader[2] >> 2) & 0b11];

	// パディング
	BYTE padding = (frameHeader[2] >> 1) & 0b01;

	// チャンネル
	BYTE channel = frameHeader[3] >> 6;


	// フレームサイズ
	// ビットレートはkbit/secなのでbit/secに変換するために1000倍する
	const int samplePerFrame[][4] = {
		// layer1
		{ 384, 384, 384, -1 },
		// layer2
		{ 1152, 1152, 1152, -1 },
		// layer3
		{ 1152, 576, 576, -1 }
	};
	WORD frameBlockSize = ((samplePerFrame[0b11 - layer][sampleRateTableIndex] * bitRate * 1000 / 8) / sampleRate) + padding;

	// ②取得した情報をもとにMEPGLAYER3WAVEFORMATにデータ設定
	/*----------
	 *	MPEGLAYER3WAVEFORMAT
	 *	http://www.cactussoft.co.jp/Sarbo/divMPeg3UnmanageStruct.html
	 *	 - mpegオーディオレイヤー3(mp3)を扱うために拡張されたWAVEFORMAT構造体
	 *	.wfx.cbSize				- 構造体の拡張サイズ、MPEGLAYER3_WFX_EXTRA_BYTESを指定
	 *	.wfx.nChannels			- チャンネル数、モノラル＝１、ステレオ＝２
	 *	.wfx.wFormatTag			- mp3を表す、WAVE_FORMAT_MPEGLAYER3を指定
	 *	.wfx.nBlockAlign		- 1を指定
	 *	.wfx.wBitsPerSample		- 0を指定
	 *	.wfx.nSamplesPerSec		- サンプリング周波数
	 *	.wfx.nAvgBytesPerSec	- 1秒間に必要なデータサイズ
	 *	wID						- MPEGLAYER3_ID_MPEGを指定
	 *	fdwFlags				- パディングの挿入があれば指定
	 *	nFramesPerBlock			- 1つのブロックに配置するフレーム数
	 *	nBlockSize				- ブロックサイズ(フレームサイズ * フレーム数)
	 *	nCodecDelay				- 1393(0x571)を指定
	 *----------*/

	 // channel
	 // sample
	 // bitRate
	 // padding
	pFrame->channel = channel == 0x11 ? 1 : 2;
	pFrame->sampleRate = sampleRate;
	pFrame->bitRate = bitRate;
	pFrame->padding = padding;
	pFrame->frameSize = frameBlockSize;

	return pFrame->frameSize;
}

/**
 * @brief MP3サウンドデータ読み込み
 * @param[in] hFile ファイルポインタ
 * @param[in] seek フレーム読み込み位置
 * @param[in] size 読み込みデータ量
 * @param[in] pFrame フレーム情報
 * @param[out] pData サウンドデータ
 * @return 読み込みサイズ
 */
DWORD ReadMP3Data(HANDLE hFile, DWORD seek, DWORD size, MP3FrameInfo* pFrame, SoundData* pData)
{
	// 変換フォーマット作成
	MPEGLAYER3WAVEFORMAT mp3WavFormat;
	mp3WavFormat.wfx.cbSize = MPEGLAYER3_WFX_EXTRA_BYTES;
	mp3WavFormat.wfx.nChannels = pFrame->channel;
	mp3WavFormat.wfx.wFormatTag = WAVE_FORMAT_MPEGLAYER3;
	mp3WavFormat.wfx.nBlockAlign = 1;
	mp3WavFormat.wfx.wBitsPerSample = 0;
	mp3WavFormat.wfx.nSamplesPerSec = pFrame->sampleRate;
	mp3WavFormat.wfx.nAvgBytesPerSec = (pFrame->bitRate * 1000) / 8;

	mp3WavFormat.wID = MPEGLAYER3_ID_MPEG;
	mp3WavFormat.fdwFlags = pFrame->padding ? MPEGLAYER3_FLAG_PADDING_ON : MPEGLAYER3_FLAG_PADDING_OFF;
	mp3WavFormat.nFramesPerBlock = 1;
	mp3WavFormat.nBlockSize = static_cast<WORD>(pFrame->frameSize * mp3WavFormat.nFramesPerBlock);
	mp3WavFormat.nCodecDelay = 0x571;

	// mp3をwavへ変換可能か
	// 変換可能であれば,wavFormatへデータを設定
	WAVEFORMATEX wavFormat;
	wavFormat.wFormatTag = WAVE_FORMAT_PCM;
	MMRESULT mmr;
	mmr = acmFormatSuggest(NULL, &mp3WavFormat.wfx, &wavFormat, sizeof(WAVEFORMATEX), ACM_FORMATSUGGESTF_WFORMATTAG);
	if (mmr != MMSYSERR_NOERROR) {
		return 0;
	}

	// ACMストリームオープン
	// mp3からwavへフォーマット変換
	HACMSTREAM has;
	mmr = acmStreamOpen(&has, NULL, &mp3WavFormat.wfx, &wavFormat, NULL, NULL, NULL, 0);

	// MP3のブロックサイズからWAVE形式へデコード後のサイズを取得
	DWORD waveBlockSize;
	acmStreamSize(has, size, &waveBlockSize, ACM_STREAMSIZEF_SOURCE);

	// 変換データセット
	ACMSTREAMHEADER ash = { 0 };
	ash.cbStruct = sizeof(ACMSTREAMHEADER);
	ash.cbSrcLength = size;
	ash.pbSrc = new BYTE[ash.cbSrcLength];
	ash.cbDstLength = waveBlockSize;
	ash.pbDst = new BYTE[ash.cbDstLength];

	// デコード
	acmStreamPrepareHeader(has, &ash, 0);
	DWORD readSize;
	SetFilePointer(hFile, seek, NULL, FILE_BEGIN);
	ReadFile(hFile, ash.pbSrc, ash.cbSrcLength, &readSize, NULL);
	mmr = acmStreamConvert(has, &ash, 0);
	acmStreamUnprepareHeader(has, &ash, 0);
	acmStreamClose(has, 0);

	// wavデータコピー
	if (ash.cbDstLengthUsed > 0) {
		pData->bufSize = ash.cbDstLengthUsed;
		pData->pBuffer = new BYTE[pData->bufSize];
		pData->format = wavFormat;
		memcpy_s(pData->pBuffer, pData->bufSize,
			ash.pbDst, ash.cbDstLengthUsed);
	}

	delete[] ash.pbSrc;
	delete[] ash.pbDst;

	CloseHandle(hFile);

	return ash.cbSrcLengthUsed;
}
#endif


// EOF

#endif
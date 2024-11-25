#ifndef __DIRECTX_H__
#define __DIRECTX_H__

#include<math.h>
#include<d3d11.h>
#include <DirectXMath.h>
#include<timeapi.h>
#include <d3dcompiler.h>
#include<Windows.h>
#include<Digitalv.h>

#include "VectorOperator.h"

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

typedef struct
{
	DWORD time;
	DWORD oldTime;
	DWORD fpsCount;
	DWORD FPS;
	DWORD fpsTime;
}FPSTIMER;



#define SAFE_DELETE(p)			do{if(p){delete p; p = nullptr;}}while(0)
#define SAFE_DELETE_ARRAY(p)	do{if(p){delete[] p; p = nullptr;}}while(0)
#define SAFE_RELEASE(p)			do{if(p){p->Release(); p = nullptr;}}while(0)
//	テクスチャデータ用
#define POLYGON_TEXTURE ID3D11ShaderResourceView
//	ポリゴンデータ用
#define POLYGON_BUFFER	ID3D11Buffer
class RenderTarget;
class DepthStencil;

enum BlendMode
{
	BLEND_NONE,
	BLEND_ALPHA,
	BLEND_ADD,
	BLEND_ADDALPHA,
	BLEND_SUB,
	BLEND_SCREEN,
	BLEND_MAX
};

enum SamplerState
{
	SAMPLER_LINEAR,
	SAMPLER_POINT,
	SAMPLER_MAX
};

struct Vertex {
	float pos[3];
	float uv[2];
};


ID3D11Device* GetDevice();
ID3D11DeviceContext* GetContext();
IDXGISwapChain* GetSwapChain();
RenderTarget* GetDefaultRTV();
DepthStencil* GetDefaultDSV();

HRESULT InitDirectX(HWND hWnd, UINT width, UINT height, bool fullscreen);
void UninitDirectX();
void BeginDrawDirectX();
void EndDrawDirectX();

void SetRenderTargets(UINT num, RenderTarget** ppViews, DepthStencil* pView);
void SetCullingMode(D3D11_CULL_MODE cull);
void SetDepthTest(bool enable);
void SetBlendMode(BlendMode blend);
void SetSamplerState(SamplerState state);
ID3D11Buffer* CreateVertexBuffer(void* vtxData, UINT vtxNum);

#define SECONDS(sec) (1000 * sec)
#define MINUTUES(min) (60 * 60*  min)


// n次元配列の初期化。第２引数の型のサイズごとに初期化していく。
template<typename A, size_t N, typename T>
void Fill(A(&array)[N], const T& val) {
	std::fill((T*)array, (T*)(array + N), val);
}



#endif
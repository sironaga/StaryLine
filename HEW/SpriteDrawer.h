#ifndef __SPRITE_DRAWER_H__
#define __SPRITE_DRAWER_H__

#include <d3d11.h>

void InitSpriteDrawer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UINT width, UINT height);
void UninitSpriteDrawer();
void DrawSprite(ID3D11Buffer* pSprite, UINT vtxSize);

void SetSpritePos(float x, float y);
void SetSpriteAngle(float rad);
void SetSpriteScale(float x, float y);
void SetSpriteUVPos(float u, float v);
void SetSpriteUVScale(float u, float v);
void SetSpriteColor(float r, float g, float b, float a);
void SetSpriteTexture(ID3D11ShaderResourceView* pTexture);


#endif // __SPRITE_DRAWER_H__
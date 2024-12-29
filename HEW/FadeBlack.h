#pragma once

#include "Fade.h"
#include "Texture.h"
#include "Shader.h"

class CFadeBlack : public CFade
{
public:
	CFadeBlack();

	~CFadeBlack();

	// フェードイン・フェードアウト共に同じ処理で実装 
	void DrawFadeIn(float alpha) final;
	void DrawFadeOut(float alpha) final;

	// 共通フェード処理 
	void DrawFade(float alpha);

private:
	Texture* m_pTexture; // フェード中に表示する画像 
	Shader* m_pPS;
};